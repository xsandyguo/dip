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
#include <algorithm>

class EventArgs{
public:    
    EventArgs(){}    
    virtual ~EventArgs(){}
};

class IObserver{
public:
    IObserver(){}
    virtual ~IObserver(){}
};

typedef std::vector<IObserver*> ObserverCollection;
typedef std::vector<IObserver*>::iterator ObserverIter;

template<typename ObserverType, typename EventType>
void ObserverCall(IObserver* observer,
    void (ObserverType::*fun)(EventType& evt), EventType& evt){
    ObserverType* typed_observer = dynamic_cast<ObserverType*>(observer);
    if(typed_observer){
        (typed_observer->*fun)(evt);
    }
}

template<typename ObserverType, typename EventType>
void ObserverIterCall(ObserverCollection& observers,
    void (ObserverType::*fun)(EventType& evt), EventType& evt){
    //for (ObserverIter itor = observers.begin();
    //    itor != observers.end(); ++itor) {
    //    ObserverCall(*itor, fun, evt);
    //}

    for(uint32 i = 0; i < observers.size(); ++i){
        uint32 count = observers.size();
        ObserverCall(observers.at(i), fun, evt);
        if(count > observers.size()){
            --i;
        }
    }
}

void AddObserver(ObserverCollection* observers, IObserver* observer);

inline void RemoveObserver(ObserverCollection* observers, IObserver* observer){
    ObserverIter itor = std::find(observers->begin(), observers->end(), observer);
    if(itor != observers->end()){
        observers->erase(itor);
    }
}

inline void RemoveObserver(ObserverCollection* observers, IObserver& observer){
    RemoveObserver(observers, &observer);
}

inline void AddObserver(ObserverCollection* observers, IObserver& observer){
    AddObserver(observers, &observer);
}

#define OBSERVER_DECLARE() \
    ObserverCollection observers_;

#define OBSERVER_ADD(CLASS, fun_name) \
void CLASS::fun_name(IObserver* observer){ \
    AddObserver(&observers_, observer); \
}

#define OBSERVER_REMOVE(CLASS, fun_name) \
void CLASS::fun_name(IObserver* observer){ \
    RemoveObserver(&observsers_, observer); \
}