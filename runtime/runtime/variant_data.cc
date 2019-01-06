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

#include "runtime/variant_data.h"

namespace dip{

wstring VariantData::ToString() const{   
    if(type_ == DipUndefined){
        return _T("");
    }

   wstring str;
   ITypeSerializer* serializer = GetTypeDescriptor(type_).serializer;
   ASSERT1(serializer);

   VERIFY1(serializer->Write(*this, &str));
   return str;
}

void VariantData::MakeDefault(){
    // TODO:
}

bool VariantData::SetFromString(const wstring &value){    
    ITypeSerializer* serializer = GetTypeDescriptor(type_).serializer;
    ASSERT1(serializer);

    return serializer->Read(this, value);
}


static wstring FindTypeCode(const wstring& typeStr){
    for (int i = 0; i < kDipTypeMap.size; ++i) {
        if(kDipTypeMap.table[i].name == typeStr){
            return kDipTypeMap.table[i].code;
        }
    }

    return _T("");
}

DipType FindDipType(const wstring& typeStr){
    for (int i = 0; i < kDipTypeMap.size; ++i) {
        if(kDipTypeMap.table[i].name == typeStr){
            return kDipTypeMap.table[i].type;
        }
    }

    return DipUndefined;
}

wstring GetTypeString(DipType type){
    for (int i = 0; i < kDipTypeMap.size; ++i) {
        if(kDipTypeMap.table[i].type == type){
            return kDipTypeMap.table[i].name;
        }
    }

    ASSERT1(false);
    return _T("");
}

const TypeDescriptor& GetTypeDescriptor(DipType type){
    for (int i = 0; i < kDipTypeMap.size; ++i) {
        if(kDipTypeMap.table[i].type == type){
            return kDipTypeMap.table[i];
        }
    }

    ASSERT1(false);
    return *((TypeDescriptor*)NULL);
}


} // namespace dip