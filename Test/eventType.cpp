


#pragma once
#include "eventType.h"

EventType::EventType(char descriptor){
    _descriptor = descriptor;
}
bool EventType::IsEqual(EventType t){
    return t.GetDescriptor() == _descriptor;
}
char EventType::GetDescriptor(){
    return _descriptor;
}