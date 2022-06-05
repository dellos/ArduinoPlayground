
#ifndef EVENT
#define EVENT
#endif
#pragma once
#include "eventType.h"

//simple event 
class Event{
    
    private:
        EventType _type;
    public:
        Event(EventType e);
        Event();
        ~Event();
        bool IsCancel;
        //add event type , discriptor is char so ASII table is use to create eventtype
        static EventType BaseEvent;
        static EventType SerialFrameError; 
        static EventType SerialParityError; 
        static EventType SerialReadBufferFull;
};