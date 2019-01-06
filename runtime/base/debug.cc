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

#include "base/debug.h"
#include <algorithm>

const TCHAR* const kCategoryName[] = {_T("FATAL"), _T("ERROR"),
                                      _T("WARNING"), _T("INFO"), _T("DEBUG"),
                                      _T("CMD"), _T("OPT")};

const int kLogMessageMaxLength = 2048;

void LogMessageW(LOG_CATEGORY category, 
                const wchar_t* file,
                const wchar_t* func,
                int line,
                const wchar_t* fmt, ...){
    va_list args;
    va_start(args, fmt);

    CString user_log;
    _vsnwprintf_s(
        user_log.GetBufferSetLength(kLogMessageMaxLength),
        kLogMessageMaxLength,
        _TRUNCATE,
        fmt,
        args);

    GetLogging()->Log(category, file, func, line, user_log);

    va_end(args);
}

void LogMessageA(LOG_CATEGORY category, 
                const char* file,
                const char* func,
                int line,
                const char* fmt, ...){
    va_list args;
    va_start(args, fmt);

    CStringA user_log;
    _vsnprintf_s(
        user_log.GetBufferSetLength(kLogMessageMaxLength),
        kLogMessageMaxLength,
        _TRUNCATE,
        fmt,
        args);
    GetLogging()->Log(category, file, func, line, CString(user_log));

    va_end(args);
}

void Logging::AddObserver(LoggingObserver* observer){
    if(observers_.size()){
        ObserverIter itor = std::find(observers_.begin(), observers_.end(), observer);
        if(itor != observers_.end()){
            observers_.push_back(observer);
        }
    }else{
        observers_.push_back(observer);
    }
}

void Logging::RemoveObserver(LoggingObserver* observer){ 
    ObserverIter itor = std::find(observers_.begin(), observers_.end(), observer);
    if(itor != observers_.end()){
        observers_.erase(itor);
    }
}

void Logging::Log(LOG_CATEGORY category, 
                const CString& file,
                const CString& func,
                int line,
                const CString& user_log){
    SYSTEMTIME sys_time;
    ::GetLocalTime(&sys_time);    
    CString time;
    time.Format(_T("[ %02u:%02u:%02u ]"), sys_time.wHour, sys_time.wMinute, sys_time.wSecond);

    CString message;
    if(!show_file_info_){
        message.Format(_T("%s%s:%s"), time, kCategoryName[category], user_log);
    }else{ 
        message.Format(_T("%s [%s:%s] [%s::%s::%d]"), time,
                        kCategoryName[category], 
                        user_log, file, func, line);
    }
    
    OnLogged(category, message);
}

void Logging::Log(const CString& msg){
    OnLogged(LC_OPT, msg);
}

void Logging::ShowFileInfo(bool show){
    show_file_info_ = show;
}

void Logging::OnLogged(LOG_CATEGORY cat, const CString& message){
    for(ObserverIter itor = observers_.begin(); itor != observers_.end();
        ++itor){
        (*itor)->LogMessage(cat, message);
    }
}

Logging kLoggingInstance;

Logging* GetLogging(){
    return &kLoggingInstance;
}