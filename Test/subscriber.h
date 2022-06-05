


#pragma once
#ifndef SUBSCRIBER
#define SUBSCRIBER
#define HANDLER_SLOT_SIZE 10
#endif 
#include "eventHandler.h"
#include <Arduino.h>

class Subscriber{
    private:
        uint8_t _handlerSlotSize,_nextIndex;
        EventType eventListenerList[HANDLER_SLOT_SIZE]={};
        EventHandler eventHandlerList[HANDLER_SLOT_SIZE]={};
    public:
        //cant use ctor with slotSize because Subscriber use in Disathcher need to be same size when create arry
        //Subscriber(uint8_t slotSize);
        Subscriber();
        ~Subscriber();
        void Subscribe(EventType eventType, EventHandler eventHandler);
        void UnSubscribe(EventType eventType, EventHandler eventHandler);
};