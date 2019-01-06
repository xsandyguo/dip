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

#ifndef DIP_RT_PRESENTATION_H_
#define DIP_RT_PRESENTATION_H_

#include "base/scoped_ptr.h"
#include "runtime/common.h"
#include "runtime/connector.h"
#include "runtime/object.h"
#include "runtime/framework.h"
#include "runtime/class_info.h"

namespace dip{

class DIP_EXPORT Presentation : public Object, public virtual IConnectable{
public:
    DECLEAR_ABSTRACT_CLASSINFO(Presentation, Object)
        
    virtual int GetRequiredInNum();   
    virtual int GetNumInPins();
    virtual int GetNumOutPins();
    
    virtual Object& GetObject();
    virtual IConnectable* Clone();
    
protected:
    Presentation(int in_num, int out_num);
    virtual ~Presentation(){}

    void SetRequriedInNum(int num);

private:
    int required_in_num_;
    int in_num_;
    int out_num_;
    
    DISALLOW_EVIL_CONSTRUCTORS(Presentation);
};

} // namespace dip
#endif