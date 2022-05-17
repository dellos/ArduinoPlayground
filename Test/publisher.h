

#pragma once

class Publisher{
    private:
        Dispathcher _dispathcher;
    public:
        Publisher(Dispathcher d);
        ~Publisher();
        void PushEvent(Event e);//push event to Dispathcher
        
}