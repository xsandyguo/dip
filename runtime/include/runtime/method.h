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

#ifndef DIP_RT_METHOD_H_
#define DIP_RT_METHOD_H_

#include "runtime/common.h"
#include "runtime/variant_data.h"

namespace dip{

class Object;
class DIP_EXPORT Method{
public:
    Method(){
        this->arg_count_ = 0;
        this->has_return_val_ = false;
    }

    virtual ~Method() { }

    virtual void Copy(Method** m) = 0;
    virtual VariantData* Invoke(VariantData** arguments, Object *source=0 ) = 0;

    wstring GetName() {  return this->name_; }
    void   SetName(const wstring& name) { this->name_ = name; }
    int    GetArgCount() { return this->arg_count_; }
    bool   HasReturn() { return this->has_return_val_; }
    virtual Object *GetSource() = 0;
    virtual void SetSource(Object *source) = 0;

    DipType GetArgumentType( const int& idx );    
    virtual wstring GetDescription(){return _T("");}
    
protected:
    wstring arg_types_;
    int    arg_count_;
    bool   has_return_val_;
    wstring name_;
};

/**
*Base template class for methods that do NOT return values
*/
template <class SOURCE_TYPE, class METHOD_TYPE> class TypedMethod : public Method {
public:
    TypedMethod( METHOD_TYPE methodPtr=0, const wstring& argTypes=_T(""), SOURCE_TYPE* source=0){
        this->method_prt_ = methodPtr;
        this->source_     = source;
        this->arg_types_  = argTypes;
        this->objsource_  = 0;

    }

    TypedMethod( const TypedMethod& method ) { }

    virtual ~TypedMethod() { }

    virtual Object* GetSource() { return this->objsource_; }

    virtual void SetSource(Object *source){
        this->source_ = dynamic_cast<SOURCE_TYPE*>(source);
        this->objsource_ = 0;
        if( 0 != this->source_){
            this->objsource_ = source_;
        }
    }

protected:
    SOURCE_TYPE *source_;
    Object   *objsource_;
    METHOD_TYPE method_prt_;

};

/**
*Base template class for methodsthat DO return values
*/
template <class SOURCE_TYPE, class METHOD_TYPE> class TypedMethodReturn 
    : public TypedMethod<SOURCE_TYPE, METHOD_TYPE>{
public:
    TypedMethodReturn( METHOD_TYPE methodPtr=0, const wstring argTypes=_T(""), SOURCE_TYPE* source=0) :
       TypedMethod<SOURCE_TYPE,METHOD_TYPE>( methodPtr, argTypes, source){
           this->has_return_val_ = true;
       }

       virtual ~TypedMethodReturn() { }

protected:
      VariantData return_value_;
};

/**
*Method template class for methods have 0 arguments
*/
template <class SOURCE_TYPE, class METHOD_TYPE> class TypedMethodArg0 
    : public TypedMethod<SOURCE_TYPE, METHOD_TYPE>{
public:
    TypedMethodArg0( METHOD_TYPE methodPtr=0, const wstring& argTypes=_T(""), SOURCE_TYPE* source=0) :
        TypedMethod< SOURCE_TYPE, METHOD_TYPE>( methodPtr, argTypes, source){
        this->arg_count_ = 0;
    }

    TypedMethodArg0( const TypedMethodArg0 &method){
        this->method_prt_ = method.method_prt_;
        this->source_     = method.source_;
        this->objsource_  = method.objsource_;
        this->arg_types_  = method.arg_types_;
        this->arg_count_  = method.arg_count_;
        this->name_       = method.name_;
    }

    virtual ~TypedMethodArg0() { }

    virtual VariantData* Invoke( VariantData ** arguments, Object* source=0){
        if( 0 != source ){
            this->source_ = dynamic_cast<SOURCE_TYPE *>(source);
        }
        if( 0 != this->source_ &&  0 != this->method_prt_){
            if( 0 == this->arg_count_ ){
                (this->source_->*method_prt_)();
            }
        }

        return 0;
    }

    virtual void Copy(Method** m){
        *m = new TypedMethodArg0<SOURCE_TYPE, METHOD_TYPE>(*this);
    }

};

template <class SOURCE_TYPE, class METHOD_TYPE> class TypedMethodArg1 
    : public TypedMethod<SOURCE_TYPE, METHOD_TYPE>
{
public:
    TypedMethodArg1( METHOD_TYPE methodPtr=0, const wstring& argTypes=_T(""), SOURCE_TYPE* source=0) :
        TypedMethod< SOURCE_TYPE, METHOD_TYPE>( methodPtr, argTypes, source){
        this->arg_count_ = 1;
    }

    TypedMethodArg1( const TypedMethodArg1 &method){
        this->method_prt_ = method.method_prt_;
        this->source_     = method.source_;
        this->objsource_  = method.objsource_;
        this->arg_types_  = method.arg_types_;
        this->arg_count_  = method.arg_count_;
        this->name_       = method.name_;
    }

    virtual ~TypedMethodArg1() { }

    virtual VariantData* Invoke( VariantData ** arguments, Object* source=0){
        if( 0 != source ){
            this->source_ = dynamic_cast<SOURCE_TYPE *>(source);
        }
        if( 0 != this->source_ &&  0 != this->method_prt_){
            if( 1 == this->arg_count_ ){
                (this->source_->*method_prt_)(*(arguments[0]));
            }
        }

        return 0;
    }

    virtual void Copy(Method** m){
        *m = new TypedMethodArg1<SOURCE_TYPE, METHOD_TYPE>(*this);
    }

};

template <class SOURCE_TYPE, class METHOD_TYPE> class TypedMethodArg2 
    : public TypedMethod<SOURCE_TYPE, METHOD_TYPE>{
public:
    TypedMethodArg2( METHOD_TYPE methodPtr=0, const wstring& argTypes=_T(""), SOURCE_TYPE* source=0) :
        TypedMethod< SOURCE_TYPE, METHOD_TYPE>( methodPtr, argTypes, source){
        this->arg_count_ = 2;
    }

    TypedMethodArg2( const TypedMethodArg2 &method){
        this->method_prt_ = method.method_prt_;
        this->source_     = method.source_;
        this->objsource_  = method.objsource_;
        this->arg_types_  = method.arg_types_;
        this->arg_count_  = method.arg_count_;
        this->name_       = method.name_;
    }

    virtual ~TypedMethodArg2() { }

    virtual VariantData* Invoke( VariantData ** arguments, Object* source=0){
        if( 0 != source ){
            this->source_ = dynamic_cast<SOURCE_TYPE *>(source);
        }
        if( 0 != this->source_ &&  0 != this->method_prt_){
            if( 2 == this->arg_count_ ){
                (this->source_->*method_prt_)(*(arguments[0]),
                                           *(arguments[1]) );
            }
        }

        return 0;
    }

    virtual void Copy(Method** m){
        *m = new TypedMethodArg2<SOURCE_TYPE, METHOD_TYPE>(*this);
    }

};

/**
*Accepts methds with 3 arguments - no return value
*/

template <class SOURCE_TYPE, class METHOD_TYPE> class TypedMethodArg3 
    : public TypedMethod<SOURCE_TYPE, METHOD_TYPE>
{
public:
    TypedMethodArg3( METHOD_TYPE methodPtr=0, const wstring& argTypes=_T(""), SOURCE_TYPE* source=0) :
        TypedMethod< SOURCE_TYPE, METHOD_TYPE>( methodPtr, argTypes, source){
        this->arg_count_ = 3;
    }

    TypedMethodArg3( const TypedMethodArg3 &method){
        this->method_prt_ = method.method_prt_;
        this->source_     = method.source_;
        this->objsource_  = method.objsource_;
        this->arg_types_  = method.arg_types_;
        this->arg_count_  = method.arg_count_;
        this->name_       = method.name_;
    }

    virtual ~TypedMethodArg3() { }

    virtual VariantData* Invoke( VariantData ** arguments, Object* source=0){
        if( 0 != source ){
            this->source_ = dynamic_cast<SOURCE_TYPE *>(source);
        }
        if( 0 != this->source_ &&  0 != this->method_prt_){
            if( 3 == this->arg_count_ ){
                (this->source_->*method_prt_)(*(arguments[0]),
                                           *(arguments[1]),
                                           *(arguments[2])
                                           );
            }
        }

        return 0;
    }

    virtual void Copy(Method** m){
        *m = new TypedMethodArg3<SOURCE_TYPE, METHOD_TYPE>(*this);
    }

};

/**
*Accepts methds with 4 arguments - no return value
*/

template <class SOURCE_TYPE, class METHOD_TYPE> class TypedMethodArg4 
    : public TypedMethod<SOURCE_TYPE, METHOD_TYPE>
{
public:
    TypedMethodArg4( METHOD_TYPE methodPtr=0, const wstring& argTypes=_T(""), SOURCE_TYPE* source=0) :
        TypedMethod< SOURCE_TYPE, METHOD_TYPE>( methodPtr, argTypes, source){
        this->arg_count_ = 4;
    }

    TypedMethodArg4( const TypedMethodArg4 &method){
        this->method_prt_ = method.method_prt_;
        this->source_     = method.source_;
        this->objsource_  = method.objsource_;
        this->arg_types_  = method.arg_types_;
        this->arg_count_  = method.arg_count_;
        this->name_       = method.name_;
    }

    virtual ~TypedMethodArg4() { }

    virtual VariantData* Invoke( VariantData ** arguments, Object* source=0){
        if( 0 != source ){
            this->source_ = dynamic_cast<SOURCE_TYPE *>(source);
        }
        if( 0 != this->source_ &&  0 != this->method_prt_){
            if( 4 == this->arg_count_ ){
                (this->source_->*method_prt_)(*(arguments[0]),
                                           *(arguments[1]),
                                           *(arguments[2]),
                                           *(arguments[3])
                                           );
            }
        }

        return 0;
    }

    virtual void Copy(Method** m){
        *m = new TypedMethodArg4<SOURCE_TYPE, METHOD_TYPE>(*this);
    }

};

/**
*Method template class for methods with 0 argument and a return value
*/
template <class SOURCE_TYPE, class METHOD_TYPE> class TypedMethodArg0Return 
    : public TypedMethodReturn<SOURCE_TYPE, METHOD_TYPE>
{
public:
    TypedMethodArg0Return( METHOD_TYPE methodPtr=0, const wstring& argTypes=_T(""), SOURCE_TYPE* source=0) :
        TypedMethodReturn< SOURCE_TYPE, METHOD_TYPE>( methodPtr, argTypes, source){
        this->arg_count_ = 0;
    }

    TypedMethodArg0Return( const TypedMethodArg0Return &method){
        this->method_prt_ = method.method_prt_;
        this->source_     = method.source_;
        this->objsource_  = method.objsource_;
        this->arg_types_  = method.arg_types_;
        this->arg_count_  = method.arg_count_;
        this->name_       = method.name_;
    }

    virtual ~TypedMethodArg0Return() { }

    virtual VariantData* Invoke( VariantData ** arguments, Object* source=0){
        if( 0 != source ){
            this->source_ = dynamic_cast<SOURCE_TYPE *>(source);
        }
        if( 0 != this->source_ &&  0 != this->method_prt_){
            if( 0 == this->arg_count_ ){
                this->return_value_ = (this->source_->*method_prt_)();
            }
        }

        return &(this->return_value_);
    }

    virtual void Copy(Method** m){
        *m = new TypedMethodArg0Return<SOURCE_TYPE, METHOD_TYPE>(*this);
    }

};

/**
*Method template class for methods with 1 argument and a return value
*/
template <class SOURCE_TYPE, class METHOD_TYPE> class TypedMethodArg1Return
    : public TypedMethodReturn<SOURCE_TYPE, METHOD_TYPE>
{
public:
    TypedMethodArg1Return( METHOD_TYPE methodPtr=0, const wstring& argTypes=_T(""), SOURCE_TYPE* source=0) :
        TypedMethodReturn< SOURCE_TYPE, METHOD_TYPE>( methodPtr, argTypes, source){
        this->arg_count_ = 1;
    }

    TypedMethodArg1Return( const TypedMethodArg1Return &method){
        this->method_prt_ = method.method_prt_;
        this->source_     = method.source_;
        this->objsource_  = method.objsource_;
        this->arg_types_  = method.arg_types_;
        this->arg_count_  = method.arg_count_;
        this->name_       = method.name_;
    }

    virtual ~TypedMethodArg1Return() { }

    virtual VariantData* Invoke( VariantData ** arguments, Object* source=0){
        if( 0 != source ){
            this->source_ = dynamic_cast<SOURCE_TYPE *>(source);
        }
        if( 0 != this->source_ &&  0 != this->method_prt_){
            if( 1 == this->arg_count_ ){
                this->return_value_ = (this->source_->*method_prt_)(*(arguments[0]));
            }
        }

        return &(this->return_value_);
    }

    virtual void Copy(Method** m){
        *m = new TypedMethodArg1Return<SOURCE_TYPE, METHOD_TYPE>(*this);
    }

};

/**
*Method template class for methods with 2 argument and a return value
*/
template <class SOURCE_TYPE, class METHOD_TYPE> class TypedMethodArg2Return
    : public TypedMethodReturn<SOURCE_TYPE, METHOD_TYPE>
{
public:
    TypedMethodArg2Return( METHOD_TYPE methodPtr=0, const wstring& argTypes=_T(""), SOURCE_TYPE* source=0) :
        TypedMethodReturn< SOURCE_TYPE, METHOD_TYPE>( methodPtr, argTypes, source){
        this->arg_count_ = 2;
    }

    TypedMethodArg2Return( const TypedMethodArg2Return &method){
        this->method_prt_ = method.method_prt_;
        this->source_     = method.source_;
        this->objsource_  = method.objsource_;
        this->arg_types_  = method.arg_types_;
        this->arg_count_  = method.arg_count_;
        this->name_       = method.name_;
    }

    virtual ~TypedMethodArg2Return() { }

    virtual VariantData* Invoke( VariantData ** arguments, Object* source=0){
        if( 0 != source ){
            this->source_ = dynamic_cast<SOURCE_TYPE *>(source);
        }
        if( 0 != this->source_ &&  0 != this->method_prt_){
            if( 2 == this->arg_count_ ){
                this->return_value_ = (this->source_->*method_prt_)(*(arguments[0]),
                                                             *(arguments[1])
                                                            );
            }
        }

        return &(this->return_value_);
    }

    virtual void Copy(Method** m){
        *m = new TypedMethodArg2Return<SOURCE_TYPE, METHOD_TYPE>(*this);
    }

};

/**
*Method template class for methods with 3 argument and a return value
*/
template <class SOURCE_TYPE, class METHOD_TYPE> class TypedMethodArg3Return
    : public TypedMethodReturn<SOURCE_TYPE, METHOD_TYPE>
{
public:
    TypedMethodArg3Return( METHOD_TYPE methodPtr=0, const wstring& argTypes=_T(""), SOURCE_TYPE* source=0) :
        TypedMethodReturn< SOURCE_TYPE, METHOD_TYPE>( methodPtr, argTypes, source){
        this->arg_count_ = 3;
    }

    TypedMethodArg3Return( const TypedMethodArg3Return &method){
        this->method_prt_ = method.method_prt_;
        this->source_     = method.source_;
        this->objsource_  = method.objsource_;
        this->arg_types_  = method.arg_types_;
        this->arg_count_  = method.arg_count_;
        this->name_       = method.name_;
    }

    virtual ~TypedMethodArg3Return() { }

    virtual VariantData* Invoke( VariantData ** arguments, Object* source=0){
        if( 0 != source ){
            this->source_ = dynamic_cast<SOURCE_TYPE *>(source);
        }
        if( 0 != this->source_ &&  0 != this->method_prt_){
            if( 3 == this->arg_count_ ){
                this->return_value_ = (this->source_->*method_prt_)(*(arguments[0]),
                                                             *(arguments[1]),
                                                             *(arguments[2])
                                                            );
            }
        }

        return &(this->return_value_);
    }

    virtual void Copy(Method** m){
        *m = new TypedMethodArg3Return<SOURCE_TYPE, METHOD_TYPE>(*this);
    }

};

/**
*Method template class for methods with 4 argument and a return value
*/
template <class SOURCE_TYPE, class METHOD_TYPE> class TypedMethodArg4Return 
    : public TypedMethodReturn<SOURCE_TYPE, METHOD_TYPE>
{
public:
    TypedMethodArg4Return( METHOD_TYPE methodPtr=0, const wstring& argTypes=_T(""), SOURCE_TYPE* source=0) :
        TypedMethodReturn< SOURCE_TYPE, METHOD_TYPE>( methodPtr, argTypes, source){
        this->arg_count_ = 4;
    }

    TypedMethodArg4Return( const TypedMethodArg4Return &method){
        this->method_prt_ = method.method_prt_;
        this->source_     = method.source_;
        this->objsource_  = method.objsource_;
        this->arg_types_  = method.arg_types_;
        this->arg_count_  = method.arg_count_;
        this->name_       = method.name_;
    }

    virtual ~TypedMethodArg4Return() { }

    virtual VariantData* Invoke( VariantData ** arguments, Object* source=0){
        if( 0 != source ){
            this->source_ = dynamic_cast<SOURCE_TYPE *>(source);
        }
        if( 0 != this->source_ &&  0 != this->method_prt_){
            if( 4 == this->arg_count_ ){
                this->return_value_ = (this->source_->*method_prt_)(*(arguments[0]),
                                                             *(arguments[1]),
                                                             *(arguments[2]),
                                                             *(arguments[3])
                                                            );
            }
        }

        return &(this->return_value_);
    }

    virtual void Copy(Method** m){
        *m = new TypedMethodArg4Return<SOURCE_TYPE, METHOD_TYPE>(*this);
    }

};

} // namespace dip
#endif