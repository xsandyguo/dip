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

#ifndef DIP_RT_VARIANT_DATA_H_
#define DIP_RT_VARIANT_DATA_H_

#include <string>

#include "runtime/common.h"
#include "runtime/enumerator.h"

namespace dip{

enum DipType{
    DipUndefined = 0,
    DipInt,
    DipUInt,
    DipLong,
    DipULong,
    DipShort,
    DipUShort,
    DipChar,
    DipUChar,
    DipDouble,
    DipFloat,
    DipObj,
    DipCon,
    DipBool,
    DipString,
    DipEnum,
    DT_MAX
};

class VariantData;

class ITypeSerializer{
public:
    virtual bool Read(VariantData* to, const wstring& from) = 0;
    virtual bool Write(const VariantData& data, wstring* to) = 0;

protected:
    ITypeSerializer(){}
    virtual ~ITypeSerializer(){}
};

struct TypeDescriptor{
    DipType type; 
    const wchar_t* name;
    const wchar_t* code;
    ITypeSerializer* const serializer;
};

struct TypeMap{
const TypeDescriptor* table;
int size;
};

extern const TypeMap kDipTypeMap;

class DIP_EXPORT VariantData{
public:
    DipType GetType() const { return this->type_; }
    void SetType(DipType type) { this->type_ = type; }

    VariantData()                          { this->type_ = DipUndefined                    ; }
    VariantData(const int &val)            { this->int_ = val                              ; type_ = DipInt; }
    VariantData(const unsigned int &val)   { this->uint_ = val                             ; type_ = DipUInt;}
    VariantData(const long &val)           { this->long_ = val                             ; type_ = DipLong; }
    VariantData(const unsigned long &val)  { this->ulong_ = val                            ; type_ = DipULong; }
    VariantData(const short &val)          { this->short_ = val                            ; type_ = DipShort; }
    VariantData(const unsigned short &val) { this->ushort_ = val                           ; type_ = DipUShort; }
    VariantData(const char &val)           { this->char_ = val                             ; type_ = DipChar; }
    VariantData(const unsigned char &val)  { this->uchar_ = val                            ; type_ = DipUChar; }
    VariantData(const float &val)          { this->float_ = val                            ; type_ = DipFloat; }
    VariantData(const double &val)         { this->double_ = val                           ; type_ = DipDouble; }
    VariantData(const bool &val)           { this->bool_ = val                             ; type_ = DipBool; }
    VariantData(const wstring &val)        { this->str_ = val                              ; type_ = DipString; }
    VariantData(const wchar_t *val)        { this->str_ = val                              ; type_ = DipString; }
    VariantData(wchar_t *val)              { this->str_ = val                              ; type_ = DipString; }
    VariantData(const EnumBase& val)       { this->enum_.Set(const_cast<EnumBase *>(&val) ); type_ = DipEnum; }
    VariantData(EnumBase& val)             { this->enum_.Set((&val) )                      ; type_ = DipEnum; }
    VariantData(EnumBase* val)             { this->enum_.Set((val) )                       ; type_ = DipEnum; }
   
    operator int   ()         const {return int_; }
    operator unsigned int ()  const {return uint_; }
    operator long  ()         const {return long_; }
    operator unsigned long () const {return ulong_; }
    operator short ()         const {return short_; }
    operator unsigned short() const {return ushort_; }
    operator char ()          const {return char_; }
    operator unsigned char()  const {return uchar_; }
    operator float ()         const {return float_; }
    operator double ()        const {return double_; }
    operator bool  ()         const {return bool_; }
    operator wstring ()       const {return str_; }
    operator const wchar_t*() const {return str_.c_str(); }
    operator wchar_t*()       const {return (wchar_t *) str_.c_str(); }
    operator EnumBase* ()     const {return this->enum_.GetEnum(); }
    operator EnumBase& ()     const {return *this->enum_.GetEnum(); }

    VariantData& operator =(const int &val) {           int_    = val                          ; type_ = DipInt;      return *this; }
    VariantData& operator =(const unsigned int &val) {  uint_   = val                          ; type_ = DipUInt;     return *this; }
    VariantData& operator =(const long &val) {          long_   = val                          ; type_ = DipLong;     return *this; }
    VariantData& operator =(const unsigned long &val) { ulong_  = val                          ; type_ = DipInt;      return *this; }
    VariantData& operator =(const short &val) {         short_  = val                          ; type_ = DipShort;    return *this; }
    VariantData& operator =(const unsigned short &val) {ushort_ = val                          ; type_ = DipUShort;   return *this; }
    VariantData& operator =(const char &val) {          char_   = val                          ; type_ = DipChar;     return *this; }
    VariantData& operator =(const unsigned char &val) { uchar_  = val                          ; type_ = DipUChar;    return *this; }
    VariantData& operator =(const float &val) {         float_  = val                          ; type_ = DipFloat;    return *this; }
    VariantData& operator =(const double &val) {        double_ = val                          ; type_ = DipDouble;   return *this; }
    VariantData& operator =(const bool &val) {          bool_   = val                          ; type_ = DipBool;     return *this; }
    VariantData& operator =(const wstring &val) {       str_ = val                          ; type_ = DipString;   return *this; }
    VariantData& operator =(const wchar_t *val) {       str_ = val                          ; type_ = DipString;   return *this; }
    VariantData& operator =(wchar_t *val) {             str_ = val                          ; type_ = DipString;   return *this; }
    VariantData& operator =(EnumBase *val) {            enum_.Set(val)                         ; type_ = DipEnum;     return *this; }
    VariantData& operator =(const EnumBase &val) {      enum_.Set(const_cast<EnumBase*>(&val)) ; type_ = DipEnum;     return *this; }
    
    virtual wstring ToString() const;
    virtual void MakeDefault();
    bool SetFromString(const wstring &value);

private:
    union{
        int            int_;
        unsigned int   uint_;
        long           long_;
        unsigned long  ulong_;
        short          short_;
        unsigned short ushort_;
        char           char_;
        unsigned char  uchar_;
        float          float_;
        double         double_;
        bool           bool_;
        EnumValue      enum_;
    };

    wstring str_;
    DipType type_;
};

DIP_EXPORT wstring FindTypeCode(const wstring& typeStr);
DIP_EXPORT DipType FindDipType(const wstring& typeStr);
DIP_EXPORT wstring GetTypeString(DipType type);
DIP_EXPORT const TypeDescriptor& GetTypeDescriptor(DipType type);

} // namespace dip
#endif