

#include "serial.h"
//write a byte for TxBuffer to hardware buffer regist->UDR0

void SerialController::write(){
    if(txBuffer->isEmpty()){
        //stop transmit? or set data to notifi outer device their is no data
        
    }
    else{
        UDR0 = txBuffer->deQueue();
    }
}
//read a byte form hardware buffer to RxBuffer

void SerialController::read(){
    //check for FE0 flag, received data have Frame error (stop bit = 0 -> failed ) . must check before read
    if(FE0){

    }
    //check for UPE0 flag, received data have parity erorr, must check before read.
    else if(UPE0){

    }
    //check if can move to buffer. if not send dat to notifi outer divice it cant get more data
    else if(rxBuffer->isFull()){

    }
    else{
        uint8_t dat = UDR0; // read rxb
        rxBuffer->enQueue(dat); //write to buffer
    }
}
//Note: that disable transmitt will not stop ongoing transmit (shift register and RXB register data will still transmit to end)
//Note: disable reciver will be immediate, ongoing data will lost
//pause Recive: flg = "r"
//pause All: flg = "a"
//Pause Tranmite = "t"
// not clear buffer in 3 mode
//set back last byte before pause
void SerialController::pause(char flg){
    if(flg == 'a'){
        UCSR0B &= ~((1<<RXEN0) |(1<<TXEN0));
    }
    else if(flg == 'r'){
        UCSR0B &= ~(1<<RXEN0);
    }
    else if(flg == 't'){
        UCSR0B &= ~(1<<TXEN0);
    }
}
//same as pause but do clearnup buffer
void SerialController::stop(char flg){
    pause(flg);
    resetBuffer(flg);
}
//write array to TxBuffer
//if give array bigger than TxBuffer return False
bool SerialController::writeBuffer(uint8_t dat[]) {
    int arrSize = *(&dat + 1) - dat;
    if(txBuffer->capacity() > arrSize){
        foreach()
    }
}

//Read RxBuffer, cleanup readed bytes
uint8_t * SerialController::readBuffer(char flg){
    
}
//'a' : all
//'t' : transmit
//'r' : recives
//set TxBuffer , RxBuffer to fornt , rear = -1 , -1
void SerialController::resetBuffer(char flg){
    if(flg == 'a'){
        rxBuffer->resetQueue();
        txBuffer->resetQueue();
    }
    else if(flg == 't'){
        txBuffer->resetQueue();
    }
    else if (flg =='r'){
        rxBuffer->resetQueue();
    }
}
void SerialController::init(int fcpu, int brate){
    //clear status
    UCSR0A = 0;
    
    #if DoubleBaudRate 
        uint16_t baudRate = (fcpu / (16L * brate)) - 1;
        UCSR0A &= ~(1 << U2X0); // baud doubler off
    #else
        uint16_t baudRate = (fcpu / (8L * brate)) - 1;
        UCSR0A |= (1 << U2X0); // baud doubler on
    #endif
    //set baudRate
    UBRR0H = baudRate >> 8;
    UBRR0L = baudRate;
   
    /* Enable receiver and transmitter */
    UCSR0B = 0;
    UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
    //Enable interupt
    UCSR0B |= (1<<UDRIE0); //empty transmit data 
    UCSR0B |= (1<<RXCIE0); //complete Revice
    UCSR0B |= (1<<TXCIE0); //complete Transmit

    /* Set frame format: 8data, 2stop bit */
    UCSR0C = 0;
    UCSR0C |= (0<<UMSEL01) | (0<<UMSEL00); //Asynchronous
    UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00); //8 bit USCZ02 is 0
    UCSR0C |= (1<<UPM01) | (0<<UPM00); // even parity
    UCSR0C |= (1<<USBS0); // 2 stop bit
    
}


//turn out ISR is static fucntion so it no need to define in class
//=> serial class must support function will use in ISR 

