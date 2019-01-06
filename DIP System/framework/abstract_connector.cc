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

#include "framework/abstract_connector.h"

#include <algorithm>
#include "io/serialize_const.h"
#include "flow/geometry.h"

AbstractConnector::AbstractConnector()
:owner_(NULL),
hint_visible_(false),
is_connected_(false){}                   

AbstractConnector::AbstractConnector(IFigure* owner)
    :owner_(owner),
    hint_visible_(false),
    is_connected_(false) {
}

IFigure* AbstractConnector::Owner() {
    return owner_;
}

Rect AbstractConnector::GetBounds() {
    return Owner()->GetBounds();
}

bool AbstractConnector::Contains(const Point& p) {
    return Owner()->Contains(p);
}

void AbstractConnector::AddConnectorObserver(IConnectorObserver* observer){
    AddObserver(&observers_, observer);
}

void AbstractConnector::RemoveConnectorObserver(IConnectorObserver* observer){
    RemoveObserver(&observers_, observer);
}

void AbstractConnector::Hint(bool visible){
    hint_visible_ = visible;
    OnAreaInvalidated();
}

void AbstractConnector::OnAreaInvalidated(){
    ConnectorEventArgs evt(this, GetBounds());
    ObserverIterCall(observers_, &IConnectorObserver::AreaInvalidated, evt);    
}

void AbstractConnector::SetConnected(bool is_connected ){
    is_connected_ = is_connected;
}

void AbstractConnector::Read(IDOMInput& in){
    in.OpenElement(kOwner);    
    owner_ = (IFigure*) in.ReadObject(0);
    in.CloseElement();
}

void AbstractConnector::Write(IDOMOutput& out){
    out.OpenElement(kOwner);
    out.WriteObject(Owner());
    out.CloseElement();
}