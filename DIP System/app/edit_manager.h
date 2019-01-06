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

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/shared_ptr.h"

class IHistoryCmd{
public:
    IHistoryCmd(){}
    virtual ~IHistoryCmd(){}

    virtual void Undo() = 0;
    virtual void Redo() = 0;
    virtual bool CanUndo() = 0;
    virtual bool CanRedo() = 0;

    virtual const std::wstring& name() = 0;

private:
    DISALLOW_COPY_AND_ASSIGN(IHistoryCmd);
};

class AbstractCommand : public IHistoryCmd{
public:
    AbstractCommand(){}
    virtual ~AbstractCommand(){}

    virtual bool CanUndo(){ return true; }
    virtual bool CanRedo(){ return true; }
    virtual const std::wstring& name(){ return name_; }

private:
    std::wstring name_;

    DISALLOW_COPY_AND_ASSIGN(AbstractCommand);
};

class EditManager{
public:

    uint32 count(){ return commands_.size(); }
    bool is_empty(){ return !!commands_.size();}

    IHistoryCmd* current();

    void Undo();
    void Redo();

    void Push(IHistoryCmd* cmd);
    void Pop();

private:
    EditManager();

    typedef std::vector<shared_ptr<IHistoryCmd>> CommandColletion;
    typedef CommandColletion::iterator CmdIter;

    CommandColletion commands_;

    int cursor_;

    DISALLOW_COPY_AND_ASSIGN(EditManager);
};