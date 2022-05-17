


#pragma once
#include "event.h"

Event::Event(EventType e):
_type(e)
{
}
Event::Event():
_type(BaseEvent)
{
}
Event::~Event(){

}
EventType Event::BaseEvent = EventType('\0');
EventType Event::SerialFrameError = EventType('f');
EventType Event::SerialParityError = EventType('p');
EventType Event::SerialReadBufferFull = EventType('R');