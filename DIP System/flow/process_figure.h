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

#ifndef DIP_FLOWEDITOR_PROCESS_FIGURE_H_
#define DIP_FLOWEDITOR_PROCESS_FIGURE_H_

#include <afxstr.h>
#include "base/basictypes.h"
#include "base/scoped_ptr.h"

#include "framework/framework.h"
#include "runtime/framework.h"
#include "flow/node_figure.h"
#include "io/factory.h"
#include "io/serialize_const.h"

using namespace dip;

class ProcessFigure : public NodeFigure{
public:
    ProcessFigure(IFigureFactory* factory);
    virtual ~ProcessFigure();

    virtual void Write(IDOMOutput& out);
    virtual void Read(IDOMInput& in);
    ProcessFigure* Clone();
    virtual void Draw(Graphics& g);

    IConnectable& GetFilter() const ;
    void SetFilter(IConnectable* filter);
    void GetToolTipAt(std::wstring *content, Point* p);
    const CString& GetName();
    virtual Rect GetDrawingArea();
    virtual void Release();

protected:

    virtual void DrawLabel(Graphics& g);
    virtual void WriteFilter(IDOMOutput& out);
    virtual void ReadFilter(IDOMInput& in);
    virtual void WriteConnector(IDOMOutput& out);
    virtual void ReadConnector(IDOMInput& in);

    virtual vector<IConnector*>& GetConnectors();
    virtual void CreateConnector();
    virtual RectF GetTextBound();
    virtual bool Contains(const Point& p);
    virtual std::wstring GetDescription();

private:
    void Register();
    void UnRegister();

    CString name_;
    scoped_ptr<IConnectable>   filter_;
    scoped_ptr<Gdiplus::Font>  font_;
    scoped_ptr<StringFormat>   format_;
    scoped_ptr<IFigureFactory> facotry_;

    typedef std::vector<IConnector*>::iterator ConnectorIter;
    std::vector<IConnector*> connectors_;

    DISALLOW_EVIL_CONSTRUCTORS(ProcessFigure);
};


#endif