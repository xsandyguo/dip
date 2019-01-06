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

#include "io/tixml_dom.h"

#include "base/debug.h"
#include "base/string.h"
#include "io/serialize_const.h"
#include "io/factory.h"

#define  UTF82T(x) \
    Utf8ToWideChar(x, strlen(x))

#define  T2UTF8(x) \
    WideToUtf8(x)

const char* const kXmlEncoding = "UTF-8";

TiXmlDomInput::TiXmlDomInput(const CString & file_path, IDOMFactory& factory)
                            :current_(NULL),
                            document_(NULL),
                            factory_(factory){
    document_.reset(new TiXmlDocument());
    document_->SetCondenseWhiteSpace(false);

    
    document_->LoadFile(T2UTF8(file_path), TIXML_ENCODING_UTF8);

    current_ = document_->FirstChildElement(kDesign);
}

TiXmlDomInput::~TiXmlDomInput(){
}

CString TiXmlDomInput::GetTagName(){
    return  CString(current_->Value());
}

CString TiXmlDomInput::GetAttribute(const CString& name, const CString& defaultValue){
    string val;
    
    if(TIXML_SUCCESS != current_->QueryStringAttribute(T2UTF8(name), &val)){
        return defaultValue;
    }

    return UTF82T(val.c_str());
}

int TiXmlDomInput::GetAttribute(const CString& name, int defaultValue){
    int val = 0;
    
    if(TIXML_SUCCESS != current_->QueryIntAttribute(T2UTF8(name), &val)){
        return defaultValue;
    }
    return val;
}

double TiXmlDomInput::GetAttribute(const CString& name, double defaultValue){
    double val = 0.0;
    
    if(TIXML_SUCCESS != current_->QueryDoubleAttribute(T2UTF8(name), &val)){
        return defaultValue;
    }
    return val;
}

bool TiXmlDomInput::GetAttribute(const CString& name, bool defaultValue){
    bool val = false;
    
    if(TIXML_SUCCESS != current_->QueryBoolAttribute(T2UTF8(name), &val)){
        return defaultValue;
    }
    return val;
}

int TiXmlDomInput::GetElementCount(){
    int count = 0;

    TiXmlNode* node = current_->FirstChild();
    while(node){
        count ++;
        node = current_->IterateChildren(node);
    }
    return count;
}

void TiXmlDomInput::OpenElement(const CString& tagName){
    stack_.push(current_);
    
    current_ = current_->FirstChildElement(T2UTF8(tagName));
}

void TiXmlDomInput::CloseElement(){
    current_ = stack_.top();
    stack_.pop();
}

void TiXmlDomInput::OpenElement(int i){
    stack_.push(current_);

    TiXmlElement* element = current_->FirstChildElement();
    while(i-- > 0){
        element = element->NextSiblingElement();
    }

    current_ = element;
}

IDOMStorable* TiXmlDomInput::ReadObject(){
    return ReadObject(0);
}

IDOMStorable* TiXmlDomInput::ReadObject(int i){
    OpenElement(i);
    IDOMStorable* storable = NULL;
    int id  = GetAttribute(_T("id"), -1);
    int ref = GetAttribute(_T("ref"), -1);

    if(id != -1){
        storable = factory_.Read(*this);
        ASSERT1(storable);

        idobjects_[id] = storable;

        storable->Read(*this);
    }else{
        if(ref != -1 && idobjects_.count(ref)){
            storable = idobjects_[ref];
        }        
    }
    CloseElement();
    ASSERT1(storable);

    return storable;
}


TiXmlDomOutput::TiXmlDomOutput(IDOMFactory& factory)
                                :document_(NULL),
                                current_(NULL),
                                factory_(factory){
    document_.reset(new TiXmlDocument());
    document_->SetCondenseWhiteSpace(false);

    TiXmlDeclaration * header = new TiXmlDeclaration( "1.0", kXmlEncoding, "" );
    document_->LinkEndChild( header );
    TiXmlElement* root = new TiXmlElement(kDesign);
    document_->LinkEndChild(root);

    current_ = root;
}

TiXmlDomOutput::~TiXmlDomOutput(){
}

void TiXmlDomOutput::Save(const CString& file_path){
    ASSERT1(document_.get());
    
    document_->SaveFile(T2UTF8(file_path));    
}

void TiXmlDomOutput::AddAttribute(const CString& name, const CString& val){
    
    current_->SetAttribute(T2UTF8(name), T2UTF8(val));
}

void TiXmlDomOutput::AddAttribute(const CString& name, int val){
    
    current_->SetAttribute(T2UTF8(name), val);
}
void TiXmlDomOutput::AddAttribute(const CString& name, double val){
    
    current_->SetDoubleAttribute(T2UTF8(name), val);
}

void TiXmlDomOutput::AddAttribute(const CString& name, bool val){
    
    current_->SetAttribute(T2UTF8(name), val);
}

CString TiXmlDomOutput::GetTagName(){
    return CString(current_->Value());
}

void TiXmlDomOutput::OpenElement(const CString& tagName){
    
    TiXmlElement* element = new TiXmlElement(T2UTF8(tagName));
    current_->LinkEndChild(element);
    stack_.push(current_);

    current_ = element;
}

void TiXmlDomOutput::CloseElement(){
    current_ = stack_.top();
    stack_.pop();
}

void TiXmlDomOutput::WriteObject(IDOMStorable* storable){
     CString tagName = factory_.GetName(storable);
     ASSERT1(tagName.GetLength());

     OpenElement(tagName);

     if(idobjects_.count(storable)){
         AddAttribute(_T("ref"), idobjects_[storable]);
     }else{
         AddAttribute(_T("id"), static_cast<int>(idobjects_.size()));   
         idobjects_[storable] = idobjects_.size();

         factory_.Write(*this, storable);
     }
     CloseElement();
}