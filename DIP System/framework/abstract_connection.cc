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

#include "framework/abstract_connection.h"

#include <base/debug.h>
#include "base/prefs_const.h"

#include "io/serialize_const.h"
#include "flow/geometry.h"

AbstractConnection::AbstractConnection():
start_(NULL),
end_(NULL){
    SetCanResize(true);
    SetCanMove(false);
}

AbstractConnection::~AbstractConnection(){
}

void AbstractConnection::Release(){
    DisconnectStart();
    DisconnectEnd();

    __super::Release();
}

int AbstractConnection::GetLayer(){
    return 1;
}

bool AbstractConnection::CanConnect() {
    return false;
}

Rect AbstractConnection::GetBounds(){
    Rect rect = Geometry::MakeRect(points_);
    rect.Inflate(4, 4);
    return rect;
}

void AbstractConnection::SetConnectStart(IConnector* start) {
    DisconnectStart();

    start_ = start;
    start->SetConnected(true);
    GetStartFigure()->AddFigureObserver(this);
}

void AbstractConnection::SetConnectEnd(IConnector* end) {
    DisconnectEnd();

    end_ = end;    
    end->SetConnected(true);
    GetEndFigure()->AddFigureObserver(this);
}

void AbstractConnection::DisconnectStart() {
    if(start_){
        start_->SetConnected(false);
        GetStartFigure()->RemoveFigureObserver(this);
        start_ = NULL;
    }
}

void AbstractConnection::DisconnectEnd() {
    if(end_){
        end_->SetConnected(false);
        GetEndFigure()->RemoveFigureObserver(this);
        end_ = NULL;
    }
}

IFigure*  AbstractConnection::GetStartFigure() {
    if (GetConnectStart() != NULL)
        return GetConnectStart()->Owner();
    return NULL;
}

IFigure*  AbstractConnection::GetEndFigure() {
    if (GetConnectEnd() != NULL)
        return GetConnectEnd()->Owner();
    return NULL;
}

IConnector*  AbstractConnection::GetConnectStart() {
    return start_;
}

IConnector*  AbstractConnection::GetConnectEnd() {
    return end_;
}

bool  AbstractConnection::CanConnect(IConnector* start, IConnector* end) {
    return start != end 
        && start->CanStart() 
        && end->CanEnd() 
        && start->Owner() != end->Owner();
}

void AbstractConnection::SetStartPoint(const Point& p) {
    AbstractFigure::WillChange();
    if (points_.size() == 0){
        points_.push_back(p);
    }else{
        points_.at(0) = p;
    }

    AbstractFigure::Changed();
}

void AbstractConnection::SetEndPoint(const Point& p) {
    AbstractFigure::WillChange();
    if (points_.size() < 2)
        points_.push_back(p);
    else
        points_.at(points_.size() - 1) = p;
    AbstractFigure::Changed();
}

Point  AbstractConnection::GetStartPoint(){
    return *points_.begin();
}

Point  AbstractConnection::GetEndPoint() {
    return *points_.rbegin();
}

/**
* Updates the connection.
*/
void AbstractConnection::UpdateConnection() {
    if(start_){
        SetStartPoint(Geometry::Center(start_->GetBounds()));
    }

    if(end_){
        SetEndPoint(Geometry::Center(end_->GetBounds()));
    }
}

void AbstractConnection::BasicSetBounds(const Point& orgin, const Point& corner){
    SetStartPoint(orgin);
    SetEndPoint(corner);
}


void AbstractConnection::FigureChanged(FigureEventArgs& evt){
    UpdateConnection();
}

void AbstractConnection::AttributeChanged(FigureEventArgs& evt){
}

void AbstractConnection::AreaInvalidated(FigureEventArgs& evt){    
}

void AbstractConnection::RequestRemove(FigureEventArgs& evt){
    IFigure* figure = evt.GetFigure();
    ASSERT1(figure == GetStartFigure() ||
            figure == GetEndFigure());

    Release();
}

void AbstractConnection::Write(IDOMOutput& out){
    out.OpenElement(kStartConnector);
    out.WriteObject(GetConnectStart());
    out.CloseElement();

    out.OpenElement(kEndConnector);
    out.WriteObject(GetConnectEnd());
    out.CloseElement();
}

void AbstractConnection::Read(IDOMInput& in){
    in.OpenElement(kStartConnector);
    SetConnectStart(dynamic_cast<IConnector*>(in.ReadObject()));
    in.CloseElement();

    in.OpenElement(kEndConnector);
    SetConnectEnd(dynamic_cast<IConnector*>(in.ReadObject()));
    in.CloseElement();

    UpdateConnection();
}