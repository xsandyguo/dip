/*
 * Copyright (c) 1997, 2019. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact xsandyguo@qq.com if you need additional information or have any
 * questions.
 *
 */

#include "flow/ui/flow_document.h"

#include <runtime/vm.h>

#include "ui/main_frame.h"
#include "io/tixml_dom.h"
#include "io/flow_factory.h"
#include "flow/ui/flow_view.h"
#include "flow/ui/auto_layout.h"

#include "resource.h"

IMPLEMENT_DYNCREATE(CFlowDocument, CDocument)

BEGIN_MESSAGE_MAP(CFlowDocument, CDocument)
    ON_COMMAND(ID_BUILD_RUN                 , &CFlowDocument::OnBuildRun)
    ON_COMMAND(ID_BUILD_DEBUG               , &CFlowDocument::OnBuildDebug)
    ON_COMMAND(ID_BUILD_STOP                , &CFlowDocument::OnBuildStop)
    ON_COMMAND(ID_ALIGN_LEFT                , &CFlowDocument::OnAlignLeft)
    ON_COMMAND(ID_ALIGN_RIGHT               , &CFlowDocument::OnAlignRight)
    ON_COMMAND(ID_ALIGN_TOP                 , &CFlowDocument::OnAlignUp)
    ON_COMMAND(ID_ALIGN_DOWN                , &CFlowDocument::OnAlignDown)
    ON_COMMAND(ID_FLOW_AUTOLAYOUT           , &CFlowDocument::OnAutoLayout)

    ON_UPDATE_COMMAND_UI(ID_BUILD_RUN       , &CFlowDocument::OnUpdateBuildRun)
    ON_UPDATE_COMMAND_UI(ID_BUILD_STOP      , &CFlowDocument::OnUpdateBuildStop)

    ON_COMMAND(ID_LAYER_BOTTOM      , &CFlowDocument::OnLayerBottom)
    ON_COMMAND(ID_LAYER_TOP         , &CFlowDocument::OnLayerTop)
    ON_COMMAND(ID_LAYER_UP          , &CFlowDocument::OnLayerUp)
    ON_COMMAND(ID_LAYER_DOWN        , &CFlowDocument::OnLayerDown)
END_MESSAGE_MAP()

CFlowDocument::CFlowDocument(){   
    drawing_.reset(new DrawingContainer());
}

CFlowDocument::~CFlowDocument(){
}

BOOL CFlowDocument::OnNewDocument(){
    return CDocument::OnNewDocument();
}


void CFlowDocument::DeleteContents(){
    ASSERT1(drawing_.get());

    POSITION pos = GetFirstViewPosition(); 
    while (pos != NULL) { 
        IView* view = dynamic_cast<IView*>(GetNextView(pos));
        if(view){
            view->ClearSelection();
        }
    } 

    drawing_->Clear();
}

BOOL CFlowDocument::OnOpenDocument(LPCTSTR lpszPathName){
    try{
        drawing_->Clear();

        TiXmlDomInput in(lpszPathName, FlowFactory::Instance());

        drawing_->Read(in);
        return TRUE;
    } catch (CException* e){
        UNREFERENCED_PARAMETER(e);
        AfxMessageBox(_T("Failed to open file selected file."));
        return FALSE;
    }
}

BOOL CFlowDocument::OnSaveDocument(LPCTSTR lpszPathName){
    ASSERT1(drawing_.get());
    try{
        TiXmlDomOutput out(FlowFactory::Instance());
        drawing_->Write(out);
        out.Save(lpszPathName);

        return TRUE;
    }catch(CException* e){
        UNREFERENCED_PARAMETER(e);
        AfxMessageBox(_T("Failed to save as specified file."));
        return FALSE;
    }
}

IDrawing* CFlowDocument::GetDrawing(){
    return drawing_.get();
}

void CFlowDocument::CommandInputed(CommandEventArgs& evt) {   
    VM::Instance().ExecCmd(evt.GetObjectName(),
                           evt.GetMethodName(),
                           evt.GetArguments());
}

void CFlowDocument::OnBuildRun(){
    VM::Instance().Run();
}

void CFlowDocument::OnBuildDebug(){
    VM::Instance().CheckConnectivity();
}

void CFlowDocument::OnBuildStop(){
    VM::Instance().Stop();
}

void CFlowDocument::OnAlignLeft(){
    std::vector<IFigure*> figures(GetSelectedFigures());
    AlignLayoutor layoutor(AO_Left, figures);
    layoutor.Layout();
}

void CFlowDocument::OnAlignRight(){
    std::vector<IFigure*> figures(GetSelectedFigures());
    AlignLayoutor layoutor(AO_Right, figures);
    layoutor.Layout();
}

void CFlowDocument::OnAlignUp(){
    std::vector<IFigure*> figures(GetSelectedFigures());
    AlignLayoutor layoutor(AO_Top, figures);
    layoutor.Layout();
}

void CFlowDocument::OnAlignDown(){
    std::vector<IFigure*> figures(GetSelectedFigures());
    AlignLayoutor layoutor(AO_Bottom, figures);
    layoutor.Layout();
}

void CFlowDocument::OnAutoLayout(){
    AutoLayoutor layouter(GetDrawing()->GetChildren());
    layouter.Layout();
}


void CFlowDocument::OnUpdateBuildStop(CCmdUI* pCmdUI){
    pCmdUI->Enable(VM::Instance().IsRunning());
}

void CFlowDocument::OnUpdateBuildRun(CCmdUI* pCmdUI){
    pCmdUI->Enable(!VM::Instance().IsRunning());
}


void CFlowDocument::OnLayerBottom(){
    if (GetSelectionCount() != 1){
        return;
    }

    GetDrawing()->SendToBack(GetSelectedFigures().at(0));
}

void CFlowDocument::OnLayerTop(){
    if (GetSelectionCount() != 1){
        return;
    }

    GetDrawing()->BringToFront(GetSelectedFigures().at(0));
}

void CFlowDocument::OnLayerUp(){
    if (GetSelectionCount() != 1){
        return;
    }

    IFigure* target = GetSelectedFigures().at(0);

    IDrawing* parent = GetDrawing();
    int index = parent->IndexOf(target);
    ASSERT1(index >= 0);
    if ( index < parent->GetChildCount() - 2){
        parent->MoveToLayer(target, ++index);
    } 
}


void CFlowDocument::OnLayerDown(){
    if (GetSelectionCount() != 1){
        return;
    }

    IFigure* target = GetSelectedFigures().at(0);
    IDrawing* parent = GetDrawing();
    int index = parent->IndexOf(target);

    if(index > 0){
        parent->MoveToLayer(target, --index);    
    }
}

IView* CFlowDocument::GetView(){
    POSITION pos = this->GetFirstViewPosition();
    return dynamic_cast<IView*>(GetNextView(pos));
}


const std::vector<IFigure*>& CFlowDocument::GetSelectedFigures(){
    return GetView()->GetSelectedFigures();
}

size_t CFlowDocument::GetSelectionCount(){
    return GetSelectedFigures().size();
}