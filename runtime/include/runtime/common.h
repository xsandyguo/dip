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

#ifndef DIP_RT_COMMON_H_
#define DIP_RT_COMMON_H_

#include "base/basictypes.h"
#include "base/debug.h"

namespace dip{

#pragma warning(disable : 4244) //'variable' : conversion from 'type' to 'type', possible loss of data
#pragma warning(disable : 4305) //'identifier' : truncation from 'type1' to 'type2'
#pragma warning(disable : 4309) // 'conversion' : truncation of constant value
#pragma warning(disable : 4996) 
#pragma warning(disable : 4251) // 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
#pragma warning(disable : 4250) // 'class1' : inherits 'class2::member' via dominance
#pragma warning(disable : 4407) // cast between different pointer to member representations, compiler may generate incorrect code
#pragma warning(disable : 4075)  //init_seg

using namespace std;

} //namespace dip
#endif