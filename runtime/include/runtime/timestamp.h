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

#ifndef DIP_RT_TIME_STAMP_H_
#define DIP_RT_TIME_STAMP_H_

#include "runtime/common.h"

namespace dip{

typedef uint16 time16;
typedef uint64 time64;

class DIP_EXPORT TimeStamp {
public:
    TimeStamp();

    static TimeStamp *New();
    void Delete() {delete this;};
    virtual const wchar_t *GetClassName() {return _T("TimeStamp");};	
    void Modified();
    uint64 GetModifiedTime() const {return this->modified_time_;};
    
    int operator>(TimeStamp& ts) {
    return (this->modified_time_ > ts.modified_time_);};
    int operator<(TimeStamp& ts) {
    return (this->modified_time_ < ts.modified_time_);};

    operator uint64() {return this->modified_time_;};

private:
    uint64 modified_time_;
    bool high_timer_available_;
};

} // namespace dip
#endif