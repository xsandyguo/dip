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

#include "framework/abstract_handle.h"

#include <afxwin.h>
#include "base/prefs_const.h"

AbstractHandle::AbstractHandle(IFigure* owner):
    owner_(owner),
    view_(NULL){
    this->owner_->AddFigureObserver(this);
}

AbstractHandle::~AbstractHandle(){
    if (owner_){
        owner_->RemoveFigureObserver(this);
    }
}

IDrawing* AbstractHandle::GetContainer(){
    return view_->GetDrawing();
}

IView* AbstractHandle::GetView(){
    return view_;
}

void AbstractHandle::SetView(IView* view){
    this->view_ = view;
}
void AbstractHandle::AddHandleObserver(IHandleObserver* observer){
    AddObserver(&observers_, observer);
}

void AbstractHandle::RemoveHandleObserver(IHandleObserver* observer){
    RemoveObserver(&observers_, observer);
}

void AbstractHandle::Draw(Graphics& g){
}

HCURSOR AbstractHandle::GetCursor(){
    return  AfxGetApp()->LoadStandardCursor((LPCTSTR)IDC_ARROW);
}

IFigure* AbstractHandle::GetOwner(){
    return this->owner_;
}

Rect AbstractHandle::BasicGetBounds(){
    return Rect(Locate(), kHandleSize);
}

Rect AbstractHandle::GetBounds(){
    if (bounds_.IsEmptyArea()){
        bounds_ = BasicGetBounds();
    }
    return this->bounds_;
}

bool AbstractHandle::Contains(const Point& p){
    return this->GetBounds().Contains(p) == TRUE;
}
 
void AbstractHandle::OnAreaInvalidated(){
    OnAreaInvalidated(GetBounds());
}

void AbstractHandle::Invalidate(){
    bounds_ = kEmptyRect;
}

void AbstractHandle::OnAreaInvalidated(Rect rect){
    HandleEventArgs evt(this, rect);
    ObserverIterCall(observers_, &IHandleObserver::AreaInvalidated, evt);
}

void AbstractHandle::AttributeChanged(FigureEventArgs& e){
}

void AbstractHandle::AreaInvalidated(FigureEventArgs& e){
}
void AbstractHandle::RequestRemove(FigureEventArgs& evt){

}

void AbstractHandle::UpdateBounds(){
    Rect newBounds = BasicGetBounds();
    if (!bounds_.IsEmptyArea() || !newBounds.Equals(bounds_)){		 
        OnAreaInvalidated(bounds_);		 
        bounds_ = newBounds;
        OnAreaInvalidated();
    }
}
void AbstractHandle::FigureChanged(FigureEventArgs& e){
    UpdateBounds();
} 