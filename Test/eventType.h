

#pragma once
#ifndef EVENT_TYPE
#define EVENT_TYPE
#endif
class EventType{
    private:
        char _descriptor;
    public:
        EventType(char descriptor);
        bool IsEqual(EventType t);
        char GetDescriptor();
};