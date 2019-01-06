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

#include "io/flow_factory.h"

#include <atlconv.h>
#include <string>

#include "runtime/reflector.h"
#include "runtime/vm.h"
#include "runtime/framework.h"

#include "flow/process_figure.h"
#include "flow/data_line.h"
#include "flow/inout_port.h"

FlowFactory* FlowFactory::instance_ = NULL;

FlowFactory::FlowFactory(){
    Init();
}

void FlowFactory::Init(){
    type_map_.push_back(new TypeTriple(_T("process"), typeid(ProcessFigure), new ProcessFigure(this)));
    type_map_.push_back(new TypeTriple(_T("line")   , typeid(DataLine),      new DataLine()));
    type_map_.push_back(new TypeTriple(_T("port")   , typeid(InOutPort),     new InOutPort()));
}

FlowFactory& FlowFactory::Instance(){
    if(!instance_){
        instance_ = new FlowFactory();
    }

    return *instance_;
}

IFigure* FlowFactory::CreateFigure(const dip::ClassId& id){   
    IConnectable* unit = CreateObject(id);
    ProcessFigure* figure = dynamic_cast<ProcessFigure*>(Create(_T("process")));
    figure->SetFilter(unit);

    return figure;
}

dip::IConnectable* FlowFactory::CreateObject(const dip::ClassId& id){
    IConnectable* process = NULL;
    dip::Class* type = dip::Reflector::GetClassFromID(id);
    if(type){
        process = dynamic_cast<IConnectable*>(type->CreateInstance());
    }
    ASSERT1(process);
    return process;
}

CString FlowFactory::GetName(IDOMStorable* o){
    for(size_t i = 0; i < type_map_.size(); ++i){
        if(typeid(*o) == type_map_[i]->type){
            return type_map_[i]->name;
        }
    }

    LOG_ERROR(_T("[FlowFactory::GetName][Not support type.]"));
    ASSERT1(false);

    return CString(_T("unknown"));
}

IDOMStorable* FlowFactory::Create(const CString& name){
    for(size_t i = 0; i < type_map_.size(); ++i){
        if(name == type_map_[i]->name){
            return type_map_[i]->prototype->Clone();
        }
    }
    
    LOG_ERROR(_T("[FlowFactory::Create][Not support type.]"));
    ASSERT1(false);
    return NULL;
}

void FlowFactory::Write(IDOMOutput& out, IDOMStorable* object){
    object->Write(out);
}

IDOMStorable* FlowFactory::Read(IDOMInput& in){    
    return Create(in.GetTagName()); 
}