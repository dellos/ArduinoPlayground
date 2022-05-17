

#pragma once
#include "event.h"

#ifndef DispathEvent
#define DispathEvent
#endif
class Dispathcher{
    private:

        uint8_t _numOfSubscriber;
        uint8_t _sizeOfEventSlots;
        uint8_t _subscriberSlotIndex;
        uint8_t _eventSlotIndex;

        Event *dispathEventList;
        Subscriber *subscriberList;
         

    public:
        Dispathcher(unsigned int dispathEventSlotSize, unsigned int subscriberSlotSize);
        ~Dispathcher();
        void AddEvent();
        void Dispath();
        void RegistSubscriber();
}