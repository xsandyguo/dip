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

#include <afxwin.h>
#include <afxstr.h>
#include <vector>

#include <base/basictypes.h>

class IAppModel{
public:
    virtual ~IAppModel(){}

    virtual bool Initialize()                   = 0;
    virtual CMultiDocTemplate& GetDocTemplate() = 0;
    virtual CDocument* CreateDocument()         = 0;
    virtual CString GetFilterStr()       = 0;
    virtual void OpenFile(const wchar_t* file_name) = 0;
    virtual bool IsSupport(const wchar_t* file_ext) = 0;

protected:
    IAppModel(){}
private:
    DISALLOW_COPY_AND_ASSIGN(IAppModel);
};

class AppModel : public IAppModel{
public:
    AppModel():doc_template_(NULL){}
    virtual ~AppModel(){}

    CMultiDocTemplate& GetDocTemplate(){ return *doc_template_; }

protected:
    CMultiDocTemplate* doc_template_;
private:
    DISALLOW_COPY_AND_ASSIGN(AppModel);
};

extern std::vector<IAppModel*> g_app_models;
