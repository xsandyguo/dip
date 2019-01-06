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

#ifndef DIP_RT_CLASS_INFO_H_
#define DIP_RT_CLASS_INFO_H_

#include "runtime/reflector.h"

namespace dip{

template <class CLASS_TYPE> class ClassInfo {
public:
    ClassInfo( const wstring& className, 
        const wstring& superClassName,
        const wstring& classID,
        DynamicObjectCreate create){
        this->is_class_registered_ = RegisterClass<CLASS_TYPE>(0,
            className,
            superClassName,
            classID, 
            create);
    }

    virtual ~ClassInfo() { }

    bool IsClassRegistered(){
        return this->is_class_registered_;
    }
private:
    bool is_class_registered_;
    DISALLOW_EVIL_CONSTRUCTORS(ClassInfo);
};

template <class CLASS_TYPE> class AbstractClassInfo{
public:
    AbstractClassInfo( const wstring& className,
        const wstring& superClassName,
        const wstring& classID){
        this->is_class_registered_ = RegisterAbstractClass<CLASS_TYPE>(0,
            className, 
            superClassName,
            classID);
    }

    virtual ~AbstractClassInfo() { }

    bool IsClassRegistered(){
        return this->is_class_registered_;
    }
private:
    bool is_class_registered_;
    DISALLOW_EVIL_CONSTRUCTORS(AbstractClassInfo);
};


#define DECLEAR_CLASSINFO( ClassType, SuperClass) \
    class ClassType##Info : public ClassInfo<ClassType> { \
    public: \
        ClassType##Info();\
        virtual ~ClassType##Info(){}; \
    };\
    static ClassType* New(){ \
    Object *ret = Reflector::CreateNewInstance(_T(#ClassType)); \
        if(ret) \
        { \
            return (ClassType *) ret; \
        } \
        else \
        { \
            return new ClassType; \
        } \
    }\
    static Object* Create() { \
        return new ClassType; \
    }\
    virtual const wchar_t *GetClassName() {return _T(#ClassType); }\
    virtual bool IsA(wstring type) \
    { \
        if ( type == this->ClassType::GetClassName() ) \
        { \
            return true; \
        } \
        return this->SuperClass::IsA(type); \
    } \
    static ClassType* SafeDownCast(Object *o) \
    { \
        if ( o->IsA(_T(#ClassType))) \
        { \
            return (ClassType *)o; \
        } \
        return 0;\
    } \
    \


#define DECLEAR_ABSTRACT_CLASSINFO( ClassType, SuperClass) \
    class ClassType##Info : public AbstractClassInfo<ClassType> { \
    public: \
        ClassType##Info();\
        virtual ~ClassType##Info(){}; \
    };\
    static ClassType* New(){ \
        return (ClassType *) Reflector::CreateNewInstance(_T(#ClassType)); \
    }\
    virtual const wchar_t *GetClassName() {return _T(#ClassType);}\
    virtual bool IsA(wstring type) \
    { \
        if ( type == this->ClassType::GetClassName() ) \
        { \
            return true; \
        } \
        return this->SuperClass::IsA(type); \
    } \
    static ClassType* SafeDownCast(Object *o) \
    { \
        if ( o->IsA(_T(#ClassType)) ) \
        { \
            return (ClassType *)o; \
        } \
        return 0;\
    } \
    \


#define METHOD_VOID( methodName, classType, methodPtr ) \
    typedef  void ( classType::* _T_##classType##methodPtr )(void); \
    dip::RegisterVoidMethodArg0<classType,_T_##classType##methodPtr##>( NULL, \
    tmpClassName, wstring(_T(methodName)), &methodPtr ); \
      \

#define METHOD_1VOID( methodName, classType, argType, methodPtr ) \
    typedef  void ( classType::* _T_##classType##methodPtr )(argType); \
    dip::RegisterVoidMethodArg1<classType,_T_##classType##methodPtr##>( NULL, \
    tmpClassName, wstring(_T(methodName)), &methodPtr, wstring(_T(#argType)) ); \
      \

#define METHOD_2VOID( methodName, classType, argType1, argType2, methodPtr ) \
    typedef  void ( classType::* _T_##classType##methodPtr )(argType1, argType2); \
    dip::RegisterVoidMethodArg2<classType,_T_##classType##methodPtr##>( NULL, \
    tmpClassName, wstring(_T(methodName)), &methodPtr, wstring(_T(#argType1)), wstring(_T(#argType2)) ); \
      \

#define METHOD_3VOID( methodName, classType, argType1, argType2, argType3, methodPtr ) \
    typedef  void ( classType::* _T_##classType##methodPtr )(argType1, argType2, argType3); \
    dip::RegisterVoidMethodArg3<classType,_T_##classType##methodPtr##>( NULL, tmpClassName, \
     wstring(_T(methodName)), &methodPtr, wstring(_T(#argType1)), wstring(_T(#argType2)), wstring(_T(#argType3)) ); \
      \

#define METHOD_4VOID( methodName, classType, argType1, argType2, argType3, argType4, methodPtr ) \
    typedef  void ( classType::* _T_##classType##methodPtr )(argType1, argType2, argType3, argType4); \
    dip::RegisterVoidMethodArg4<classType,_T_##classType##methodPtr##>( NULL, tmpClassName,  \
    wstring(_T(methodName)), &methodPtr, wstring(_T(#argType1)), wstring(_T(#argType2)), wstring(_T(#argType3)), \
    wstring(_T(#argType4)) ); \
      \


#define METHOD_RETURN( methodName, classType, returnType, methodPtr ) \
    typedef  returnType ( classType::* _T_##classType##methodPtr )(void); \
    dip::RegisterMethod0Return<classType,_T_##classType##methodPtr##>( NULL, \
    tmpClassName, wstring(_T(methodName)), &methodPtr ); \
      \

#define METHOD_1RETURN( methodName, classType, returnType, argType, methodPtr ) \
    typedef  returnType ( classType::* _T_##classType##methodPtr )(argType); \
    dip::RegisterMethod1Return<classType,_T_##classType##methodPtr##>( NULL, \
    tmpClassName, wstring(_T(methodName)), &methodPtr, wstring(_T(#argType)) ); \
      \

#define METHOD_2RETURN( methodName, classType, returnType, argType1, argType2, methodPtr ) \
    typedef  returnType ( classType::* _T_##classType##methodPtr )(argType1, argType2); \
    dip::RegisterMethod2Return<classType,_T_##classType##methodPtr##>( NULL, \
    tmpClassName, wstring(_T(methodName)), &methodPtr, wstring(_T(#argType1)), wstring(_T(#argType2)) ); \
      \

#define METHOD_3RETURN( methodName, classType, returnType, argType1, argType2, argType3, methodPtr ) \
    typedef  returnType ( classType::* _T_##classType##methodPtr )(argType1, argType2, argType3); \
    dip::RegisterMethod3Return<classType,_T_##classType##methodPtr##>( NULL, \
    tmpClassName, wstring(_T(methodName)), &methodPtr, wstring(_T(#argType1)), \
    wstring(_T(#argType2)), wstring(_T(#argType3)) ); \
      \

#define METHOD_4RETURN( methodName, classType, returnType, argType1, argType2, argType3, argType4, methodPtr ) \
    typedef  returnType ( classType::* _T_##classType##methodPtr )(argType1, argType2, argType3, argType4); \
    dip::RegisterMethod4Return<classType,_T_##classType##methodPtr##>( NULL, tmpClassName, wstring(_T(methodName)), \
    &methodPtr, wstring(_T(#argType1)), wstring(_T(#argType2)), wstring(_T(#argType3)), wstring(_T(#argType4)) ); \
      \


#define PROPERTY( type, propName, getFunc, setFunc) \
    dip::RegisterProperty<type>( tmpClassName, wstring(_T(propName)), \
                                        (TypedProperty<type>::GetFunction)&(getFunc), \
                                        (TypedProperty<type>::SetFunction)&(setFunc), \
                                       wstring(_T(#type)) ); \
                                        \

#define MULTI_PROPERTY_2( type, propName, getFunc, setFunc) \
    dip::RegisterProperty<type>( tmpClassName, wstring(_T(propName))+_T("0"), \
                                       (TypedProperty<type>::GetFunction)&(getFunc##0), \
                                       (TypedProperty<type>::SetFunction)&(setFunc##0), \
                                       wstring(_T(#type)) ); \
    dip::RegisterProperty<type>( tmpClassName, wstring(_T(propName)1)+_T("1"), \
                                       (TypedProperty<type>::GetFunction)&(getFunc##1), \
                                       (TypedProperty<type>::SetFunction)&(setFunc##1), \
                                       wstring(_T(#type)) ); \
                                       \

#define MULTI_PROPERTY_4( type, propName, getFunc, setFunc) \
    dip::RegisterProperty<type>( tmpClassName, wstring(_T(propName))+_T("0"), \
                                       (TypedProperty<type>::GetFunction)&(getFunc##0), \
                                       (TypedProperty<type>::SetFunction)&(setFunc##0), \
                                       wstring(_T(#type)) ); \
    dip::RegisterProperty<type>( tmpClassName, wstring(_T(propName))+_T("1"), \
                                       (TypedProperty<type>::GetFunction)&(getFunc##1), \
                                       (TypedProperty<type>::SetFunction)&(setFunc##1), \
                                       wstring(_T(#type)) ); \
    dip::RegisterProperty<type>( tmpClassName, wstring(_T(propName))+_T("2"), \
                                       (TypedProperty<type>::GetFunction)&(getFunc##2), \
                                       (TypedProperty<type>::SetFunction)&(setFunc##2), \
                                       wstring(_T(#type)) ); \
    dip::RegisterProperty<type>( tmpClassName, wstring(_T(propName))+_T("3"), \
                                       (TypedProperty<type>::GetFunction)&(getFunc##3), \
                                       (TypedProperty<type>::SetFunction)&(setFunc##3), \
                                       wstring(_T(#type)) ); \
                                       \
    

#define MULTI_PROPERTY_8( type, propName, getFunc, setFunc) \
    dip::RegisterProperty<type>( tmpClassName, wstring(_T(propName))+_T("0"), \
                                    (TypedProperty<type>::GetFunction)&(getFunc##0), \
                                    (TypedProperty<type>::SetFunction)&(setFunc##0), \
                                       wstring(_T(#type)) ); \
    dip::RegisterProperty<type>( tmpClassName, wstring(_T(propName))+_T("1"), \
                                       (TypedProperty<type>::GetFunction)&(getFunc##1), \
                                       (TypedProperty<type>::SetFunction)&(setFunc##1), \
                                       wstring(_T(#type)) ); \
    dip::RegisterProperty<type>( tmpClassName, wstring(_T(propName))+_T("2"), \
                                       (TypedProperty<type>::GetFunction)&(getFunc##2), \
                                       (TypedProperty<type>::SetFunction)&(setFunc##2), \
                                       wstring(_T(#type)) ); \
    dip::RegisterProperty<type>( tmpClassName, wstring(_T(propName))+_T("3"), \
                                       (TypedProperty<type>::GetFunction)&(getFunc##3), \
                                       (TypedProperty<type>::SetFunction)&(setFunc##3), \
                                       wstring(_T(#type)) ); \
    dip::RegisterProperty<type>( tmpClassName, wstring(_T(propName))+_T("4"), \
                                       (TypedProperty<type>::GetFunction)&(getFunc##4), \
                                       (TypedProperty<type>::SetFunction)&(setFunc##4), \
                                       wstring(_T(#type)) ); \
    dip::RegisterProperty<type>( tmpClassName, wstring(_T(propName))+_T("5"), \
                                       (TypedProperty<type>::GetFunction)&(getFunc##5), \
                                       (TypedProperty<type>::SetFunction)&(setFunc##5), \
                                       wstring(_T(#type)) ); \
    dip::RegisterProperty<type>( tmpClassName, wstring(_T(propName))+_T("6"), \
                                       (TypedProperty<type>::GetFunction)&(getFunc##6), \
                                       (TypedProperty<type>::SetFunction)&(setFunc##6), \
                                       wstring(_T(#type)) ); \
    dip::RegisterProperty<type>( tmpClassName, wstring(_T(propName))+_T("7"), \
                                       (TypedProperty<type>::GetFunction)&(getFunc##7), \
                                       (TypedProperty<type>::SetFunction)&(setFunc##7), \
                                       wstring(_T(#type)) ); \
                                        \
                                    

#define MULTI_PROPERTY_16( type, propName, getFunc, setFunc) \
    dip::RegisterProperty<type>( tmpClassName, wstring(_T(propName))+_T("0"), \
                                       (TypedProperty<type>::GetFunction)(&getFunc##0), \
                                       (TypedProperty<type>::SetFunction)(&setFunc##0), \
                                       wstring(_T(#type)) ); \
    dip::RegisterProperty<type>( tmpClassName, wstring(_T(propName))+_T("1"), \
                                       (TypedProperty<type>::GetFunction)(&getFunc##1), \
                                       (TypedProperty<type>::SetFunction)(&setFunc##1), \
                                       wstring(_T(#type)) ); \
    dip::RegisterProperty<type>( tmpClassName, wstring(_T(propName))+_T("2"), \
                                       (TypedProperty<type>::GetFunction)(&getFunc##2), \
                                       (TypedProperty<type>::SetFunction)(&setFunc##2), \
                                       wstring(_T(#type)) ); \
    dip::RegisterProperty<type>( tmpClassName, wstring(_T(propName))+_T("3"), \
                                       (TypedProperty<type>::GetFunction)(&getFunc##3), \
                                       (TypedProperty<type>::SetFunction)(&setFunc##3), \
                                       wstring(_T(#type)) ); \
    dip::RegisterProperty<type>( tmpClassName, wstring(_T(propName))+_T("4"), \
                                       (TypedProperty<type>::GetFunction)(&getFunc##4), \
                                       (TypedProperty<type>::SetFunction)(&setFunc##4), \
                                       wstring(_T(#type)) ); \
    dip::RegisterProperty<type>( tmpClassName, wstring(_T(propName))+_T("5"), \
                                       (TypedProperty<type>::GetFunction)(&getFunc##5), \
                                       (TypedProperty<type>::SetFunction)(&setFunc##5), \
                                       wstring(_T(#type)) ); \
    dip::RegisterProperty<type>( tmpClassName, wstring(_T(propName))+_T("6"), \
                                       (TypedProperty<type>::GetFunction)(&getFunc##6), \
                                       (TypedProperty<type>::SetFunction)(&setFunc##6), \
                                       wstring(_T(#type)) ); \
    dip::RegisterProperty<type>( tmpClassName, wstring(_T(propName))+_T("7"), \
                                       (TypedProperty<type>::GetFunction)(&getFunc##7), \
                                       (TypedProperty<type>::SetFunction)(&setFunc##7), \
                                       wstring(_T(#type)) ); \
    dip::RegisterProperty<type>( tmpClassName, wstring(_T(propName))+_T("8"), \
                                       (TypedProperty<type>::GetFunction)(&getFunc##8), \
                                       (TypedProperty<type>::SetFunction)(&setFunc##8), \
                                       wstring(_T(#type)) ); \
    dip::RegisterProperty<type>( tmpClassName, wstring(_T(propName))+_T("9"), \
                                       (TypedProperty<type>::GetFunction)(&getFunc##9), \
                                       (TypedProperty<type>::SetFunction)(&setFunc##9), \
                                       wstring(_T(#type)) ); \
    dip::RegisterProperty<type>( tmpClassName, wstring(_T(propName))+_T("10"), \
                                       (TypedProperty<type>::GetFunction)(&getFunc##10), \
                                       (TypedProperty<type>::SetFunction)(&setFunc##10), \
                                       wstring(_T(#type)) ); \
    dip::RegisterProperty<type>( tmpClassName, wstring(_T(propName))+_T("11"), \
                                       (TypedProperty<type>::GetFunction)(&getFunc##11), \
                                       (TypedProperty<type>::SetFunction)(&setFunc##11), \
                                       wstring(_T(#type)) ); \
    dip::RegisterProperty<type>( tmpClassName, wstring(_T(propName))+_T("12"), \
                                       (TypedProperty<type>::GetFunction)(&getFunc##12), \
                                       (TypedProperty<type>::SetFunction)(&setFunc##12), \
                                       wstring(_T(#type)) ); \
    dip::RegisterProperty<type>( tmpClassName, wstring(_T(propName))+_T("13"), \
                                       (TypedProperty<type>::GetFunction)(&getFunc##13), \
                                       (TypedProperty<type>::SetFunction)(&setFunc##13), \
                                       wstring(_T(#type)) ); \
    dip::RegisterProperty<type>( tmpClassName, wstring(_T(propName))+_T("14"), \
                                       (TypedProperty<type>::GetFunction)(&getFunc##14), \
                                       (TypedProperty<type>::SetFunction)(&setFunc##14), \
                                       wstring(_T(#type)) ); \
    dip::RegisterProperty<type>( tmpClassName, wstring(_T(propName))+_T("15"), \
                                       (TypedProperty<type>::GetFunction)(&getFunc##15), \
                                       (TypedProperty<type>::SetFunction)(&setFunc##15), \
                                       wstring(_T(#type)) ); \
                                           \


#define PATH_PROPERTY( type, propName, getFunc, setFunc, postfixes) \
    dip::RegisterPathProperty<type>( tmpClassName, wstring(_T(propName)), \
                                       (TypedProperty<type>::GetFunction)&getFunc, \
                                       (TypedProperty<type>::SetFunction)&setFunc, \
                                       _T(postfixes)); \
                                        \
                                        
#define READONLY_PROPERTY( type, propName, getFunc) \
                dip::RegisterReadOnlyProperty<type>( tmpClassName, wstring(_T(propName)), \
                                                (TypedProperty<type>::GetFunction)&getFunc, \
                                                wstring(_T(#type)) ); \
                                                            \



#define ENUM_PROPERTY(type, propName, getFunc, setFunc, lower, upper) \
                dip::RegisterProperty<type>( tmpClassName, wstring(_T(propName)), \
                                        (TypedEnumProperty<type>::GetFunction)&getFunc, \
                                        (TypedEnumProperty<type>::SetFunction)&setFunc, \
                                        lower, \
                                        upper); \
                                         \


#define LABELED_ENUM_PROPERTY(type, propName, getFunc, setFunc, lower, upper, enumNames) \
                dip::RegisterProperty<type>( tmpClassName, wstring(_T(propName)), \
                                        (TypedEnumProperty<type>::GetFunction)&getFunc, \
                                        (TypedEnumProperty<type>::SetFunction)&setFunc, \
                                        lower, \
                                        upper, \
                                        enumNames );\
                                                   \

#define READONLY_ENUM_PROPERTY(type, propName, getFunc,lower, upper) \
                dip::RegisterReadOnlyProperty<type>( tmpClassName, wstring(_T(propName)), \
                                                (TypedEnumProperty<type>::GetFunction)&getFunc, \
                                                lower, \
                                                upper); \
                                                        \


#define READONLY_LABELED_ENUM_PROPERTY(type, propName, getFunc, lower, upper, enumNames) \
                dip::RegisterReadOnlyProperty<type>( tmpClassName, wstring(_T(propName)), \
                                                (TypedEnumProperty<type>::GetFunction)&getFunc, \
                                                lower, \
                                                upper, \
                                                enumNames ); \
                                                             \

#define END_CLASSINFO( ClassType ) \
            } \
        }; \
         \
         \
        virtual ~ClassType##Info(){}; \
     \
    }; \
    \


#define BEGIN_REGISTER_CLASSINFO( ClassType, SuperClass, classID) \
    ClassType::ClassType##Info::ClassType##Info(): \
    ClassInfo<ClassType>( _T(#ClassType), _T(#SuperClass), _T(classID), ClassType##::Create ){ \
            wstring tmpClassName = _T(#ClassType); \
            Class *clazz = Reflector::GetClass(tmpClassName); \
            if ( true == IsClassRegistered()  ){ \
            \

#define BEGIN_REGISTER_ABSTRACT_CLASSINFO( ClassType, SuperClass, classID) \
    ClassType::ClassType##Info::ClassType##Info(): \
        AbstractClassInfo<ClassType>( _T(#ClassType), _T(#SuperClass), _T(classID)){ \
            wstring tmpClassName = _T(#ClassType); \
            Class *clazz = Reflector::GetClass(tmpClassName); \
            if ( true == IsClassRegistered()  ){ \
            \


#define END_REGISTER_CLASSINFO( ClassType ) \
            } \
        }; \
    ClassType::ClassType##Info classInfoFor##ClassType; \
                                                   \


#define DESCRIPT(Discription) \
        if(clazz) \
        {\
            wstring tmp = clazz->GetDescription(); \
            tmp += _T(Discription); \
            tmp += _T("\n"); \
            clazz->SetDescription(tmp); \
        } \
                              \


#define REGISTER_CLASSINFO_EXTERNAL( ClassType ) \
    ClassType##Info classInfoFor##ClassType; \
                                                   \

} // namespace dip

#endif

            