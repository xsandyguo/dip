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

#include "runtime/object.h"
#include "runtime/reflector.h"
namespace dip{

BEGIN_REGISTER_CLASSINFO( Object, NullObject,\
    "{3C46EF30-D712-4506-B767-022C1116EFD4}")
    READONLY_PROPERTY (wstring, "name" , get_name)
END_REGISTER_CLASSINFO( Object)

Object::Object(){
}

Object::~Object(){
}

Class* Object::GetClass(){
    return Reflector::GetClass(this);
}

wstring Object::GetTypeName(){
    return GetClass()->GetClassName();
}

void Object::Delete(){
    delete this;
}

uint64 Object::GetModifiedTime(){
    return modified_time_.GetModifiedTime();
}

void Object::Modified(){
    modified_time_.Modified();
}

} // namespace dip