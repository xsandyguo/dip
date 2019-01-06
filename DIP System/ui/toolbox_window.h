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

#include <afxstr.h>
#include <afxdockablepane.h>
#include <GdiPlus.h>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/scoped_ptr.h"
#include "runtime/class.h"
#include "framework/framework.h"
#include "ui/toolbox_ctrl.h"

class CToolBoxToolBar : public CMFCToolBar {
public:
    virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler){
        CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
    }

    virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CToolBoxWindow : public CDockablePane {
    DECLARE_DYNAMIC(CToolBoxWindow)

public:
    CToolBoxWindow();
    virtual ~CToolBoxWindow();
    void AddObserver(IToolboxObserver* observer);
    void RemoveObserver(IToolboxObserver* observer);

    CToolElement* GetCurSel() const { return toolbox_ctrl_.GetCurSel(); }
    void SetCurSel(int nClass, BOOL bRedraw = TRUE) {
        toolbox_ctrl_.SetCurSel(nClass, bRedraw);
    }
    
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
protected:
    static void ChangeCallback(SelectionCallback* callback);
    void InitToolList();
    void CreateObjectGroup(const wstring& group_name, const wstring &type_name);
    void InvalidateDesc();

    DECLARE_MESSAGE_MAP()

private:
    CToolBoxCtrl toolbox_ctrl_;
    CToolBoxToolBar toolbar_;
    CString descbox_str_;
    ObserverCollection observers_;
    SelectionCallback callback_;
    scoped_ptr<Gdiplus::Font> font_;
    scoped_ptr<Gdiplus::StringFormat> format_;
    Gdiplus::RectF desc_bounds_;

    DISALLOW_EVIL_CONSTRUCTORS(CToolBoxWindow);
};


