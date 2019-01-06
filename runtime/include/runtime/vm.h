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
#include <vector>

#include "runtime/common.h"
#include "runtime/object.h"
#include "runtime/thread.h"

namespace dip{

class DIP_EXPORT VM{
public:
    static VM& Instance();

    bool Run();
    bool IsRunning();
    void Stop();
    bool CheckConnectivity();

    int GetInstanceNum(Class* type);
    bool ExecCmd(const wstring& object, 
                const wstring& method, 
                const std::vector<std::wstring>& args);

    void Add(IThread *thread);
    void Add(IEndPoint* render);
    void Add(IProcess* process);

    void Remove(IThread *thread);
    void Remove(IEndPoint* render);
    void Remove(IProcess* process);

private:
    VM();
    
    void AddInternal(IConnectable* object);
    void RemoveInternal(IConnectable* object);
    wstring CreateUniqName(Object* object);    

    typedef std::vector<IThread*>::iterator ThreadIter;
    typedef std::vector<IConnectable*>::iterator ObjectIter;

    bool is_running_;
    std::vector<IThread*> threads_;
    std::vector<IConnectable*> objects_;
    static VM* instance_;

    DISALLOW_EVIL_CONSTRUCTORS(VM);
};

}