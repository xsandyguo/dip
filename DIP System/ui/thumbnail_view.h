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
#include "ui/view_tree.h"

class CThumbnailToolBar : public CMFCToolBar {
    virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler){
        CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
    }

    virtual BOOL AllowShowOnList() const { return FALSE; }
private:
};

class CThumbnailView : public CDockablePane {
public:
    CThumbnailView();
    virtual ~CThumbnailView();

    void AdjustLayout();
    void OnChangeVisualStyle();
    virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
    CThumbnailToolBar toolbar_;
    CViewTree cache_files_view_;
    CImageList image_list_;
    UINT current_sort_;

    void FillTree();
protected:

    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    afx_msg void OnClassAddMemberFunction();
    afx_msg void OnClassAddMemberVariable();
    afx_msg void OnClassDefinition();
    afx_msg void OnClassProperties();
    afx_msg void OnRefreshFolder();
    afx_msg void OnNewFolder();
    afx_msg void OnPaint();
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg LRESULT OnChangeActiveTab(WPARAM, LPARAM);
    afx_msg void OnSort(UINT id);
    afx_msg void OnUpdateSort(CCmdUI* pCmdUI);

    DECLARE_MESSAGE_MAP()

private:
    DISALLOW_EVIL_CONSTRUCTORS(CThumbnailView);
};

