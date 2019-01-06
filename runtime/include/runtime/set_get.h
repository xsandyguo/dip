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

#ifndef DIP_RT_SET_GET_H_
#define DIP_RT_SET_GET_H_
#include <string.h>
#include <strstream>

#include "base/debug.h"
#include "runtime/common.h"

namespace dip{
//
// Set built-in type.  Creates member Set"name"() (e.g., SetVisibility());
//
#define SetMacro(name,type) \
virtual void set_##name (type _arg) \
  { \
  if (this->name##_ != _arg) \
    { \
    this->name##_ = _arg; \
    this->Modified(); \
    } \
  } 
//
// Set character wstring.  Creates member Set"name"() 
// (e.g., SetFilename(wchar_t *));
//
#define SetStringMacro(name) \
virtual void set_##name (const wchar_t* _arg) \
  { \
  if ( this->name##_ && _arg && (!strcmp(this->name##_, _arg))) { return;} \
  if (this->name##_) { delete [] this->name##_; } \
  if (_arg) \
    { \
    this->name##_ = new char[strlen(_arg)+1]; \
    strcpy(this->name##_, _arg); \
    } \
   else \
    { \
    this->name##_ = NULL; \
    } \
  this->Modified(); \
  } 

//
// General set vector macro creates a single method that copies specified
// number of values into object.
// Examples: void SetColor(c,3)
//
#define SetVectorMacro(name,type,count) \
virtual void set_##name(type data[]) \
{ \
  int i; \
  for (i=0; i < count; i++) { if ( data[i] != this->name##_[i] ) { break; }} \
  if ( i < count ) \
    { \
    this->Modified(); \
    for (i = 0; i < count; i++) { this->name##_[i] = data[i]; }\
    } \
}

//
// Get vector macro defines two methods. One returns pointer to type 
// (i.e., array of type). This is for efficiency. The second copies data
// into user provided array. This is more object-oriented.
// Examples: float *GetColor() and void GetColor(float c[count]).
//
#define GetVectorMacro(name,type,count) \
virtual type *get_##name () \
{ \
  return this->name##_; \
} \
virtual void get_##name (type data[count]) \
{ \
  for (int i=0; i<count; i++) { data[i] = this->name##_[i]; }\
}

#define SetVector4Macro(name,type) \
virtual void set_##name (type _arg1, type _arg2, type _arg3, type _arg4) \
  { \
  if ((this->name##_[0] != _arg1)||(this->name##_[1] != _arg2)||(this->name##_[2] != _arg3)||(this->name##_[3] != _arg4)) \
    { \
    this->Modified(); \
    this->name##_[0] = _arg1; \
    this->name##_[1] = _arg2; \
    this->name##_[2] = _arg3; \
    this->name##_[3] = _arg4; \
    } \
  }; \
virtual void set_##name (type _arg[4]) \
  { \
  this->set_##name (_arg[0], _arg[1], _arg[2], _arg[3]);\
  } 

//
// Get built-in type.  Creates member Get"name"() (e.g., GetVisibility());
//
#define GetMacro(name,type) \
virtual type get_##name () { \
  return this->name##_; \
  } 

//
// Get character wstring.  Creates member Get"name"() 
// (e.g., wchar_t *GetFilename());
//
#define GetStringMacro(name) \
virtual char* Get##name () { \
  return this->name; \
  } 


//
// Set built-in type where value is constrained between min/max limits.
// Create member Set"name"() (e.q., SetRadius()). #defines are 
// convienience for clamping open-ended values.
//
#define SetClampMacro(name,type,min,max) \
virtual void set_##name (type _arg) \
  { \
  if (this->name##_ != (_arg<min?min:(_arg>max?max:_arg))) \
    { \
    this->name##_ = (_arg<min?min:(_arg>max?max:_arg)); \
    this->Modified(); \
    } \
  } 


//
// Create members "name"On() and "name"Off() (e.g., DebugOn() DebugOff()).
// Set method must be defined to use this macro.
//
#define BooleanMacro(name,type) \
virtual void name##_on () { this->set_##name((type)1);}; \
virtual void name##_off () { this->set_##name((type)0);}

} // namespace dip
#endif