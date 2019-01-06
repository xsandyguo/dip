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

#include "framework/abstract_tool.h"

class ConnectionTool : public AbstractTool {
public:
    ConnectionTool(IConnectionFigure* prototype);
    virtual ~ConnectionTool();

    void MouseMove(MouseEventArgs& e);
    void MouseDown(MouseEventArgs& e);
    void MouseUp(MouseEventArgs& e);

protected:
    IConnectionFigure* CreateConnection(IConnectionFigure* prototype);
    void TrackConnector(const Point& p);
    
    IConnectionFigure* FindConnection(const Point& p, IDrawing* drawing);
    void SetConnection(IConnectionFigure* newConnection);
    IConnectionFigure* GetConnection() ;

    IConnector* FindConnector(const Point& p, IDrawing* drawing);
    IConnector* FindStartConnector(const Point& p, IDrawing* drawing);
    IConnector* FindEndConnector(const Point& p, IDrawing* drawing);

    IFigure* FindConnectableFigure(const Point& p, IDrawing* drawing);

    void SetStartConnector(IConnector* newStartConnector);
    void SetEndConnector(IConnector* newEndConnector);
    IConnector* GetStartConnector();
    IConnector* GetEndConnector();    
    IConnectionFigure* CreateConnection();

private:
    void ResetTool();
    void ConnectAtIfNecessary(const Point& p);

    IConnector*   start_connector_;
    IConnector*   end_connector_;
    IConnector*   tracing_connector_;

    IConnectionFigure*  connection_;
    IConnectionFigure*  prototype_;

    DISALLOW_EVIL_CONSTRUCTORS(ConnectionTool);
};
