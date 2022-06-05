

#pragma once
#ifndef DISPATHCHER
#define DISPATHCHER
#endif
#include "event.h"
#include "subscriber.h"
#include <Arduino.h>

class Dispathcher{
    private:

        uint8_t _numOfSubscriber;
        uint8_t _sizeOfEventSlots;
        uint8_t _subscriberSlotIndex;
        uint8_t _eventSlotIndex;

        Event *dispathEventList;
        Subscriber *subscriberList;
         

    public:
        Dispathcher(uint8_t dispathEventSlotSize, uint8_t subscriberSlotSize);
        ~Dispathcher();
        void AddEvent();
        void Dispath();
        void RegistSubscriber();
};