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

#include "flow/node_figure.h"

#include "io/serialize_const.h"
#include "base/prefs_const.h"
#include "flow/change_connection_end_handle.h"
#include "flow/change_connection_start_handle.h"
#include "flow/geometry.h"


NodeFigure::NodeFigure(void)
    :bounds_(0, 0, kNodeFigureSize.Width, kNodeFigureSize.Height){
}


NodeFigure::~NodeFigure(void){
}

void NodeFigure::Draw(Graphics& g){
    DrawBorder(g);
    DrawConnector(g);
}


void NodeFigure::DrawBorder(Graphics& g){
    SolidBrush sb(IsSelected() ? kNodeSelectedFillColor : kNodeFillColor );
    
    Rect bounds = bounds_;

    Pen pen(IsSelected() ? kNodeSelectedBorderColor : kNodeBorderColor);
    pen.SetWidth(2.0);
    pen.SetAlignment(PenAlignmentInset); 
    
    GraphicsPath path;
    Geometry::MakeRoundPath(bounds, 15, &path);   
    g.FillPath(&sb, &path); 
    g.DrawPath(&pen, &path);  
}

void NodeFigure::DrawConnector(Graphics& g){
    for (vector<IConnector*>::iterator itor = GetConnectors().begin();
        itor != GetConnectors().end();
        ++itor) {
        (*itor)->Draw(g);
    }
}

void NodeFigure::BasicSetBounds(const Point& orgin, const Point& corner){
    bounds_ = Rect(orgin, Size(corner.X - orgin.X, corner.Y - orgin.Y));
}

Rect NodeFigure::GetBounds(){
    return bounds_;
}

Rect NodeFigure::GetDrawingArea(){
    Rect bounds = GetBounds();

    for (vector<IConnector*>::iterator itor = GetConnectors().begin();
        itor != GetConnectors().end();
        ++itor) {
        Rect::Union(bounds, bounds, (*itor)->GetBounds());
    }
    bounds.Inflate(1, 1);
    return bounds;
}

bool NodeFigure::Contains(const Point& p){
    return bounds_.Contains(p) || ConnectorAt(p) != NULL;
}

IConnector* NodeFigure::ConnectorAt(const Point& p){
    for (vector<IConnector*>::iterator itor = GetConnectors().begin();
        itor != GetConnectors().end();
        ++itor) {
            if((*itor)->Contains(p)){
                return *itor;
            }
    }

    return NULL;
}

void NodeFigure::AreaInvalidated(ConnectorEventArgs& evt){
    OnAreaInvalidated(evt.GetInvalidatedArea());
}

void NodeFigure::Write(IDOMOutput& out){
    out.AddAttribute(kLocationX, bounds_.X);
    out.AddAttribute(kLocationY, bounds_.Y);
}

void NodeFigure::Read(IDOMInput& in){
    bounds_.X = in.GetAttribute(kLocationX, 0);
    bounds_.Y = in.GetAttribute(kLocationY, 0);
}