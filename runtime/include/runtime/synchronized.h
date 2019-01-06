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

#ifndef DIP_RT_MUTEX_LOCK_H_
#define DIP_RT_MUTEX_LOCK_H_

#include "runtime/common.h"
#include "runtime/object.h"

#include <winbase.h>

namespace dip{

class DIP_EXPORT Lockable{
public:
    virtual ~Lockable(){}
    virtual void Lock()   const = 0;
    virtual void Unlock() const = 0;
};

// Mutex lock that is not a DipObject.
class DIP_EXPORT SimpleMutexLock : public Lockable {
public:
    SimpleMutexLock(const wchar_t* name = 0);
    virtual ~SimpleMutexLock();

    virtual void Lock() const;
    virtual void Unlock() const;

protected:
    mutable HANDLE lock_;

private:
    DISALLOW_EVIL_CONSTRUCTORS(SimpleMutexLock);
};

class DIP_EXPORT LLock : public Lockable{
public:
    LLock();
    virtual ~LLock();

    virtual void Lock() const;
    virtual void Unlock() const;

private:
    mutable CRITICAL_SECTION   critical_sec_;
    DISALLOW_EVIL_CONSTRUCTORS(LLock);
};

class DIP_EXPORT MutexLock{
public:
    MutexLock();
    void Lock( void ) const;
    void Unlock( void ) const;

protected:
    SimpleMutexLock   lock_;

private:
    DISALLOW_EVIL_CONSTRUCTORS(MutexLock);
};

inline void MutexLock::Lock( void ) const{
    this->lock_.Lock();
}

inline void MutexLock::Unlock( void ) const{
    this->lock_.Unlock();
}

class DIP_EXPORT LockWrap{
public:
    LockWrap(Lockable *locker);

    ~LockWrap();
private:
    Lockable *locker_;
    DISALLOW_EVIL_CONSTRUCTORS(LockWrap);
};

// Scope based mutual exclusion. Locks
// the object on construction and unlocks
// during destruction. Very convinient to use
// with the macros __mutexScope and __mutexBlock
class DIP_EXPORT AutoSync {
    bool first_time_;
public:
    explicit AutoSync(const Lockable *pLock);
    explicit AutoSync(const Lockable &rLock);
    ~AutoSync();
    // this function is only needed to use with
    // the macro __mutexBlock
    bool FirstTime();
private:
    const Lockable * lock_;
    DISALLOW_EVIL_CONSTRUCTORS(AutoSync);
};

// This macros are used to create a unique name
// We need to go through two steps of expansion.
// Macro kMakeName1 will expand to wstring + number
// and macro kMakeName1 will put them together to create
// the unique name.
#define MAKE_NAME2(x, y) x##y
#define MAKE_NAME1(x, y) MAKE_NAME2(x, y)
#define MAKE_NAME(x) MAKE_NAME1(x, __COUNTER__)

#define __mutexScope(lock) AutoSync MAKE_NAME(hiddenLock)(lock)
#define __mutexBlock(lock) \
    for (AutoSync hiddenLock(lock); hiddenLock.FirstTime(); )

} // namespace dip
#endif