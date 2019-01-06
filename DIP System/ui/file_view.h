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

#pragma once

#include <afxcontrolbars.h>
#include "base/basictypes.h"
#include "ui/splitter_ctrl.h"

class CMyShellListCtrl : public CMFCShellListCtrl{
public:
    CMyShellListCtrl(){}

protected:
    void DoDefault(int iItem);
    HRESULT EnumObjects(LPSHELLFOLDER pParentFolder, LPITEMIDLIST pidlParent);

private:
    bool OpenFile(const CString& file_path);

    DISALLOW_EVIL_CONSTRUCTORS(CMyShellListCtrl);
};

class CFileView : public CDockablePane {
public:
    CFileView();
    virtual ~CFileView();

protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg	LRESULT OnNotifyRecycleBin(WPARAM wParam, LPARAM lParam);
    afx_msg void OnDestroy();
    
    DECLARE_MESSAGE_MAP()
    
private:
    ULONG recycle_bin_;
    CMFCShellTreeCtrl shell_tree_;
    CMyShellListCtrl shell_list_;
    CSplitterCtrl splitter_;
    
    DISALLOW_EVIL_CONSTRUCTORS(CFileView);
};

