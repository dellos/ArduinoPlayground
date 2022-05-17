

#pragma once
#include "eventType.h"
//simple event 
class Event{
    
    private:
        EventType _type;
    public:
        Event(EventType t);
        ~Event();
        bool IsCancel;
        //add event type , discriptor is char so ASII table is use to create eventtype
        static EventType SerialFrameError = new EventType('f');
        static EventType SerialParityError = new EventType('p');
        static EventType SerialReadBufferFull = new EventType('R');
}