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

#ifndef DIP_RT_TYPE_DESCRIPTOR_H_
#define DIP_RT_TYPE_DESCRIPTOR_H_

#include "runtime/variant_data.h"
namespace dip{

template<typename VALUE_TYPE>
class NumericSerializer : public ITypeSerializer{
public:
    NumericSerializer(const wchar_t* const fmt)
        :format_(fmt){}

    virtual bool Read(VariantData* to, const wstring& from){
        ASSERT1(to);
        VALUE_TYPE val;
        if(_sntscanf(from.c_str(), from.size(), format_.c_str(), &val) == 1){
            *to = val;
            return true;
        }

        return false;
    }

    virtual bool Write(const VariantData& data, wstring* to){
        ASSERT1(to);

        TCHAR buffer[64];
        buffer[0] = _T('\0');
        VALUE_TYPE val = data;
        _sntprintf_s(buffer, sizeof(buffer), format_.c_str(), val);

        *to = wstring(buffer);

        return true;
    }

private:
    wstring format_;
    DISALLOW_EVIL_CONSTRUCTORS(NumericSerializer);
};

class BoolSerializer : public ITypeSerializer{
public:
    BoolSerializer(){}
    virtual bool Read(VariantData* to, const wstring& from);
    virtual bool Write(const VariantData& data, wstring* to);
private:
    DISALLOW_EVIL_CONSTRUCTORS(BoolSerializer);
};

class StringSerializer : public ITypeSerializer{
public:
    StringSerializer(){}

    virtual bool Read(VariantData* to, const wstring& from);
    virtual bool Write(const VariantData& data, wstring* to);
private:
    DISALLOW_EVIL_CONSTRUCTORS(StringSerializer);
};

class EnumSerializer : public ITypeSerializer{
public:
    EnumSerializer(){}

    virtual bool Read(VariantData* to, const wstring& from);
    virtual bool Write(const VariantData& data, wstring* to);
private:
    DISALLOW_EVIL_CONSTRUCTORS(EnumSerializer);
};

}

#endif