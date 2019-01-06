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

#include "runtime/type_descriptor.h"
#include "base/debug.h"

namespace dip{
/*
      code	|	primitive type
    ----------------------------	
      "-i"  |		integer
      "+i"  |       unsigned integer
      "-l"	|		long
      "+l"	|		unsigned long
      "-h"	|		short
      "+h"	|		unsigned short
      "-c"	|		char
      "+c"	|		unsigned char
      "-f"	|		float
      "-d"	|		double
      "-b"	|		bool
      "-s"	|		String or String&
      "-e"	|		Enum* 
*/

// NOTE:This table must be initialized before any all other constructors
// Any other constructors is located in .CRT$XCU
#pragma init_seg(".CRT$XCT")
const TypeDescriptor kTypeDescriptorTable [] = {
    {DipInt,    _T("int")           ,_T("-i"), new NumericSerializer<int>(_T("%d"))},
    {DipUInt,   _T("unsigned int")  ,_T("+i"), new NumericSerializer<unsigned int>(_T("%u"))},
    {DipLong,   _T("long")          ,_T("-l"), new NumericSerializer<long>(_T("%d"))},
    {DipULong,  _T("unsigned long") ,_T("+l"), new NumericSerializer<unsigned long>(_T("%u"))},
    {DipULong,  _T("DWORD")         ,_T("+l"), new NumericSerializer<DWORD>(_T("%u"))},
    {DipShort,  _T("short")         ,_T("-h"), new NumericSerializer<short>(_T("%d"))},
    {DipUShort, _T("unsigned short"),_T("+h"), new NumericSerializer<unsigned short>(_T("%u"))},
    {DipUShort, _T("WORD")          ,_T("+h"), new NumericSerializer<WORD>(_T("%u"))},
    {DipChar,   _T("char")          ,_T("-c"), new NumericSerializer<char>(_T("%c"))},
    {DipUChar,  _T("unsigned char") ,_T("+c"), new NumericSerializer<unsigned char>(_T("%c"))},
    {DipUChar,  _T("BYTE")          ,_T("+c"), new NumericSerializer<BYTE>(_T("%c"))},
    {DipFloat,  _T("float")         ,_T("-f"), new NumericSerializer<float>(_T("%f"))},
    {DipDouble, _T("double")        ,_T("-d"), new NumericSerializer<double>(_T("%f"))},
    {DipBool,   _T("bool")          ,_T("-b"), new BoolSerializer()},
    {DipString, _T("wchar_t *")     ,_T("-s"), new StringSerializer()},
    {DipString, _T("wstring")       ,_T("-s"), new StringSerializer()},
    {DipEnum,   _T("enum")          ,_T("-e"), new EnumSerializer()}};

const TypeMap kDipTypeMap = {kTypeDescriptorTable, arraysize(kTypeDescriptorTable) };


bool BoolSerializer::Read(VariantData* to, const wstring& from){
    ASSERT1(to);

    (*to) = from == _T("true");
    return true;
}

bool BoolSerializer::Write(const VariantData& data, wstring* to){
    ASSERT1(to);

    *to = data ? _T("true") : _T("false");
    return true;
}

bool StringSerializer::Read(VariantData* to, const wstring& from){
    ASSERT1(to);

    (*to) = from.c_str();
    return true;
}

bool StringSerializer::Write(const VariantData& data, wstring* to){
    ASSERT1(to);

    *to = (wstring)data;
    return true;
}

bool EnumSerializer::Read(VariantData* to, const wstring& from){
    ASSERT1(to);

    EnumBase& enumVal = *to;
    enumVal.SetFromString(from);
    return true;
}

bool EnumSerializer::Write(const VariantData& data, wstring* to){
    ASSERT1(to);

    *to = ((EnumBase&)data).ToString();
    return true;
}


}