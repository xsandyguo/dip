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

#ifndef DIP_RT_END_POINT_H_
#define DIP_RT_END_POINT_H_

#include "base/scoped_ptr.h"
#include "runtime/object.h"
#include "runtime/common.h"
#include "runtime/connector.h"
#include "runtime/image_data.h"
#include "runtime/framework.h"
#include "runtime/presentation.h"

namespace dip{

class DIP_EXPORT EndPoint : public Presentation, public IEndPoint{
public:
    DECLEAR_ABSTRACT_CLASSINFO(EndPoint, Presentation)

    virtual bool CanConnect(IConnectable *prev, int outIdx, int inIdx);
    virtual void Connect(IConnectable *prev, int outIdx, int inIdx)   ;
    virtual void Disconnect(IConnectable *prev, int outIdx, int inIdx);

    virtual bool Initialize(){ return true; }
    virtual int GetValidInNum();

    void Register();
    void UnRegister();

protected:
    EndPoint(int in_num);
    virtual ~EndPoint();  

    const ImageData* GetInData(int inIdx = 0);

private:
    scoped_array<Connector*> inputs_; 

    DISALLOW_EVIL_CONSTRUCTORS(EndPoint);
};

} // namespace end
#endif