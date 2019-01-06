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

#pragma once

#include <windows.h>
#include "base/basictypes.h"
#include "runtime/thread_pool.h"

namespace dip{
template <typename T>
class ThreadPoolCallBack0 : public UserWorkItem {
public:
    explicit ThreadPoolCallBack0(T* obj, void (T::*fun)())
        : obj_(obj), fun_(fun) {}
private:
    virtual void DoProcess() {
        (obj_->*fun_)();
    }
    T* obj_;
    void (T::*fun_)();

    DISALLOW_COPY_AND_ASSIGN(ThreadPoolCallBack0);
};

template <typename T, typename P1>
class ThreadPoolCallBack1 : public UserWorkItem {
public:
    explicit ThreadPoolCallBack1(T* obj, void (T::*fun)(P1), P1 p1)
        : obj_(obj), fun_(fun), p1_(p1) {}
private:
    virtual void DoProcess() {
        (obj_->*fun_)(p1_);
    }
    T* obj_;
    void (T::*fun_)(P1);
    P1 p1_;

    DISALLOW_COPY_AND_ASSIGN(ThreadPoolCallBack1);
};

template <typename P1>
class StaticThreadPoolCallBack1 : public UserWorkItem {
public:
    explicit StaticThreadPoolCallBack1(void (*fun)(P1), P1 p1)
        : fun_(fun), p1_(p1) {}
private:
    virtual void DoProcess() {
        (*fun_)(p1_);
    }

    void (*fun_)(P1);
    P1 p1_;

    DISALLOW_COPY_AND_ASSIGN(StaticThreadPoolCallBack1);
};

template <typename T, typename P1, typename P2>
class ThreadPoolCallBack2 : public UserWorkItem {
public:
    explicit ThreadPoolCallBack2(T* obj, void (T::*fun)(P1, P2), P1 p1, P2 p2)
        : obj_(obj), fun_(fun), p1_(p1), p2_(p2) {}
private:
    virtual void DoProcess() {
        (obj_->*fun_)(p1_, p2_);
    }
    T* obj_;
    void (T::*fun_)(P1, P2);
    P1 p1_;
    P2 p2_;

    DISALLOW_COPY_AND_ASSIGN(ThreadPoolCallBack2);
};

} // namespace dip