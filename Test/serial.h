
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
        int _txBufferSize, _rxBufferSize;
        Queue rxBuffer;
        Queue txBuffer;
        void turnDataRegisterEmpty(bool flag);
        char _rxStatus, _txStatus;

    public:
       
        SerialController(int txBufferSize, int rxBufferSize, unsigned long ubrrl_value, unsigned long ubrrh_value, bool  isUse2x);
        void init(unsigned long ubrrl_value,unsigned long ubrrh_value, bool isUse2x);
        void write();
        void read();
        bool writeBuffer(uint8_t dat[]);
        bool writeBuffer(uint8_t dat);
        void readBufferByChar(char toChar, uint8_t *inArray, int arraySize, int startIndex = 0); //due to bad when return local stack ////use pointer prarameter 
        void readBufferByLenght(int lenght, uint8_t *inArray, int startIndex=0); 
        void resetBuffer(char flg);
        void stop(char flg);
        void pause(char flg);
        void resume(char flg);
        uint8_t readBuffer();
};