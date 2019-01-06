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

#include "runtime/enumerator.h"

namespace dip{

EnumRegistry* EnumRegistry::instance_ = NULL;

EnumRegistry* EnumRegistry::GetInstance(){
    if(instance_ = NULL){
        instance_ = new EnumRegistry();
    }

    return instance_;
}

bool EnumRegistry::AddEnum(const wstring& name, EnumBase *e){    
    if(!GetEnum(name)){
        GetInstance()->enum_map_.insert(make_pair(name, e));
        return true;
    }

    return false;
}

EnumBase *EnumRegistry::GetEnum(const wstring& name){
    map<wstring, EnumBase *>& enumMap = GetInstance()->enum_map_;
    map<wstring, EnumBase *>::iterator iter = enumMap.find(name);

    return iter == enumMap.end() ? NULL : iter->second;
}

EnumBase *EnumRegistry::FindEnum(const wstring& name){
    return GetEnum(name);
}

}