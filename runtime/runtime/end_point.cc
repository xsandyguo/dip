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

#include "runtime/end_point.h"

#include "base/debug.h"
#include "runtime/reflector.h"
#include "runtime/vm.h"

namespace dip{

BEGIN_REGISTER_ABSTRACT_CLASSINFO( EndPoint, Presentation, \
    "{F96533F7-B5EC-49C1-927A-1263DCD3D7E8}")
END_REGISTER_CLASSINFO( EndPoint)

EndPoint::EndPoint(int in_num)
    :Presentation(in_num, 1){
    inputs_.reset(new Connector*[GetNumInPins()]);
    ZeroMemory(inputs_.get(), sizeof(Connector*) * GetNumInPins());      
}

EndPoint::~EndPoint(void){
}

void EndPoint::Register(){
    VM::Instance().Add(this);
}

void EndPoint::UnRegister(){
    VM::Instance().Remove(this);
}

bool EndPoint::CanConnect(IConnectable *prev, int outIdx, int inIdx){
    IProcess* process = dynamic_cast<IProcess*>(prev);
    if(!process){
        LOG_ERROR(_T("[EndPoint::CanConnect][Only Process can connects to EndPoint]"));
        return false;
    }

    return true;
}

const ImageData* EndPoint::GetInData(int inIdx){
    ASSERT1(inIdx < GetNumInPins());
    Connector* connector = inputs_[inIdx];    
    return connector->target->GetOutData(connector->index);
}

void EndPoint::Connect(IConnectable *prev, int outIdx, int inIdx){    
    ASSERT1(CanConnect(prev, outIdx, inIdx));

    IProcess* process = dynamic_cast<IProcess*>(prev);  

    if(!inputs_[inIdx]){
        inputs_[inIdx] = new Connector;
    }

    inputs_[inIdx]->target = process;
    inputs_[inIdx]->index = outIdx;
}

void EndPoint::Disconnect(IConnectable *prev, int outIdx, int inIdx){
   inputs_[inIdx]->target = NULL;   
}

int EndPoint::GetValidInNum(){
    int counter = 0;
    if(!inputs_.get()){
        return counter;
    }

    for(int i = 0; i < GetNumInPins(); i++){
        if(inputs_[i] && inputs_[i]->target){
            counter ++;
        }
    }

    return counter;
}

}