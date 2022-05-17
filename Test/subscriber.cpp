

#pragma once
#include "subscriber.h"

// Subscriber::Subscriber(uint8_t slotSize){
//     _handlerSlotSize = slotSize;
//     _nextIndex=0;
//     eventListenerList = new EventType[slotSize];
//     eventHandlerList = new EventHandler[slotSize];
// }
Subscriber::Subscriber(){
    _handlerSlotSize = HANDLER_SLOT_SIZE;
    _nextIndex = 0;
}