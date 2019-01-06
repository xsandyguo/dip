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
#include <GdiPlus.h>

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/observer.h"
#include "io/io_format.h"

using namespace Gdiplus;
using namespace std;

extern const Rect& kEmptyRect;
class IDrawingEditor;
class IFigure;
class IView;
class IHandle;
class IConnector;
class IConnectionFigure;
class IHandleObserver;
class IFigureObserver;
class IToolObserver;
class IKeyObserver;
class IMouseObserver;
class ITool;
class ILocator;
class IFigureSelectionObserver;
class IContainerObserver;
class IConnectorObserver;

class FigureAdapter;
class ToolAdapter;
class IFigureFactory;
class IDrawing;

class HandleEventArgs;
class FigureEventArgs;
class FigureSelectionEventArgs;
class DrawingEventArgs;
class ConnectorEventArgs;
class ToolEventArgs;

class CWnd;

class FigureEventArgs
    : public EventArgs{
public:
    FigureEventArgs();
    FigureEventArgs(IFigure* source, Rect invalidatedArea) ;
    IFigure* GetFigure();
    Rect GetInvalidatedArea();

    virtual ~FigureEventArgs();
private:
    IFigure* figure_;
    Rect invalidated_area_;

    DISALLOW_EVIL_CONSTRUCTORS(FigureEventArgs);
};

class IContainerObserver : public IObserver {
public:
    IContainerObserver(){};
    virtual ~IContainerObserver(){}
    virtual void FigureRemoved(DrawingEventArgs& evt) = 0;
    virtual void FigureAdded(DrawingEventArgs& evt)   = 0;
private:
    DISALLOW_EVIL_CONSTRUCTORS(IContainerObserver);
};

class DrawingEventArgs
    : public EventArgs{
public:
    DrawingEventArgs(IDrawing* container, IFigure* child, Rect rc, int z_index);	
    ~DrawingEventArgs();	
    IFigure*	GetTargetFigure(); 
    IDrawing* GetContainer(); 
    int GetIndex();
    Rect GetInvalidatedArea();

private:
    Rect invalidated_area_;
    int z_index_;
    IDrawing* container_;
    IFigure* figure_; 

    DISALLOW_EVIL_CONSTRUCTORS(DrawingEventArgs);
};

class IFigureObserver : public IObserver {
public:
    IFigureObserver(void){}
    virtual ~IFigureObserver(void){}

    virtual void AttributeChanged(FigureEventArgs& evt) = 0;   
    virtual void AreaInvalidated(FigureEventArgs& evt)  = 0;
    virtual void FigureChanged(FigureEventArgs& evt)    = 0;
    virtual void RequestRemove(FigureEventArgs& evt)    = 0;

private:
    DISALLOW_EVIL_CONSTRUCTORS(IFigureObserver);
};

class FigureAdapter :public IFigureObserver {
public:
    FigureAdapter(){}
    virtual ~FigureAdapter(){}

    virtual void AttributeChanged(FigureEventArgs& evt){}
    virtual void AreaInvalidated(FigureEventArgs& evt){}
    virtual void FigureChanged(FigureEventArgs& evt){}
    virtual void RequestRemove(FigureEventArgs& evt){}

private:
    DISALLOW_EVIL_CONSTRUCTORS(FigureAdapter);
};

class FigureSelectionEventArgs
    : public EventArgs{
public:
    FigureSelectionEventArgs(IView* source, 
        vector<IFigure*>* oldSelection, 
        vector<IFigure*>* newSelection);

    virtual ~FigureSelectionEventArgs();

    vector<IFigure*>& GetNewSelection();
    vector<IFigure*>& GetOldSelection();
    IView* GetSource();

private:
    vector<IFigure*> old_selection_;
    vector<IFigure*> new_selection_;
    IView* source_;
    DISALLOW_EVIL_CONSTRUCTORS(FigureSelectionEventArgs);
};

class IFigureSelectionObserver : public IObserver {
public:
    IFigureSelectionObserver(){}
    virtual ~IFigureSelectionObserver(){}
    virtual void SelectionChanged(FigureSelectionEventArgs& evt) = 0;

private:
    DISALLOW_EVIL_CONSTRUCTORS(IFigureSelectionObserver);
};

class IFigure : public IDOMStorable{
public:
    IFigure(){}
    virtual ~IFigure(){}
    virtual void AddFigureObserver(IFigureObserver* observer) = 0;
    virtual void RemoveFigureObserver(IFigureObserver* observer) = 0;
    virtual void Draw(Graphics& g) = 0;

    virtual void SetBounds(const Point& start, const Point& end) = 0;
    virtual void SetPosition(const Point& pos) = 0;
    virtual void SetBounds(const Rect& rect) = 0;
    virtual Rect GetBounds() = 0;
    virtual Rect GetDrawingArea() = 0;

    virtual bool Contains(const Point& p) = 0;
    virtual bool IsVisible() = 0;
    virtual bool IsRemovable() = 0;
    virtual bool IsSelected() = 0;
    virtual void SetSelected(bool selected) = 0;

    virtual int GetLayer() = 0;

    virtual bool CanMove() = 0;
    virtual bool CanResize() = 0;
    virtual bool CanConnect() = 0;
    virtual void Changed() = 0;
    virtual IFigure* Clone() = 0;
    virtual void CreateHandles(vector<IHandle*>* handles) = 0;
    virtual IConnector* ConnectorAt(const Point& p) = 0;
    virtual void GetToolTipAt(std::wstring *content, Point* p) = 0;
    virtual	void WillChange() = 0;
    virtual void Invalidate() = 0;

    virtual void Release() = 0;
private:
    DISALLOW_EVIL_CONSTRUCTORS(IFigure);
};

class IConnectorObserver : public IObserver {
public:
    IConnectorObserver(){}
    virtual ~IConnectorObserver(){}
    virtual void AreaInvalidated(ConnectorEventArgs& evt) = 0;
private:
    DISALLOW_EVIL_CONSTRUCTORS(IConnectorObserver);
};

class IConnector : public IDOMStorable {
public:
    IConnector(){}
    virtual ~IConnector(){}
    virtual void AddConnectorObserver(IConnectorObserver* observer) = 0;
    virtual void RemoveConnectorObserver(IConnectorObserver* observer) = 0;
    
    virtual void Hint(bool visible) = 0;
    virtual void Draw(Graphics& g) = 0;
    virtual IFigure* Owner() = 0;
    virtual Rect GetBounds() = 0;

    virtual bool CanStart() = 0;
    virtual bool CanEnd() = 0;
    virtual bool IsConnected() = 0;
    virtual void SetConnected(bool is_connected ) = 0;
    virtual bool Contains(const Point& p) = 0;         
private:
    DISALLOW_EVIL_CONSTRUCTORS(IConnector);
};

class ConnectorEventArgs : public EventArgs{
public:
    ConnectorEventArgs(IConnector* connector, Rect invalidatedArea);
    Rect GetInvalidatedArea(){return invalidated_area_;}
    IConnector* GetSource();
private:
    Rect invalidated_area_;
    IConnector* source_;
    DISALLOW_EVIL_CONSTRUCTORS(ConnectorEventArgs);
};

class IConnectionFigure : public virtual IFigure, public IFigureObserver {
public:
    IConnectionFigure(){}
    virtual ~IConnectionFigure(){}
    virtual void SetConnectStart(IConnector* start) = 0;
    virtual void SetConnectEnd(IConnector* end) = 0;
    virtual IConnector* GetConnectStart() = 0;
    virtual IConnector* GetConnectEnd() = 0;
    virtual void DisconnectStart() = 0;
    virtual void DisconnectEnd() = 0;

    virtual void SetStartPoint(const Point& p) = 0;
    virtual void SetEndPoint(const Point& p) = 0;
    virtual Point GetStartPoint() = 0;
    virtual Point GetEndPoint() = 0;

    virtual void UpdateConnection() = 0;
    virtual bool CanConnect(IConnector* start, IConnector* end) = 0;

private:
    DISALLOW_EVIL_CONSTRUCTORS(IConnectionFigure);
};

/**  @brief 可编辑组件接口 */
class IEditableComponent{
public:
    IEditableComponent(){}
    virtual ~IEditableComponent(){}

    virtual void DoDelete()    = 0;
    virtual void DoCut()       = 0;
    virtual void DoCopy()      = 0;
    virtual void DoPaste()     = 0;
    virtual void DoSelectAll() = 0;
private:
    DISALLOW_EVIL_CONSTRUCTORS(IEditableComponent);
};

class ILocator{
public:
    ILocator(){}
    virtual ~ILocator(){}
    virtual Point Locate(const Point& p) = 0;
private:
    DISALLOW_EVIL_CONSTRUCTORS(ILocator);
};

class IGrid : ILocator{
public:
    IGrid(){}
    virtual ~IGrid(){}

    virtual void Draw(Graphics& g) = 0;
    virtual void Enable(bool enable) = 0;
    virtual bool IsEnabled() = 0;

private:
    DISALLOW_EVIL_CONSTRUCTORS(IGrid);
};

class IView{
public:
    IView(){}
    virtual ~IView(){}

    virtual IDrawingEditor* GetEditor()               = 0;
    virtual void AddNotify(IDrawingEditor* editor)    = 0;
    virtual void RemoveNotify(IDrawingEditor* editor) = 0;

    virtual void AddSelectionObserver(IFigureSelectionObserver* observer)    = 0;	
    virtual void RemoveSelectionObserver(IFigureSelectionObserver* observer) = 0;	
    virtual void AddMouseObserver(IMouseObserver* observer)                  = 0;	
    virtual void RemoveMouseObserver(IMouseObserver* observer)               = 0;	
    virtual void AddKeyObserver(IKeyObserver* observer)                      = 0;	
    virtual void RemoveKeyObserver(IKeyObserver* observer)                   = 0;	

    virtual IHandle* FindHandle(const Point& p)        = 0;	
    virtual IDrawing* GetDrawing()                 = 0;	
    virtual IEditableComponent* GetEditableComponent() = 0;
    virtual ILocator& GetLocator()                     = 0;
    virtual ITool* GetTool()                           = 0;	
    virtual vector<IFigure*>& GetSelectedFigures()      = 0;	
    virtual int GetSelectionCount()                    = 0;
    virtual void ClearSelection()                      = 0;	

    virtual void AddToSelection(IFigure* figure)      = 0;	
    virtual void RemoveFromSelection(IFigure* figure) = 0;	
    virtual void ExtendViewportTo(const Point& gain)  = 0;
    virtual void Invalidate(const Rect& rc)           = 0;
    virtual void Invalidate()                         = 0;
    virtual CWnd& GetWindow()                         = 0;	
private:
    DISALLOW_EVIL_CONSTRUCTORS(IView);
};

enum MouseButton{    
    MUB_LEFT,    
    MUB_MIDDLE,    
    MUB_RIGHT,    
    MUB_NONE
};

class MouseEventArgs 
    : public EventArgs{
public:
    /** 构造器 
    * @param source 事件源
    * @param nFlags 指示用户是否按下了某修饰键，参见MSDN关于WM_xBUTTONxx消息获得可能的值
    * @param point 当前光标的位置
    * @param button 相关联的鼠标按键
    */
    MouseEventArgs(IView* source, UINT nFlags, const Point& point, MouseButton button = MUB_NONE);
    virtual ~MouseEventArgs();	
    IView* GetSource();

    /** 获得标志指示用户是否按下了某修饰键，参见WM_xBUTTONxx消息获得可能的值 
    * @return 获得标志
    */
    UINT GetFlags();	
    Point GetPoint();	
    int GetX();	
    int GetY();	
    MouseButton GetButton();
private:
    IView* source_;
    UINT flags_;
    Point point_;
    MouseButton button_;
    DISALLOW_EVIL_CONSTRUCTORS(MouseEventArgs);
};

/**
* @brief 键盘事件参数 
* 请参看MSND关于WM_KEYDOWN WM_KEYUP消息定义
*/
class KeyEventArgs
    : public EventArgs{
public :
    /** 构造器 
    * @param source 事件源
    * @param nChar Specifies the virtual key code of the given key. For a list of of standard virtual key codes, see Winuser.h 
    * @param nRepCnt Repeat count (the number of times the keystroke is repeated as a result of the user holding down the key). 
    * @param nFlags Specifies the scan code, key-transition code, previous key state, and context code
    */
    KeyEventArgs(IView* source, UINT nChar, UINT nRepCnt, UINT nFlags);
    virtual ~KeyEventArgs();	
    IView* GetSource();	
    UINT GetChar();	
    UINT GetRepCount();	
    UINT GetFlags();
private:
    IView* source_;
    UINT char_;
    UINT repate_count_;
    UINT flags_;
    DISALLOW_EVIL_CONSTRUCTORS(KeyEventArgs);;
};

class IMouseObserver : public IObserver {
public:
    IMouseObserver(){}
    virtual ~IMouseObserver(){}

    virtual	void MouseMove(MouseEventArgs& evt)   = 0;
    virtual void MouseDown(MouseEventArgs& evt)   = 0;
    virtual void MouseUp(MouseEventArgs& evt)     = 0;
    virtual void MouseDblClk(MouseEventArgs& evt) = 0;
private:
    DISALLOW_EVIL_CONSTRUCTORS(IMouseObserver);;
};

class IKeyObserver : public IObserver {
public:
    IKeyObserver(){}
    virtual ~IKeyObserver(){}
    virtual void KeyDown(KeyEventArgs& evt) = 0;
    virtual void KeyUp(KeyEventArgs& evt)   = 0;
};

class IToolObserver : public IObserver {
public:
    IToolObserver(){}
    virtual ~IToolObserver(){}
    virtual void ToolStart(ToolEventArgs& evt)       = 0;
    virtual void ToolDone(ToolEventArgs& evt)        = 0;
    virtual void AreaInvalidated(ToolEventArgs& evt) = 0;
private:
    DISALLOW_EVIL_CONSTRUCTORS(IToolObserver);
};


class ITool :
    public IKeyObserver,
    public IMouseObserver{
public:
    ITool(){}
    virtual ~ITool(){}

    virtual void Draw(Graphics& g) = 0; 
    virtual void AddToolObserver(IToolObserver* observer)    = 0;
    virtual void RemoveToolObserver(IToolObserver* observer) = 0; 
    virtual void Active(IView* view)              = 0;
    virtual void Deactive(IView* view)            = 0;
private:
    DISALLOW_EVIL_CONSTRUCTORS(ITool);
};

class ToolEventArgs: public EventArgs{
public: 
    ToolEventArgs(ITool * tool, IView* view, Rect invalidatedArea);	
    ITool* get_tool();	
    IView * get_view();
    Rect GetInvalidatedArea();

private:
    ITool* tool_;
    IView* view_;
    Rect invalidated_area_;
    DISALLOW_EVIL_CONSTRUCTORS(ToolEventArgs);
};

class ToolAdapter : public IToolObserver {
public:
    ToolAdapter(){}
    virtual ~ToolAdapter(){}

    virtual void ToolStart(ToolEventArgs& evt){}
    virtual void ToolDone(ToolEventArgs& evt){}
    virtual void AreaInvalidated(ToolEventArgs& evt){}
private:
    DISALLOW_EVIL_CONSTRUCTORS(ToolAdapter);
};

class IHandle{
public:
    IHandle(){}
    virtual ~IHandle(){}

    virtual void AddHandleObserver(IHandleObserver* observer)    = 0;	
    virtual void RemoveHandleObserver(IHandleObserver* observer) = 0;

    virtual void Draw(Graphics& g)    = 0;
    virtual IFigure* GetOwner()       = 0;	
    virtual Rect GetBounds()          = 0;	
    virtual void SetView(IView* view) = 0;
    virtual bool Contains(const Point& p)    = 0;	
    virtual void Invalidate()         = 0;	
    virtual HCURSOR GetCursor()       = 0;

    virtual void TrackStart(const Point& anchor) = 0;

    virtual void TrackStep(const Point& anchor, const Point& lead, uint32 flag) = 0;
    virtual void TrackEnd(const Point& anchor, const Point& lead, uint32 flag)  = 0;
private:
    DISALLOW_EVIL_CONSTRUCTORS(IHandle);
};

class HandleEventArgs : public EventArgs{
public:
    HandleEventArgs(IHandle* handle, const Rect& rect);	
    IHandle* GetHandle();	
    Rect GetInvalidatedArea();
private:
    Rect invalidated_area_;
    IHandle* handle_;
    DISALLOW_EVIL_CONSTRUCTORS(HandleEventArgs);
};

class IHandleObserver : public IObserver {
public:
    IHandleObserver() {}
    virtual ~IHandleObserver(){}

    virtual void AreaInvalidated(HandleEventArgs& evt) = 0;
private:
    DISALLOW_EVIL_CONSTRUCTORS(IHandleObserver);
};

class IDrawing : public virtual IFigure{
public:
    IDrawing(){}
    virtual ~IDrawing(){}
    virtual void AddContainerObserver(IContainerObserver* observer) = 0;	
    virtual void RemoveContainerObserver(IContainerObserver* observer) = 0;

    virtual void AddFigure(IFigure* figure) = 0;	
    virtual void AddFigure(IFigure* figure, int index) = 0;

    virtual void BasicAddFigure(IFigure* figure)            = 0; 
    virtual void BasicAddFigure(IFigure* figure, int index) = 0; 
    virtual void RemoveFigure(IFigure* figure)              = 0;	
    virtual void BasicRemoveFigure(IFigure* figure)         = 0;	

    virtual void BringToFront(IFigure* figure)           = 0;	
    virtual void SendToBack(IFigure* figure)             = 0;  
    virtual void MoveToLayer(IFigure* figure, int layer) = 0;

    virtual IFigure* FindFigure(const Point& p) = 0;	

    virtual int IndexOf(IFigure* figure) = 0; 	
    virtual IFigure* GetChild(int index) = 0;	

    virtual void Clear(void) = 0;	
    virtual int GetChildCount(void) = 0;	

    typedef std::vector<IFigure*> FigureCollection;
    typedef std::vector<IFigure*>::iterator FigureIter;
    
    virtual const FigureCollection& GetChildren(void) const = 0;	
    virtual bool Contains(IFigure* figure) = 0;	

private:
    DISALLOW_EVIL_CONSTRUCTORS(IDrawing);
};

class IDrawingEditor{
public:
    IDrawingEditor(){}
    virtual ~IDrawingEditor(){}

    virtual IView* GetActiveView() = 0;
    virtual ITool* GetTool()       = 0;
private:
    DISALLOW_EVIL_CONSTRUCTORS(IDrawingEditor);
};

typedef std::wstring ClassId;

class ToolboxEventArgs : public EventArgs{
public:
    ToolboxEventArgs(CWnd& toolbox, 
        const ClassId& old_val,
        const ClassId& new_val)
        :toolbox_(toolbox), old_val_(old_val), new_val_(new_val){}

    ToolboxEventArgs(CWnd& toolbox, 
        const ClassId& clicked)
        :toolbox_(toolbox), new_val_(clicked){}

    CWnd& Source(){ return toolbox_ ;}
    const ClassId& GetOldSelection() const { return old_val_; }
    const ClassId& GetNewSelection() const { return new_val_;}
    const ClassId& GetClicked() const { return new_val_; }

private:
    ClassId old_val_;
    ClassId new_val_;
    CWnd& toolbox_;
};

class IToolboxObserver : public IObserver{
public:
    IToolboxObserver(){}
    virtual ~IToolboxObserver(){}
    virtual void SelectionChanged(ToolboxEventArgs& evt) = 0;
    virtual void ItemClicked(ToolboxEventArgs& evt) = 0;
};

class ToolboxAdapter : public IToolboxObserver{
public:
    ToolboxAdapter(){}
    virtual ~ToolboxAdapter(){}
    virtual void SelectionChanged(ToolboxEventArgs& evt){}
    virtual void ItemClicked(ToolboxEventArgs& evt){}

private:
    DISALLOW_EVIL_CONSTRUCTORS(ToolboxAdapter);
};

class CommandEventArgs : public EventArgs{
public:
    CommandEventArgs(CWnd& source,
        const wstring& object_name, 
        const wstring& method_name,
        std::vector<wstring>& argument)
        :source_(source),
        object_name_(object_name),
        method_name_(method_name),
        arguments_(argument){
    }

    CWnd& GetSource(){ return source_ ;}
    const wstring& GetObjectName() const{ return object_name_;}
    const wstring& GetMethodName() const { return method_name_; }
    const std::vector<wstring>& GetArguments() const{ return arguments_; }

private:
    const wstring& object_name_;
    const wstring& method_name_;
    const std::vector<wstring>& arguments_;
    CWnd& source_;
    DISALLOW_EVIL_CONSTRUCTORS(CommandEventArgs);
};

class ICommandObserver :  public IObserver{
public:
    ICommandObserver(){}
    virtual ~ICommandObserver(){}
    virtual void CommandInputed(CommandEventArgs& evt) = 0;
private:
    DISALLOW_EVIL_CONSTRUCTORS(ICommandObserver);
};
