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

#include "runtime/process.h"

#include "runtime/vm.h"
#include "runtime/image_data.h"

namespace dip{

BEGIN_REGISTER_ABSTRACT_CLASSINFO(Process, Presentation, \
    "{E7FC954E-5A99-4EB0-886E-49B1F3B8B827}")

END_REGISTER_CLASSINFO(Process)

Process::Process(int input_num , int output_num)
    :Presentation(input_num, output_num),
    required_input_num_(input_num),
    updating_(0),
    is_initialized_(0){
    CreateImageData();
}

Process::~Process(){
    for(int i = 0; i < GetNumOutPins(); ++i){
        ImageData* data = this->outputs_[i];
        if(data){
            delete data;
        }
    }    
}


void Process::Register(){
    VM::Instance().Add(this);
}

void Process::UnRegister(){
    VM::Instance().Remove(this);
}

int Process::GetValidInNum(){
    int counter = 0;
    for(int i = 0; i < GetNumInPins(); ++i){
        counter += inputs_[i] && inputs_[i]->target ? 1 : 0;
    }

    return counter;
}

void Process::RemoveInput(int idx){
    ASSERT1(idx < GetNumInPins());

    if(inputs_[idx]){        
        inputs_[idx]->target = NULL;
        inputs_[idx]->index = -1;
    }
}

const ImageData* Process::GetInData(int idx){
    Connector* pair = inputs_[idx];    
    if(pair && pair->target){
        return pair->target->GetOutData(pair->index);
    }

    return NULL;
}

ImageData* Process::GetOutDataDirect(int index){
    return outputs_[index];
}

ImageData* Process::DoGetOutData(int idx){
    ASSERT1(idx < GetNumOutPins());
    ImageData* data = outputs_[idx];

    uint64 input_changed_time = 0;
    for(int i = 0; i < GetNumInPins(); ++i){
        const ImageData* data = GetInData(i);
        if(data){
            uint64 creation_time = data->GetModifiedTime();
            if(creation_time > input_changed_time){
                input_changed_time = creation_time;
            }            
        }
    }

    uint64 last_exec_time = 0;
    for(int i = 0; i < GetNumOutPins(); ++i){
        ImageData* data = GetOutDataDirect(i);
        uint64 creation_time = data->GetModifiedTime();
        if(creation_time > last_exec_time){
            last_exec_time = creation_time;
        }                    
    }    
    
    uint64 self_mtime = GetModifiedTime();
    if(self_mtime > last_exec_time ||          // new config
       input_changed_time > last_exec_time){       // new in
        if(!Execute()){
            LOG_ERROR(_T("[Process %s executes failed]"), get_name().c_str());
        }

        for(int i = 0; i < GetNumOutPins(); ++i){
            ImageData* data = GetOutDataDirect(i);
            data->Modified();
        }
    }

    return data;
}

const ImageData* Process::GetOutData(int idx){
    __mutexScope(lock_);

    return InitalizeIfnecessary() ? DoGetOutData(idx) : NULL;
}

void Process::CreateImageData(){
    if(GetNumInPins() > 0){
        inputs_.reset(new Connector*[GetNumInPins()]);
        for(int i = 0; i < GetNumInPins(); i++){
            inputs_[i] = NULL;
        }
    }

    if(GetNumOutPins() > 0){
        outputs_.reset(new ImageData*[GetNumOutPins()]);
        for(int i = 0; i < GetNumOutPins(); ++i){
            ImageData* data = new ImageData;
            data->set_owner(this);
            this->outputs_[i] = data;
        }
    }
}

bool Process::InitalizeIfnecessary(){
    if(::InterlockedCompareExchange(&is_initialized_, true, false)){
        return true;
    }

    if(!Initialize()){
        LOG_ERROR(_T("[%s initialize failed][Execution VM will be stopped.]"), 
                    get_name().c_str());
        return false;
    }else{
        // Update Timestamp
        for(int i = 0; i < GetNumOutPins(); ++i){
            ImageData* data = GetOutDataDirect(i);
            data->Modified();
        }
        Modified();
    }

    return true;
}

bool Process::Execute(){
    return true;
}

bool Process::Terminate(){
    is_initialized_ = false;

    return true;
}

ImageFormat& Process::GetOutFormat(int index){
    ASSERT1(index < GetNumOutPins());

    return outputs_[index]->get_format();
}

bool Process::CanAccept(int inIdx, ImageFormat& format){        
    return true;
}

bool Process::CanConnect(IProcess& prev, int outIdx, int inIdx){
    return CanAccept(inIdx, prev.GetOutFormat(outIdx));
}

void Process::Connect(IConnectable *prev, int outIdx, int inIdx){
    IProcess* process = dynamic_cast<IProcess*>(prev);
    ASSERT1(CanConnect(prev, outIdx, inIdx));

    Connect(*process, outIdx, inIdx);
}

void Process::Connect(IProcess& prev, int outIdx, int inIdx)   {
    if(!inputs_[inIdx]){
        inputs_[inIdx] = new Connector;
    }

    inputs_[inIdx]->target = &prev;
    inputs_[inIdx]->index = outIdx;
}

void Process::Disconnect(IProcess& prev, int outIdx, int inIdx){
    RemoveInput(inIdx);
}

bool Process::CanConnect(IConnectable *prev, int outIdx, int inIdx){
    ASSERT1(prev);     
    if(inIdx >= GetNumInPins() || prev->GetType() != DOT_PROCESS){
        false;
    }

    IProcess* process = dynamic_cast<IProcess*>(prev);
    ASSERT1(process);

    return CanConnect(*process, outIdx, inIdx);
}

void Process::Disconnect(IConnectable *prev, int outIdx, int inIdx){
    ASSERT1(prev);     
    ASSERT1(inIdx < GetNumInPins());

    IProcess* process = dynamic_cast<IProcess*>(prev);
    if(!process){
        LOG_ERROR(_T("That is implemented IConnectable can be connected."));
        return;
    }
    Disconnect(*process, outIdx, inIdx);
}


} // namespace dip