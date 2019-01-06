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

#include "ui/thumbnail_view.h"
#include <atlpath.h>
#include <stack>
#include <vector>
#include "ui/main_frame.h"
#include "resource.h"
#include "app/flow_model.h"

class CThumbnailViewMenuButton : public CMFCToolBarMenuButton {
    friend class CThumbnailView;

    DECLARE_SERIAL(CThumbnailViewMenuButton)

public:
    CThumbnailViewMenuButton(HMENU hMenu = NULL) 
        : CMFCToolBarMenuButton((UINT)-1, hMenu, -1){
    }

    virtual void OnDraw(CDC* pDC,
        const CRect& rect, 
        CMFCToolBarImages* pImages,
        BOOL bHorz = TRUE,
        BOOL bCustomizeMode = FALSE,
        BOOL bHighlight = FALSE, 
        BOOL bDrawBorder = TRUE,
        BOOL bGrayDisabledButtons = TRUE){
        pImages = CMFCToolBar::GetImages();

        CAfxDrawState ds;
        pImages->PrepareDrawImage(ds);

        CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, 
            bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

        pImages->EndDrawImage(ds);
    }
};

IMPLEMENT_SERIAL(CThumbnailViewMenuButton, CMFCToolBarMenuButton, 1)

CThumbnailView::CThumbnailView(){
    current_sort_ = ID_SORTING_GROUPBYTYPE;
}

CThumbnailView::~CThumbnailView(){
}

BEGIN_MESSAGE_MAP(CThumbnailView, CDockablePane)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_CONTEXTMENU()
    ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)
    ON_COMMAND(ID_CLASS_ADD_MEMBER_VARIABLE, OnClassAddMemberVariable)
    ON_COMMAND(ID_CLASS_DEFINITION         , OnClassDefinition)
    ON_COMMAND(ID_CLASS_PROPERTIES         , OnClassProperties)
    ON_COMMAND(ID_NEW_FOLDER               , OnNewFolder)
    ON_COMMAND(ID_REFRESH_FOLDER           , OnRefreshFolder)
    ON_WM_PAINT()
    ON_WM_SETFOCUS()
    ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
    ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, 
        ID_SORTING_SORTBYACCESS, 
        OnUpdateSort)
END_MESSAGE_MAP()

int CThumbnailView::OnCreate(LPCREATESTRUCT lpCreateStruct){
    if (CDockablePane::OnCreate(lpCreateStruct) == -1)
        return -1;

    CRect rectDummy;
    rectDummy.SetRectEmpty();

    // 创建视图:
    const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT 
        | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

    if (!cache_files_view_.Create(dwViewStyle, rectDummy, this, 2)){
        TRACE0("未能创建类视图\n");
        return -1;      // 未能创建
    }

    // 加载图像:
    toolbar_.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
    toolbar_.LoadToolBar(IDR_SORT, 0, 0, TRUE /* 已锁定*/);

    OnChangeVisualStyle();

    toolbar_.SetPaneStyle(toolbar_.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
    toolbar_.SetPaneStyle(toolbar_.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC 
        | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

    toolbar_.SetOwner(this);

    // 所有命令将通过此控件路由，而不是通过主框架路由:
    toolbar_.SetRouteCommandsViaFrame(FALSE);

    CMenu menuSort;
    menuSort.LoadMenu(IDR_POPUP_SORT);

    toolbar_.ReplaceButton(ID_SORT_MENU, CThumbnailViewMenuButton(
        menuSort.GetSubMenu(0)->GetSafeHmenu()));

    CThumbnailViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CThumbnailViewMenuButton,
        toolbar_.GetButton(0));

    if (pButton != NULL){
        pButton->m_bText = FALSE;
        pButton->m_bImage = TRUE;
        pButton->SetImage(GetCmdMgr()->GetCmdImage(current_sort_));
        pButton->SetMessageWnd(this);
    }

    // 填入一些静态树视图数据(此处只需填入虚拟代码，而不是复杂的数据)
    FillTree();

    return 0;
}

void CThumbnailView::OnSize(UINT nType, int cx, int cy){
    CDockablePane::OnSize(nType, cx, cy);
    AdjustLayout();
}


void CThumbnailView::FillTree(){
    HTREEITEM parent = TVI_ROOT;
    HTREEITEM root = NULL;
    std::stack<CString> work_stack;
    work_stack.push(g_flow_model.GetAcquisitionPath());

    CFileFind finder;
    SHFILEINFO file_info;
    std::vector<HTREEITEM> nodes;

    while(!work_stack.empty()){        
        CPath file_path = work_stack.top();
        work_stack.pop();
        
        SHGetFileInfo(file_path, 0, &file_info, sizeof(SHFILEINFO), SHGFI_ICON);
        parent = cache_files_view_.InsertItem(PathFindFileName(file_path.m_strPath),
            file_info.iIcon,
            file_info.iIcon,
            parent);
        nodes.push_back(parent);

        file_path.Append(L"*.*");
        BOOL working = finder.FindFile(file_path);
        while (working) {
            working = finder.FindNextFile();
            if(finder.IsDots()){
                continue;;
            }

            CPath file_name = finder.GetFileName();
            if (finder.IsDirectory()) {
                work_stack.push(finder.GetFilePath());
            } else{
                SHGetFileInfo(file_name.GetExtension(), 
                    0,
                    &file_info,
                    sizeof(SHFILEINFO),
                    SHGFI_USEFILEATTRIBUTES | SHGFI_SYSICONINDEX);  

                cache_files_view_.InsertItem(finder.GetFileName(), 
                    file_info.iIcon,
                    file_info.iIcon, 
                    parent);
            }
        }
    }

    for (std::vector<HTREEITEM>::iterator itor = nodes.begin();
        itor != nodes.end();
        ++itor){
        cache_files_view_.Expand(*itor, TVE_EXPAND);
    }
}

void CThumbnailView::OnContextMenu(CWnd* pWnd, CPoint point){
    CTreeCtrl* pWndTree = (CTreeCtrl*)&cache_files_view_;
    ASSERT_VALID(pWndTree);

    if (pWnd != pWndTree){
        CDockablePane::OnContextMenu(pWnd, point);
        return;
    }

    if (point != CPoint(-1, -1)){
        // 选择已单击的项:
        CPoint ptTree = point;
        pWndTree->ScreenToClient(&ptTree);

        UINT flags = 0;
        HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
        if (hTreeItem != NULL){
            pWndTree->SelectItem(hTreeItem);
        }
    }

    pWndTree->SetFocus();
    CMenu menu;
    menu.LoadMenu(IDR_POPUP_SORT);

    CMenu* pSumMenu = menu.GetSubMenu(0);

    if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx))){
        CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

        if (!pPopupMenu->Create(this, point.x, point.y, 
            (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
            return;

        ((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
        UpdateDialogControls(this, FALSE);
    }
}

void CThumbnailView::AdjustLayout(){
    if (GetSafeHwnd() == NULL){
        return;
    }

    CRect rectClient;
    GetClientRect(rectClient);

    int cyTlb = toolbar_.CalcFixedLayout(FALSE, TRUE).cy;
    toolbar_.SetWindowPos(NULL,
        rectClient.left, 
        rectClient.top, 
        rectClient.Width(), 
        cyTlb, 
        SWP_NOACTIVATE | SWP_NOZORDER);

    cache_files_view_.SetWindowPos(NULL,
        rectClient.left + 1, 
        rectClient.top + cyTlb + 1, 
        rectClient.Width() - 2, 
        rectClient.Height() - cyTlb - 2,
        SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL CThumbnailView::PreTranslateMessage(MSG* pMsg){
    return CDockablePane::PreTranslateMessage(pMsg);
}

void CThumbnailView::OnSort(UINT id){
    if (current_sort_ == id){
        return;
    }

    current_sort_ = id;

    CThumbnailViewMenuButton* pButton =  DYNAMIC_DOWNCAST(
        CThumbnailViewMenuButton, toolbar_.GetButton(0));

    if (pButton != NULL){
        pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
        toolbar_.Invalidate();
        toolbar_.UpdateWindow();
    }
}

void CThumbnailView::OnUpdateSort(CCmdUI* pCmdUI){
    pCmdUI->SetCheck(pCmdUI->m_nID == current_sort_);
}

void CThumbnailView::OnClassAddMemberFunction(){
    AfxMessageBox(_T("添加成员函数..."));
}

void CThumbnailView::OnClassAddMemberVariable(){
    // TODO: 在此处添加命令处理程序代码
}

void CThumbnailView::OnClassDefinition(){
    // TODO: 在此处添加命令处理程序代码
}

void CThumbnailView::OnClassProperties(){
    // TODO: 在此处添加命令处理程序代码
}

void CThumbnailView::OnNewFolder(){
    AfxMessageBox(_T("新建文件夹..."));
}

void CThumbnailView::OnRefreshFolder(){

    AfxMessageBox(_T("新建文件夹..."));
}

void CThumbnailView::OnPaint(){
    CPaintDC dc(this); // 用于绘制的设备上下文

    CRect rectTree;
    cache_files_view_.GetWindowRect(rectTree);
    ScreenToClient(rectTree);

    rectTree.InflateRect(1, 1);
    dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CThumbnailView::OnSetFocus(CWnd* pOldWnd){
    CDockablePane::OnSetFocus(pOldWnd);

    cache_files_view_.SetFocus();
}

void CThumbnailView::OnChangeVisualStyle(){
    image_list_.DeleteImageList();

    SHFILEINFO file_info;
    HIMAGELIST image_list = (HIMAGELIST)SHGetFileInfo(L"",
        0,
        &file_info,  
        sizeof (SHFILEINFO), 
        SHGFI_SYSICONINDEX | SHGFI_SMALLICON); 	
    image_list_.Attach(image_list);
    cache_files_view_.SetImageList(&image_list_, TVSIL_NORMAL);
    
    toolbar_.CleanUpLockedImages();
    toolbar_.LoadBitmap(IDB_SORT_24, 0, 0, TRUE /* 锁定*/);
}
