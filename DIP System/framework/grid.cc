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

#include "framework/grid.h"

#include "base/debug.h"
#include "base/prefs_const.h"

Grid::Grid(IView* view):view_(view), enable_(true){
    ASSERT1(view);
}


void Grid::Draw(Graphics& g){
    if(!enable_){
        return;
    }

    CRect client;
    view_->GetWindow().GetClientRect(&client);

    Rect rect;
    g.GetClipBounds(&rect);    
    Rect::Union(rect, rect, 
        Rect(client.left, client.top, client.Width(), client.Height()));

    Pen pen(kGridColor);
    pen.SetDashStyle(DashStyleDot);

    Point p(0, 0);
    int ver_count = rect.Width / kGridSize.Width + 1;
    int hro_count = rect.Height / kGridSize.Height + 1;
    for(int i = 1; i < ver_count; ++i){
        p.X = i * kGridSize.Width;
        g.DrawLine(&pen, p.X, p.Y, p.X, rect.Height);
    }

    p = Point(0, 0);
    for(int j = 1;  j < hro_count; ++j){
        p.Y = j * kGridSize.Height;
        g.DrawLine(&pen, p.X, p.Y, rect.Width, p.Y);
    }
}

bool Grid::IsEnabled(){
    return enable_;
}

void Grid::Enable(bool enable){
    enable_ = enable;

    view_->Invalidate();
}

Point Grid::Locate(const Point& p){
    if(!IsEnabled()){
        return p;
    }

    return Point((p.X / kGridSize.Width) * kGridSize.Width,
                 (p.Y / kGridSize.Height) * kGridSize.Height);
}