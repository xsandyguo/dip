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

#include "runtime/presentation.h"

namespace dip{

BEGIN_REGISTER_ABSTRACT_CLASSINFO(Presentation, Object, \
    "{7695557a-13fb-4de3-a977-df77b9503993}")

END_REGISTER_CLASSINFO(Presentation)

Presentation::Presentation(int in_num, int out_num)
    :in_num_(in_num), out_num_(out_num), required_in_num_(in_num){
    ASSERT1(in_num >=0);
    ASSERT1(out_num >= 0);
}

int Presentation::GetNumInPins(){
    return in_num_;
}

int Presentation::GetNumOutPins() {
    return out_num_;
}

int Presentation::GetRequiredInNum(){
    return required_in_num_;
}

void Presentation::SetRequriedInNum(int num){
    required_in_num_ = num;
}

Object& Presentation::GetObject(){
    return *this;
}
    
IConnectable* Presentation::Clone(){
    return  (Presentation*)(Reflector::CreateNewInstance(
        this->GetClassName()));  
}


} // namespace dip