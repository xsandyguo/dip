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

#include "app/edit_manager.h"

EditManager::EditManager() : cursor_(-1){

}

void EditManager::Push(IHistoryCmd* cmd){
    CmdIter begin = commands_.begin() + cursor_;
    commands_.erase(begin, commands_.end());
    commands_.push_back(shared_ptr<IHistoryCmd>(cmd));

    cursor_ = count() - 1;
}

IHistoryCmd* EditManager::current(){
    if(is_empty()){
        return NULL;
    }

    return commands_.at(cursor_).get();
}

void EditManager::Undo(){
    shared_ptr<IHistoryCmd> cmd = commands_.at(cursor_);
    cmd->Undo();
    cursor_--;
}

void EditManager::Redo(){
    shared_ptr<IHistoryCmd> cmd = commands_.at(cursor_);
    cmd->Undo();
    cursor_++;
}