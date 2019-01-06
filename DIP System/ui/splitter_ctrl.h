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

#include <afxwin.h>
#include "base/basictypes.h"

class CSplitterCtrl : public CWnd{
public:
    enum SplitingDirection{
        SD_Horizontal,
        SD_Vertical
    };

    CSplitterCtrl();
    void SetPanel(CWnd* wnd0, CWnd* wnd2);
    
    virtual BOOL Create(DWORD dwStyle,
                        const RECT& rect,
                        CWnd* pParentWnd, 
                        UINT nID);

protected:
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnPaint();

    DECLARE_MESSAGE_MAP()

private:
    void ResizeWindows(int CxBarAt, int len );
    void ResizePanels();

    SplitingDirection direction_;
    CWnd* wnd0_;
    CWnd* wnd1_;

    CRect spliter_bar_;

    HCURSOR spliter_cursor_;
    CPoint  old_point_;
    bool    is_mouse_down_;
    bool is_draging_; 

    static const int bar_thickness_ = 8;

    DISALLOW_COPY_AND_ASSIGN(CSplitterCtrl);
};
