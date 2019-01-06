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

#include "framework/abstract_tool.h"

#include <afxwin.h>
AbstractTool::AbstractTool():
view_(NULL),
is_dragging_(false){
}

AbstractTool::~AbstractTool(){
}

void AbstractTool::AddToolObserver(IToolObserver* observer){
    AddObserver(&observers_, observer);
}

void AbstractTool::RemoveToolObserver(IToolObserver* observer){
    RemoveObserver(&observers_, observer);
}

void AbstractTool::Deactive(IView* editor){
    view_ = editor;
}

void AbstractTool::Active(IView* editor){
    view_ = editor;
}

void AbstractTool::OnToolDone(){
    ToolEventArgs evt(this, get_view(), kEmptyRect);
    ObserverIterCall(observers_, &IToolObserver::ToolDone, evt);
}

void AbstractTool::OnToolStart(){
    ToolEventArgs evt(this, get_view(), kEmptyRect);
    ObserverIterCall(observers_, &IToolObserver::ToolStart, evt);
}

IView* AbstractTool::get_view(){
    return view_;
}

IDrawing* AbstractTool::get_drawing(){
    return get_view()->GetDrawing();
}

const Point& AbstractTool::get_anchor() const{
    return anchor_;
}

void AbstractTool::KeyUp(KeyEventArgs& evt){

}

void AbstractTool::KeyDown(KeyEventArgs& evt){
    
}

void AbstractTool::MouseMove(MouseEventArgs& evt){   
    
}

void AbstractTool::MouseDown(MouseEventArgs& evt){  
    this->anchor_ = evt.GetPoint();
    this->is_dragging_ = true; 
    OnToolStart();
}

void AbstractTool::MouseUp(MouseEventArgs& evt){ 
    this->is_dragging_ = false;
}  

void AbstractTool::MouseDblClk(MouseEventArgs& evt){

}

bool AbstractTool::IsDragging(){
    return is_dragging_;
}

void AbstractTool::UpdateCursor(const Point& point){
    IView* view = get_view();
    IHandle* handle = view->FindHandle(point);
    if (handle){
        ::SetCursor(handle->GetCursor());	
    } else{
        ::SetCursor(AfxGetApp()->LoadStandardCursor((LPCTSTR)IDC_ARROW));	
    }
}

void AbstractTool::OnAreaInvalidated(Rect invalidatedArea){
    ToolEventArgs evt(this, get_view(), invalidatedArea);
    ObserverIterCall(observers_, &IToolObserver::AreaInvalidated, evt);
}


Point AbstractTool::Locate(const Point& p){
    return get_view()->GetLocator().Locate(p);
}