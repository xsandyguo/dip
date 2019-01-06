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
#include "framework/abstract_handle.h"

class ChangeConnectionHandle : public AbstractHandle {
public:
    virtual void TrackStart(const Point& anchor);
    virtual void TrackStep(const Point& anchor, const Point& lead, UINT nFlag);
    virtual void TrackEnd(const Point& anchor, const Point& lead, UINT nFlag);

    IConnector* FindConnector(const Point& p);
    void Draw(Graphics& g) ;

protected: 
    ChangeConnectionHandle(IFigure* owner) ;
    virtual IConnector* Target() = 0;
    virtual IConnector* Source();

    virtual void Disconnect() = 0;
    virtual void Connect(IConnector* c) = 0;
    virtual void SetPoint(const Point& p) = 0;
    virtual bool CanConnect(IConnector* c) = 0;

protected:
    IConnector*         original_target;
    IConnector*         target_;
    IConnectionFigure*  connection_;
    Point               start_;

private:
    DISALLOW_EVIL_CONSTRUCTORS(ChangeConnectionHandle);
};
