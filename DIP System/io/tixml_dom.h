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
#include <stack>
#include <map>
#include <tinyxml/tinyxml.h>

#include "io/io_format.h"
#include "base/utility.h"
#include "base/scoped_ptr.h"

class IDOMFactory;
class TiXmlDomInput : public IDOMInput {
public:
    TiXmlDomInput(const CString& file_path, IDOMFactory& factory);
    virtual ~TiXmlDomInput();
    virtual CString GetTagName();

    virtual CString GetAttribute(const CString& name, const CString& defaultValue);
    virtual int GetAttribute(const CString& name, int defaultValue);
    virtual double GetAttribute(const CString& name, double defaultValue);
    virtual bool GetAttribute(const CString& name, bool defaultValue);

    virtual int GetElementCount();
    virtual void OpenElement(const CString& tagName);
    virtual void OpenElement(int i);
    virtual void CloseElement();

    virtual IDOMStorable* ReadObject();
    virtual IDOMStorable* ReadObject(int i);
private:
    std::stack<TiXmlElement*> stack_;
    TiXmlElement* current_;
    scoped_ptr<TiXmlDocument> document_;
    IDOMFactory& factory_;
    std::map<int, IDOMStorable*> idobjects_;
};

class TiXmlDomOutput : public IDOMOutput{
public:
    TiXmlDomOutput(IDOMFactory& factory);
    virtual ~TiXmlDomOutput();
    virtual void AddAttribute(const CString& name, const CString& val);
    virtual void AddAttribute(const CString& name, int val);
    virtual void AddAttribute(const CString& name, double val);
    virtual void AddAttribute(const CString& name, bool val);

    virtual CString GetTagName();
    virtual void OpenElement(const CString& tagName);
    virtual void CloseElement();
    virtual void Save(const CString& file_path);

    virtual void WriteObject(IDOMStorable* storable);
private:
    std::stack<TiXmlElement*> stack_;
    TiXmlElement* current_;
    scoped_ptr<TiXmlDocument> document_;
    IDOMFactory& factory_;
    std::map<IDOMStorable*, int> idobjects_;
};
