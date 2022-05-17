


#pragma once

#ifndef EVENT_HANDLER
#define EVENT_HANDLER
#define HANDLER_SZIE = 10
#endif

class EventHandler{
    private:
        uint8_t _nextIndex;
        uint8_t _size = HANDLER_SZIE;
        void (*handlerList[HANDLER_SIZE])(Event)={};
    public:
        EventHandler();
        ~EventHandler();
        bool AddHandler(void (*fn)(Event));
        bool RemoveHandler(void (*fn)(Event));
}

