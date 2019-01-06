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


#include "framework/abstract_figure.h"

AbstractFigure::AbstractFigure(void)
:is_selected_(false),
is_removable_(true),
can_resizable_(false),
is_movable_(true),
is_visible_(true){
}

AbstractFigure::~AbstractFigure(void){
}

bool AbstractFigure::IsRemovable(){
    return this->is_removable_;
}

bool AbstractFigure::CanConnect(){
    return true;
}

IConnector* AbstractFigure::ConnectorAt(const Point& p){
    return NULL;
}

void AbstractFigure::GetToolTipAt(std::wstring *content, Point* p){
}

bool AbstractFigure::IsVisible(){
    return is_visible_;
}

bool AbstractFigure::IsSelected(){
    return is_selected_;
}

int AbstractFigure::GetLayer(){
    return 0;
}

bool AbstractFigure::Contains(const Point& p){ 	
    return this->GetBounds().Contains(p) == TRUE 
        || ConnectorAt(p) != NULL;
}

void AbstractFigure::AddFigureObserver(IFigureObserver* observer){
    AddObserver(&observers_, observer);
}

void AbstractFigure::RemoveFigureObserver(IFigureObserver* observer){
    RemoveObserver(&observers_, observer);
} 

void AbstractFigure::OnAttributeChanged(FigureEventArgs& evt){
    ObserverIterCall(observers_, &IFigureObserver::AttributeChanged, evt);
}

bool AbstractFigure::CanMove(){
    return this->is_movable_;
}

void AbstractFigure::SetCanMove(bool val){
     this->is_movable_ = val;
}

void AbstractFigure::SetCanResize(bool val){
    this->can_resizable_ = val;
}

void AbstractFigure::SetVisible(bool val){
    is_visible_ = val;
    Changed();
}

void AbstractFigure::SetRemovale(bool val){
    this->is_removable_ = val;
}

bool AbstractFigure::CanResize(){
    return can_resizable_;
}

void AbstractFigure::SetSelected(bool val){
    this->is_selected_ = val; 
    Changed();
}

void AbstractFigure::SetBounds(const Rect& rect){   	
    SetBounds(Point(rect.X, rect.Y), Point(rect.X + rect.Width, rect.Y + rect.Height)); 
}

void AbstractFigure::SetPosition(const Point& pos){
    SetBounds(pos, Point(pos.X + GetBounds().Width, pos.Y + GetBounds().Height)); 
}

void AbstractFigure::SetBounds(const Point& start, const Point& end){
    WillChange();
    BasicSetBounds(start, end);    
    Changed();
}

Rect AbstractFigure::GetDrawingArea(){
    return GetBounds();
}

void AbstractFigure::CreateHandles(vector<IHandle*>* handles){
}

void AbstractFigure::WillChange(){
    OnAreaInvalidated();
}

void AbstractFigure::Changed(){
    FigureEventArgs evt(this, GetDrawingArea());
    OnAreaInvalidated(evt);
    OnFigureChanged(evt);
}

void AbstractFigure::OnAreaInvalidated(){
    OnAreaInvalidated(GetDrawingArea());
}

void AbstractFigure::OnAreaInvalidated(Rect rect){ 
    OnAreaInvalidated(FigureEventArgs(this, rect));
}

void AbstractFigure::OnAreaInvalidated(FigureEventArgs& evt){
    ObserverIterCall(observers_, &IFigureObserver::AreaInvalidated, evt);
}

void AbstractFigure::OnFigureChanged(){
    OnFigureChanged(GetDrawingArea());
}

void AbstractFigure::OnFigureChanged(Rect rect){
    OnFigureChanged(FigureEventArgs(this, rect));
}

void AbstractFigure::OnFigureChanged(FigureEventArgs& evt){
    ObserverIterCall(observers_, &IFigureObserver::FigureChanged, evt);
}

void AbstractFigure::OnRequestRemove(){
    FigureEventArgs evt(this, GetDrawingArea());

    OnRequestRemove(evt);
}

void AbstractFigure::OnRequestRemove(FigureEventArgs& evt){
    ObserverIterCall(observers_, &IFigureObserver::RequestRemove, evt);
}

void AbstractFigure::Invalidate(){
    OnAreaInvalidated();
}

void AbstractFigure::Release(){
    OnRequestRemove();

    delete this;
}