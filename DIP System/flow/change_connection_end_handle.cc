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

#include "flow/change_connection_end_handle.h"
#include "base/prefs_const.h"

ChangeConnectionEndHandle::ChangeConnectionEndHandle(IFigure* owner)
:ChangeConnectionHandle(owner){
}

IConnector* ChangeConnectionEndHandle::Target() {
    return connection_->GetConnectEnd();
}

void ChangeConnectionEndHandle::Disconnect() {
    connection_->DisconnectEnd();
}

void ChangeConnectionEndHandle::Connect(IConnector* c) {
    connection_->SetConnectEnd(c);
}

void ChangeConnectionEndHandle::SetPoint(const Point& p) {
    connection_->SetEndPoint(p);
}

Point ChangeConnectionEndHandle::Locate() {
    Point center = connection_->GetEndPoint();
    center.X -= kHandleSize.Width / 2;
    center.Y -= kHandleSize.Height / 2;
    return center;
}

bool ChangeConnectionEndHandle::CanConnect(IConnector* connector){
    return connection_->CanConnect(connection_->GetConnectStart(), connector);
}
