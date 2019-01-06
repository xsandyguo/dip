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
#include <afxstr.h>
#include <runtime/class.h>
#include <runtime/framework.h>

#include "io/io_format.h"
#include "framework/framework.h"

class IDOMFactory{
public:
    IDOMFactory(){}
    virtual ~IDOMFactory(){}
    virtual CString GetName(IDOMStorable* o) = 0;
    virtual void Write(IDOMOutput& out, IDOMStorable* object) = 0;
    virtual IDOMStorable* Read(IDOMInput& in) = 0;

private:
    DISALLOW_EVIL_CONSTRUCTORS(IDOMFactory);
};

using dip::ClassId;

class IFigureFactory{
public:
    IFigureFactory(){}
    virtual ~IFigureFactory(){}
    virtual dip::IConnectable* CreateObject(const ClassId& id) = 0;
    virtual IFigure* CreateFigure(const ClassId& id) = 0;
private:
    DISALLOW_EVIL_CONSTRUCTORS(IFigureFactory);
};

