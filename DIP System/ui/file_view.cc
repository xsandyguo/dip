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

#include "ui/file_view.h"

#include "app/app_model.h"
#include "resource.h"

#define SHCNF_ACCEPT_INTERRUPTS     0x0001
#define SHCNF_ACCEPT_NON_INTERRUPTS 0x0002

UINT UWM_NOTIFY_RECYCLE_BIN = ::RegisterWindowMessage(
                                  _T("UWM_NOTIFY_RECYCLE_BIN"));


void CMyShellListCtrl::DoDefault(int iItem) {
    LVITEM lvItem;

    ZeroMemory(&lvItem, sizeof(lvItem));
    lvItem.mask = LVIF_PARAM;
    lvItem.iItem = iItem;

    if (!GetItem(&lvItem)) {
        return;
    }

    LPAFX_SHELLITEMINFO pInfo = (LPAFX_SHELLITEMINFO) lvItem.lParam;
    if (pInfo == NULL || pInfo->pParentFolder == NULL || pInfo->pidlRel == NULL) {
        ASSERT(FALSE);
        return;
    }

    CComPtr<IShellFolder> psfFolder = pInfo->pParentFolder;
    if (psfFolder == NULL) {
        HRESULT hr = SHGetDesktopFolder(&psfFolder);
        if (FAILED(hr)) {
            ASSERT(FALSE);
            return;
        }
    }
    if (psfFolder == NULL) {
        return;
    }

    // If specified element is a folder, try to display it:
    ULONG ulAttrs = SFGAO_FOLDER;
    psfFolder->GetAttributesOf(1, (const struct _ITEMIDLIST **) &pInfo->pidlRel,
                               &ulAttrs);

    if (ulAttrs & SFGAO_FOLDER) {
        DisplayFolder(pInfo);
        return;
    }

    bool handled = true;
    CString file_name;
    if(GetItemPath(file_name, iItem)){
        handled = OpenFile(file_name);
    }
    
    if(!handled){
        CMFCShellListCtrl::DoDefault(iItem);
    }
}

HRESULT CMyShellListCtrl::EnumObjects(LPSHELLFOLDER pParentFolder,
                                      LPITEMIDLIST pidlParent) {
    return CMFCShellListCtrl::EnumObjects(pParentFolder, pidlParent);
}

bool CMyShellListCtrl::OpenFile(const CString& file_path){
    CString file_ext(::PathFindExtension(file_path));
    for(std::vector<IAppModel*>::iterator iter = g_app_models.begin();
        iter != g_app_models.end();
        ++iter){
        if((*iter)->IsSupport(file_ext)){
            (*iter)->OpenFile(file_path);
            return true;
        }
    }    

    return false;
}

CFileView::CFileView() {
}

CFileView::~CFileView() {
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_PAINT()
    ON_WM_SETFOCUS()
    ON_WM_ERASEBKGND()
    ON_REGISTERED_MESSAGE(UWM_NOTIFY_RECYCLE_BIN, OnNotifyRecycleBin)
    ON_WM_DESTROY()
END_MESSAGE_MAP()

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct) {
    if (CDockablePane::OnCreate(lpCreateStruct) == -1)
        return -1;

    CRect rectDummy (0, 0, 0, 0);
    splitter_.SetPanel(&shell_tree_, &shell_list_);
    splitter_.Create(WS_CHILD | WS_VISIBLE, rectDummy, this, 501);

    BOOL ret = shell_tree_.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | TVS_HASLINES |
                                  TVS_LINESATROOT | TVS_HASBUTTONS,
                                  rectDummy, &splitter_, 502);

    shell_list_.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT,
                       rectDummy, &splitter_, 503);

    shell_tree_.SetRelatedList(&shell_list_);

    LPITEMIDLIST ppidl;
    if (SHGetSpecialFolderLocation(GetSafeHwnd(),CSIDL_BITBUCKET,
                                   &ppidl) != NOERROR) {
        return 0;
    }

    SHChangeNotifyEntry scnIDL;
    scnIDL.pidl = ppidl;
    scnIDL.fRecursive = TRUE;

    recycle_bin_ = SHChangeNotifyRegister(m_hWnd,
                                          SHCNF_ACCEPT_INTERRUPTS | SHCNF_ACCEPT_NON_INTERRUPTS,
                                          SHCNE_ALLEVENTS,
                                          UWM_NOTIFY_RECYCLE_BIN,
                                          1,
                                          &scnIDL);
    return 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy) {
    CDockablePane::OnSize(nType, cx, cy);

    splitter_.SetWindowPos (NULL, 0, 0,
                            cx, cy,
                            SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL CFileView::OnEraseBkgnd(CDC* /*pDC*/) {
    return TRUE;
}

void CFileView::OnSetFocus(CWnd* pOldWnd) {
    CDockablePane::OnSetFocus(pOldWnd);
    splitter_.SetFocus ();
}


LRESULT CFileView::OnNotifyRecycleBin(WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(wParam);
    if (lParam == SHCNE_UPDATEIMAGE) {
        shell_tree_.Refresh();
    }

    if (lParam == SHCNE_FREESPACE) {
        LPITEMIDLIST ppidl;
        if (SHGetSpecialFolderLocation(GetSafeHwnd(), CSIDL_BITBUCKET,
                                       &ppidl) != NOERROR) {
            return TRUE;
        }

        shell_tree_.SelectPath(ppidl);

        CMFCShellListCtrl* pList = shell_tree_.GetRelatedList();
        ASSERT_VALID(pList);

        pList->Refresh();
    }

    return TRUE;
}

void CFileView::OnDestroy() {
    CDockablePane::OnDestroy();

    if (recycle_bin_ != 0) {
        SHChangeNotifyDeregister(recycle_bin_);
    }
}