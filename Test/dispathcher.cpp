


#pragma once
#include "publisher.h"

Dispathcher::Dispathcher(uint8_t dispathEventSlot, uint8_t subscriberSlot ){
    _numOfSubscriber = subscriberSlot;
    _sizeOfEventSlots = dispathEventSlot;
    _subscriberSlotIndex = 0;
    _eventSlotIndex = 0;
    dispathEventList = new Event[dispathEventSlot];
    subscriberList = new Subscriber[subscriberSlot];

}
Dispatcher::~Dispathcher(){
    delete[] dispathEvent;
    delete[] subscriber;
}
void Dispathcher::AddEvent(Event e){
    if(_eventSlotIndex < _sizeOfEventSlots){
        dispathEventList[_eventSlotIndex] = e;
        _eventSlotIndex ++; 
    }else{

        Dispath(e);
    }
}
void Dispatcher::Dispath(Event e){
    for(int i = 0; i < subscriberList; i++){
        subscriberList[i].
    }
    _eventSlotIndex = 0; //reset
}
bool Dispatcher::RegistSubscriber(Subscriber s){
    bool result = false;
    if(_subscriberSlotIndex < _numOfSubscriber){
        //check for presented
        bool existed = false;
        if(_subscriberSlotIndex > 0){
            for(int i = 0; i < _subscriberSlotIndex){
                if(subscriberList[i] ==  s) 
                {
                    existed = true;
                    break;
                }
            }
        }
        if(!existed){
            subscriberList[_subscriberSlotIndex] = s;
            _subscriberSlotIndex ++;
            result = true;
        }
    }
    return result;
}