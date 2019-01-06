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

#include "flow/line_figure.h"

#include "base/prefs_const.h"
#include "flow/geometry.h"
#include "flow/change_connection_start_handle.h"
#include "flow/change_connection_end_handle.h"

LineFigure::LineFigure(){}

bool LineFigure::Contains(const Point& p){
    return Geometry::LineContains(GetStartPoint(), GetEndPoint(), p);
}

void LineFigure::Draw(Graphics& g){
    Pen pen(IsSelected() ?  kLineSelectionColor: kLineColor);
    AdjustableArrowCap end_cap(6, 4, true);

    pen.SetCustomEndCap(&end_cap);

    GraphicsContainer memo =  g.BeginContainer();
    g.SetSmoothingMode(SmoothingModeHighQuality);
    g.DrawLine(&pen, GetStartPoint(), GetEndPoint());

    g.EndContainer(memo);
}


void LineFigure::CreateHandles(vector<IHandle*>* handles) {          
    IFigure* owner = static_cast<IConnectionFigure*>(this);

    handles->push_back(new ChangeConnectionStartHandle(owner));
    handles->push_back(new ChangeConnectionEndHandle(owner));
}
