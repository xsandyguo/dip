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

// ChildFrm.cpp : CChildFrame 类的实现
//

#include "app/app.h"

#include "ui/child_frame.h"


IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
    ON_COMMAND(ID_FILE_PRINT                  , &CChildFrame::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT           , &CChildFrame::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW          , &CChildFrame::OnFilePrintPreview)
    ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, &CChildFrame::OnUpdateFilePrintPreview)
END_MESSAGE_MAP()

CChildFrame::CChildFrame(){
}

CChildFrame::~CChildFrame(){
}

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext){
    return wnd_splitter_.Create(this,
                                2, 2,			// TODO: 调整行数和列数
                                CSize(10, 10),	// TODO: 调整最小窗格大小
                                pContext);
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs){
    // TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或样式
    if( !CMDIChildWndEx::PreCreateWindow(cs) )
        return FALSE;

    return TRUE;
}

void CChildFrame::ActivateFrame(int nCmdShow ){
    __super::ActivateFrame(nCmdShow);

    //IView* view =  GetActiveView();
    //if(view && !view->GetEditor()){
    //    view->AddNotify(this);

    //    view->AddSelectionObserver(&widget_properties_);
    //    view->AddSelectionObserver(&widget_command_);
    //}
}

void CChildFrame::OnFilePrint(){
    if (m_dockManager.IsPrintPreviewValid()){
        PostMessage(WM_COMMAND, AFX_ID_PREVIEW_PRINT);
    }
}

void CChildFrame::OnFilePrintPreview(){
    if (m_dockManager.IsPrintPreviewValid()){
        PostMessage(WM_COMMAND, AFX_ID_PREVIEW_CLOSE);
    }
}

void CChildFrame::OnUpdateFilePrintPreview(CCmdUI* pCmdUI){
    pCmdUI->SetCheck(m_dockManager.IsPrintPreviewValid());
}
