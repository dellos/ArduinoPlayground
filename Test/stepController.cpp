
#pragma once
#include "stepController.h"

StepController::StepController(){
    
}
//prescale form: 0 -> 7 ;
//set timer 1 in ctc mode use OCR1A to compare
//enable interrupt
void StepController::init(uint8_t prescale, uint16_t count){
    //reset defaut 
    TCCR1A = 0;
    TCCR1B = 0;
    TIFR1 = 1;
    TIMSK1 = 0;
    //set
    TCNT1=0;
    OCR1A=count;
    TCCR1B |= (1<<WGM12);
    TCCR1B |= prescale & 0x07; //get only 3 lowest bit
    TIMSK1 |= (1<<OCIE1A); //enable ISR

}
//add port to empty place and return position
//-1 if couldnt added
//can't add duplicated portnum
int8_t StepController::addOutPut(uint8_t portNum){
    int8_t position = -1;
    for(int i=0;i < 8;i++){
        if(portNum == ports[i]){
            position= -1;
            break;
        }
        if(ports[i] == -1){
            if(position == -1) position == i ;
        }

    }
    if(position != -1) ports[position] = portNum;
    return position;
}
//force overwrite if have one port already in given position
//return portNum if do overWrite
//other while return position
//check for duplication, should not allow duplication ?>?
int8_t StepController::addOuput(uint8_t portNum, uint8_t position){

}
void StepController::oneStepISR(){
    for(int i=0;i < 8 ; i ++){
        if(ports[i] > -1 ){
            digitalWrite(ports[i],(1&(dat>>i))); //get bit i of number dat
        }
    }
}
//remove port if have 
//return position if success
//return -1 if didn't removed
void StepController::removeOuput(uint8_t portNum){

}

