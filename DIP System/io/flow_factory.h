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

#include <afxstr.h>
#include <map>
#include "base/basictypes.h"
#include "io/factory.h"

struct TypeTriple{
    TypeTriple(const TCHAR* n, const type_info& t, IDOMStorable* const p)
        :name(n), type(t), prototype(p){}
    const TCHAR* name;
    const type_info& type;
    IDOMStorable* const prototype;
};

class FlowFactory : public IDOMFactory,
    public IFigureFactory{
public:    
    static FlowFactory& Instance();
    CString GetName(IDOMStorable* o);
    IDOMStorable* Create(const CString& name);
    void Write(IDOMOutput& out, IDOMStorable* object);
    IDOMStorable* Read(IDOMInput& in);

    IFigure* CreateFigure(const dip::ClassId& id);
    dip::IConnectable* CreateObject(const dip::ClassId& id);

private:
    FlowFactory();
    void Init();

    static FlowFactory* instance_;
    std::vector<TypeTriple*> type_map_;
    std::map<TCHAR*, IDOMStorable*> prototypes_;
    DISALLOW_EVIL_CONSTRUCTORS(FlowFactory);
};