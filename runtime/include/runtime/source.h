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

#ifndef DIP_RT_IMAGE_SOURCE_H_
#define DIP_RT_IMAGE_SOURCE_H_

#include <ostream>

#include "runtime/common.h"
#include "runtime/process.h"

namespace dip{

class DIP_EXPORT Source : public Process{
public:
    DECLEAR_ABSTRACT_CLASSINFO(Source, Process)
    
protected:
    Source(int output_num = 1);
    virtual ~Source(){};

private:
    DISALLOW_EVIL_CONSTRUCTORS(Source);
};

} //namespace dip

#endif
