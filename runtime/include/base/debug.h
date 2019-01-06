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

#include <assert.h>
#include "base/basictypes.h"

#ifdef _AFX
#include <afxstr.h>
#else
#include <atlstr.h>
#endif

#include <vector>

#define ASSERT1(x) assert(x)
 
#if defined DEBUG || defined _DEBUG
#define VERIFY1(x) assert(x)
#else
#define VERIFY1(x) x
#endif

enum LOG_CATEGORY{
    LC_FATAL = 0,
    LC_ERROR,
    LC_WARNING,
    LC_INFO,
    LC_DEBUG,
    LC_CMD,
    LC_OPT,
    LC_COUNT
};

DIP_EXPORT extern const TCHAR* const kCategoryName[];

class DIP_EXPORT LoggingObserver{
public:
    LoggingObserver(){}
    virtual ~LoggingObserver(){}
    virtual void LogMessage(LOG_CATEGORY cat, const CString& msg) = 0;
};

template class DIP_EXPORT std::allocator<LoggingObserver*>;
template class DIP_EXPORT std::vector<LoggingObserver*, std::allocator<LoggingObserver*> >;
class DIP_EXPORT Logging{
public:
    Logging():show_file_info_(false){}
    void AddObserver(LoggingObserver* observer);
    void RemoveObserver(LoggingObserver* observer);

    void Log(LOG_CATEGORY category, 
            const CString& file,
            const CString& func,
            int line,
            const CString& user_log);
    void Log(const CString& msg);
    void ShowFileInfo(bool show);
    
private:

    void OnLogged(LOG_CATEGORY cat, const CString& message);    

    typedef std::vector<LoggingObserver*>::iterator ObserverIter;
    std::vector<LoggingObserver*> observers_;
    bool show_file_info_;
    DISALLOW_EVIL_CONSTRUCTORS(Logging);
};

DIP_EXPORT Logging* GetLogging();

// TODO: If atlstr.h could be used in MFC, You can define 
// the wstring param for LogMessage as CString.
DIP_EXPORT void LogMessageW(LOG_CATEGORY category, 
                           const wchar_t* file,
                           const wchar_t* func,
                           int line,
                           const wchar_t* fmt, ...);

DIP_EXPORT void LogMessageA(LOG_CATEGORY category, 
                            const char* file,
                            const char* func,
                            int line,
                            const char* fmt, ...);

#ifndef LOG_ANSI
#define LOG_MESSAGE(cat, x, ...) \
do{ \
LogMessageW(cat, \
            _T(__FILE__), \
            _T(__FUNCTION__), \
            __LINE__, \
            x, \
            __VA_ARGS__); \
}while(0)
#else
#define LOG_MESSAGE(cat, x, ...) \
do{ \
    LogMessageA(cat, \
                __FILE__, \
                __FUNCTION__, \
                __LINE__, \
                x, \
                __VA_ARGS__); \
}while(0)
#endif

#define LOG_ERROR(x, ...)\
    LOG_MESSAGE(LC_ERROR, x, __VA_ARGS__)
    
#define LOG_INFO(x, ...)\
    LOG_MESSAGE(LC_INFO, x, __VA_ARGS__)

#define LOG_WARNING(x, ...)\
    LOG_MESSAGE(LC_WARNING, x, __VA_ARGS__)

#define LOG_FATAL(x, ...)\
    LOG_MESSAGE(LC_FATAL,   x, __VA_ARGS__)

#define LOG_DEBUG(x, ...)\
    LOG_MESSAGE(LC_DEBUG, x, __VA_ARGS__)

#define LOG_CMD(x, ...)\
    LOG_MESSAGE(LC_CMD, x, __VA_ARGS__)