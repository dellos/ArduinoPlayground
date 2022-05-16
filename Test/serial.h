
/*
    Note about avr uart


*/
#pragma once
#include "queue.h"
#include "model.h"
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
        SerialStatus _rxStatus;
        SerialStatus _txStatus;
        void turnDataRegisterEmpty(bool flag);
        void setInterrupt(SerialInterruptMode flg,bool isOn);
        void setParityMode(SerialParityMode flg);
        void setStopBitMode(SerialStopBit flg );
        void setTranmitMode(SerialTranmitMode flg);
        void setFrameMode(SerialFrameMode flg);
    public:
       
        SerialController(int txBufferSize, int rxBufferSize, unsigned long ubrrl_value, unsigned long ubrrh_value, bool  isUse2x);
        void init(unsigned long ubrrl_value,unsigned long ubrrh_value, bool isUse2x);
        void write();
        void read();
        bool writeBuffer(uint8_t dat[]);
        bool writeBuffer(uint8_t dat);
        void readBufferByChar(char toChar, uint8_t *inArray, int arraySize, int startIndex = 0); //due to bad when return local stack ////use pointer prarameter 
        void readBufferByLenght(int lenght, uint8_t *inArray, int startIndex=0); 
        void resetBuffer(SerialPart flg);
        void stop(SerialPart flg);
        void pause(SerialPart flg);
        void resume(SerialPart flg);
        uint8_t readBuffer();
};