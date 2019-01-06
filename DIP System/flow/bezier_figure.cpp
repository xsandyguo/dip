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

#include "flow/bezier_figure.h"

#include <math.h>

#include "base/prefs_const.h"
#include "flow/change_connection_start_handle.h"
#include "flow/change_connection_end_handle.h"
#include "flow/geometry.h"


BezierFigure::BezierFigure(){}


bool BezierFigure::Contains(const Point& p){
    Point start(GetStartPoint());
    Point end(GetEndPoint());

    Point start_ctl_point(start.X + 40, start.Y);
    Point end_ctl_point(end.X - 40, end.Y);

    return Geometry::IsPointOnBezier(p, start, start_ctl_point, end_ctl_point, end);
}

Rect BezierFigure::GetBounds(){
    Rect bounds(__super::GetBounds());
    bounds.Inflate(8, 8);

    return bounds;
}

void BezierFigure::Draw(Graphics& g){
    Pen pen(IsSelected() ?  kLineSelectionColor: kLineColor);
    AdjustableArrowCap end_cap(6, 4, true);

    pen.SetCustomEndCap(&end_cap);

    GraphicsContainer memo =  g.BeginContainer();
    g.SetSmoothingMode(SmoothingModeHighQuality);

    Point start(GetStartPoint());
    Point end(GetEndPoint());

    Point start_ctl_point(start.X + 40, start.Y);
    Point end_ctl_point(end.X - 40, end.Y);
    g.DrawBezier(&pen, start, start_ctl_point, end_ctl_point, end);

    g.EndContainer(memo);
}

void  BezierFigure::CreateHandles(vector<IHandle*>* handles) {        
    IFigure* owner = static_cast<IConnectionFigure*>(this);

    handles->push_back(new ChangeConnectionStartHandle(owner));
    handles->push_back(new ChangeConnectionEndHandle(owner));
}
