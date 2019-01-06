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

#include "framework/framework.h"

const Rect& kEmptyRect = Rect(0, 0, 0, 0);

KeyEventArgs::KeyEventArgs(IView* source, UINT nChar, UINT nRepCnt, UINT nFlags){
    this->source_ = source;
    this->char_ = nChar;
    this->repate_count_ = nRepCnt;
    this->flags_ = nFlags;
}

KeyEventArgs::~KeyEventArgs(){

}

IView* KeyEventArgs::GetSource(){
    return this->source_;
}

UINT KeyEventArgs::GetChar(){
    return this->char_;
}

UINT KeyEventArgs::GetRepCount(){
    return this->repate_count_;
}

UINT KeyEventArgs::GetFlags(){
    return this->flags_;
}


MouseEventArgs::MouseEventArgs(IView* source,
    UINT nFlags,
    const Point& point,
    MouseButton button){
        this->flags_ = nFlags;
        this->source_ = source;
        this->point_ = point;
        this->button_ = button;
}
MouseEventArgs::~MouseEventArgs(){
}

IView* MouseEventArgs::GetSource(){
    return this->source_;
}

UINT MouseEventArgs::GetFlags(){
    return this->flags_;
}

Point MouseEventArgs::GetPoint(){
    return this->point_;
}

int MouseEventArgs::GetX(){
    return this->point_.X;
}

int MouseEventArgs::GetY(){
    return this->point_.Y;
}

MouseButton MouseEventArgs::GetButton(){
    return this->button_;
}


HandleEventArgs::HandleEventArgs(IHandle* handle, const Rect& rect){
    this->handle_ = handle;
    this->invalidated_area_ = rect;
}

IHandle* HandleEventArgs::GetHandle(){
    return this->handle_;
}

Rect HandleEventArgs::GetInvalidatedArea(){
    return this->invalidated_area_;
}

ToolEventArgs::ToolEventArgs(ITool * tool, IView* view, Rect invalidatedArea)
    :tool_(tool),
    view_(view),
    invalidated_area_(invalidatedArea){
}

Rect ToolEventArgs::GetInvalidatedArea(){
    return this->invalidated_area_;
}

ITool* ToolEventArgs::get_tool(){
    return tool_;
}

IView* ToolEventArgs::get_view(){
    return view_;
}


FigureEventArgs::FigureEventArgs()
    :figure_(NULL),
    invalidated_area_(kEmptyRect){

}

FigureEventArgs::FigureEventArgs(IFigure* source, Rect invalidatedArea){
    this->figure_ = source;
    this->invalidated_area_ = invalidatedArea;
}

IFigure* FigureEventArgs::GetFigure(){
    return this->figure_;
}

Rect FigureEventArgs::GetInvalidatedArea(){
    return invalidated_area_;
}

FigureEventArgs::~FigureEventArgs(){
}


FigureSelectionEventArgs::FigureSelectionEventArgs(IView* source,
    vector<IFigure*>* oldSelection, 
    vector<IFigure*>* newSelection):
source_(source){

    if(newSelection){
        new_selection_.assign(newSelection->begin(), newSelection->end());
    }

    if(oldSelection){
        old_selection_.assign(oldSelection->begin(), oldSelection->end());
    }
}

FigureSelectionEventArgs::~FigureSelectionEventArgs(){

}

vector<IFigure*>& FigureSelectionEventArgs::GetNewSelection(){
    return this->new_selection_;
}

vector<IFigure*>& FigureSelectionEventArgs::GetOldSelection(){
    return this->old_selection_;
}

IView* FigureSelectionEventArgs::GetSource(){
    return this->source_;
}


ConnectorEventArgs::ConnectorEventArgs(IConnector* connector, Rect invalidatedArea)
    :source_(connector),
    invalidated_area_(invalidatedArea){
        invalidated_area_ = invalidatedArea;
}

IConnector* ConnectorEventArgs::GetSource(){
    return source_;
}


DrawingEventArgs::DrawingEventArgs(IDrawing* container, 
                                       IFigure* child, 
                                       Rect rc, 
                                       int z_index):
                            z_index_(z_index),
                            invalidated_area_(rc),
                            container_(container),
                            figure_(child){
}
DrawingEventArgs::~DrawingEventArgs(){

}

Rect DrawingEventArgs::GetInvalidatedArea(){
    return this->invalidated_area_;
}

IFigure* DrawingEventArgs::GetTargetFigure(){
    return this->figure_;
}

IDrawing* DrawingEventArgs::GetContainer(){
    return this->container_;
}

int DrawingEventArgs::GetIndex(){
    return this->z_index_;
}
