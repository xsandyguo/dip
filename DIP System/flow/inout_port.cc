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

#include "io/serialize_const.h"
#include "flow/inout_port.h" 
#include "flow/geometry.h"
#include "flow/node_figure.h"
#include "base/prefs_const.h"
#include "flow/process_figure.h"

InOutPort::InOutPort()
:index_(0){
}

InOutPort::InOutPort(ProcessFigure* owner, 
                    bool can_start, 
                    unsigned int index)
    :AbstractConnector(owner), 
    can_start_(can_start), 
    index_(index){    
}

InOutPort::~InOutPort(){
}

void InOutPort::Draw(Graphics& g){
    SolidBrush sb(kPortFillColor);
    Rect bounds = GetBounds();

    Pen pen(Hint() ? kPortOutlineHintColor : kPortOutlinColor);
    if (Hint()){
        pen.SetWidth(1.0);
    }

    if(IsConnected()){
        g.FillRectangle(&sb, bounds);
        g.DrawRectangle(&pen, 
            bounds.X, 
            bounds.Y, 
            bounds.Width - 1, 
            bounds.Height - 1);
    }
    else{
        g.FillEllipse(&sb, bounds);
        g.DrawEllipse(&pen, bounds);
    }
}

bool InOutPort::CanStart(){
    return can_start_;
}

bool InOutPort::Contains(const Point& p){
    return !!GetBounds().Contains(p);
}

Rect InOutPort::GetBounds(){
    return Rect(Locate(), kConnectorSize);
}

dip::IConnectable* InOutPort::GetFilter(){
    ProcessFigure* owner = dynamic_cast<ProcessFigure*>(Owner());
    return &owner->GetFilter();
}

Point InOutPort::Locate(){
    Rect owner_bounds = Owner()->GetBounds();
    int space         = kConnectorSize.Height;
    int count         = GetPortNum();
    int top_margin    = owner_bounds.Height/2 - (space/2 +
                        (count%2 == 0 ? (count-1) : (count/2)*2) * space);

    int offset = (owner_bounds.Y + top_margin) + space * index_ * 2;
    if(can_start_){
        return Point(owner_bounds.GetRight(), offset);
    }else{
        return Point(owner_bounds.X - kConnectorSize.Width, offset);
    }
}

void InOutPort::SetConnected(bool is_connected){
    __super::SetConnected(is_connected);

    OnAreaInvalidated();
}

uint32 InOutPort::GetPortNum(){
    return can_start_ ? GetFilter()->GetNumOutPins() : GetFilter()->GetNumInPins();
}

InOutPort* InOutPort::Clone(){
    return new InOutPort(dynamic_cast<ProcessFigure*>(Owner()), can_start_, index_);
}

void InOutPort::Write(IDOMOutput& out){
    AbstractConnector::Write(out);

    out.AddAttribute(kIndex   , (int)index_);
    out.AddAttribute(kCanStart, can_start_);
}

void InOutPort::Read(IDOMInput& in){
    AbstractConnector::Read(in);

    index_     = in.GetAttribute(kIndex   , 0);
    can_start_ = in.GetAttribute(kCanStart, false);
}