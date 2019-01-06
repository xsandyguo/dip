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

#include "flow/data_line.h"

#include "base/debug.h"
#include "flow/inout_port.h"
#include "runtime/framework.h"

using namespace dip;

void DataLine::SetConnectStart(IConnector* start){
    __super::SetConnectStart(start);
    DoConnect();
}

void DataLine::SetConnectEnd(IConnector* end){
    __super::SetConnectEnd(end);
    DoConnect();
}

void DataLine::DoConnect(){
    InOutPort* start_port = dynamic_cast<InOutPort*>(GetConnectStart());
    InOutPort* end_port = dynamic_cast<InOutPort*>(GetConnectEnd());

    if(!start_port || !end_port){
        return;
    }

    IConnectable* source = start_port->GetFilter();
    IConnectable* target = end_port->GetFilter();
    target->Connect(source, start_port->Index(), end_port->Index());
}

void DataLine::DoDisconnect(){
    InOutPort* start_port = dynamic_cast<InOutPort*>(GetConnectStart());
    InOutPort* end_port   = dynamic_cast<InOutPort*>(GetConnectEnd());

    if(!start_port || !end_port){
        return;
    }

    IConnectable* source = start_port->GetFilter();
    IConnectable* target = end_port->GetFilter();
    target->Disconnect(source, start_port->Index(), end_port->Index());
}

void DataLine::DisconnectStart(){
    DoDisconnect();

    __super::DisconnectStart();
}

bool DataLine::CanConnect(IConnector* start, IConnector* end){
    bool can = __super::CanConnect(start, end);
    if(!can){
        return can;
    }

    InOutPort* start_port = dynamic_cast<InOutPort*>(start);
    InOutPort* end_port   = dynamic_cast<InOutPort*>(end);

    ASSERT1(start_port);
    ASSERT1(end_port);

    IConnectable* source = start_port->GetFilter();
    IConnectable* target = end_port->GetFilter();

    return target->CanConnect(source, start_port->Index(), end_port->Index());
}

void DataLine::DisconnectEnd(){
    __super::DisconnectEnd();
    DoDisconnect();
}

DataLine* DataLine::Clone(){
    return new DataLine();
}
