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
#include "framework/abstract_tool.h"

/** 
*  @brief ����ѡ������� 
* ʵ�����϶�ѡ��ѡ��Ԫ�� 
*/
class SelectionAreaTracker : public AbstractTool{
public:
    SelectionAreaTracker();
    virtual ~SelectionAreaTracker();
    virtual void MouseDown(MouseEventArgs& evt);
    virtual void MouseUp(MouseEventArgs& evt);
    virtual void MouseMove(MouseEventArgs& evt);
    virtual void Draw(Graphics& g);
private:
    void ClearRubberBand();

    Rect rubber_band_;

    DISALLOW_EVIL_CONSTRUCTORS(SelectionAreaTracker);
};