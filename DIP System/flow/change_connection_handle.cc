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

#include "flow/change_connection_handle.h"
#include "base/prefs_const.h"
#include "flow/geometry.h"


ChangeConnectionHandle::ChangeConnectionHandle(IFigure* owner)
:AbstractHandle(owner),
target_(NULL),
connection_(dynamic_cast<IConnectionFigure*>(owner)){
}

IConnector* ChangeConnectionHandle::Source() {
    if (Target() == connection_->GetConnectStart()){
        return connection_->GetConnectEnd();
    }

    return connection_->GetConnectStart();
}

void ChangeConnectionHandle::TrackStart(const Point& p) {
    original_target = Target();
    start_ = p;
    Disconnect();
}

/**
* Finds a new target of the connection.
*/
void ChangeConnectionHandle::TrackStep(const Point& anchor, const Point& lead, UINT nFlag) {   
    IConnector* f = FindConnector(lead);
    // track the figure containing the mouse
    Point p = lead;
    if (f != target_) {
        target_ = f;
        if(target_){
            p  = Geometry::Center(target_->GetBounds());
        }
    }

    SetPoint(p);
}

/**
* Connects the figure to the new target. If there is no
* new target the connection reverts to its original one.
*/
void ChangeConnectionHandle::TrackEnd(const Point& anchor, const Point& lead, UINT nFlag) {
    IConnector* target = FindConnector(lead);
    if (target == NULL){
        target = original_target;
    }

    SetPoint(lead);
    Connect(target);
    connection_->UpdateConnection();
    
    target_ = NULL;
}

IConnector* ChangeConnectionHandle::FindConnector(const Point& p) {
    IConnector* connector = NULL;
    const vector<IFigure*>& figures = GetContainer()->GetChildren();
    vector<IFigure*>::const_reverse_iterator itor = figures.rbegin();
    while (itor != figures.rend()) {
        if((*itor)->Contains(p) && (*itor)->CanConnect()){
            connector = (*itor)->ConnectorAt(p);
            break;
        }

        ++itor;
    }
    
    if(connector){
        return CanConnect(connector) ? connector : NULL;
    }
    return NULL;
}

/**
* Draws this handle.
*/
void ChangeConnectionHandle::Draw(Graphics& g) {
    Rect r = GetBounds();

    SolidBrush sb(kHandleFillColor); 
    g.FillRectangle(&sb, r.X, r.Y, r.Width, r.Height);

    Pen pen(kHandleOutlineColor);
    g.DrawRectangle(&pen, 
                    r.X, 
                    r.Y, 
                    r.Width - 1, 
                    r.Height - 1);
}
