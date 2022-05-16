
#include "serial.h"

SerialController::SerialController(int txBufferSize, int rxBufferSize, unsigned long ubrrl_value, unsigned long ubrrh_value, bool  isUse2x):
    rxBuffer(Queue(rxBufferSize)),
    txBuffer(Queue(txBufferSize))
{
    _txBufferSize = txBufferSize;
    _rxBufferSize = rxBufferSize;
    _txStatus = SerialStatus::Inited;
    _rxStatus = SerialStatus::Inited;
    //init(ubrrl_value,ubrrh_value,isUse2x); //dont setup hardware in Ctor
}

//write a byte for TxBuffer to hardware buffer regist->UDR0

void SerialController::write(){
    if(txBuffer.isEmpty()){
        //write null charr to be tranmit to outer
        if( _txStatus == SerialStatus::Active) turnDataRegisterEmpty(false);
    }
    else{

        UDR0 = txBuffer.deQueue();
    }
}
//read a byte form hardware buffer to RxBuffer
//note:
//When interrupt driven data reception is used, 
//the receive complete routine must read the received data from UDRn in order to
//clear the RXCn Flag, otherwise a new interrupt will occur once the interrupt routine terminates
void SerialController::read(){
    //check for FE0 flag, received data have Frame error (stop bit = 0 -> failed ) . must check before read
   
    if(UCSR0A & (1<<FE0)){
        uint8_t dat = UDR0; // read rxb
        rxBuffer.enQueue('f');

    }
    //check for UPE0 flag, received data have parity erorr, must check before read.
    else if(UCSR0A & (1<<UPE0)){
        uint8_t dat = UDR0; // read rxb
        rxBuffer.enQueue('u');
    }
    //check if can move to buffer. if not send dat to notifi outer divice it cant get more data
    else if(rxBuffer.isFull()){
        //todo: notifi outer divice to stop them Tx
        //pause('r'); //stop interput by Rx
        rxBuffer.enQueue('l');
    }
    else{

        uint8_t dat = UDR0; // read rxb
        rxBuffer.enQueue(dat); //write to buffer
    }
}
//Note: that disable transmitt will not stop ongoing transmit (shift register and RXB register data will still transmit to end)
//Note: disable reciver will be immediate, ongoing data will lost
//pause Recive: flg = "r"
//pause All: flg = "a"
//Pause Tranmite = "t"
// not clear buffer in 3 mode
//set back last byte before pause
void SerialController::pause(SerialPart flg){
    if(flg == SerialPart::All){
        UCSR0B &= ~(1<<UDRIE0);
        UCSR0B &= ~((1<<RXEN0) |(1<<TXEN0));
        _rxStatus = SerialStatus::Pause;
        _txStatus = SerialStatus::Pause;
    }
    else if(flg == SerialPart::Rx){
        UCSR0B &= ~(1<<RXEN0);
        _rxStatus = SerialStatus::Pause;
    }
    else if(flg == SerialPart::Tx){
        UCSR0B &= ~(1<<UDRIE0);
        UCSR0B &= ~(1<<TXEN0);
        _txStatus = SerialStatus::Pause;
    }
}
//same as pause but do clearnup buffer
void SerialController::stop(SerialPart flg){
    if(flg == SerialPart::All){
        UCSR0B &= ~(1<<UDRIE0);
        UCSR0B &= ~((1<<RXEN0) |(1<<TXEN0));
        _rxStatus = SerialStatus::Stop;
        _txStatus = SerialStatus::Stop;
    }
    else if(flg == SerialPart::Rx){
        UCSR0B &= ~(1<<RXEN0);
        _rxStatus = SerialStatus::Stop;
    }
    else if(flg == SerialPart::Tx){
        UCSR0B &= ~(1<<UDRIE0);
        UCSR0B &= ~(1<<TXEN0);
        _txStatus = SerialStatus::Stop;
    }

    resetBuffer(flg);
    
}
//is it just simple set bit to 1 ? 
void SerialController::resume(SerialPart flg){

     if(flg == SerialPart::All){
        UCSR0B |= (1<<UDRIE0);
        UCSR0B |= (1<<RXEN0) |(1<<TXEN0);
        _rxStatus = SerialStatus::Active;
        _txStatus = SerialStatus::Active;
    }
    else if(flg == SerialPart::Rx){
        UCSR0B |= (1<<RXEN0);
        _rxStatus = SerialStatus::Active;
    }
    else if(flg == SerialPart::Tx){
        UCSR0B |= (1<<UDRIE0);
        UCSR0B |= (1<<TXEN0);
        _txStatus = SerialStatus::Active;
    }
}
//write array to TxBuffer
//if give array bigger than TxBuffer return False
bool SerialController::writeBuffer(uint8_t dat[]) {
    bool result = false;
    int arrSize = *(&dat + 1) - dat; //this work becasue dat is constant pointer to uint8_t array.
    if(txBuffer.capacity() > arrSize ){
        //do loop to move copy
        for(int i=0; i < arrSize;i++){
            txBuffer.enQueue(dat[i]);
        }
        result = true;
    }
    if(_txStatus == SerialStatus::Stop){
        turnDataRegisterEmpty(true);
    }
    return result;
    
}
bool SerialController::writeBuffer(uint8_t dat){
    bool result = false;
    if(txBuffer.capacity() > 0){
        txBuffer.enQueue(dat);
    }
    if(_txStatus == SerialStatus::Stop){
        turnDataRegisterEmpty(true);
    }
    return result;
}
uint8_t SerialController::readBuffer(){
    return rxBuffer.deQueue();
}
//Read RxBuffer, cleanup readed bytes
//fill inArray by num of char. make sure array size greater than count
void SerialController::readBufferByChar(char toChar, uint8_t *inArray,int arraySize,int startIndex = 0){
    int count = rxBuffer.findChar(toChar);
    if(arraySize >= (count + startIndex)){
        for(int i=0;i<count; i++){
            inArray[i+startIndex] = rxBuffer.deQueue();
        }
    }
 
}
//if length > buffer capacity -> fill array by 0 in empty space 
//this is dynamic created array
void SerialController::readBufferByLenght(int lenght, uint8_t *inArray, int startIndex = 0){ 
    for(int i= 0 ; i < lenght;i++){
        inArray[i + startIndex] = rxBuffer.deQueue();
    }
}
//'a' : all
//'t' : transmit
//'r' : recives
//set TxBuffer , RxBuffer to fornt , rear = -1 , -1
void SerialController::resetBuffer(SerialPart flg){
    if(flg == SerialPart::All){
        rxBuffer.resetQueue();
        txBuffer.resetQueue();

    }
    else if(flg == SerialPart::Tx){
        txBuffer.resetQueue();
    }
    else if (flg ==SerialPart::Rx){
        rxBuffer.resetQueue();
    }
}

void SerialController::init(unsigned long ubrrl_value, unsigned long ubrrh_value, bool  isUse2x){
    //clear status
    UCSR0A = 0;
    UBRR0H = ubrrh_value;
    UBRR0L = ubrrl_value;
    if(isUse2x){
        UCSR0A |= (1 << U2X0);
    }
    else{
        UCSR0A &= ~(1 << U2X0);
    }
     
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
    //UCSR0C |= (1<<UPM01) | (0<<UPM00); // even parity
   

    //UCSR0C |= (1<<USBS0); // 2 stop bit
    //set status
    _rxStatus = SerialStatus::Active;
    _txStatus = SerialStatus::Active;
    
}
//enable or disable TX hardware buffer register empty interupt
void SerialController::turnDataRegisterEmpty(bool flag){
    if(flag == true){
        UCSR0B |= (1<<UDRIE0);
        _txStatus = SerialStatus::Active;
    }else{
        UCSR0B &= ~(1<<UDRIE0);
        _txStatus = SerialStatus::Stop; //stop interrupt
    }
}

void SerialController::setInterrupt(SerialInterruptMode flg, bool isOn = true){
    
}

void SerialController::setParityMode(SerialParityMode flg){

}
void SerialController::setStopBitMode(SerialStopBitMode flg){

}
void SerialController::setTranmitMode(SerialTranmitMode flg){

}
void SerialController::setFrameMode(SerialFrameMode flg){

}