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

#include "framework/abstract_view.h"

#include <stack>
#include <map>

#include "base/debug.h"
#include "resource.h"

BEGIN_MESSAGE_MAP(AbstractView, CScrollView )
    ON_WM_RBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP() 
    ON_WM_LBUTTONDBLCLK()
    ON_WM_KEYDOWN()
    ON_WM_KEYUP()
END_MESSAGE_MAP()

AbstractView::AbstractView(): editor_(NULL){
    event_handler_.reset(new InnerEventHandler(this));
}

AbstractView::~AbstractView(){	
}

void AbstractView::DrawCanvas(Graphics& g){
    Rect clip;
    g.GetClipBounds(&clip);
    SolidBrush sb(Color(255, 255, 255, 255));
    g.FillRectangle(&sb, clip);
}

void AbstractView::DrawHandle(Graphics& g){
    vector<IHandle*>::iterator itor = selected_handles_.begin();
    while (itor != selected_handles_.end()){
        (*itor)->Draw(g);
        itor++;
    }
}

void AbstractView::DrawTool(Graphics& g){
    ITool* tool = GetTool();
    if(tool){
        tool->Draw(g);
    }
}

void AbstractView::DrawFigure(Graphics& g){
    IDrawing * drawing = GetDrawing();
    if (drawing){
        drawing->Draw(g);	
    }
}

void AbstractView::OnDraw(CDC* pDC){
    Graphics g(pDC->m_hDC); 	

    CRect clipbox;
    pDC->GetClipBox(&clipbox);
    Rect clip(clipbox.left, clipbox.top, clipbox.Width(), clipbox.Height());   

    Size prefer_size = Size(clipbox.right, clipbox.bottom);
    Bitmap buffer_img(prefer_size.Width, prefer_size.Height);
    scoped_ptr<Graphics> buffer_g(Graphics::FromImage(&buffer_img)); 
    
    buffer_g->SetClip(clip);
    DrawCanvas(*buffer_g.get());
    DrawFigure(*buffer_g.get());
    DrawHandle(*buffer_g.get()); 
    DrawTool(*buffer_g.get());  

    g.DrawImage(&buffer_img, 
                clip, 
                clip.X, 
                clip.Y, 
                clip.Width, 
                clip.Height, 
                UnitPixel);
}
void AbstractView::AddSelectionObserver(IFigureSelectionObserver* observer){
    AddObserver(&observers_, observer);
}

void AbstractView::RemoveSelectionObserver(IFigureSelectionObserver* observer){
   RemoveObserver(&observers_, observer);
}

void AbstractView::AddMouseObserver(IMouseObserver* observer){
    AddObserver(&observers_, observer);
}

void AbstractView::AddKeyObserver(IKeyObserver* observer){
   AddObserver(&observers_, observer);
}

void AbstractView::RemoveMouseObserver(IMouseObserver* observer){   
    RemoveObserver(&observers_, observer);
}

void AbstractView::RemoveKeyObserver(IKeyObserver* observer){
    RemoveObserver(&observers_, observer);
}

IDrawingEditor* AbstractView::GetEditor(){
    return editor_;
}

void AbstractView::AddNotify(IDrawingEditor* editor){    
    editor_ = editor;
}

void AbstractView::RemoveNotify(IDrawingEditor* editor){
    editor_ = NULL;
}

IHandle* AbstractView::FindHandle(const Point& p){
    vector<IHandle*>::reverse_iterator itor = selected_handles_.rbegin();
    while (itor != selected_handles_.rend()){	
        if ((*itor)->Contains(p)){
            return (*itor);		
        }
        itor++;
    }
    return NULL;
}

ITool* AbstractView::GetTool(){
    if(editor_){
        return editor_->GetTool();
    }

    return NULL;
}

vector<IFigure*>& AbstractView::GetSelectedFigures(void){
    //vector<IFigure*> figures;
    //figures.assign(selected_figures_.begin(), selected_figures_.end());

    //return figures;

    return selected_figures_;
}

void AbstractView::OnSelectionChanged(vector<IFigure*>* old, vector<IFigure*>* newOne){
    size_t sz = observers_.size();
    FigureSelectionEventArgs evt(this, old, newOne);
    ObserverIter itor = observers_.begin();
    while(itor != observers_.end()){
        ObserverCall(*itor, &IFigureSelectionObserver::SelectionChanged, evt);
        itor++;
    }
}

int AbstractView::GetSelectionCount(void){
    return (int)this->selected_figures_.size();
}

void AbstractView::ClearSelection(){
    vector<IFigure*>* old = new vector<IFigure*>(selected_figures_);
    vector<IFigure*>::iterator witor = old->begin();
    while(witor != old->end()){
        (*witor)->SetSelected(false); 
        witor++;
    }
    this->selected_figures_.clear();

    Rect invalidateArea = kEmptyRect;
    vector<IHandle*>::iterator hitor = selected_handles_.begin();
    while(hitor != selected_handles_.end()){
        if (invalidateArea.IsEmptyArea()){
            invalidateArea = (*hitor)->GetBounds();
        }else{
            Rect::Union(invalidateArea, invalidateArea, (*hitor)->GetBounds());
        }

        delete (*hitor);
        hitor ++;
    }

    selected_handles_.clear();
    this->OnSelectionChanged(old, NULL); 
    Invalidate(invalidateArea);
}

void AbstractView::AddToSelection(IFigure* figure){
    figure->SetSelected(true);

    //if (GetSelectionCount() == 1) { 
    //    Rect invalidateArea(kEmptyRect);
    //    vector<IHandle*>::iterator itor = selected_handles_.begin();
    //    while (itor != selected_handles_.end()){
    //        if (invalidateArea.IsEmptyArea()){
    //            invalidateArea = (*itor)->GetBounds();
    //        } else {
    //            Rect::Union(invalidateArea, invalidateArea, (*itor)->GetBounds());
    //        }

    //        delete (*itor);
    //        itor ++;
    //    }
    //    selected_handles_.clear();

    //    vector<IHandle*> leve0;
    //    selected_figures_.at(0)->CreateHandles(&leve0);
    //    itor = leve0.begin();
    //    while (itor != leve0.end()){
    //        (*itor)->SetView(this);
    //        if (invalidateArea.IsEmptyArea()){
    //            invalidateArea = (*itor)->GetBounds();
    //        }else{
    //            Rect::Union(invalidateArea, invalidateArea, (*itor)->GetBounds());
    //        }
    //        selected_handles_.push_back(*itor);

    //        (*itor)->AddHandleObserver(event_handler_.get());
    //        itor ++;
    //    }
    //    
    //    Invalidate(invalidateArea);
    //}

    vector<IFigure*> old(selected_figures_);
    this->selected_figures_.push_back(figure);

    Rect invalidateArea(kEmptyRect); 
    vector<IHandle*> handles;
    figure->CreateHandles(&handles);
    vector<IHandle*>::iterator hitor = handles.begin();
    while (hitor != handles.end()){
        IHandle* handle = *hitor;
        handle->SetView(this);
        this->selected_handles_.push_back(handle);

        if (invalidateArea.IsEmptyArea()){
            invalidateArea = handle->GetBounds();
        }else{
            Rect::Union(invalidateArea, invalidateArea, handle->GetBounds());			
        }
        handle->AddHandleObserver(event_handler_.get());
        hitor ++;
    }
    
    this->OnSelectionChanged(&old, &selected_figures_); 
    Invalidate(invalidateArea);
}

void AbstractView::RemoveFromSelection(IFigure* figure){
    vector<IFigure*>* old = new vector<IFigure*>(selected_figures_);
    vector<IFigure*>::iterator witor;
    for (witor = selected_figures_.begin(); witor != selected_figures_.end(); witor++){
        if((*witor) == figure){
            selected_figures_.erase(witor);
            break;
        }
    }

    figure->SetSelected(false);

    Rect invalidatedArea = kEmptyRect;
    vector<IHandle*>::iterator hitor = selected_handles_.begin();
    while (hitor != selected_handles_.end()){
        if ((*hitor)->GetOwner() == figure){
            vector<IHandle*>::iterator first = hitor;
            while (hitor != selected_handles_.end() && (*hitor)->GetOwner() == figure){
                if (invalidatedArea.IsEmptyArea()){
                    invalidatedArea = (*hitor)->GetBounds();
                }else{
                    Rect::Union(invalidatedArea, invalidatedArea, (*hitor)->GetBounds());
                }
                hitor ++;
            }
            selected_handles_.erase(first, hitor);
            break;
        }

        delete (*hitor);
        hitor ++;
    }

    this->OnSelectionChanged(old, &selected_figures_);
    delete old;

    Invalidate(invalidatedArea);
}

void AbstractView::OnKeyDown(KeyEventArgs& evt){
    ObserverIterCall(observers_, &IKeyObserver::KeyDown, evt);
    ASSERT1(GetTool());
    GetTool()->KeyDown(evt);
}

void AbstractView::OnKeyUp(KeyEventArgs& evt){
    ObserverIterCall(observers_, &IKeyObserver::KeyUp, evt);
    ASSERT1(GetTool());
    GetTool()->KeyUp(evt);
}

void AbstractView::OnMouseDblClk(MouseEventArgs& evt){
    ObserverIterCall(observers_, &IMouseObserver::MouseDblClk, evt); 
    ASSERT1(GetTool());   
    GetTool()->MouseDblClk(evt);
}

void AbstractView::OnMouseMove(MouseEventArgs& evt){
    ObserverIterCall(observers_, &IMouseObserver::MouseMove, evt);
    ASSERT1(GetTool());
    GetTool()->MouseMove(evt);
}

void AbstractView::OnMouseDown(MouseEventArgs& evt){
    ObserverIterCall(observers_, &IMouseObserver::MouseDown, evt);
    ASSERT1(GetTool());
    GetTool()->MouseDown(evt);
}

void AbstractView::OnMouseUp(MouseEventArgs& evt){
    ObserverIterCall(observers_, &IMouseObserver::MouseUp, evt);
    ASSERT1(GetTool());
    GetTool()->MouseUp(evt);
}

void AbstractView::Invalidate(const Rect& rc){
    CRect rect = VirtualToClient(rc);
    CWnd::InvalidateRect(&rect); 
}

void AbstractView::Invalidate(){
    CWnd::Invalidate();
}

void AbstractView::DoCut(){

}

void AbstractView::DoCopy(){

}

void AbstractView::DoPaste(){

}

void AbstractView::DoSelectAll(){

}

void AbstractView::DoDelete(){
    vector<IFigure*> selected(GetSelectedFigures());
    ClearSelection();

    vector<IFigure*>::iterator itor = selected.begin();
    while(itor != selected.end()){
        if((*itor)->IsRemovable()){
            this->GetDrawing()->RemoveFigure(*itor);
        }

        itor++;
    }
}

void AbstractView::InnerEventHandler::AreaInvalidated(HandleEventArgs& evt){
    this->view_->Invalidate(evt.GetInvalidatedArea());
}


Point AbstractView::ClientToVirtual(const CPoint& point){
    CPoint sp = GetScrollPosition();

    return Point(point.x + sp.x, point.y + sp.y);
}

Rect AbstractView::ClientToVirtual(const CRect& rect){
    CPoint sp = GetScrollPosition();
    return Rect(rect.left + sp.x, rect.top + sp.y, rect.Width(), rect.Height());
}

CPoint AbstractView::VirtualToClient(const Point& p){
    CPoint scroll_pos = GetScrollPosition();
    return CPoint(p.X - scroll_pos.x, p.Y - scroll_pos.y);
}

CRect AbstractView::VirtualToClient(const Rect& rc){
    CPoint p = GetScrollPosition();
    Point location;
    location.X = rc.X - p.x;
    location.Y = rc.Y - p.y;
    return CRect(location.X, location.Y , location.X + rc.Width, 
        location.Y + rc.Height);
}

void AbstractView::OnLButtonDblClk(UINT nFlags, CPoint point){
    MouseEventArgs evt(this, nFlags, ClientToVirtual(point), MUB_LEFT);
    OnMouseDblClk(evt);
    CScrollView::OnLButtonDblClk(nFlags, point);
}

void AbstractView::OnMouseMove(UINT nFlags, CPoint point){
    MouseEventArgs evt(this, nFlags, ClientToVirtual(point));
    OnMouseMove(evt);
    CScrollView::OnMouseMove(nFlags, point);
}

void AbstractView::OnLButtonDown(UINT nFlags, CPoint point){
    SetCapture();

    MouseEventArgs evt(this, nFlags, ClientToVirtual(point), MUB_LEFT);
    OnMouseDown(evt);
    CScrollView::OnLButtonDown(nFlags, point);
}

void AbstractView::OnLButtonUp(UINT nFlags, CPoint point){
    ReleaseCapture();
    MouseEventArgs evt(this, nFlags, ClientToVirtual(point), MUB_LEFT);
    OnMouseUp(evt);
    CScrollView::OnLButtonUp(nFlags, point);
}

void AbstractView::OnRButtonDown(UINT nFlags, CPoint point){
    MouseEventArgs evt(this, nFlags, ClientToVirtual(point), MUB_RIGHT);
    OnMouseDown(evt);
    CScrollView::OnRButtonDown(nFlags, point);
}

void AbstractView::OnRButtonUp(UINT nFlags, CPoint point){
    MouseEventArgs evt(this, nFlags, ClientToVirtual(point), MUB_RIGHT);
    OnMouseUp(evt);
    CScrollView::OnRButtonUp(nFlags, point);

    ClientToScreen(&point);
    OnContextMenu(this, point);
}

void AbstractView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
    KeyEventArgs evt(this, nChar, nRepCnt, nFlags);
    OnKeyDown(evt);
    CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void AbstractView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags){
    KeyEventArgs evt(this, nChar, nRepCnt, nFlags);
    OnKeyUp(evt);
    CScrollView::OnKeyUp(nChar, nRepCnt, nFlags);
}
