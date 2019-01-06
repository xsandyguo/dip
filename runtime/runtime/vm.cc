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

#include "runtime/vm.h"

#include <algorithm>
#include "base/debug.h"
#include "base/scoped_ptr.h"

namespace dip{

VM* VM::instance_ = NULL;

VM::VM()
:is_running_(false){
}

VM& VM::Instance(){
    if(!instance_){
        instance_ = new VM();
    }

    return *instance_;
}

void VM::Add(IThread *thread){
    ASSERT1(thread);
    threads_.push_back(thread);
    AddInternal(thread);
}

void VM::Add(IEndPoint* render){
    ASSERT1(render);
    AddInternal(render);
}

void VM::Add(IProcess* process){
    ASSERT1(process);
    AddInternal(process);
}

void VM::Remove(IThread *thread){
    ASSERT1(thread);

    ThreadIter tpos = std::find(threads_.begin(), threads_.end(), thread);
    if(tpos != threads_.end()){
        threads_.erase(tpos);
    }

    RemoveInternal(thread);
}

void VM::Remove(IEndPoint* render){
    ASSERT1(render);
    RemoveInternal(render);
}

void VM::Remove(IProcess* process){
    ASSERT1(process);
    RemoveInternal(process);
}

void VM::AddInternal(IConnectable* object){
    objects_.push_back(object);

    object->GetObject().set_name(CreateUniqName(&object->GetObject()));
}

void VM::RemoveInternal(IConnectable* object){
    ObjectIter pos = std::find(objects_.begin(), objects_.end(), object);
    if(pos != objects_.end()){
        objects_.erase(pos);
    }else{
        LOG_ERROR(_T("[%s doesn't exist in execution engine]"), 
            object->GetObject().get_name().c_str());
    }            
}

wstring VM::CreateUniqName(Object* object){
    TCHAR buffer[128];
    buffer[0] = _T('\0');
    uint32 num = 1;
    bool existed;

    wstring unique;
    do {
        existed = false;
        _sntprintf(buffer, arraysize(buffer), _T("%s%u"), object->GetClassName(), 
            num);
        unique = wstring(buffer);
        for(ObjectIter iter = objects_.begin(); iter != objects_.end(); ++iter) {            
            if((*iter)->GetObject().get_name() == unique){
                existed = true;
                break;
            }
        }
        ++num;
    } while (existed);

    return unique;
}

bool VM::Run(){
    LOG_INFO(_T("[Run vm begin...]"));

    if(is_running_){
        LOG_ERROR(_T("[VM is already running]"));
        return false;
    }

    if(!CheckConnectivity()){
        return false;
    }

    for(ThreadIter iter = threads_.begin(); iter != threads_.end(); ++iter){
        IThread* thread = *iter;
        if(!thread->Run()){
            return false;
        }
    }

    LOG_INFO(_T("[Run vm complete!]"));
    return is_running_ = true;
}

bool VM::IsRunning(){
    return is_running_;
}

void VM::Stop(){
    if(!is_running_){
        LOG_ERROR(_T("[VM is not running.]"));

        return;
    }

    for(ThreadIter iter = threads_.begin(); iter != threads_.end(); ++iter){
        IThread* thread = *iter;
        thread->Stop();
    }

    for(ObjectIter iter = objects_.begin(); iter != objects_.end(); ++iter){        
        IProcess* process = dynamic_cast<IProcess*>(*iter);
        if(process){
            if(!process->Terminate()){
                LOG_ERROR(_T("[Terminate failed %s]"),
                          process->GetObject().get_name().c_str());
            }
        }
    }

    is_running_ = false;
}

int VM::GetInstanceNum(Class* type){
    int counter = 0;
    for(ObjectIter iter = objects_.begin(); iter != objects_.end(); ++iter){        
        Object& obj = (*iter)->GetObject();
        if(obj.GetClass() == type){
            counter++;
        }
    }

    return counter;
}

bool VM::CheckConnectivity(){
    if(!threads_.size()){
        LOG_ERROR(_T("[Thread block is required.]"));
        return false;
    }

    bool check = true;
    for (ObjectIter iter = objects_.begin(); iter != objects_.end(); ++iter){
        IConnectable* connectable = *iter;
        if(connectable->GetValidInNum() < connectable->GetRequiredInNum()){
            LOG_ERROR(_T("[%s requires in num:%d, but valid input is %d]"),
                    connectable->GetObject().get_name().c_str(),
                    connectable->GetRequiredInNum(),
                    connectable->GetValidInNum());

            check = false;
        }
    }

    return check;
}


bool VM::ExecCmd(const wstring& object_name, 
                              const wstring& method_name, 
                              const std::vector<std::wstring>& args){                                  
    LOG_INFO(_T("[Prepared to execute command on object of %s.]"), 
                 object_name.c_str());
    Object* obj = NULL;
    for (ObjectIter iter = objects_.begin(); iter != objects_.end(); ++iter){
        IConnectable* item = *iter;
        if(item->GetObject().get_name() == object_name){
            obj = &item->GetObject();
            break;
        }
    }

    if(!obj){
        LOG_ERROR(_T("[VM::ExecCmd][Object %s is not found.]"), 
                 object_name.c_str());
        return false;
    }

    Class* meta = obj->GetClass();
    vector<Method*> methods;
    meta->EnumMethods(methods);

    Method* method = NULL;
    for (vector<Method*>::iterator iter = methods.begin(); 
        iter != methods.end();
        ++iter){
        if((*iter)->GetName() == method_name){
            method = *iter;
            break;
        }
    }

    if(!method){
        LOG_ERROR(_T("[VM::ExecCmd][Method %s is not found on %s.]"), 
                method_name.c_str(),
                obj->get_name().c_str());

        return false;
    }

    if(method->GetArgCount() != args.size()){
        LOG_ERROR(_T("[VM::ExecCmd][Parameter for method %s doesn't match.]")
                    _T("[Need %d but only input %d]"), 
                    method_name.c_str(),
                    method->GetArgCount(),
                    args.size());
        return false;
    }

    try{
        int arg_count = method->GetArgCount();

        scoped_array<VariantData*> arguments(new VariantData*[arg_count]);
        for(int i = 0; i < arg_count; ++i){
            DipType type = method->GetArgumentType(i);
            scoped_ptr<VariantData> data(new VariantData());
            data->SetType(type);
            ASSERT1(type != DipUndefined);

            if(!data->SetFromString(args[i])){
                LOG_ERROR(_T("[VM::ExecCmd][Can't convert %s to type %s]"),
                          args[i].c_str(),
                          GetTypeString(type));

                return false;
            }

            arguments[i] = data.release();
        }

        // don't release the returned object
        VariantData* retval =  method->Invoke(arguments.get(), obj);
        for(int i = 0; i < method->GetArgCount(); ++i){
            delete arguments[i];
        }

        if(method->HasReturn()){
            if(retval){
                LOG_INFO(_T("[VM::ExecCmd][The call %s %s returns value is:%s]"),
                     retval->ToString().c_str());
            }
        }
    }catch(...){
        LOG_ERROR(_T("[VM::ExecCmd][An error occured when execute %s]"),
                 method_name.c_str());
        return true;
    }

    LOG_INFO(_T("[Execute command complete!]"));
    return true;
}

}