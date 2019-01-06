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

#ifndef DIP_RT_OBJECT_H_
#define DIP_RT_OBJECT_H_

#include <string>
#include <iostream>
#include "runtime/common.h"
#include "runtime/indent.h"
#include "runtime/timestamp.h"
#include "runtime/set_get.h"
#include "runtime/class_info.h"

namespace dip{
using namespace std;
class DIP_EXPORT NullObject{
public:
    virtual bool IsA(wstring type){
        return false;
    } 
};

class DIP_EXPORT Object : public NullObject{
public:
    DECLEAR_CLASSINFO(Object, NullObject)
      
    Class *GetClass();
    wstring GetTypeName();
    virtual void Delete(); 
    uint64 GetModifiedTime();
    void Modified();

    wstring get_name() {return this->name_; }
    void set_name(const wstring& name) { this->name_ = name; } 

protected:
    Object(); 
    virtual ~Object(); 

private:

    TimeStamp modified_time_;
    wstring name_;

    DISALLOW_EVIL_CONSTRUCTORS(Object);
};

} //namespace dip
#endif

