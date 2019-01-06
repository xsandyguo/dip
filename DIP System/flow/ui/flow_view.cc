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

#include "flow/ui/flow_view.h"

#include <algorithm>

#include "app/app.h"
#include "base/prefs_const.h"
#include "flow/ui/flow_document.h"
#include "flow/geometry.h"
#include "flow/creation_tool.h"
#include "flow/delegation_selection_tool.h"
#include "flow/data_line.h"
#include "flow/creation_tool.h"
#include "flow/connection_tool.h"
#include "io/flow_factory.h"

#include "resource.h"

#include "ui/widget_manager.h"

IMPLEMENT_DYNCREATE(CFlowView, AbstractView)

BEGIN_MESSAGE_MAP(CFlowView, AbstractView)
    ON_WM_CONTEXTMENU()
    ON_WM_SETCURSOR()
    ON_WM_SIZE() 
    ON_WM_ERASEBKGND()
    ON_COMMAND(ID_FILE_PRINT        , &CScrollView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT , &CScrollView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CFlowView::OnFilePrintPreview)
    ON_COMMAND(ID_VIEW_GRID          , &CFlowView::OnViewGrid)
    ON_UPDATE_COMMAND_UI(ID_VIEW_GRID,&CFlowView::OnUpdateViewGrid)
    ON_UPDATE_COMMAND_UI(ID_EDIT_COPY,&CFlowView::OnUpdateEditCopy)
    ON_UPDATE_COMMAND_UI(ID_STATUSBAR_POS        , &CFlowView::OnUpdateStatusPos)
    ON_UPDATE_COMMAND_UI(ID_STATUSBAR_ZOOM_SLIDER, &CFlowView::OnUpdateStatusbarZoomSlider)
    ON_UPDATE_COMMAND_UI(ID_STATUSBAR_ZOOM_LIST  , &CFlowView::OnUpdateStatusbarZoomList)    

    ON_COMMAND(ID_TOOL_POINTER              , &CFlowView::OnToolPointer)
    ON_COMMAND(ID_TOOL_LINE                 , &CFlowView::OnToolLine)
END_MESSAGE_MAP()

CFlowView::CFlowView():grid_(this), current_tool_(NULL){
    event_handler_.reset(new InnerEventHandler(this));
    viewport_size_.cx = 0;
    viewport_size_.cy = 0;
}

CFlowView::~CFlowView(){
    WidgetManager::Unregister(this);
}

void CFlowView::DrawCanvas(Graphics& g){
    __super::DrawCanvas(g);
    grid_.Draw(g);
}

Size CFlowView::GetPreferSize(){
    CRect client_area;
    GetClientRect(&client_area);

    Rect document_bound = GetDrawing()->GetBounds();
    CSize scroll_size = CSize(client_area.Width(), client_area.Height());// GetTotalSize();
    Size prefer = Size(std::max<int>(document_bound.Width, scroll_size.cx),
                std::max<int>(document_bound.Height, scroll_size.cy));
    
    return prefer;
}

void CFlowView::OnInitialUpdate(){
    __super::OnInitialUpdate();
    
    IDrawing* drawing =  GetDrawing();
    drawing->AddContainerObserver(event_handler_.get());
    drawing->AddFigureObserver(event_handler_.get());

    WidgetManager::Register(this);

    ToolDone();
}

void CFlowView::OnFilePrintPreview(){
    AFXPrintPreview(this);
}

BOOL CFlowView::OnPreparePrinting(CPrintInfo* pInfo){
    return DoPreparePrinting(pInfo);
}

void CFlowView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/){
}

void CFlowView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/){
}

void CFlowView::OnContextMenu(CWnd* /* pWnd */, CPoint point){
    kTheApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT,
                                                    point.x,
                                                    point.y, 
                                                    this, 
                                                    TRUE);
}

CFlowDocument* CFlowView::GetDocument() const {
    ASSERT1(m_pDocument->IsKindOf(RUNTIME_CLASS(CFlowDocument)));
    return (CFlowDocument*)m_pDocument;
}

IDrawing* CFlowView::GetDrawing(){
    return GetDocument()->GetDrawing();
}

BOOL CFlowView::OnEraseBkgnd(CDC* pDC){
    return TRUE;
}

ILocator& CFlowView::GetLocator(){
    return *((ILocator*)(&grid_));
}

void CFlowView::OnSize(UINT nType, int cx, int cy){
    __super::OnSize(nType, cx, cy);
    RelayoutView();
}

void CFlowView::RelayoutView(){
    IDrawing* container = GetDrawing();
    if (!container) {
        return;
    }
    Rect figure_area = container->GetBounds();
    int32 figure_width  = figure_area.Width  + kFigureMargin.Width;
    int32 figure_height = figure_area.Height + kFigureMargin.Height;

    if(viewport_size_.cx == 0 || viewport_size_.cy == 0){
        viewport_size_.cx = figure_width;
        viewport_size_.cy = figure_height;
    }else{
        viewport_size_.cx = max(viewport_size_.cx, figure_width);
        viewport_size_.cy = max(viewport_size_.cy, figure_height);
    }
         
    SetScrollSizes(MM_TEXT, viewport_size_);
} 

void CFlowView::ExtendViewportTo(const Point& pos){
    viewport_size_.cx = max(viewport_size_.cx, pos.X);
    viewport_size_.cy = max(viewport_size_.cy, pos.Y);

    RelayoutView();

    /*SetScrollPos(SB_HORZ, viewport_size_.cx);
    SetScrollPos(SB_VERT, viewport_size_.cy);*/
}

BOOL CFlowView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
    static bool flag = false;
    if (!flag){
        __super::OnSetCursor(pWnd, nHitTest, message);
        flag = true;
    }
    return TRUE;
}

void CFlowView::InnerEventHandler::AreaInvalidated(HandleEventArgs& evt){
    this->view_->Invalidate(evt.GetInvalidatedArea());
}
CFlowView::InnerEventHandler::InnerEventHandler(CFlowView* view){
    this->view_ = view;
}
CFlowView::InnerEventHandler::~InnerEventHandler(){
}

void CFlowView::InnerEventHandler::AttributeChanged(FigureEventArgs& evt){
}

void CFlowView::InnerEventHandler::AreaInvalidated(FigureEventArgs& evt){
    this->view_->Invalidate(evt.GetInvalidatedArea());
}

void CFlowView::InnerEventHandler::FigureChanged(FigureEventArgs& evt){
    this->view_->Invalidate(evt.GetInvalidatedArea());  
}

void CFlowView::InnerEventHandler::FigureRemoved(DrawingEventArgs& evt){
    this->view_->RemoveFromSelection(evt.GetTargetFigure());
    this->view_->Invalidate(evt.GetInvalidatedArea());
}

void CFlowView::InnerEventHandler::FigureAdded(DrawingEventArgs& evt){
    this->view_->Invalidate(evt.GetInvalidatedArea());  
}

void CFlowView::InnerEventHandler::RequestRemove(FigureEventArgs& evt){}

void CFlowView::OnMouseMove(MouseEventArgs& evt){
    __super::OnMouseMove(evt);

    cursor_position_ = evt.GetPoint();
}


void CFlowView::OnViewGrid(){
    grid_.Enable(!grid_.IsEnabled());
}

void CFlowView::OnUpdateViewGrid(CCmdUI* pCmdUI){
    pCmdUI->SetCheck(grid_.IsEnabled());
}

void CFlowView::OnUpdateStatusPos(CCmdUI *pCmdUI){
    pCmdUI->Enable(TRUE);
    CString status_pos_text;
    status_pos_text.LoadString(ID_STATUSBAR_POS);
    status_pos_text.TrimRight();
    status_pos_text.AppendFormat(L" X:%d Y:%d", cursor_position_.X, cursor_position_.Y);
    pCmdUI->SetText(status_pos_text);
    
    CMFCRibbonStatusBar* status_bar = 
        dynamic_cast<CMFCRibbonStatusBar*>(
        dynamic_cast<CMFCRibbonCmdUI*>(pCmdUI)->m_pOther);
    status_bar->AdjustLayout();
    status_bar->RecalcLayout();
}

void CFlowView::OnUpdateStatusbarZoomSlider(CCmdUI *pCmdUI){
    pCmdUI->Enable(TRUE);
    // TODO: Add your command update UI handler code here
}

void CFlowView::OnUpdateStatusbarZoomList(CCmdUI *pCmdUI){
    pCmdUI->Enable(TRUE);
    // TODO: Add your command update UI handler code here
}

void CFlowView::OnUpdateEditCopy(CCmdUI* pCmdUI){
    pCmdUI->Enable(selected_figures_.size());
}

CreationTool* CFlowView::GetCreationTool(){
    if(!creation_tool_.get()){
        creation_tool_.reset(new CreationTool(&FlowFactory::Instance()));
        creation_tool_->AddToolObserver(this);
    }

    return creation_tool_.get();
}

ITool* CFlowView::GetConnectionTool(){
    if(!connection_tool_.get()){
        connection_tool_.reset(new ConnectionTool(new DataLine()));
        connection_tool_->AddToolObserver(this);
    }

    return connection_tool_.get();
}

ITool* CFlowView::GetSelectionTool(){
    if(!selection_tool_.get()){
        selection_tool_.reset(new DelegationSelectionTool(
            new ConnectionTool(new DataLine())));
        selection_tool_->AddToolObserver(this);
    }

    return selection_tool_.get();
}

ITool* CFlowView::GetTool(){
    return current_tool_;
}

void CFlowView::SetTool(ITool* tool){
    if(current_tool_){
        current_tool_->Deactive(this);
    }

    current_tool_ = tool;
    if(current_tool_){
        current_tool_->Active(this);
    }
}


void CFlowView::ItemClicked(ToolboxEventArgs& evt){
    CreationTool* creation_tool = GetCreationTool();
    creation_tool->SetCreationClass(evt.GetNewSelection());

    SetTool(creation_tool);
}


void CFlowView::ToolDone(ToolEventArgs& evt){
    ToolDone();
}

void CFlowView::ToolDone(){
    SetTool(GetSelectionTool());
}

void CFlowView::AreaInvalidated(ToolEventArgs& evt){
    Invalidate(evt.GetInvalidatedArea());
}

void CFlowView::OnToolPointer(){
    ToolDone();
}

void CFlowView::OnToolLine(){
    SetTool(GetConnectionTool());
}

