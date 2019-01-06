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

#include "ui/splitter_ctrl.h"

#include "resource.h"
BEGIN_MESSAGE_MAP(CSplitterCtrl, CWnd)
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_SIZE()
    ON_WM_CREATE()
    ON_WM_PAINT()
END_MESSAGE_MAP()

CSplitterCtrl::CSplitterCtrl()
    :direction_(SD_Vertical),
    wnd0_(NULL),
    wnd1_(NULL),
    spliter_bar_(0, 0, 0, 0),
    is_draging_(false){
    spliter_cursor_ = LoadCursor(NULL, IDC_SIZENS);
}

BOOL CSplitterCtrl::Create(DWORD dwStyle,
                           const RECT& rect,
                           CWnd* pParentWnd,
                           UINT nID){
    return CWnd::Create(afxGlobalData.RegisterWindowClass(_T("dip:SplitterCtrl")),
                        _T(""),
                        dwStyle,
                        rect, 
                        pParentWnd, 
                        nID, 
                        NULL);
}

int CSplitterCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct){
    int ret = CWnd::OnCreate(lpCreateStruct);

    CRect rect;
    GetClientRect(&rect);

    spliter_bar_ = CRect(rect.left, rect.Height() / 2 - bar_thickness_ / 2,
        rect.right, rect.Height() / 2 + bar_thickness_ / 2);

    return ret;
}

void CSplitterCtrl::SetPanel(CWnd* wnd0, CWnd* wnd2) {
    wnd0_ = wnd0;
    wnd1_ = wnd2;
}

void CSplitterCtrl::OnPaint(){
    // TODO:

    CWnd::OnPaint();
}

void CSplitterCtrl::OnMouseMove(UINT nFlags, CPoint point) {
    if (spliter_bar_.PtInRect(point)) {
        SetCursor(spliter_cursor_);
    }

    if(!is_draging_){
        return;
    }
    InvalidateRect(&spliter_bar_);

    CPoint offset(point.x - old_point_.x, point.y - old_point_.y);
    CRect spliter_bar = CRect(spliter_bar_.left, spliter_bar_.top + offset.y, 
                         spliter_bar_.right,
                         spliter_bar_.top + offset.y + bar_thickness_);

    CRect rect;
    GetClientRect(&rect);
    if(spliter_bar.bottom > rect.bottom - bar_thickness_ ||
       spliter_bar.top <= 0){
       return;
    }

    spliter_bar_ = spliter_bar;
    old_point_ = point;

    ResizePanels();
}

void CSplitterCtrl::OnLButtonDown(UINT nFlags, CPoint point) {
    if (spliter_bar_.PtInRect(point) ) {
        is_draging_ = true;
        old_point_ = point;
    }

    SetCapture();
}

void CSplitterCtrl::OnLButtonUp(UINT nFlags, CPoint point) {
    is_draging_ = false;
    ReleaseCapture();
}

void CSplitterCtrl::ResizePanels(){
    if(!wnd0_ || !wnd1_ || !::IsWindow(*wnd0_) || !::IsWindow(*wnd1_)){
        return;
    }

    CRect rect;
    GetClientRect(&rect);

    CRect bound(rect.left, rect.top, rect.right, spliter_bar_.top);
    wnd0_->MoveWindow(&bound);

    bound = CRect(rect.left, spliter_bar_.bottom, rect.right, rect.bottom);
    wnd1_->MoveWindow(&bound);
}

void CSplitterCtrl::OnSize(UINT nType, int cx, int cy) {
    CRect rect;
    GetClientRect(&rect);

    if(spliter_bar_.IsRectEmpty()){
        spliter_bar_ = CRect(rect.left, rect.Height() / 2 - bar_thickness_ / 2,
            rect.right, rect.Height() / 2 + bar_thickness_ / 2);
    }else{
        int top = spliter_bar_.top;
        if(top > rect.bottom - bar_thickness_){
            //top = rect.bottom - bar_thickness_;
        }

        spliter_bar_ = CRect(rect.left, top,
            rect.right, top + bar_thickness_);
    }

    ResizePanels();
}