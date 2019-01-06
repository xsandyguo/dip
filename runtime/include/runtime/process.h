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

#ifndef DIP_RT_IMAGE_PROCESS_H_
#define DIP_RT_IMAGE_PROCESS_H_

#include "base/scoped_ptr.h"

#include "runtime/common.h"
#include "runtime/connector.h"
#include "runtime/framework.h"
#include "runtime/image_format.h"
#include "runtime/image_data.h"
#include "runtime/presentation.h"
#include "runtime/synchronized.h"

namespace dip{
    
class DIP_EXPORT Process : public Presentation, public virtual IProcess {
public:
    DECLEAR_ABSTRACT_CLASSINFO(Process, Presentation)
        
    int GetValidInNum();
    virtual int get_required_inputs() { return this->required_input_num_; }

    virtual void RemoveInput(int idx);
    
    virtual void Connect(IConnectable *prev, int outIdx, int inIdx);
    virtual void Disconnect(IConnectable *prev, int outIdx, int inIdx);
    virtual bool CanConnect(IConnectable *prev, int outIdx, int inIdx);

    virtual bool Execute();
    virtual bool Terminate();
    virtual bool CanAccept(int inIdx, ImageFormat& format);

    const ImageData* GetOutData(int idx);
    const ImageData* GetInData(int idx);
    ImageFormat& GetOutFormat(int index);

    void SetInput(IProcess& source, int in_index);
    void Register();
    void UnRegister();

protected:
    Process(int input_num = 1, int output_num = 1);
    virtual ~Process();

    virtual bool Initialize(){ return true; }

    virtual bool CanConnect(IProcess& prev, int outIdx, int inIdx);
    virtual void Connect(IProcess& prev, int outIdx, int inIdx)   ;
    virtual void Disconnect(IProcess& prev, int outIdx, int inIdx);

    void set_required_inputs(int value){ this->required_input_num_ = value;}    
    void set_input_num(int num);
    void set_output_num(int num);

    ImageData* GetOutDataDirect(int index);

private:
    void CreateImageData();
    ImageData* DoGetOutData(int index);
    bool InitalizeIfnecessary();

    int required_input_num_;
    int updating_;  
    volatile LONG is_initialized_;

    LLock lock_;
    scoped_array<ImageData*> outputs_;     // An Array of the outputs to the filter
    scoped_array<Connector*> inputs_;     // An Array of the inputs to the filter

    DISALLOW_EVIL_CONSTRUCTORS(Process);
};
} // namespace
#endif