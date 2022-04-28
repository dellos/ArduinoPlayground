
/*
    Note about avr uart


*/
#pragma once
#include "queue.h"
#include <avr/io.h>
#include <avr/interrupt.h>

//Asynchronous mode
//double
//Serial Frames 8 bit
//1 start bit 
//2 stop bit
//event parity
class SerialController{
    private:
        Queue *rxBuffer;
        Queue *txBuffer;
        void init(int fcpu,int baudRate);
    public:
        SerialController(int txBufferSize, int rxBufferSize, int fcpu, int baudRate){
            rxBuffer = new Queue(rxBufferSize);
            txBuffer = new Queue(txBufferSize);
            init(fcpu,baudRate);
        }
        void write();
        void read();
        bool writeBuffer(uint8_t dat[]);
        uint8_t * readBuffer(char flg);
        void resetBuffer(char flg);
        void stop(char flg);
        void pause(char flg);
        

};