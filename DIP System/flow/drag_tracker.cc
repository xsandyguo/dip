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

#include "flow/drag_tracker.h"

#include "base/debug.h"
#include "base/prefs_const.h"

DragTracker::DragTracker():
dragged_figure_(NULL),
ghost_bounds_(kEmptyRect){
}

DragTracker::~DragTracker(){

}

void DragTracker::SetDraggedFigure(IFigure *figure){
    ASSERT1(figure);
    this->dragged_figure_ = figure;
}

void DragTracker::MouseUp(MouseEventArgs& evt){
    __super::MouseUp(evt);

    Point current = Locate(evt.GetPoint());	
    Point offset(current.X - get_anchor().X, current.Y - get_anchor().Y);

    if(!(offset.X == 0 && offset.Y == 0)){       
        vector<IFigure*>& selected = get_view()->GetSelectedFigures();
        for(vector<IFigure*>::iterator itor = selected.begin();
                itor != selected.end(); ++itor){			 
            if ((*itor)->CanMove()){
                Rect rc = (*itor)->GetBounds();                
                rc.Offset(offset.X, offset.Y);

                (*itor)->SetBounds(rc); 
            }  
        } 
    }

    ClearGhost();
    evt.GetSource()->ExtendViewportTo(current);

    OnToolDone();
}

void DragTracker::MouseDown(MouseEventArgs& evt){
    __super::MouseDown(evt);
    this->old_point_ = get_anchor();
    this->ghost_bounds_ = dragged_figure_->GetBounds();
}

void DragTracker::MouseMove(MouseEventArgs& evt){
    __super::MouseMove(evt);
    Point current = Locate(evt.GetPoint());	

    evt.GetSource()->ExtendViewportTo(current);

    UpdateCursor(current);

    OnAreaInvalidated(ghost_bounds_);
    Point offset(current.X - old_point_.X, current.Y - old_point_.Y);

    ghost_bounds_.X += offset.X;
    ghost_bounds_.Y += offset.Y;

    old_point_ = current;
    OnAreaInvalidated(ghost_bounds_);
}

void DragTracker::ClearGhost(){
    Rect old = ghost_bounds_;
    ghost_bounds_ = kEmptyRect;
    OnAreaInvalidated(old);
}


void DragTracker::Draw(Graphics& g){
    if (!ghost_bounds_.IsEmptyArea()){		 	
        Pen pen(kDragShadowColor);
        pen.SetDashStyle(DashStyleDot);

        g.DrawRectangle(&pen,
            ghost_bounds_.X,
            ghost_bounds_.Y,
            ghost_bounds_.Width - 1,
            ghost_bounds_.Height - 1);
    }
}

void DragTracker::UpdateCursor(const Point& point){
    SetCursor(AfxGetApp()->LoadStandardCursor((LPCTSTR)IDC_SIZEALL));
}
