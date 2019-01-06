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

#ifndef DIP_RT_FRAMEWORK_H_
#define DIP_RT_FRAMEWORK_H_

#include "runtime/common.h"
#include "runtime/image_format.h"
#include "runtime/image_data.h"

namespace dip{

class IConnectable;
class IProcess;
class IThread;
class IRender;
class IMapper;

enum DipObjectType{
    DOT_PROCESS,
    DOT_THREAD,
    DOT_WINDOW,
    DOT_MAPPER
};

class DIP_EXPORT IConnectable{
public:
    IConnectable(){}
    virtual ~IConnectable(){}

    virtual int GetRequiredInNum() = 0;
    virtual int GetValidInNum() = 0;
    virtual int GetNumInPins() = 0;
    virtual int GetNumOutPins() = 0;

    virtual bool CanConnect(IConnectable *prev, int outIdx, int inIdx) = 0;
    virtual void Connect(IConnectable *prev, int outIdx, int inIdx)    = 0;
    virtual void Disconnect(IConnectable *prev, int outIdx, int inIdx) = 0;
    
    virtual Object& GetObject()    = 0;
    virtual IConnectable* Clone()  = 0;

    virtual void Register() = 0;
    virtual void UnRegister() = 0;

    virtual DipObjectType GetType() = 0;

private:
    DISALLOW_EVIL_CONSTRUCTORS(IConnectable);
};

class DIP_EXPORT IProcess : public virtual IConnectable{
public:
    IProcess(){}
    virtual ~IProcess(){}
    
    virtual bool Initialize() = 0;
    virtual bool Execute()    = 0;
    virtual bool Terminate()  = 0;

    virtual const ImageData* GetInData(int in_index)   = 0;
    virtual const ImageData* GetOutData(int out_index) = 0;

    virtual ImageFormat& GetOutFormat(int out_index) = 0;

    virtual bool CanConnect(IProcess& prev, int outIdx, int inIdx) = 0;
    virtual void Connect(IProcess& prev, int outIdx, int inIdx)    = 0;
    virtual void Disconnect(IProcess& prev, int outIdx, int inIdx) = 0;

    DipObjectType GetType(){ return DOT_PROCESS; }
private:
    DISALLOW_EVIL_CONSTRUCTORS(IProcess);
};


class DIP_EXPORT IEndPoint: public virtual IConnectable{
public:
    IEndPoint(){}
    virtual ~IEndPoint(){}

    virtual bool Initialize() = 0;
    virtual bool Execute()  = 0;
    virtual bool Dispose()   = 0;

private:
    DISALLOW_EVIL_CONSTRUCTORS(IEndPoint);
};

class DIP_EXPORT IRender: public virtual IEndPoint{
public:
    IRender(){}
    virtual ~IRender(){}

    DipObjectType GetType(){ return DOT_WINDOW; }
private:
    DISALLOW_EVIL_CONSTRUCTORS(IRender);
};

class DIP_EXPORT IMapper : public virtual IEndPoint{
public:
    IMapper(){}
    virtual ~IMapper(){}

    DipObjectType GetType(){ return DOT_MAPPER; }
private:
    DISALLOW_EVIL_CONSTRUCTORS(IMapper);
};

class DIP_EXPORT IThread: public virtual IConnectable{
public:
    IThread(){}
    virtual ~IThread(){}

    virtual bool Run()  = 0;
    virtual void Stop() = 0;
    virtual bool IsRunning() = 0;

    DipObjectType GetType(){ return DOT_THREAD; }
private:
    DISALLOW_EVIL_CONSTRUCTORS(IThread);
};

}

#endif