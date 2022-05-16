/*
    driver Plus by data
    use timer 1, it got higher ISR than uart0
    add port , port is 
    support 8 port
*/
#pragma once
#include "queue.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <Arduino.h>

class StepController{
    private:
        volatile uint8_t dat = 0;
        volatile int8_t ports[8] = {-1};
             
    public:
        StepController();
        void init(uint8_t preSacel, uint16_t count);
        void oneStepISR();
        int8_t addOutPut(uint8_t portNum);
        int8_t addOuput(uint8_t portNum,uint8_t position);
        void removeOuput(uint8_t portNum);
        void setDat(uint8_t dat);
};  