

#pragma once

class EventType{
    private:
        char _descriptor;
    public:
        EventType(char descriptor);
        bool IsEqual(EventType t);
        char GetDescriptor();
}