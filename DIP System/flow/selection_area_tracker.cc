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

#include "flow/selection_area_tracker.h"

#include <algorithm>
#include "base/prefs_const.h"

SelectionAreaTracker::SelectionAreaTracker():
rubber_band_(kEmptyRect){	 
}

SelectionAreaTracker::~SelectionAreaTracker(){
}
 
void SelectionAreaTracker::MouseDown(MouseEventArgs& evt){
    __super::MouseDown(evt);
    ClearRubberBand();
}

void SelectionAreaTracker::MouseUp(MouseEventArgs& evt){
    __super::MouseUp(evt);

    IView* view = get_view();
    view->ClearSelection();
    
    if (!rubber_band_.IsEmptyArea()){	
        IDrawing* container = view->GetDrawing();

        const vector<IFigure*>& children = container->GetChildren();
        vector<IFigure*>::const_iterator itor = children.begin();
        while(itor != children.end()){
            if (rubber_band_.Contains((*itor)->GetBounds())){
                view->AddToSelection(*itor);
            }
            itor ++;
        } 
    }
    ClearRubberBand();

    OnToolDone();
}

void SelectionAreaTracker::MouseMove(MouseEventArgs& evt){
    __super::MouseMove(evt);

    UpdateCursor(evt.GetPoint());
    if (!IsDragging()){
        return;
    }

    Rect invalidatedArea = rubber_band_;
    rubber_band_.X = std::min<int>(get_anchor().X, evt.GetX());
    rubber_band_.Y = std::min<int>(get_anchor().Y, evt.GetY());
    rubber_band_.Width = std::abs(get_anchor().X - evt.GetX());
    rubber_band_.Height = std::abs(get_anchor().Y - evt.GetY());

    if (invalidatedArea.IsEmptyArea()){
        invalidatedArea = rubber_band_;
    }else{
         Rect::Union(invalidatedArea, invalidatedArea, rubber_band_);
    }

    OnAreaInvalidated(invalidatedArea);
}

void SelectionAreaTracker::Draw(Graphics& g){
    if (rubber_band_.IsEmptyArea()){
        return;
    }

    SolidBrush sb(kSelectionAreaFillColor);
    g.FillRectangle(&sb, rubber_band_);

   Pen pen(kSelectionAreaBorderColor);
    g.DrawRectangle(&pen,
        rubber_band_.X,
        rubber_band_.Y, 
        rubber_band_.Width - 1,
        rubber_band_.Height - 1);
}

void SelectionAreaTracker::ClearRubberBand(){
    if(!rubber_band_.IsEmptyArea()){
        OnAreaInvalidated(rubber_band_);
    }

    rubber_band_ = kEmptyRect;
}
