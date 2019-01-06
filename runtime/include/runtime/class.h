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

#ifndef DIP_RT_CLASS_H_
#define DIP_RT_CLASS_H_

#include <typeinfo.h>
#include <map>
#include <vector>
#include <string>
#include "runtime/common.h"
#include "runtime/method.h"
#include "runtime/property.h"

namespace dip{
typedef std::wstring ClassId;
typedef std::map<wstring, Method*> TMethodMap;
typedef std::map<wstring, Property*> TPropertyMap;

class DIP_EXPORT Class {
public:
    Class(const wstring& classname, 
        const wstring& classID, 
        const wstring& superClass, 
        bool is_abstract);
    virtual ~Class();

    Class* GetSuperClass();
    wstring GetID();
    wstring GetClassName();

    void AddMethod(Method *method);
    bool HasMethod(const wstring& name);
    int GetMethodCount();
    Method* GetMethod(const wstring& name);
    void AddProperty(Property *prop);
    bool HasProperty(const wstring& name);
    bool IsAbstract() const{ return is_abstract_;}

    int GetPropertyCount();
    Property* GetProperty(const wstring& name);
    virtual bool IsEqual(Object *object) const { return CompareObject(object) ; }

    virtual Object *CreateInstance() = 0;	 
    virtual bool CompareObject(Object *object) const = 0;
    
    void SetSource(Object* source);

    void EnumMethodNames(vector<wstring> &names);
    void EnumProperties(vector<Property*> &props);
    void EnumMethods(vector<Method*> &methods);

    void SetDescription(wstring str);
    wstring GetDescription();

protected:
    bool HasSupper();

    wstring class_name_;
    wstring class_id_;
    wstring super_name_;
    Class*  super_;
    bool is_abstract_;

    typedef std::map<wstring, Method*>::iterator MethodIter;
    typedef std::map<wstring, Property*>::iterator PropertyIter;
   
    map<wstring, Method*>   methods_;
    map<wstring, Property*> properties_;

    wstring description_;
private:
    DISALLOW_EVIL_CONSTRUCTORS(Class);
};

typedef Object* (*DynamicObjectCreate)();

template<class CLASS_TYPE> class TypedClass : public Class {
public:
    TypedClass(const wstring& className, 
                const wstring& classID,
                const wstring& superClass, 
                DynamicObjectCreate create=0) :
      Class(className, classID, superClass, false){
          this->create_fun_ = create;
      }
      virtual ~TypedClass() { }

      virtual Object* CreateInstance(){
          if(this->create_fun_){
            return this->create_fun_();
          }

          return 0;
      }

      virtual bool CompareObject(Object *object) const {
//		  return object->IsA(this->ClassName);
          return ( typeid(*object) == typeid(CLASS_TYPE) );
      }

private:
    DynamicObjectCreate create_fun_;
    DISALLOW_EVIL_CONSTRUCTORS(TypedClass);
};

template<class CLASS_TYPE> class TypedAbstractClass : public Class{
public:
    TypedAbstractClass(const wstring& className, 
                        const wstring& classID, 
                        const wstring& superClass) :
      Class(className, classID, superClass, true){
          
      }
      virtual ~TypedAbstractClass() { }

      virtual Object* CreateInstance(){
          return 0;
      }

      virtual bool CompareObject(Object *object) const  {
//		  return object->IsA(this->ClassName);
          return ( typeid(*object) == typeid(CLASS_TYPE) );
      }
private:
    DISALLOW_EVIL_CONSTRUCTORS(TypedAbstractClass);
};
} // namespace dip

#endif
