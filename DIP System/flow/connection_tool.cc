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


#include "flow/connection_tool.h"
#include "flow/geometry.h"

ConnectionTool::ConnectionTool(IConnectionFigure* prototype)
:start_connector_(NULL),
end_connector_(NULL),
connection_(NULL),
tracing_connector_(NULL),
prototype_(prototype){
}

ConnectionTool::~ConnectionTool(){
    if(prototype_){
        delete prototype_;
    }
}

void ConnectionTool::ResetTool(){
    if(start_connector_){
        start_connector_->Hint(false);
    }

    if(end_connector_){
        end_connector_->Hint(false);
    }

    start_connector_   = NULL;
    end_connector_     = NULL;
    connection_        = NULL;
    tracing_connector_ = NULL;
}

IConnector* ConnectionTool::FindConnector(const Point& p, 
                                          IDrawing* drawing){
    IConnector* connector = FindStartConnector(p, drawing);
    if(!connector){
        connector = FindEndConnector(p, drawing);
    }

    return connector;
}

IConnector* ConnectionTool::FindStartConnector(const Point& p, 
                                               IDrawing* drawing){
    IFigure* figure = FindConnectableFigure(p, drawing);
    if(figure){
        IConnector* connector = figure->ConnectorAt(p);
        if(connector && connector->CanStart() && !connector->IsConnected()){
            return connector;
        }
    }

    return NULL;
}

IConnector* ConnectionTool::FindEndConnector(const Point& p, 
                                             IDrawing* drawing){
    IFigure* figure = FindConnectableFigure(p, drawing);
    if(figure){
        IConnector* connector = figure->ConnectorAt(p);
        if(connector && connector->CanEnd() && !connector->IsConnected()){
            return connector;
        }
    }

    return NULL;
}

void ConnectionTool::TrackConnector(const Point& p){
    IConnector* connector = FindConnector(p, get_drawing());
    if(tracing_connector_ != connector){
        if(tracing_connector_){
            tracing_connector_->Hint(false);
        }
    }else{
        return;
    }

    tracing_connector_ = connector;

    if(tracing_connector_){
        IConnector* start = GetStartConnector();
        if (!start || GetConnection()->CanConnect(start, tracing_connector_)){             
            tracing_connector_->Hint(true);
        }
    }
}

void ConnectionTool::MouseMove(MouseEventArgs& e) {
    __super::MouseMove(e);

    Point p = e.GetPoint();
    TrackConnector(p);

    if(IsDragging()){
        if (GetConnection() != NULL) {
            GetConnection()->SetEndPoint(p);
        }
    }
}

void ConnectionTool::MouseDown(MouseEventArgs& e) {
    __super::MouseDown(e);

    ResetTool();
    Point p = e.GetPoint();
    TrackConnector(p);

    SetStartConnector(FindStartConnector(p, get_drawing()));
    if (GetStartConnector() != NULL) {    
        SetConnection(CreateConnection());
        GetConnection()->SetStartPoint(p);
        GetConnection()->SetEndPoint(p);
        get_drawing()->AddFigure(GetConnection());
        GetConnection()->UpdateConnection();
    }
}

void ConnectionTool::MouseUp(MouseEventArgs& e) {
    __super::MouseDown(e);

    ConnectAtIfNecessary(e.GetPoint());

    ResetTool();

    OnToolDone();
}

void ConnectionTool::ConnectAtIfNecessary(const Point& p){
    bool successful = false;
    IFigure* figure = NULL;
    if (GetStartConnector() != NULL) {
        SetEndConnector(FindEndConnector(p, get_drawing()));
        if (GetEndConnector() != NULL && 
            GetConnection()->CanConnect(GetStartConnector(), GetEndConnector())) {            
            GetConnection()->SetConnectStart(GetStartConnector());
            GetConnection()->SetConnectEnd(GetEndConnector());
            GetConnection()->UpdateConnection();
            successful = true;
        }
    }
    if(!successful && GetConnection()){
        IConnectionFigure *connection = GetConnection();
        if(connection->IsSelected()){
            get_view()->RemoveFromSelection(connection);
        }
        get_drawing()->RemoveFigure(connection);
    }
}

IConnectionFigure* ConnectionTool::CreateConnection(){
    return dynamic_cast<IConnectionFigure*>(prototype_->Clone());
}

void ConnectionTool::SetConnection(IConnectionFigure* newConnection) {    
    connection_ = newConnection;
}

IConnectionFigure* ConnectionTool::GetConnection() {
    return connection_;
}

IFigure* ConnectionTool::FindConnectableFigure(const Point& p, IDrawing* drawing) {
    const vector<IFigure*>& figures = drawing->GetChildren();
    vector<IFigure*>::const_reverse_iterator itor = figures.rbegin();
    while (itor != figures.rend()) {
        IFigure* figure = *itor;
        if(figure->Contains(p) && figure->CanConnect()){
            return figure;
        }

        ++itor;
    }

    return NULL;
}

void ConnectionTool::SetStartConnector(IConnector* start) {
    start_connector_ = start;
}

IConnector* ConnectionTool::GetStartConnector() {
    return start_connector_;
}

void ConnectionTool::SetEndConnector(IConnector* end) {
    end_connector_ = end;
}

IConnector* ConnectionTool::GetEndConnector() {
    return end_connector_;
}