/*
    
    TimerClockFrequency(Hz) = OCFrqeuency/rescalcer => 16Mhz/1
    TimePerTick(s) = 1/TimerClockFrequency(Hz) 
    TimePerCount(s) = TimePerTick(s)*Count
    Count = OCRxx => which 16 bit register 65535 is max count
    ATMega is 32 bit microprocessor
*/
#define __AVR_ATmega2560__ //must define to use right io.h 
#define BAUD 115200UL
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
// #include "queue.h"
#include "serial.h"
#include "dataController.h"

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <util/setbaud.h>
#include <util/atomic.h>
//#include <Wire.h>

#define DoubleBaudRate 1


volatile SerialController serial = SerialController(128,128,UBRRL_VALUE,UBRRH_VALUE,USE_2X);
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,16,2);

void serialInit(){

    serial.init(UBRRL_VALUE,UBRRH_VALUE,USE_2X);
}
//int queue for value
void dataTranferInit(){
        
}

void lcdInit(){
    lcd.init();                      // initialize the lcd 
    // Print a message to the LCD.
    lcd.backlight();
    lcd.setCursor(16,0);
    lcd.autoscroll();
    lcd.print("123");
    
}

void setup(){
   
    serialInit();
    lcdInit();
    
}
volatile uint8_t c;

void loop(){
    // display each character to the LCD
    char localC;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        
    }
    c = serial.readBuffer();
    
    if(c != '\0'){
        serial.writeBuffer(c);
        lcd.print(c);
        c = '\0';
    }
}

// call when txb is empty and can set new byte
ISR(USART0_UDRE_vect){
    serial.write(); 
} 
//call when all bit of Frame is shifted into RXB and rxb not read out 
ISR(USART0_RX_vect){
    serial.read(); //move to serial buffer
} 
ISR(USART0_TX_vect){

}

ISR(USART0_TX_vect){
    //add???
}
