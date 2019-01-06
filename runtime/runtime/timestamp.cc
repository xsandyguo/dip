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

#include "runtime/timestamp.h"

#include "base/debug.h"
namespace dip{

time64 FileTimeToTime64(const FILETIME & file_time) {
    return static_cast<time64>(
        file_time.dwHighDateTime) << 32 | file_time.dwLowDateTime;
}

time64 GetCurrent100NSTime() {
    // get the current time in 100-nanoseconds intervals
    FILETIME file_time;
    ::GetSystemTimeAsFileTime(&file_time);

    time64 time = FileTimeToTime64(file_time);
    return time;
}

#define kMillisecsTo100ns (10000ULL)
#define kSecsTo100ns (1000 * kMillisecsTo100ns)

int32 Time64ToInt32(const time64 & time) {
    // convert to 32-bit format
    // time() (32-bit) measures seconds since 1970/01/01 00:00:00 (UTC)
    // FILETIME (64-bit) measures 100-ns intervals since 1601/01/01 00:00:00 (UTC)

    // seconds between 1601 and 1970
    time64 t32 = (time / kSecsTo100ns) -
        ((time64(60*60*24) * time64(365*369 + 89)));
    ASSERT1(t32 == (t32 & 0x7FFFFFFF));  // make sure it fits

    // cast at the end (avoids overflow/underflow when computing 32-bit value)
    return static_cast<int32>(t32);
}

TimeStamp::TimeStamp():modified_time_(0){
    LARGE_INTEGER ticks;
    high_timer_available_ = !!QueryPerformanceCounter(&ticks);
}

TimeStamp *TimeStamp::New(){
    return new TimeStamp();
}

void TimeStamp::Modified(){
    LARGE_INTEGER ticks;

    if(high_timer_available_){
        QueryPerformanceCounter(&ticks);
        modified_time_ = ticks.QuadPart;
    }else{
        this->modified_time_ = GetCurrent100NSTime();
    }
}

} // namespace dip