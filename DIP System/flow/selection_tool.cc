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

#include "flow/selection_tool.h"

#include <windows.h>
#include "base/debug.h"

SelectionTool::SelectionTool(ITool* connection_tool)
:tracker_(NULL){ 
    ASSERT1(connection_tool);

    tracker_handler_.reset(new InnerEventHandler(this));
    connection_tool_.reset(connection_tool);
}

SelectionTool::~SelectionTool(){
}

void SelectionTool::KeyUp(KeyEventArgs& evt){
    __super::KeyUp(evt);    
}

void SelectionTool::KeyDown(KeyEventArgs& evt){
    __super::KeyDown(evt);

    OnMoveByKey(evt);
    if(evt.GetChar() == VK_DELETE){
        IEditableComponent* editableComponent = get_view()->GetEditableComponent();
        if(editableComponent){
            editableComponent->DoDelete(); 
        }
    }
}

void SelectionTool::MouseMove(MouseEventArgs& evt){
    __super::MouseMove(evt); 
    if(this->tracker_){
        this->tracker_->MouseMove(evt);
    }
}

ITool* SelectionTool::FindTracker(MouseEventArgs& evt){
    Point point     = evt.GetPoint();
    IView* view     = get_view();
    IHandle* handle = view->FindHandle(point);

    if (handle){
        return GetHandleTracker(handle);
    }    
    
    if(CanConnectAt(point)){
        return GetConnectionTool();
    }        
        
    IFigure* figure = get_drawing()->FindFigure(point);
    if (figure && figure->IsVisible()){ 
        if(!figure->IsSelected()){
            if(!(evt.GetFlags() & MK_SHIFT)){                    
                view->ClearSelection(); 
            }
            view->AddToSelection(figure);
        }

        return figure->CanMove() ? GetDragTracker(figure) : NULL;
    }
             
    return GetSelectionAreaTracker();   
}

void SelectionTool::MouseDown(MouseEventArgs& evt){	
    __super::MouseDown(evt);

    if (evt.GetButton() == MUB_RIGHT) 
        return;

    ITool* tracker = FindTracker(evt);
    set_tracker(tracker);
    if(tracker){
        tracker->MouseDown(evt);
    }
}

void SelectionTool::MouseUp(MouseEventArgs& evt){	
    __super::MouseUp(evt);  

    if(this->tracker_){
        this->tracker_->MouseUp(evt);
    }else{
        OnToolDone();
    }
} 

bool SelectionTool::CanConnectAt(const Point& p){
    IFigure* figure = get_drawing()->FindFigure(p);

    if(figure && figure->CanConnect()){
        IConnector* connector = figure->ConnectorAt(p);
        if(connector && connector->CanStart() && !connector->IsConnected()){
            return true;
        }
    }

    return false;    
}

ITool* SelectionTool::GetDragTracker(IFigure* figure){
    if (!drag_tracker_.get()){
        drag_tracker_.reset(new DragTracker());
        drag_tracker_->AddToolObserver(tracker_handler_.get());
    }
    drag_tracker_->SetDraggedFigure(figure);
    return drag_tracker_.get();
}

ITool* SelectionTool::GetHandleTracker(IHandle* handle){
    if (!this->handle_tracker_.get()){
        handle_tracker_.reset(new HandleTracker());
        handle_tracker_->AddToolObserver(tracker_handler_.get());
    }
    this->handle_tracker_->SetHandle(handle);
    return this->handle_tracker_.get();
}

ITool* SelectionTool::GetConnectionTool(){
    return this->connection_tool_.get();
}

ITool* SelectionTool::GetSelectionAreaTracker(){
    if (this->selection_area_tracker_ == NULL){
        selection_area_tracker_.reset(new SelectionAreaTracker());
        selection_area_tracker_->AddToolObserver(tracker_handler_.get());
    }
    return selection_area_tracker_.get();
}

ITool* SelectionTool::get_tracker(){
    return tracker_;
}

void SelectionTool::set_tracker(ITool* tracker){
    if (tracker == tracker_){
        return; 
    }
     
    if (tracker_){				
        tracker_->Deactive(get_view());
        tracker_->RemoveToolObserver(tracker_handler_.get());
    }
    this->tracker_ = tracker;
     
    if (tracker_){	
        tracker_->Active(get_view());
        tracker_->AddToolObserver(tracker_handler_.get());
    }
}

/// 响应方向键消息并作对应的移动
void SelectionTool::OnMoveByKey(KeyEventArgs& evt){
    Point offset(0, 0);
    switch(evt.GetChar()){
    case VK_DOWN:
        offset.Y++;	
        break;
    case VK_UP:
        offset.Y--; 
        break;
    case VK_RIGHT:
        offset.X++; 
        break;
    case VK_LEFT:
        offset.X--; 
        break; 
    }  	

    if(offset.X != 0 || offset.Y != 0){
        vector<IFigure*>& selected = get_view()->GetSelectedFigures();
        for(vector<IFigure*>::iterator itor = selected.begin();
            itor != selected.end(); ++itor){
            if ((*itor)->CanMove()){
                Rect rc = (*itor)->GetBounds();				
                rc.Offset(offset);			
                (*itor)->SetBounds(rc);
            }
        }
    }
}

void SelectionTool::Draw(Graphics& g){
    if (tracker_){
        this->tracker_->Draw(g);
    } 
}

SelectionTool::InnerEventHandler::InnerEventHandler(SelectionTool* tool){
    this->selection_tool_ = tool;
}

SelectionTool::InnerEventHandler::~InnerEventHandler(){

}

void SelectionTool::InnerEventHandler::AreaInvalidated(ToolEventArgs& evt){
    this->selection_tool_->OnAreaInvalidated(evt.GetInvalidatedArea());
}

void SelectionTool::InnerEventHandler::ToolDone(ToolEventArgs& evt){
    this->selection_tool_->set_tracker(selection_tool_->GetSelectionAreaTracker());
    this->selection_tool_->OnToolDone();
}
