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

#include "runtime/reflector.h"

#include <map>
#include <vector>
#include "base/scoped_ptr.h"
#include "runtime/object.h"

namespace dip{
Reflector *Reflector::instance_ = NULL;

Class* Reflector::GetClass(const wstring& className){
    Reflector& classReg = Reflector::GetInstance();
    TClassMap::iterator iter = classReg.class_map_.find(className);
    if(iter != classReg.class_map_.end()){
        return iter->second;
    }

    return NULL;
}

Class* Reflector::GetClassFromID(const wstring& classID){
    Reflector& classReg = Reflector::GetInstance();
    TClassIDMap::iterator iter = classReg.class_id_map_.find(classID);
    if(iter != classReg.class_id_map_.end()){
        return iter->second;
    }

    return NULL;
}


Class* Reflector::GetClass(Object *object){
    return Reflector::GetInstance().GetClass(object->GetClassName());
}

Object* Reflector::CreateNewInstance(const wstring& className){
    Class* class_meta = GetClass(className);
    if(class_meta){
        return class_meta->CreateInstance();
    }

    return NULL;
}

Object* Reflector::CreateNewInstanceFromClassID(const wstring& classID){
    Class* class_meta = GetClassFromID(classID);

    if(class_meta){
        return class_meta->CreateInstance();
    }

    return NULL;
}

void Reflector::AddClass(const wstring& className, Class* classToRegister){
    Reflector& classReg = Reflector::GetInstance();
    classReg.class_map_[className] = classToRegister;
    classReg.class_id_map_[classToRegister->GetID()] = classToRegister;
}

bool Reflector::RegisterMethod(Method* method, const wstring& className){    
    Class* class_meta = GetClass(className);

    if(class_meta){
        class_meta->AddMethod(method);
        return true;
    }

    return false;
}

bool Reflector::RegisterMethodByClassID(Method* method, const wstring& classID){
    Class* class_meta = GetClassFromID(classID);
    if(class_meta){
        class_meta->AddMethod(method);
        return true;
    }

    return false;
}

void Reflector::Init(){

}

void Reflector::Release(){
    Reflector& registry = GetInstance();
    vector<HMODULE>& libraries = registry.libraries_;
    for(vector<HMODULE>::iterator itor = libraries.begin();
        itor != libraries.end();
        ++ itor){
            ::FreeLibrary(*itor);
    }
}

bool Reflector::IsDerivedFrom(const wstring& childName ,
                                  const wstring& supperName){
    Reflector& class_registry = Reflector::GetInstance();

    Class* desc = class_registry.GetClass(childName);
    Class* parent = desc->GetSuperClass();
    while(parent){
        if(parent->GetClassName() == supperName){
            return true;
        }else{
            parent = parent->GetSuperClass();
        }
    }

    return false;
}

Reflector& Reflector::GetInstance(){
    if(instance_ == NULL){
        instance_ = new Reflector();
    }

    return *instance_;
}

void Reflector::Dump(wostream& os){
}

void Reflector::EnumClassNames(vector<wstring> &names){
    Reflector& classReg = Reflector::GetInstance();
    TClassIDMap::iterator iter = classReg.class_map_.begin();
    while(iter != classReg.class_map_.end()){
        names.push_back( iter->first);

        ++iter;
    }
}

void Reflector::Load(const wchar_t* path){
    Reflector& registry = GetInstance();
    WIN32_FIND_DATA ffd;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    std::wstring dir_path(path);
    dir_path.append(_T("\\*.dll"));

    hFind = FindFirstFile(dir_path.c_str(), &ffd);
    if (INVALID_HANDLE_VALUE == hFind){
        return;
    } 
    scoped_array<TCHAR> buffer(new TCHAR[MAX_PATH]);

    do{
        if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)){
            buffer[0] = _T('\0');
            ::PathAppend(buffer.get(), path);
            ::PathAppend(buffer.get(), ffd.cFileName);
            HMODULE mudule = ::LoadLibraryEx(buffer.get(), NULL, 
                LOAD_WITH_ALTERED_SEARCH_PATH);
            
            if(mudule){
                registry.libraries_.push_back(mudule);
            }else{
                LOG_INFO(_T("[Failed to load plugin][0x%x]"),                     
                    GetLastError());
            }
        }
    }while (FindNextFile(hFind, &ffd) != 0);

    FindClose(hFind);
}

} // namespace dip