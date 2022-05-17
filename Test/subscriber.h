


#pragma once

class Subscriber{
    private:
        uint8_t _handlerSlotSize,_nextIndex;
        EventType *eventListenerList;
        EventHandler *eventHandlerList;
    public:
        Subscriber(uint8_t slotSize);
        ~Subscriber();
        void Subscribe(EventType eventType, EventHandler eventHandler);
        void UnSubscribe(EventType eventType, EventHandler eventHandler);
}