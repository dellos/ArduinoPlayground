

#pragma once
#include "publisher.h"

Publisher::Publisher(Dispathcher d){
    _dispathcher = d;
}
Publisher::~Publisher(){
    _dispathcher = NULL;
}
Publisher::PushEvent(Event e){
    if(_dispathcher) _dispathcher.AddEvent(e);
}