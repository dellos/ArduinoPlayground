

#pragma once
#ifndef PUBLISHER
#define PUBLISHER
#endif
#include <Arduino.h>
#include "dispatcher.h"

class Publisher{
    private:
        Dispathcher _dispathcher;
    public:
        Publisher(Dispathcher d);
        ~Publisher();
        void PushEvent(Event e);//push event to Dispathcher
        
};