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
#include <afxstr.h>
#include <string>

class IDOMStorable;

class  IDOMInput {
public:
    IDOMInput(){}
    virtual ~IDOMInput(){}
    virtual CString GetTagName() = 0;
    virtual CString GetAttribute(const CString& name, const CString& defaultValue) = 0;
    virtual int GetAttribute(const CString& name, int defaultValue) = 0;
    virtual double GetAttribute(const CString& name, double defaultValue) = 0;
    virtual bool GetAttribute(const CString& name, bool defaultValue) = 0;

    virtual int GetElementCount() = 0;
    virtual void OpenElement(const CString& tagName) = 0;
    virtual void OpenElement(int i) = 0;
    virtual void CloseElement() = 0;
    virtual IDOMStorable* ReadObject() = 0;
    virtual IDOMStorable* ReadObject(int i) = 0;
private:
    DISALLOW_EVIL_CONSTRUCTORS(IDOMInput);
};

class  IDOMOutput {
public:
    IDOMOutput(){}
    virtual ~IDOMOutput(){}
    virtual CString GetTagName() = 0;
    virtual void AddAttribute(const CString& name, const CString& val) = 0;
    virtual void AddAttribute(const CString& name, int val) = 0;
    virtual void AddAttribute(const CString& name, double val) = 0;
    virtual void AddAttribute(const CString& name, bool val) = 0;

    virtual void OpenElement(const CString& tagName) = 0;
    virtual void CloseElement() = 0;
    virtual void WriteObject(IDOMStorable* figure) = 0;
private:
    DISALLOW_EVIL_CONSTRUCTORS(IDOMOutput);
};

class IDOMStorable{
public:
    IDOMStorable(){}
    virtual ~IDOMStorable(){}
    virtual void Write(IDOMOutput& out) = 0;
    virtual void Read(IDOMInput& in) = 0;
    virtual IDOMStorable* Clone() = 0;

private:
    DISALLOW_EVIL_CONSTRUCTORS(IDOMStorable);
};
