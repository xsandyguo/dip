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
#include "framework/framework.h"
#include "framework/abstract_figure.h"

class NodeFigure : public AbstractFigure, public IConnectorObserver{
public:
    NodeFigure(void);
    virtual ~NodeFigure(void);
    virtual void Draw(Graphics& g);
    virtual Rect GetBounds();
    virtual Rect GetDrawingArea();

    virtual bool Contains(const Point& p);
    virtual IConnector* ConnectorAt(const Point& p);
    void AreaInvalidated(ConnectorEventArgs& evt);

    virtual void Write(IDOMOutput& out);
    virtual void Read(IDOMInput& in);

protected:
    virtual void BasicSetBounds(const Point& orgin, const Point& corner);
    virtual void DrawConnector(Graphics& g);
    virtual void DrawBorder(Graphics& g);
    virtual vector<IConnector*>& GetConnectors() = 0;
    
private:
    Rect bounds_;

    DISALLOW_EVIL_CONSTRUCTORS(NodeFigure);
};

