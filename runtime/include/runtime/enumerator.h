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

#ifndef DIP_RT_ENUMERATOR_H_
#define DIP_RT_ENUMERATOR_H_

#include <string>
#include <vector>
#include <map>

#include "runtime/common.h"

namespace dip{

class DIP_EXPORT EnumBase {
public:
    EnumBase(){};
    virtual ~EnumBase(){};

    virtual int  Next()  = 0;
    virtual int  End()   = 0;
    virtual int  Get()   = 0;
    virtual int  Begin() = 0;
    virtual void Set( const int& intVal ) = 0;
    virtual bool IsOneEntry(const wstring& name) = 0;

    operator int () { return Get();	}
    
    virtual wstring ToString() = 0;
    virtual void SetFromString( const wstring& textVal ){
        int i = 0;		
        _stscanf( textVal.c_str(), _T("%d"), &i );
        Set( i );
    };

    typedef vector<wstring>::const_iterator NameIter;
    const vector<wstring> & GetEnumNames() { return this->enum_names_; }

    void SetEnumNames(const unsigned long& enumNameCount, wstring* enumNames){
        this->enum_names_.clear();
        for(unsigned long i = 0; i < enumNameCount; i++){
            this->enum_names_.push_back(enumNames[i]);
        }

        this->names_available_ = this->enum_names_.size() > 0;
    }

protected:
    int lower_;
    int upper_;
    vector<wstring> enum_names_;
    bool names_available_;
};

class DIP_EXPORT EnumRegistry{
public:
    static bool AddEnum(const wstring& name, EnumBase *e);
    static EnumBase *GetEnum(const wstring& name);
    static EnumBase *FindEnum(const wstring& name);
    static EnumRegistry* GetInstance();

protected:
    map<wstring, EnumBase *> enum_map_;
    static EnumRegistry *instance_;
};

template <class ENUM_TYPE> class TypedEnum : public EnumBase {
public:
    TypedEnum( const ENUM_TYPE& lower, const ENUM_TYPE& upper ){
        this->enum_value_ = lower;
        this->lower_      = (int)lower;
        this->upper_      = (int)upper;
        this->enum_names_.clear();
        this->names_available_ = false;
    };
    
    TypedEnum( const ENUM_TYPE& lower, 
        const ENUM_TYPE& upper, 
        const unsigned long& enumNameCount, 
        wstring* enumNames ){
        this->enum_value_ = lower;
        this->lower_      = (int)lower;
        this->upper_      = (int)upper;
        this->enum_names_.clear();
        if ( enumNameCount > 0 ){
            for ( unsigned long i = 0; i < enumNameCount; i++ ){
                this->enum_names_.push_back( enumNames[i] );
            }
        }
        this->names_available_ = this->enum_names_.size() > 0 ? true : false;
    };
 
    virtual ~TypedEnum() {};

    virtual int Next(){
        int i = (int)this->enum_value_;
        i++;		
        if ( i > this->upper_ ){			
            i = this->lower_;
        }
        this->enum_value_ = (ENUM_TYPE) i;

        return i;
    };

    virtual int Begin(){
        this->enum_value_ = (ENUM_TYPE) this->lower_;
        return (int)this->enum_value_;
    };

    virtual int End(){
        this->enum_value_ = (ENUM_TYPE) this->upper_;
        return (int) this->enum_value_;
    };

    virtual int Get(){
        return (int) this->enum_value_;
    };
    
    void Set( const ENUM_TYPE& val ){
        this->enum_value_ = val;
    };

    virtual void Set( const int& intVal ){
        this->enum_value_ = (ENUM_TYPE) intVal;
    }

    ENUM_TYPE GetValAsEnum(){
        return this->enum_value_;
    };

    operator ENUM_TYPE (){
        return GetValAsEnum();
    };	

    TypedEnum<ENUM_TYPE>& operator=( const ENUM_TYPE& val ){
        Set( val );
        return *this;
    };	
    
    virtual bool IsOneEntry(const wstring& name){
        bool found = false;
        if( true == this->names_available_){
            vector<wstring>::iterator it = this->enum_names_.begin();

            while ( it != this->enum_names_.end() ){
                if ( (*it) == name ){
                    found = true;
                    break;
                }
                it++;
            }			
        }

        return found;
    }

    virtual void SetFromString( const wstring& textVal ){
        if ( true == this->names_available_ ){
            vector<wstring>::iterator it = this->enum_names_.begin();
            int i = 0;
            bool found = false;
            while ( it != this->enum_names_.end() ){
                if ( (*it) == textVal ){
                    found = true;
                    break;
                }
                i++;
                it++;
            }
            if ( true == found ){
                Set( i );
            }else{
                EnumBase::SetFromString( textVal );	
            }
        }else{
            EnumBase::SetFromString( textVal );
        }
    };

    virtual wstring ToString(){
        wstring result = _T("");
        
        if ( true == names_available_ ){
            ASSERT1(Get() >= 0);
            ASSERT1(Get() < static_cast<int>(enum_names_.size()));
            result = enum_names_[Get()];
        }else{
            TCHAR tmp[25];
            ZeroMemory( tmp, sizeof(tmp));
            _sntscanf_s( tmp, sizeof(tmp), _T("%d"), Get() );
            result += tmp;
        }
        return result;
    };

private:
    ENUM_TYPE enum_value_;
};

class DIP_EXPORT EnumValue {
public:
    EnumBase* GetEnum() const{
        if ( 0 != this->enum_ ) {
            this->value_ = this->enum_->Get();
        }
        return this->enum_;
    }
    
    int GetEnumVal() const{
        return 	this->value_;
    }

    void Set( const EnumValue& enumVal ){
        this->enum_ = enumVal.enum_;
        if ( 0 != this->enum_ ){
            this->value_ = this->enum_->Get();
        }else{
            this->value_ = 0;
        }
    }

    void Set( const wstring& wstringVal ){
        if ( 0 != this->enum_ ){
            this->enum_->SetFromString( wstringVal );
            this->value_ = this->enum_->Get();
        }else{
            this->value_ = 0;
        }
    }

    void Set( EnumBase* enumVal ){
        this->enum_ = enumVal;
        if ( 0 != this->enum_ ){
            this->value_ = this->enum_->Get();
        }else {
            this->value_ = 0;
        }
    }
    
protected:
    mutable int  value_;
    EnumBase*    enum_;    
};

} // namespace dip
#endif