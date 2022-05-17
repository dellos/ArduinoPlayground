

#pragma once
#include "eventHandler.h"

EventHandler::EventHandler(){
    _nextIndex = 0;
    handlerList = {};
    _size = HANDLER_SZIE;
}
EventHandler::~EventHandler(){
    delete[] handlerList;
}
//dont except dulicate adding
bool EventHandler::AddHandler(void (*fn)(Event)){
    bool result = false;
    for(int i = 0; i < _size){
        if(handlerList[i] == fn){
            return result;
        }
    }
    if(_nextIndex < _size){
        handlerList[_nextIndex] = fn;
        _nextIndex ++;
        result = true;
    }

    return result;

}
//remove handler in array and null this item
bool EventHandler::RemoveHandler(void (*fn)(Event)){
    bool result = false;
    for(int i = 0; i < _size){
        if(handlerList[i] == fn){

            result = true;
            return result;
        }
    }
    return result;
}