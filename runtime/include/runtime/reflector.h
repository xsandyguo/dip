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

#ifndef DIP_RT_CLASS_REGISTRY_H_
#define DIP_RT_CLASS_REGISTRY_H_

#include <map>
#include <string>

#include "runtime/common.h"
#include "runtime/class.h"

namespace dip{
using std::map;
using std::vector;
using std::wstring;

typedef map<wstring, Class *> TClassMap;
typedef map<wstring, Class *> TClassIDMap;

class DIP_EXPORT Reflector{
public:
    static Class* GetClass(const wstring& className);
    static Class* GetClassFromID(const wstring& classID);
    static Class* GetClass(Object *object);

    static Object* CreateNewInstance(const wstring& className);
    static Object* CreateNewInstanceFromClassID(const wstring& classID);
    static void AddClass(const wstring& className, Class* classToRegister);
    static bool RegisterMethod(Method* method, const wstring& className);
    static bool RegisterMethodByClassID(Method* method, const wstring& classID);

    static void Init();
    static void Release();

    static void Dump(wostream& os);
    static void EnumClassNames(vector<wstring> &names);
    static bool IsDerivedFrom(const wstring& childName , const wstring& supperName);

    static void Load( const wchar_t* path);
protected:
    static Reflector& GetInstance();

private:
    Reflector(){}
    map<wstring, Class*> class_map_;
    map<wstring, Class*> class_id_map_;
    vector<HMODULE>   libraries_;

    static Reflector *instance_;
    DISALLOW_EVIL_CONSTRUCTORS(Reflector);
};

template<class T> bool RegisterClass(T* fakeParam, const wstring& className, 
                                     const wstring& superClassName,
                                     const wstring& classID,
                                     DynamicObjectCreate create=0){
    Class *clazz = Reflector::GetClass(className);

    if( 0 == clazz ){
        TypedClass<T> *objClass = new TypedClass<T>( className, classID, superClassName, create);
        Reflector::AddClass(className, objClass);

        return true;
    }

    return false;
}

template<class T> bool RegisterAbstractClass(T* fakeParam, const wstring& className, 
                                     const wstring& superClassName, const wstring& classID){
    Class *clazz = Reflector::GetClass(className);

    if( 0 == clazz ){
        TypedAbstractClass<T> *objClass = new TypedAbstractClass<T>( className, classID, superClassName);
        Reflector::AddClass(className, objClass);

        return true;
    }

    return false;
}

template<class SOURCE_TYPE, class METHOD_TYPE>  
void RegisterMethod0Return( SOURCE_TYPE* fakeParam,
                            const wstring& className, 
                            const wstring& methodName,
                            METHOD_TYPE methodPtr){

    wstring argTypes = _T("");
    Method* newMethod = new TypedMethodArg0Return<SOURCE_TYPE,METHOD_TYPE>( methodPtr, argTypes );
    newMethod->SetName( methodName );
    if ( false == Reflector::RegisterMethod( newMethod, className) ){
        delete newMethod;
        newMethod = 0;
    }	

};

template<class SOURCE_TYPE, class METHOD_TYPE>  
void RegisterMethod1Return( SOURCE_TYPE* fakeParam,
                            const wstring& className,
                            const wstring& methodName,
                            METHOD_TYPE methodPtr, 
                            const wstring& argType0){
    wstring argTypes = FindTypeCode(argType0);
    if(argTypes.size() == 2){
        Method* newMethod = new TypedMethodArg1Return<SOURCE_TYPE,METHOD_TYPE>( methodPtr, argTypes );
        newMethod->SetName( methodName );
        if ( false == Reflector::RegisterMethod( newMethod, className) ){
            delete newMethod;
            newMethod = 0;
        }
    }

};

template<class SOURCE_TYPE, class METHOD_TYPE>  
void RegisterMethod2Return( SOURCE_TYPE* fakeParam, 
                            const wstring& className, 
                            const wstring& methodName,
                            METHOD_TYPE methodPtr, 
                            const wstring& argType0, 
                            const wstring& argType1){

    wstring argTypes = FindTypeCode(argType0) + FindTypeCode(argType1);
    if(argTypes.size() == 4){
        Method* newMethod = new TypedMethodArg2Return<SOURCE_TYPE,METHOD_TYPE>( methodPtr, argTypes );
        newMethod->SetName( methodName );
        if ( false == Reflector::RegisterMethod( newMethod, className) ){
            delete newMethod;
            newMethod = 0;
        }
    }

};

template<class SOURCE_TYPE, class METHOD_TYPE>  
void RegisterMethod3Return( SOURCE_TYPE* fakeParam, 
                            const wstring& className,
                            const wstring& methodName,
                            METHOD_TYPE methodPtr, 
                            const wstring& argType0, 
                            const wstring& argType1, 
                            const wstring& argType2){
    wstring argTypes = FindTypeCode(argType0) + FindTypeCode(argType1) + FindTypeCode(argType2);
    if(argTypes.size() == 6){
        Method* newMethod = new TypedMethodArg3Return<SOURCE_TYPE,METHOD_TYPE>( methodPtr, argTypes );
        newMethod->SetName( methodName );
        if ( false == Reflector::RegisterMethod( newMethod, className) ){
            delete newMethod;
            newMethod = 0;
        }	
    }

};

template<class SOURCE_TYPE, class METHOD_TYPE>  
void RegisterMethod4Return( SOURCE_TYPE* fakeParam, 
                            const wstring& className, 
                            const wstring& methodName,
                            METHOD_TYPE methodPtr, 
                            const wstring& argType0, 
                            const wstring& argType1,
                            const wstring& argType2, 
                            const wstring& argType3){
    wstring argTypes = FindTypeCode(argType0) + FindTypeCode(argType1)
        + FindTypeCode(argType2) + FindTypeCode(argType3);
    if(argTypes.size() == 8){
        Method* newMethod = new TypedMethodArg4Return<SOURCE_TYPE,METHOD_TYPE>( methodPtr, argTypes );
        newMethod->SetName( methodName );
        if ( false == Reflector::RegisterMethod( newMethod, className) ){
            delete newMethod;
            newMethod = 0;
        }	
    }

};

template<class SOURCE_TYPE, class METHOD_TYPE>  
void RegisterVoidMethodArg0( SOURCE_TYPE* fakeParam,
                            const wstring& className, 
                            const wstring& methodName,
                            METHOD_TYPE methodPtr){

    wstring argTypes = _T("");

    Method* newMethod = new TypedMethodArg0<SOURCE_TYPE,METHOD_TYPE>( methodPtr, argTypes );
    newMethod->SetName( methodName );
    if ( false == Reflector::RegisterMethod( newMethod, className) ){
        delete newMethod;
        newMethod = 0;
    }	
    

};

template<class SOURCE_TYPE, class METHOD_TYPE>  
void RegisterVoidMethodArg1( SOURCE_TYPE* fakeParam, 
                            const wstring& className,
                            const wstring& methodName,
                           METHOD_TYPE methodPtr, const wstring& argType0){

    wstring argTypes = FindTypeCode(argType0);
    if(argTypes.size() == 2){
        Method* newMethod = new TypedMethodArg1<SOURCE_TYPE,METHOD_TYPE>( methodPtr, argTypes );
        newMethod->SetName( methodName );
        if ( false == Reflector::RegisterMethod( newMethod, className) ){
            delete newMethod;
            newMethod = 0;
        }	
    }

};

template<class SOURCE_TYPE, class METHOD_TYPE>  
void RegisterVoidMethodArg2( SOURCE_TYPE* fakeParam, const wstring& className, const wstring& methodName,
                           METHOD_TYPE methodPtr, const wstring& argType0, const wstring& argType1){

    wstring argTypes = FindTypeCode(argType0) + FindTypeCode(argType1);
    if(argTypes.size() == 4){
        Method* newMethod = new TypedMethodArg2<SOURCE_TYPE,METHOD_TYPE>( methodPtr, argTypes );
        newMethod->SetName( methodName );
        if ( false == Reflector::RegisterMethod( newMethod, className) ){
            delete newMethod;
            newMethod = 0;
        }	
    }

};

template<class SOURCE_TYPE, class METHOD_TYPE>  
void RegisterVoidMethodArg3( SOURCE_TYPE* fakeParam, const wstring& className, const wstring& methodName,
                           METHOD_TYPE methodPtr, const wstring& argType0, const wstring& argType1,
                           const wstring& argType2){

    wstring argTypes = FindTypeCode(argType0) + FindTypeCode(argType1) + FindTypeCode(argType2);
    if(argTypes.size() == 6){
        Method* newMethod = new TypedMethodArg3<SOURCE_TYPE,METHOD_TYPE>( methodPtr, argTypes );
        newMethod->SetName( methodName );
        if ( false == Reflector::RegisterMethod( newMethod, className) ){
            delete newMethod;
            newMethod = 0;
        }	
    }

};

template<class SOURCE_TYPE, class METHOD_TYPE>  
void RegisterVoidMethodArg4( SOURCE_TYPE* fakeParam, const wstring& className, const wstring& methodName,
                           METHOD_TYPE methodPtr, const wstring& argType0, const wstring& argType1, 
                           const wstring& argType2, const wstring& argType3){

    wstring argTypes = FindTypeCode(argType0) + FindTypeCode(argType1) 
        + FindTypeCode(argType2) + FindTypeCode(argType3);
    if(argTypes.size() == 8){
        Method* newMethod = new TypedMethodArg4<SOURCE_TYPE,METHOD_TYPE>( methodPtr, argTypes );
        newMethod->SetName( methodName );
        if ( false == Reflector::RegisterMethod( newMethod, className) ){
            delete newMethod;
            newMethod = 0;
        }	
    }

};

template <class PROPERTY_TYPE>  void RegisterProperty( 
    const wstring& className,
    const wstring& propertyName,
    typename TypedProperty<PROPERTY_TYPE>::GetFunction propertyGetFunction,
    typename TypedProperty<PROPERTY_TYPE>::SetFunction propertySetFunction,
    const wstring& propType ){
    
    DipType propertyFieldDescriptor = FindDipType(propType);

    if(propertyFieldDescriptor != DipUndefined){
        Class* clazz = Reflector::GetClass( className );
        if ( NULL != clazz ){
            if ( false == clazz->HasProperty( propertyName ) ){
                TypedProperty<PROPERTY_TYPE>* newProperty = 
                                new TypedProperty<PROPERTY_TYPE>( propertyGetFunction, 
                                                             propertySetFunction, 
                                                             propertyFieldDescriptor );
                newProperty->SetName( propertyName );
                clazz->AddProperty( newProperty );
            }
        }
    }
}

template <class PROPERTY_TYPE>  void RegisterPathProperty( 
    const wstring& className,
    const wstring& propertyName,
    typename TypedProperty<PROPERTY_TYPE>::GetFunction propertyGetFunction,
    typename TypedProperty<PROPERTY_TYPE>::SetFunction propertySetFunction,
    wstring postfixes){
    
    Class* clazz = Reflector::GetClass( className );
    if ( NULL != clazz ){
        if ( false == clazz->HasProperty( propertyName ) ){
            TypedProperty<PROPERTY_TYPE>* newProperty = 
                            new TypedPathProperty<PROPERTY_TYPE>( propertyGetFunction, 
                                                         propertySetFunction, 
                                                         DipString, postfixes);
            newProperty->SetName( propertyName );
            clazz->AddProperty( newProperty );
        }
    }
}

template <class PROPERTY_TYPE>  void RegisterReadOnlyProperty( 
    const wstring& className, 
    const wstring& propertyName,
    typename TypedProperty<PROPERTY_TYPE>::GetFunction propertyGetFunction,												 
    const wstring& propType ){
    DipType propertyFieldDescriptor = FindDipType(propType);

    if(propertyFieldDescriptor != DipUndefined){	
        Class* clazz = Reflector::GetClass( className );
        if ( NULL != clazz ){
            if ( false == clazz->HasProperty( propertyName ) ){
                TypedProperty<PROPERTY_TYPE>* newProperty = 
                                new TypedProperty<PROPERTY_TYPE>( propertyGetFunction, 
                                                             propertyFieldDescriptor );
                newProperty->SetName( propertyName );
                clazz->AddProperty( newProperty );
            }
        }
    }
}

template <class PROPERTY_TYPE>  void RegisterReadOnlyProperty( 
    const wstring& className, const wstring& propertyName,
    typename TypedEnumProperty<PROPERTY_TYPE>::GetFunction propertyGetFunction,
    PROPERTY_TYPE lower, PROPERTY_TYPE upper ){
    
    
    Class* clazz = Reflector::GetClass( className );
    if ( NULL != clazz ){
        if ( false == clazz->HasProperty( propertyName ) ){
            TypedEnumProperty<PROPERTY_TYPE>* newProperty = 
                            new TypedEnumProperty<PROPERTY_TYPE>( propertyGetFunction,
                                                                    lower, 
                                                                    upper );
            newProperty->SetName( propertyName );
            clazz->AddProperty( newProperty );
        }
    }
};

template <class PROPERTY_TYPE>  void RegisterReadOnlyProperty( 
    const wstring& className, const wstring& propertyName,
    typename TypedEnumProperty<PROPERTY_TYPE>::GetFunction propertyGetFunction,
    PROPERTY_TYPE lower, PROPERTY_TYPE upper,
    const unsigned long& enumNameCount, 
    wstring* enumNames ){
    
    
    Class* clazz = Reflector::GetClass( className );
    if ( NULL != clazz ){
        if ( false == clazz->HasProperty( propertyName ) ){
            TypedEnumProperty<PROPERTY_TYPE>* newProperty = 
                            new TypedEnumProperty<PROPERTY_TYPE>( propertyGetFunction, 
                                                             NULL,			
                                                             lower, upper,
                                                             enumNameCount,
                                                             enumNames );
            newProperty->SetName( propertyName );
            clazz->AddProperty( newProperty );
        }
    }
};

template <class PROPERTY_TYPE>  void RegisterProperty( 
    const wstring& className, const wstring& propertyName,
    typename TypedEnumProperty<PROPERTY_TYPE>::GetFunction propertyGetFunction,
    typename TypedEnumProperty<PROPERTY_TYPE>::SetFunction propertySetFunction,
    PROPERTY_TYPE lower, PROPERTY_TYPE upper ){    
    
    Class* clazz = Reflector::GetClass( className );
    if ( NULL != clazz ){
        if ( false == clazz->HasProperty( propertyName ) ){
            TypedEnumProperty<PROPERTY_TYPE>* newProperty = 
                            new TypedEnumProperty<PROPERTY_TYPE>( propertyGetFunction, 
                                                             propertySetFunction, lower, upper );
            newProperty->SetName( propertyName );
            clazz->AddProperty( newProperty );
        }
    }
};

template <class PROPERTY_TYPE>  void RegisterProperty( 
    const wstring& className, const wstring& propertyName,
    typename TypedEnumProperty<PROPERTY_TYPE>::GetFunction propertyGetFunction,
    typename TypedEnumProperty<PROPERTY_TYPE>::SetFunction propertySetFunction,
    PROPERTY_TYPE lower, PROPERTY_TYPE upper,
    wstring* enumNames ){    
    
    Class* clazz = Reflector::GetClass( className );
    if ( NULL != clazz ){
        if ( false == clazz->HasProperty( propertyName ) ){
            TypedEnumProperty<PROPERTY_TYPE>* newProperty = 
                            new TypedEnumProperty<PROPERTY_TYPE>( propertyGetFunction, 
                                                             propertySetFunction, 
                                                             lower, upper,
                                                             enumNames );
            newProperty->SetName( propertyName );
            clazz->AddProperty( newProperty );
        }
    }
};
} // namespace dip
#endif