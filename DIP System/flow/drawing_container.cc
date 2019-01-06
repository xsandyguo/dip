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

#include "flow/drawing_container.h" 

#include <algorithm>
#include <stack>
#include "base/debug.h"
#include "io/serialize_const.h"

DrawingContainer::DrawingContainer()
    :need_sorting_(false){
    event_handler_.reset(new InnerEventHandler(this));
}

DrawingContainer::~DrawingContainer(){
    FigureIter itor = children_.begin();
    while (itor != children_.end()){         
        delete (*itor);
        itor++;
    }
}

void DrawingContainer::BasicAddFigure(IFigure* figure){
    BasicAddFigure(figure, GetChildCount());
}
 
void DrawingContainer::BasicAddFigure(IFigure* figure, int index){
    FigureIter itor = children_.begin();
    for(int i = 0; i < index; i++, itor++);
    children_.insert(itor, figure); 
    figure->AddFigureObserver(event_handler_.get());

    InvalidateSortOrder();
}

void DrawingContainer::BasicRemoveFigure(IFigure* figure){
    FigureIter itor;
    for (itor = this->children_.begin(); itor != this->children_.end(); itor++){
        if((*itor) == figure){
            this->children_.erase(itor);
            break;
        }
    } 
    figure->RemoveFigureObserver(event_handler_.get());
}

Rect DrawingContainer::GetBounds(){
    Rect bounds(0, 0, 0, 0);
    vector<IFigure*>::iterator itor = children_.begin();
    while(itor != children_.end()){
        Rect::Union(bounds, bounds, (*itor)->GetBounds());
        ++itor;
    }

    return bounds;
}
  
bool DrawingContainer::Contains(IFigure* figure){ 
    return find(children_.begin(), children_.end(), figure) != children_.end();
}

void DrawingContainer::InvalidateSortOrder(){
    need_sorting_ = true;
}

void DrawingContainer::EnsureSorted(){
    if(need_sorting_){
        sort(children_.begin(), children_.end(), [](IFigure* a, IFigure* b){
            return a->GetLayer() < b->GetLayer();
        });
    }
}
 
void  DrawingContainer::Draw(Graphics& g){
    EnsureSorted();

    vector<IFigure*>::iterator itor;
    Rect cliparea;
    g.GetClipBounds(&cliparea);

    itor = children_.begin();
    while (itor != children_.end()){
        IFigure* figure = *itor;
        if(figure->IsVisible() && 
           cliparea.IntersectsWith(figure->GetDrawingArea())){
            (*itor)->Draw(g); 
        }
        itor ++;
    }	  
}

void DrawingContainer::AddFigure(IFigure* figure, int index){
    this->BasicAddFigure(figure, index); 
    this->OnFigureAdded(figure, index);
}

void DrawingContainer::AddFigure(IFigure* figure){
    AddFigure(figure, GetChildCount());
}

void DrawingContainer::RemoveFigure(IFigure* figure){  
    if(!Contains(figure)){
        return;
    }
        
    figure->Release();
}

void DrawingContainer::TranslateChildren(const Point& offset){
    vector<IFigure*>::iterator itor = children_.begin();
    while (itor != children_.end()){
        Rect t = (*itor)->GetBounds();	  
        t.X += offset.X;
        t.Y += offset.Y; 
        (*itor)->SetBounds(Rect(t.X, t.Y, t.Width, t.Height));
        itor ++;
    } 
}

void DrawingContainer::OnFigureAdded(DrawingEventArgs& evt){
    ObserverIterCall(observers_, &IContainerObserver::FigureAdded, evt);
}

void DrawingContainer::OnFigureRemoved(DrawingEventArgs& evt){
    ObserverIterCall(observers_, &IContainerObserver::FigureRemoved, evt);
}

void DrawingContainer::OnFigureAdded(IFigure* figure, int zIndex){
    DrawingEventArgs evt(this, figure, figure->GetDrawingArea(), zIndex);
    OnFigureAdded(evt);
}

void DrawingContainer::OnFigureRemoved(IFigure* figure, int zIndex){
    DrawingEventArgs evt(this, figure, figure->GetDrawingArea(), zIndex);
    OnFigureRemoved(evt);
}

void DrawingContainer::OnRequestRemove(FigureEventArgs& evt){
    IFigure* figure = evt.GetFigure();

    int zIndex = IndexOf(figure);
    this->BasicRemoveFigure(figure); 
    this->OnFigureRemoved(figure, zIndex);
}

int DrawingContainer::GetChildCount(void){
    return (int)children_.size();
} 

void DrawingContainer::GetChildrenFrontToBack(vector<IFigure*>& children){
    vector<IFigure*>::reverse_iterator ritor = children_.rbegin();
    while(ritor != children_.rend()){
        children.push_back(*ritor);
        ritor ++;
    }
}

const IDrawing::FigureCollection& DrawingContainer::GetChildren() const{
    return this->children_;
}
  
IFigure* DrawingContainer::GetChild(int index){
    if(index < 0 || index >= GetChildCount()){
        return NULL;
    }
    return children_.at(index);
}

IFigure* DrawingContainer::FindFigure(const Point& p){
    vector<IFigure*>::reverse_iterator ritor;	
    for (ritor = children_.rbegin(); ritor != children_.rend(); ritor++) {
        IFigure* target = *ritor;
        if(target->IsVisible()  && target->Contains(p)){
            return (*ritor);           
        }
    }

    return NULL;
}

void DrawingContainer::MoveToLayer(IFigure* figure, int layer){ 
    if(layer < 0 || layer >= GetChildCount()){
        return;
    }

    if (IndexOf(figure) == layer){
        return;
    }

    figure->WillChange();	
    RemoveFigure(figure);
    AddFigure(figure, layer);
    figure->Changed();
}
void DrawingContainer::SendToBack(IFigure* figure){
    MoveToLayer(figure, 0); 
}

void DrawingContainer::BringToFront(IFigure* figure){
    MoveToLayer(figure, GetChildCount() - 1);
}

void DrawingContainer::Clear(){    
    while(children_.size()){
        IFigure* f = children_.at(0);
        RemoveFigure(f);
        delete f;        
    }
}
 
int DrawingContainer::IndexOf(IFigure* figure){
    int layer = 0;
    FigureIter itor = children_.begin();
    while (itor != children_.end()){
        if ((*itor) == figure){
            return layer;
        }
        layer ++;
        itor ++;
    }
    return -1;
} 

DrawingContainer* DrawingContainer::Clone(){
    ASSERT1(false);
    return NULL;
}

void DrawingContainer::AddContainerObserver(IContainerObserver* observer){
    AddObserver(&observers_, observer);
}

void DrawingContainer::RemoveContainerObserver(IContainerObserver* observer){
    RemoveObserver(&observers_, observer);
}

void DrawingContainer::Write(IDOMOutput& out){
    out.OpenElement(kContainer);

    vector<IFigure*>::iterator itor = children_.begin();
    while (itor != children_.end()){
        out.WriteObject(*itor);
        itor ++;
    }

    out.CloseElement();
}

void DrawingContainer::Read(IDOMInput& in){
    in.OpenElement(kContainer);

    for (int i = 0; i < in.GetElementCount(); i++) {
        IFigure* figure = reinterpret_cast<IFigure*>(in.ReadObject(i));
        ASSERT1(figure);

        AddFigure(figure);
    }

    in.CloseElement();
}