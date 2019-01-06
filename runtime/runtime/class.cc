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

#include "runtime/class.h"

#include "base/debug.h"
#include "runtime/reflector.h"

namespace dip{

Class::Class(const wstring& classname, 
    const wstring& classID, 
    const wstring& superClass, 
    bool is_abstract):
    class_name_(classname),
    class_id_(classID),
    super_name_(superClass),
    super_(NULL),
    is_abstract_(is_abstract){

}

Class::~Class(){
}

bool Class::HasSupper(){
    return !!GetSuperClass();
}

Class* Class::GetSuperClass(){
    if (this->super_ == NULL) {
        this->super_ = Reflector::GetClass(this->super_name_);
    }
    return this->super_;
}

wstring Class::GetID(){
    return this->class_id_;
}

wstring Class::GetClassName(){
    return this->class_name_;
}

void Class::AddMethod(Method *method){
    ASSERT1(method);
    ASSERT1(!HasMethod(method->GetName()));

    this->methods_[method->GetName()] = method;
}
bool Class::HasMethod(const wstring& name){
    if(methods_.find(name) != methods_.end()){
        return true;
    }else{
        return HasSupper() && GetSuperClass()->HasMethod(name);
    }
}

int Class::GetMethodCount(){
    return this->methods_.size();
}

Method* Class::GetMethod(const wstring& name){
    ASSERT1(this->HasMethod(name));

    MethodIter pos = methods_.find(name);
    if(pos != methods_.end()){
        return pos->second;
    }

    return HasSupper() ? GetSuperClass()->GetMethod(name) : NULL;
}

void Class::AddProperty(Property *prop){
    ASSERT1(prop);
    ASSERT1(!HasProperty(prop->GetName()));

    this->properties_[prop->GetName()] = prop;
}

bool Class::HasProperty(const wstring& name){
    if(properties_.find(name) != properties_.end()){
        return true;
    }else{
        return HasSupper() && GetSuperClass()->HasProperty(name);
    }
}

int Class::GetPropertyCount(){
    return this->properties_.size();
}

Property* Class::GetProperty(const wstring& name){
    ASSERT1(this->HasProperty(name));

    PropertyIter pos = properties_.find(name);
    if(pos != properties_.end()){
        return pos->second;
    }

    return HasSupper() ? GetSuperClass()->GetProperty(name) : NULL;
}

void Class::SetSource(Object* source){

}

void Class::EnumMethodNames(vector<wstring> &names){
    map<wstring, Method*>::iterator iter = this->methods_.begin();
    while(iter != this->methods_.end()){
        names.push_back(iter->first);
        ++iter;            
    }

    if(HasSupper()){
        GetSuperClass()->EnumMethodNames(names);
    }
}

void Class::EnumProperties(vector<Property*> &props){
    map<wstring, Property*>::iterator iter = this->properties_.begin();
    while(iter != this->properties_.end()){
        props.push_back(iter->second);
        ++iter;            
    }

    if(HasSupper()){
        GetSuperClass()->EnumProperties(props);
    }
}

void Class::EnumMethods(vector<Method*> &methods){
    map<wstring, Method*>::iterator iter = this->methods_.begin();
    while(iter != this->methods_.end()){
        methods.push_back(iter->second);
        ++iter;            
    }

    if(HasSupper()){
        GetSuperClass()->EnumMethods(methods);
    }
}

void Class::SetDescription(wstring str){
    this->description_ = str;
}

wstring Class::GetDescription(){
    return this->description_;
}

} // namespace dip