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

#pragma once
#include "base/basictypes.h"
#include "base/scoped_ptr.h"

#include "framework/framework.h"
#include "framework/abstract_figure.h"

#pragma warning(disable : 4250)

class DrawingContainer : public AbstractFigure, public IDrawing{
public:	
    DrawingContainer();	
    virtual ~DrawingContainer();	
    virtual void AddFigure(IFigure* figure);	
    virtual void AddFigure(IFigure* figure, int index);

    virtual void BasicAddFigure(IFigure* figure); 
    virtual void BasicAddFigure(IFigure* figure, int index); 
    virtual void RemoveFigure(IFigure* figure);	
    virtual void BasicRemoveFigure(IFigure* figure);	

    virtual void BringToFront(IFigure* figure);	
    virtual void SendToBack(IFigure* figure);  
    virtual void MoveToLayer(IFigure* figure, int layer);

    virtual IFigure* FindFigure(const Point& p);	

    virtual int IndexOf(IFigure* figure); 	
    virtual IFigure* GetChild(int index);	

    int GetChildCount(void);	
    virtual void Clear(void);	
    virtual const IDrawing::FigureCollection& GetChildren() const;	
    virtual bool Contains(IFigure* figure);	
    virtual void Draw(Graphics& g);	
    virtual void BasicSetBounds(const Point& orgin, const Point& corner){}
    virtual Rect GetBounds();
    DrawingContainer* Clone();

    void Write(IDOMOutput& out);
    void Read(IDOMInput& in);
    
    void AddContainerObserver(IContainerObserver* observer);	
    void RemoveContainerObserver(IContainerObserver* observer);

protected: 	
    virtual void GetChildrenFrontToBack(vector<IFigure*>& children);
    virtual void TranslateChildren(const Point& offset);

    virtual void OnFigureAdded(IFigure* figure, int zIndex);	
    virtual void OnFigureRemoved(IFigure* figure, int zIndex);
    virtual void OnFigureAdded(DrawingEventArgs& evt);
    virtual void OnFigureRemoved(DrawingEventArgs& evt);
    virtual void OnRequestRemove(FigureEventArgs& evt);

private:
    void InvalidateSortOrder();
    void EnsureSorted();
        
    class InnerEventHandler
        :public IFigureObserver{
    public:
        InnerEventHandler(DrawingContainer* container){container_ = container;}
        void AttributeChanged(FigureEventArgs& e){container_->OnAttributeChanged(e);}
        void AreaInvalidated(FigureEventArgs& e){container_->OnAreaInvalidated(e);}
        void FigureChanged(FigureEventArgs& e){container_->OnFigureChanged(e);}
        void RequestRemove(FigureEventArgs& e){container_->OnRequestRemove(e);}

    private:
        DrawingContainer* container_;
        DISALLOW_EVIL_CONSTRUCTORS(InnerEventHandler);
    };

    bool need_sorting_;
    vector<IFigure*> children_; 
    vector<IObserver*> observers_;
    scoped_ptr<InnerEventHandler> event_handler_;

    DISALLOW_EVIL_CONSTRUCTORS(DrawingContainer);
};