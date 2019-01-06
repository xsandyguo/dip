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

#include "ui/output_window.h"

#include "resource.h"
#include "ui/main_frame.h"

#define WM_LOG_ADDED (WM_USER + 100)

BEGIN_MESSAGE_MAP(CRichOutputCtrl, CRichEditCtrl)
    ON_WM_CREATE()
    ON_MESSAGE(WM_LOG_ADDED, OnLogAdded)
END_MESSAGE_MAP()

CRichOutputCtrl::CRichOutputCtrl(){

}

CRichOutputCtrl::~CRichOutputCtrl(){
    GetLogging()->RemoveObserver(this);
}

int CRichOutputCtrl::OnCreate(LPCREATESTRUCT create){
    __super::OnCreate(create);

    SetReadOnly(TRUE);    

    GetLogging()->AddObserver(this);
    return 0;
}

void CRichOutputCtrl::AddLog(LOG_CATEGORY cat, const wchar_t* msg){
    static const COLORREF colors[] = { RGB(255,0,0), RGB(220,19,19), 
        RGB(0,255,255), RGB(0,255,0), RGB(255,128,0), RGB(255,255,0), 
        RGB(255,0,255)};

    CHARFORMAT format;
    ZeroMemory(&format, sizeof(CHARFORMAT));
    format.cbSize = sizeof(CHARFORMAT);
    format.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE ;
    format.dwEffects &= ~CFM_BOLD;
    format.dwEffects &= ~CFM_ITALIC;
    format.dwEffects &= ~CFM_UNDERLINE;
    format.yHeight = 180;
    _stprintf(format.szFaceName,_T("宋体"));
    format.crTextColor = colors[cat];
    SetWordCharFormat(format);    

    CString log;
    log.Format(_T("%s> %s\r\n"), kCategoryName[cat], msg);
    SetSel(-1, -1);
    ReplaceSel(log);

    PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
}

LRESULT CRichOutputCtrl::OnLogAdded(WPARAM wParam, LPARAM lParam){
    __mutexScope(lock_);
    for (LogIter iter = log_cache_.begin(); iter != log_cache_.end(); ++iter) {
        AddLog(iter->first, iter->second);
    }

    log_cache_.clear();

    return 0;
}

void CRichOutputCtrl::LogMessage(LOG_CATEGORY cat, const CString& msg){
    __mutexScope(lock_);

    log_cache_.push_back(std::make_pair(cat, msg));

    PostMessage(WM_LOG_ADDED);
}

COutputWindow::COutputWindow(){
}

COutputWindow::~COutputWindow(){
}

BEGIN_MESSAGE_MAP(COutputWindow, CDockablePane)
    ON_WM_CREATE()
    ON_WM_SIZE()
END_MESSAGE_MAP()

int COutputWindow::OnCreate(LPCREATESTRUCT lpCreateStruct){
    if (CDockablePane::OnCreate(lpCreateStruct) == -1)
        return -1;

    CRect rectDummy;
    rectDummy.SetRectEmpty();

    if (!tab_ctrl_.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1)){
        TRACE0("未能创建输出选项卡窗口\n");
        return -1; 
    }

    const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;
    if (!output_debug_.Create(dwStyle | ES_MULTILINE, rectDummy, &tab_ctrl_, 2) ||
        !output_build_.Create(dwStyle, rectDummy, &tab_ctrl_, 3) ){
        TRACE0("未能创建输出窗口\n");
        return -1;
    }

    UpdateFonts();

    CString strTabName;
    BOOL bNameValid;
    bNameValid = strTabName.LoadString(IDS_DEBUG_TAB);
    ASSERT1(bNameValid);
    tab_ctrl_.AddTab(&output_debug_, strTabName, (UINT)0);

    bNameValid = strTabName.LoadString(IDS_BUILD_TAB);
    ASSERT1(bNameValid);
    tab_ctrl_.AddTab(&output_build_, strTabName, (UINT)1);   

    
    return 0;
}

void COutputWindow::OnSize(UINT nType, int cx, int cy){
    CDockablePane::OnSize(nType, cx, cy);

    // 选项卡控件应覆盖整个工作区:
    tab_ctrl_.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void COutputWindow::AdjustHorzScroll(CListBox& listbox){
    CClientDC dc(this);
    CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);

    int cxExtentMax = 0;

    for (int i = 0; i < listbox.GetCount(); i ++){
        CString strItem;
        listbox.GetText(i, strItem);
        cxExtentMax = max(cxExtentMax, dc.GetTextExtent(strItem).cx);
    }

    listbox.SetHorizontalExtent(cxExtentMax);
    dc.SelectObject(pOldFont);
}


void COutputWindow::UpdateFonts(){
    output_build_.SetFont(&afxGlobalData.fontRegular);
    output_debug_.SetFont(&afxGlobalData.fontRegular);
}

COutputList::COutputList(){
}

COutputList::~COutputList(){
}

BEGIN_MESSAGE_MAP(COutputList, CListBox)
    ON_WM_CREATE()
    ON_WM_CONTEXTMENU()
    ON_COMMAND(ID_EDIT_COPY     , OnEditCopy)
    ON_COMMAND(ID_EDIT_CLEAR    , OnEditClear)
    ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
    ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()

int COutputList::OnCreate(LPCREATESTRUCT lpCreateStruct){
    if (CListBox::OnCreate(lpCreateStruct) == -1)
        return -1;

    CFont* font = new CFont;
    font->CreatePointFont(120, _T("Arial"));
    SetFont(font);

    return 0;
}

void COutputList::OnContextMenu(CWnd* /*pWnd*/, CPoint point){
    CMenu menu;
    menu.LoadMenu(IDR_OUTPUT_POPUP);

    CMenu* pSumMenu = menu.GetSubMenu(0);

    if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx))){
        CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

        if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
            return;

        ((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
        UpdateDialogControls(this, FALSE);
    }

    SetFocus();
}

void COutputList::OnEditCopy(){
    MessageBox(_T("复制输出"));
}

void COutputList::OnEditClear(){
    ResetContent();
}

void COutputList::OnViewOutput(){
    CDockablePane* pParentBar  = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
    CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

    if (pMainFrame != NULL && pParentBar != NULL){
        pMainFrame->SetFocus();
        pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
        pMainFrame->RecalcLayout();
    }
}
