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

#include "flow/delegation_selection_tool.h"

#include <afxwin.h>

#include "base/debug.h"
#include "resource.h"

DelegationSelectionTool::DelegationSelectionTool(ITool* connection_tool)
:SelectionTool(connection_tool), is_showing_tip_(false){
}

DelegationSelectionTool::~DelegationSelectionTool(){
}

void DelegationSelectionTool::Active(IView* view){
    __super::Active(view);
    
    // TODO:create tip window in here

    //reset(tooltip_, CreateTrackingToolTip(get_view()->GetWindow()));
}

void DelegationSelectionTool::Deactive(IView* view){
    __super::Deactive(view);

    //if(valid(tooltip_)){
    //    SendMessage(get(tooltip_), TTM_TRACKACTIVATE, 
    //        (WPARAM)FALSE, (LPARAM)&toolinfo_);
    //}

    //reset(tooltip_);
}

HWND DelegationSelectionTool::CreateTrackingToolTip(HWND parent){
    HWND tooltip = CreateWindowEx(WS_EX_TOPMOST, 
                                  TOOLTIPS_CLASS,
                                  NULL, 
                                  WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP, 
                                  CW_USEDEFAULT, 
                                  CW_USEDEFAULT, 
                                  CW_USEDEFAULT, 
                                  CW_USEDEFAULT, 
                                  parent, 
                                  NULL, 
                                  AfxGetInstanceHandle(),
                                  NULL);
    ASSERT1(tooltip);
    // Set up the tool information. 
    // In this case, the "tool" is the entire parent window.
    toolinfo_.cbSize   = sizeof(TOOLINFO);
    toolinfo_.uFlags   = TTF_IDISHWND | TTF_TRACK | TTF_ABSOLUTE;
    toolinfo_.hwnd     = parent;
    toolinfo_.hinst    = AfxGetInstanceHandle();
    toolinfo_.lpszText = NULL;
    toolinfo_.uId      = (UINT_PTR)parent;

    GetClientRect(parent, &toolinfo_.rect);

    SendMessage(tooltip, TTM_SETMAXTIPWIDTH, 0, 150);
    SendMessage(tooltip, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &toolinfo_);    

    return tooltip;
}

void DelegationSelectionTool::MouseMove(MouseEventArgs& evt){
    // Make sure the mouse has actually moved. The presence of the tooltip 
    // causes Windows to send the message continuously.
    static int oldX, oldY;
    int newX, newY;

    newX = evt.GetX();    
    newY = evt.GetY();
    if (newX == oldX && newY == oldY){
        return;
    }
    oldX = newX;
    oldY = newY;

    __super::MouseMove(evt);

    HideToolTip();

    if(IsDragging()){
        return;
    }

    Point point = evt.GetPoint();
    IFigure* figure = get_drawing()->FindFigure(point);
    if(!figure){
        return;
    }

    ShowToolTip(figure, point);
}

void DelegationSelectionTool::HideToolTip(){
    if(!valid(tooltip_) || !is_showing_tip_){
        return;
    }

    is_showing_tip_ = false;

    SendMessage(get(tooltip_), TTM_TRACKACTIVATE,
        (WPARAM)FALSE, (LPARAM)&toolinfo_);
}

void DelegationSelectionTool::ShowToolTip(IFigure* figure, const Point& point){
    if(!valid(tooltip_)){
        reset(tooltip_, CreateTrackingToolTip(get_view()->GetWindow()));
    }

    std::wstring tip;
    Point pos(point);
    figure->GetToolTipAt(&tip, &pos);
    if(!tip.length()){
        return;
    }

    toolinfo_.lpszText = const_cast<wchar_t*>(tip.c_str());
    SendMessage(get(tooltip_), TTM_TRACKACTIVATE, (WPARAM)TRUE, (LPARAM)&toolinfo_);
    SendMessage(get(tooltip_), TTM_SETTOOLINFO, 0, (LPARAM)&toolinfo_);

    POINT p;
    p.x = pos.X;
    p.y = pos.Y;
    ClientToScreen(get_view()->GetWindow(), &p);
    SendMessage(get(tooltip_), TTM_TRACKPOSITION, 0, (LPARAM)MAKELONG(p.x, p.y));

    is_showing_tip_ = true;
}

void DelegationSelectionTool::MouseUp(MouseEventArgs &evt){
    __super::MouseUp(evt);

    Point point = evt.GetPoint();
    IFigure* figure = get_drawing()->FindFigure(point);

    if (!(evt.GetButton() == MUB_RIGHT
        && figure 
        && figure->IsSelected() 
        && figure->IsVisible())){
            return;
    }

    CMenu pop_menu;
    pop_menu.LoadMenu(IDR_MENU_FLOWBLOCK_CONTEXT);
    CMenu* pop = pop_menu.GetSubMenu(0);
    
    CWnd& win = get_view()->GetWindow();
    CPoint screen_point(point.X, point.Y);
    win.ClientToScreen(&screen_point);
    pop->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, screen_point.x,\
        screen_point.y, &win); 
}
