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

#ifndef DIP_RT_PROPERTY_H_
#define DIP_RT_PROPERTY_H_

#include <string>

#include "runtime/common.h"
#include "runtime/variant_data.h"

namespace dip{
using namespace std;
class Object;

class DIP_EXPORT Property {
public:
    Property(){
        this->source_ = 0;
        this->readonly_ = false;
        this->bound_ = false;
        this->path_  = false;
    };
    
    Property( const Property& prop ):value_( prop.value_ ){
        this->name_        = prop.name_;
        this->display_name_ = prop.display_name_;
        this->description_ = prop.description_;
        this->type_        = prop.type_;
        this->source_      = prop.source_;
        this->readonly_    = prop.readonly_;
        this->bound_       = prop.bound_;
    };

    virtual ~Property() { };	
    virtual void Copy( Property** copiedProperty ) = 0;
    wstring GetName() const { return this->name_; };
    void SetName( const wstring& name ){ this->name_ = name; };	

    wstring GetDisplayName() const{
        if ( this->display_name_.size() > 0 ){
            return this->display_name_;
        }else{
            return this->name_;
        }
    };

    void SetDisplayName( const wstring& displayName ) { this->display_name_ = displayName;}	
    wstring GetDescription() const { return this->description_;};	
    void SetDescription( const wstring& description ) {this->description_ = description;};
    DipType GetType() const {return this->type_; };
    VariantData* GetValue() { return &this->value_; }
    void SetType( const DipType& type ){
        this->type_       = type;
        this->value_.SetType(type);
    }

    virtual VariantData* Get( Object* source ) = 0;
    virtual void Set( Object* source, VariantData* value ) = 0;
    
    void SetSource( Object* source ){
        this->source_ = source;
        Get();
    }

    Object* GetSource(){
        return this->source_;
    }
    
    VariantData* Get(){
        if ( 0 != this->source_ ){
            return Get( this->source_ );
        }
        else 
        {
            return 0;
        }
    };

    void Set( VariantData* value ){
        if ( NULL != this->source_ ){
            Set( this->source_, value );
        }
    };

    
    void Set( Object* source, const wstring& value ){
        this->value_.SetFromString( value );
        Set( source, &this->value_ );
    };

    void Set( const wstring& value ){
        if ( 0 != this->source_ ){
            Set( this->source_, value );
        }
    };

    bool IsPath() { return (this->path_ && this->type_ == DipString); }	
    bool IsReadOnly() { return this->readonly_; }
    bool IsBound() { return this->bound_; };
    void SetBound( const bool& bound ) { this->bound_ = bound; };
    const wstring& GetPostfixes() { return postfixes_; }

protected:
    VariantData value_;	
    bool readonly_;
    bool path_;
    wstring postfixes_;

private:
    bool       bound_;
    wstring     name_;
    wstring     display_name_;
    wstring     description_;
    DipType    type_;
    Object* source_;
};

template <class PROPERTY> class TypedProperty : public Property {
public:
    typedef PROPERTY (Object::*GetFunction)(void);
    typedef void (Object::*SetFunction)(PROPERTY );

    TypedProperty( GetFunction propGetFunction, const DipType& propertyType ){
        this->Init();
        
        this->get_function_ = propGetFunction;
        this->SetType( propertyType );
        this->readonly_ = true;
    };

    TypedProperty( GetFunction propGetFunction,
        SetFunction propSetFunction,
        const DipType& propertyType ){
        this->Init();
        
        this->get_function_ = propGetFunction;
        this->set_function_ = propSetFunction;
        this->SetType( propertyType );
    };

    TypedProperty( const TypedProperty<PROPERTY>& prop ) : Property( prop ){
        this->Init();
        this->get_function_ = prop.get_function_;
        this->set_function_ = prop.set_function_;	
    };

    virtual ~TypedProperty(){};

    virtual void Copy( Property** copiedProperty ){
        *copiedProperty = 0;
        *copiedProperty = new TypedProperty<PROPERTY>(*this);
    };

    void Init(){
        this->get_function_ = 0;		
        this->set_function_ = 0;		
    };

    virtual VariantData* Get( Object* source ){		
        if ( (0 != this->get_function_) && (0 != source) ){
            this->value_.SetType(this->GetType());
            this->value_ = (source->*get_function_)();		 
            return &this->value_;
        }else{
            return 0;
        }
    };

    virtual void Set( Object* source, VariantData* value ){
        if ( (0 != this->set_function_) && (0 != source) ){
            (source->*set_function_)( *value );		
        }
    };
    
protected:	
    GetFunction get_function_;
    SetFunction set_function_;
};

template <class PROPERTY> class TypedPathProperty : public TypedProperty<PROPERTY> {
public:
    typedef PROPERTY (Object::*DipGetFunction)(void);
    typedef void (Object::*DipSetFunction)(PROPERTY );

    TypedPathProperty( DipGetFunction propGetFunction, const DipType& propertyType ) 
        : TypedProperty(propGetFunction, propertyType){
        this->path_ = true;        
    };

    TypedPathProperty( DipGetFunction propGetFunction, DipSetFunction propSetFunction,
        const DipType& propertyType, wstring postfixes)
        : TypedProperty(propGetFunction, propSetFunction, propertyType){
        this->postfixes_ = postfixes;
        this->path_ = true;
    };

    TypedPathProperty( const TypedPathProperty<PROPERTY>& prop ) : TypedProperty( prop ){
        this->postfixes_ = prop.postfixes_;

        this->path_ = true;		
    };

    virtual void Copy( Property** copiedProperty ){
        *copiedProperty = 0;
        *copiedProperty = new TypedPathProperty<PROPERTY>(*this);
    };

protected:	
    
};


template <class ENUM_PROPERTY> class TypedEnumProperty : public Property {
public:
    typedef ENUM_PROPERTY (Object::*GetFunction)(void);
    typedef void (Object::*SetFunction)( ENUM_PROPERTY );

    TypedEnumProperty( GetFunction propGetFunction,
        const ENUM_PROPERTY& lower, 
        const ENUM_PROPERTY& upper ){
        this->Init();
        this->get_function_ = propGetFunction;
        this->enum_        = new TypedEnum<ENUM_PROPERTY>(lower,upper);
        this->value_       = this->enum_;
        this->lower_       = lower;
        this->upper_       = upper;
        this->SetType( DipEnum );
        this->readonly_    = true;
    };

    TypedEnumProperty( GetFunction propGetFunction,
        SetFunction propSetFunction, 
        const ENUM_PROPERTY& lower,
        const ENUM_PROPERTY& upper ){
        this->Init();
        this->lower_        = lower;
        this->upper_        = upper;
        this->get_function_ = propGetFunction;
        this->set_function_ = propSetFunction;		
        this->enum_         = new TypedEnum<ENUM_PROPERTY>(lower,upper);		
        this->value_        = this->enum_;
        this->SetType( DipEnum );
    };

    TypedEnumProperty( GetFunction propGetFunction, SetFunction propSetFunction, 
                          const ENUM_PROPERTY& lower, const ENUM_PROPERTY& upper,
                          wstring* enumNames ){
        this->Init();
        this->lower_           = lower;
        this->upper_           = upper;
        this->get_function_    = propGetFunction;
        this->set_function_    = propSetFunction;
        this->enum_name_count_ = upper - lower + 1;
        this->enum_name_       = enumNames;
        this->enum_            = new TypedEnum<ENUM_PROPERTY>(lower, upper, this->enum_name_count_, enumNames );		
        this->value_           = this->enum_;
        this->SetType( DipEnum );
    };

    void SetEnumNames(const unsigned long& enumNameCount, wstring* enumNames){
        this->enum_name_count_ = enumNameCount;
        this->enum_name_       = enumNames;
        this->enum_->SetEnumNames(enumNameCount, enumNames);
    }

    const vector<wstring> &GetEnumNames(){
        return this->enum_->GetEnumNames();
    }

    TypedEnumProperty( const TypedEnumProperty<ENUM_PROPERTY>& prop ){
        this->Init();
        this->get_function_    = prop.get_function_;
        this->set_function_    = prop.set_function_;	
        this->enum_name_count_ = prop.enum_name_count_;
        this->enum_name_       = prop.enum_name_;
        this->lower_           = prop.lower_;
        this->upper_           = prop.upper_;
        if ( 0 < this->enum_name_count_ ){
            this->enum_ = new TypedEnum<ENUM_PROPERTY>( this->lower_,this->upper_, 
                this->enum_name_count_, this->enum_name_ );
        } else {
            this->enum_ = new TypedEnum<ENUM_PROPERTY>( this->lower_,this->upper_ );
        }		
        this->value_       = this->enum_;
        this->SetType( DipEnum );
        this->SetName( prop.GetName() );
        this->SetDisplayName( prop.GetDisplayName() );		
        this->SetDescription( prop.GetDescription() );
    };

    virtual ~TypedEnumProperty(){
        delete this->enum_;
    };
    
    virtual void Copy( Property** copiedProperty ){
        *copiedProperty = NULL;
        *copiedProperty = new TypedEnumProperty<ENUM_PROPERTY>(*this);
    };

    void Init(){		
        this->get_function_ = 0;		
        this->set_function_ = 0;		
        this->SetType( DipEnum );
        this->enum_name_count_ = 0;
    };

    virtual VariantData* Get( Object* source ){
        if ( (0 != this->get_function_) && (0 != source) ){
            this->value_.SetType(this->GetType());
            this->enum_->Set( (source->*get_function_)() );
            this->value_ = this->enum_;
            return &this->value_;
        } else {
            return 0;
        }
    };

    virtual void Set( Object* source, VariantData* value ){
        if ( (0 != this->set_function_) && (0 != source) ){
            EnumBase* e = *value;
            ENUM_PROPERTY enumVal = (ENUM_PROPERTY)e->Get();
            
            (source->*set_function_)( enumVal );                        
        }
    };

protected:	
    GetFunction get_function_;
    SetFunction set_function_;

    TypedEnum<ENUM_PROPERTY>* enum_;
    ENUM_PROPERTY lower_;
    ENUM_PROPERTY upper_;
    unsigned long enum_name_count_;
    wstring* enum_name_;	
};

} //namespace dip

#endif