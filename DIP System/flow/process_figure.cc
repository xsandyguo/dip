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

#include "flow/process_figure.h"

#include <afxstr.h>
#include <base/debug.h>
#include <runtime/reflector.h>
#include <runtime/vm.h>

#include "base/prefs_const.h"
#include "io/serialize_const.h"
#include "flow/geometry.h"
#include "flow/inout_port.h"


ProcessFigure::ProcessFigure(IFigureFactory* facotry){
    ASSERT1(facotry);

    facotry_.reset(facotry);
    font_.reset(new Gdiplus::Font(kLabelFontName, kLabelFontSize, FontStyleRegular));
    format_.reset(new StringFormat());
    format_->SetAlignment(StringAlignmentCenter);
    format_->SetLineAlignment(StringAlignmentCenter);
}

ProcessFigure::~ProcessFigure(){
    for (vector<IConnector*>::iterator itor = connectors_.begin();
        itor != connectors_.end();
        ++itor) {
        delete (*itor);
    }
    facotry_.release();

    UnRegister();
}

IConnectable& ProcessFigure::GetFilter() const{
    ASSERT1(filter_.get());
    return  *(filter_.get());
}

void ProcessFigure::Draw(Graphics& g){
    __super::Draw(g);
    DrawLabel(g);
}

void ProcessFigure::GetToolTipAt(std::wstring *content, Point* p){
    ASSERT1(content);
    ASSERT1(p);

    if(!Geometry::Contains(GetTextBound(), *p)){
        return;
    }

    CString tip;
    tip.Format(_T("Name:%s\n%s"), GetName(), GetDescription().c_str());

    *content = std::wstring(tip);

    Rect bound = GetDrawingArea();
    p->X = bound.GetLeft();
    p->Y = bound.GetBottom();
}

std::wstring ProcessFigure::GetDescription(){
    return GetFilter().GetObject().GetClass()->GetDescription();
}

const CString& ProcessFigure::GetName(){
    if(name_.IsEmpty()){
        name_ = CString(filter_->GetObject().get_name().c_str());
    }
    return name_;
}

void ProcessFigure::SetFilter(IConnectable* filter){
    UnRegister();
    filter_.reset(filter); 
    Register();
}

Rect ProcessFigure::GetDrawingArea(){
    Rect area = __super::GetDrawingArea();
    RectF text_bound = GetTextBound();
     
    Rect::Union(area, 
                area, 
                Rect(text_bound.X, 
                    text_bound.Y,
                    text_bound.Width, 
                    text_bound.Height));
    return area;
}

void ProcessFigure::Release(){
    UnRegister();

    __super::Release();
}

RectF ProcessFigure::GetTextBound(){ 
    FontFamily family;
    font_->GetFamily(&family);

    CString text = GetName();
    GraphicsPath path;
    path.AddString(text, 
                   -1,
                   &family,
                   font_->GetStyle(),
                   font_->GetSize(),
                   PointF(0, 0),
                   format_.get());

    RectF text_bound;
    path.GetBounds(&text_bound);

    text_bound.Width *= 1.5;
    text_bound.Height *= 2;
    Rect body = GetBounds();
    return RectF(body.X + body.Width/2 - text_bound.Width/2,
                body.Y + body.Height+2,
                text_bound.Width, 
                text_bound.Height);
}


void ProcessFigure::DrawLabel(Graphics& g){   
    FontFamily fontfamily;
    font_->GetFamily(&fontfamily);

    CString name = GetName();
    SolidBrush brush(kLabelColor);
    Pen pen(IsSelected() ? kNodeSelectedBorderColor : kNodeBorderColor);

    RectF bound = GetTextBound();
    bound.Height-= 2;
    bound.Width -= 2;
    
    SolidBrush label_brush(Color(119, 143, 135));
    //g.FillRectangle(&label_brush, bound);
    //g.DrawRectangle(&pen, bound);

    GraphicsPath path;
    Geometry::MakeRoundPath(Geometry::As(bound), 15, &path);   
    g.FillPath(&label_brush, &path); 
    g.DrawPath(&pen, &path);  

    g.DrawString(name, name.GetLength(), font_.get(), bound, format_.get(), &brush);
}


bool ProcessFigure::Contains(const Point& p){
    bool ret = __super::Contains(p);
    if(!ret){
        ret = !!GetTextBound().Contains(PointF(p.X, p.Y));
    }

    return ret;
}

void ProcessFigure::Write(IDOMOutput& out){
    NodeFigure::Write(out);

    WriteFilter(out);
    WriteConnector(out);    
}

void ProcessFigure::Read(IDOMInput& in){
    NodeFigure::Read(in);

    ReadFilter(in);
    ReadConnector(in);
}

void ProcessFigure::WriteConnector(IDOMOutput& out){
    out.OpenElement(kConnectors);
    for(ConnectorIter itor = connectors_.begin(); 
        itor != connectors_.end(); 
        ++itor){
        IConnector* con = *itor;
        if(con->IsConnected()){
            out.WriteObject(con);
        }
    }
    out.CloseElement();
}

void ProcessFigure::ReadConnector(IDOMInput& in){
    in.OpenElement(kConnectors);

    for(int i = 0; i < in.GetElementCount(); ++i){
        InOutPort* con = dynamic_cast<InOutPort*>(in.ReadObject(i));
        if(!con){
            LOG_ERROR(_T("[ProcessFigure::ReadConnector]")
                      _T("[Cant' convert object to InOutPort]"));
            continue;
        }

        for(ConnectorIter pos = GetConnectors().begin();
            pos != GetConnectors().end(); ++pos){
            InOutPort* port = static_cast<InOutPort*>(*pos);
            if(con->CanStart() && 
                port->CanStart() && 
                port->Index() == con->Index()){
                delete *pos;
                *pos = con;
            }
        }        
    }

    in.CloseElement();
}

void ProcessFigure::WriteFilter(IDOMOutput& out){
    out.OpenElement(kFilter);
    Class* type = Reflector::GetClass(&filter_->GetObject());
    out.AddAttribute(kClass, CString(type->GetID().c_str()));

    vector<dip::Property*> properties;
    type->EnumProperties(properties);
    out.OpenElement(kProperty);

    for(vector<dip::Property*>::iterator itor = properties.begin();
        itor != properties.end();
        ++itor){
        dip::Property* property = *itor;
        dip::VariantData* data = property->Get(&filter_->GetObject());
        
        out.OpenElement(kItem);

        out.AddAttribute(kKey, CString(property->GetName().c_str()));
        out.AddAttribute(kValue, CString(data->ToString().c_str()));
        out.CloseElement();
    }

    out.CloseElement();
    out.CloseElement();
}

void ProcessFigure::ReadFilter(IDOMInput& in){
    in.OpenElement(kFilter);

    CString class_id = in.GetAttribute(kClass, CString(""));
    IConnectable* unit = facotry_->CreateObject((const wchar_t*)class_id);
    if(!unit){
        LOG_ERROR(_T("[ProcessFigure::ReadFilter][CreateObject failed %s]"),
            class_id);

        in.CloseElement();
        return;
    }

    SetFilter(unit);

    VariantData* data       = NULL;
    Class* type             = unit->GetObject().GetClass();
    dip::Property* property = NULL;

    in.OpenElement(kProperty);
    for(int i = 0; i < in.GetElementCount(); ++i){   
        in.OpenElement(i);
        CString name = in.GetAttribute(kKey, CString(""));
        property = type->GetProperty((const wchar_t*)name);

        data = property->Get(&unit->GetObject());
        CString value = in.GetAttribute(kValue, CString(""));
        data->SetFromString((const wchar_t*)(value));
        property->Set(&unit->GetObject(), data);

        in.CloseElement();
    }

    // NOTE:It's a trick here. It doesn't succeed if set the readonly property name.
    // TODO:So consider the last property must be the name and call the set_name 
    // method of Object directly.
    unit->GetObject().set_name((wstring)*data);

    in.CloseElement();
    in.CloseElement();
}

void ProcessFigure::CreateConnector(){    
    for(int i = 0, num = filter_->GetNumInPins(); i < num; ++i){
        IConnector* connector = new InOutPort(this, false, i);
        connectors_.push_back(connector);
        connector->AddConnectorObserver(this);
    }

    for(int i = 0, num = filter_->GetNumOutPins(); i < num; ++i){
        IConnector* connector = new InOutPort(this, true, i);
        connectors_.push_back(connector);
        connector->AddConnectorObserver(this);
    }
}

vector<IConnector*>& ProcessFigure::GetConnectors(){
    if(!connectors_.size()){
        CreateConnector();
    }
    return connectors_;
}

ProcessFigure* ProcessFigure::Clone(){
    IConnectable* filter = NULL;
    if(filter_.get()){
        filter = facotry_->CreateObject(filter_->GetObject().GetClass()->GetID());
    }
    ProcessFigure* figure = new ProcessFigure(facotry_.get());
    figure->SetFilter(filter);

    return figure;
}

void ProcessFigure::Register(){
    if(filter_.get()){
        filter_->Register();
    }
}

void ProcessFigure::UnRegister(){
    if(filter_.get()){
        filter_->UnRegister();
    }
}