/*
    
    TimerClockFrequency(Hz) = OCFrqeuency/rescalcer => 16Mhz/1
    TimePerTick(s) = 1/TimerClockFrequency(Hz) 
    TimePerCount(s) = TimePerTick(s)*Count
    Count = OCRxx => which 16 bit register 65535 is max count
    ATMega is 32 bit microprocessor
*/
#define __AVR_ATmega2560__ //must define to use right io.h 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "queue.h"
#include "serial.h"

#define DoubleBaudRate
#define BAUD_RATE 115200
SerialController *serial;
void timerInit(){
   

    TCNT1 = 0;   
    OCR1A = 24999; // set interver
    //set CTC
    TCCR1A = 0;
    TCCR1B |= (1 << WGM12); 
    TCCR1B |= (0<<CS12) |(0 << CS11)|(1 << CS10); //no rescaler
    
}
void serialInit(){
    serial = new SerialController(128,128,F_CPU,BAUD_RATE);

}
//int queue for value
void bufferInit(){

}

void setup(){
    timerInit();
    serialInit();
}

void loop(){

}

ISR(TIMER1_COMPA_vect){
    
}

// call when txb is empty and can set new byte
ISR(UART0_UDRE_vect){
   serial->write(); 
}
//call when all bit of Frame is shifted into RXB and rxb not read out 
ISR(UART0_RX_vect){
    serial->read(); //move to serial buffer
}
//call when all bit of frame is shift out of Shift Register and TXB is empty
ISR(UART0_TX_vect){
    
}
    
