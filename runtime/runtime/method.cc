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

#include "runtime/method.h"

namespace dip{

DipType Method::GetArgumentType( const int& idx ){
    ASSERT1(arg_types_.length() % 2 == 0);

    if(arg_types_.length()){
        wstring type_code(arg_types_.substr(idx * 2, 2));

        for (int i = 0; i < kDipTypeMap.size; ++i) {
            if(kDipTypeMap.table[i].code == type_code){
                return kDipTypeMap.table[i].type;
            }
        }
        ASSERT1(false);
    }
    return DipUndefined;
}

}