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

#include "base/string.h"

#include <windows.h>
#include <tchar.h>
#include <base/debug.h>
#include <base/scoped_ptr.h>

const wchar_t kUnicodeBom = 0xFEFF;

// Convert Wide to ANSI directly. Use only when it is all ANSI
CStringA WideToAnsiDirect(const CString & in) {
    int in_len = in.GetLength();
    const TCHAR * in_buf = static_cast<const TCHAR*>(in.GetString());

    CStringA out;
    unsigned char * out_buf = (unsigned char *)out.GetBufferSetLength(in_len);

    for(int i = 0; i < in_len; ++i)
        out_buf[i] = static_cast<unsigned char>(in_buf[i]);

    out.ReleaseBuffer(in_len);
    return out;
}

// Transform a unicode string into UTF8, as represented in an ASCII string
CStringA WideToUtf8(const CString& w) {
    // Add a cutoff. If it's all ascii, convert it directly
    const TCHAR* input = static_cast<const TCHAR*>(w.GetString());
    int input_len = w.GetLength(), i;
    for (i = 0; i < input_len; ++i) {
        if (input[i] > 127) {
            break;
        }
    }

    // If we made it to the end without breaking, then it's all ANSI, so do a quick convert
    if (i == input_len) {
        return WideToAnsiDirect(w);
    }

    // Figure out how long the string is
    int req_bytes = ::WideCharToMultiByte(CP_UTF8, 0, w, -1, NULL, 0, NULL, NULL);

    scoped_array<char> utf8_buffer(new char[req_bytes]);

    int conv_bytes = ::WideCharToMultiByte(CP_UTF8, 0, w, -1, utf8_buffer.get(), req_bytes, NULL, NULL);
    ASSERT1(req_bytes == conv_bytes);

    // conv_bytes includes the null terminator, when we read this in, don't read the terminator
    CStringA out(utf8_buffer.get(), conv_bytes - 1);

    return out;
}

CString Utf8ToWideChar(const char* utf8, uint32 num_bytes) {
    ASSERT1(utf8);
    if (num_bytes == 0) {
        return CString();
    }

    uint32 number_of_wide_chars = ::MultiByteToWideChar(CP_UTF8, 0, utf8, num_bytes, NULL, 0);
    number_of_wide_chars += 1;  // make room for NULL terminator

    CString ret_string;
    TCHAR* buffer = ret_string.GetBuffer(number_of_wide_chars);
    DWORD number_of_characters_copied = ::MultiByteToWideChar(CP_UTF8, 0, utf8, num_bytes, buffer, number_of_wide_chars);
    ASSERT1(number_of_characters_copied == number_of_wide_chars - 1);
    buffer[number_of_wide_chars - 1] = _T('\0');  // ensure there is a NULL terminator
    ret_string.ReleaseBuffer();

    // Strip the byte order marker if there is one in the document.
    if (ret_string[0] == kUnicodeBom) {
        ret_string = ret_string.Right(ret_string.GetLength() - 1);
    }

    if (number_of_characters_copied > 0) {
        return ret_string;
    }

    // Failure case
    return CString();
}