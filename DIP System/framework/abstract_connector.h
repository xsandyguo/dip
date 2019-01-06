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

#include "base/basictypes.h"

#include "framework/framework.h"

class AbstractConnector : public IConnector {
public:
    AbstractConnector();
    virtual ~AbstractConnector(){}
    AbstractConnector(IFigure* owner) ;

    void AddConnectorObserver(IConnectorObserver* observer);
    void RemoveConnectorObserver(IConnectorObserver* observer);
    virtual void Hint(bool visible);

    virtual bool CanStart() = 0;
    virtual bool CanEnd() = 0;
    IFigure* Owner();
    virtual Rect GetBounds() ;
    virtual bool Contains(const Point& p) ;
    virtual bool IsConnected(){return is_connected_;}
    virtual void SetConnected(bool is_connected );
    virtual void Read(IDOMInput& in);
    virtual void Write(IDOMOutput& out);

protected:
    virtual void OnAreaInvalidated();
    virtual bool Hint(){return hint_visible_;}

private:
    ObserverCollection observers_;
    bool is_connected_;
    bool hint_visible_;

    IFigure *     owner_;
    DISALLOW_EVIL_CONSTRUCTORS(AbstractConnector);
};
