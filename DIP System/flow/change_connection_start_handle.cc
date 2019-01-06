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

#include "flow/change_connection_start_handle.h"
#include "base/prefs_const.h"

ChangeConnectionStartHandle::ChangeConnectionStartHandle(IFigure* owner)
:ChangeConnectionHandle(owner){
}

IConnector* ChangeConnectionStartHandle::Target() {
    return connection_->GetConnectStart();
}

void ChangeConnectionStartHandle::Disconnect() {
    connection_->DisconnectStart();
}

void ChangeConnectionStartHandle::Connect(IConnector* connector) {
    connection_->SetConnectStart(connector);
}

void ChangeConnectionStartHandle::SetPoint(const Point& p) {
    connection_->SetStartPoint(p);
}

Point ChangeConnectionStartHandle::Locate() {
    Point center = connection_->GetStartPoint();
    center.X -= kHandleSize.Width / 2;
    center.Y -= kHandleSize.Height / 2;
    return center;
}

bool ChangeConnectionStartHandle::CanConnect(IConnector* connector){
    return connection_->CanConnect(connector, connection_->GetConnectEnd());
}
