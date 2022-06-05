
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
        if( _txStatus == SerialStatus::Active){
            pause(SerialPart::Tx);
        }
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
   
    if(isFrameError()){
        uint8_t dat = SERIAL_UDRn; // read rxb
        rxBuffer.enQueue('f'); //TODO: notifi 
    }
    //check for UPE0 flag, received data have parity erorr, must check before read.
    else if(isParityError()){
        uint8_t dat = SERIAL_UDRn; // read rxb
        rxBuffer.enQueue('u');//TODO:notifi
    }
    //check if can move to buffer. if not send dat to notifi outer divice it cant get more data
    else if(rxBuffer.isFull()){
        //todo: notifi outer divice to stop them Tx
        //pause('r'); //stop interput by Rx
        rxBuffer.enQueue('l');//TODO:notifi
    }
    else{
        uint8_t dat = SERIAL_UDRn; // read rxb
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
        setEnableRecive(false);
        setInterrupt(SerialInterruptMode::OnEmptyTransmitDataRegister,false);
        setEnableTransmit(false);
        _rxStatus = SerialStatus::Pause;
        _txStatus = SerialStatus::Pause;
    }
    else if(flg == SerialPart::Rx){
         setEnableRecive(false);
        _rxStatus = SerialStatus::Pause;
    }
    else if(flg == SerialPart::Tx){
        setInterrupt(SerialInterruptMode::OnEmptyTransmitDataRegister,false);
        setEnableTransmit(false);
        _txStatus = SerialStatus::Pause;
    }
}
//same as pause but do clearnup buffer
void SerialController::stop(SerialPart flg){
    if(flg == SerialPart::All){
        setEnableRecive(false);
        setInterrupt(SerialInterruptMode::OnEmptyTransmitDataRegister,false);
        setEnableTransmit(false);
        _rxStatus = SerialStatus::Stop;
        _txStatus = SerialStatus::Stop;
    }
    else if(flg == SerialPart::Rx){
        setEnableRecive(false);
        _rxStatus = SerialStatus::Stop;
    }
    else if(flg == SerialPart::Tx){
        setInterrupt(SerialInterruptMode::OnEmptyTransmitDataRegister,false);
        setEnableTransmit(false);
        _txStatus = SerialStatus::Stop;
    }
    resetBuffer(flg);
    
}
//enable serial will inclue 2 part
//1.enable Read/Write
//2.enable OnEmptyTransmitDataRegister in case of transmit, becuase this interrupt fire evertime this register is emty
void SerialController::resume(SerialPart flg){

     if(flg == SerialPart::All){
        setEnableRecive(true);
        setEnableTransmit(true);
        setInterrupt(SerialInterruptMode::OnEmptyTransmitDataRegister);
        _rxStatus = SerialStatus::Active;
        _txStatus = SerialStatus::Active;
    }
    else if(flg == SerialPart::Rx){
        setEnableRecive(true);
        _rxStatus = SerialStatus::Active;
    }
    else if(flg == SerialPart::Tx){
        setEnableTransmit(true);
        setInterrupt(SerialInterruptMode::OnEmptyTransmitDataRegister);
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
    if(_txStatus == SerialStatus::Pause){
        resume(SerialPart::Tx);
    }
    return result;
}
bool SerialController::writeBuffer(uint8_t dat){
    bool result = false;
    if(txBuffer.capacity() > 0){
        txBuffer.enQueue(dat);
    }
    if(_txStatus == SerialStatus::Pause){
        resume(SerialPart::Tx);
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
//reset buffer of serial
//this buffer use to store date readed or writing to hardware register
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
//Default setup
// must match comunitor setup
void SerialController::init(unsigned long ubrrl_value, unsigned long ubrrh_value, bool  isUse2x){
    //clear status
    SERIAL_UCSRnA = 0;
    setBaudRate(ubrrl_value,ubrrh_value,isUse2x);     
    /* Enable receiver and transmitter */
    SERIAL_UCSRnB = 0;
    setEnableRecive(true);
    setEnableTransmit(true);
    //Enable interupt
    setInterrupt(SerialInterruptMode::OnEmptyTransmitDataRegister);
    setInterrupt(SerialInterruptMode::OnFinishedRecived);

    /* Set frame format: 8data, 2stop bit */
    UCSR0C = 0;
    setTranmitMode(SerialTransmitMode::Async);
    setFrameMode(SerialFrameMode::EightBit);

    setParityMode(SerialParityMode::Disable);
    setStopBitMode(SerialStopBitMode::OneBit);
    //set status
    _rxStatus = SerialStatus::Active;
    _txStatus = SerialStatus::Active;
    
}

/*Region: private set function*/
bool SerialController::isFrameError(){
    return SERIAL_UCSRnA & (1<<SERIAL_UCSRnA_FEn);
}
bool SerialController::isParityError(){
    return SERIAL_UCSRnA &(1<<SERIAL_UCSRnA_UPEn);
}
void SerialController::setEnableTransmit(bool isEnable){
    if(isEnable){
        SERIAL_UCSRnB |= (1<<SERIAL_UCSRnB_TXENn);
    }
    else{
        SERIAL_UCSRnB &= ~(1<<SERIAL_UCSRnB_TXENn);
    }
}
void SerialController::setEnableRecive(bool isEnable){
    if(isEnable){
        SERIAL_UCSRnB |= (1<<SERIAL_UCSRnB_RXENn);
    }
    else{
        SERIAL_UCSRnB &= ~(1<<SERIAL_UCSRnB_RXENn);
    }
}
void SerialController::setInterrupt(SerialInterruptMode flg, bool isOn = true){
    if(isOn){
        if(flg == SerialInterruptMode::OnFinishedRecived){
            SERIAL_UCSRnB |= ( 1 << SERIAL_UCSRnB_RXCIEn );
        }
        else if(flg == SerialInterruptMode::OnFinishedTranmit){
            SERIAL_UCSRnB |= ( 1 << SERIAL_UCSRnB_TXCIEn );
        }
        else if(flg == SerialInterruptMode::OnEmptyTransmitDataRegister){
            SERIAL_UCSRnB |= (1<< SERIAL_UCSRnB_UDRIEn);
        }
    }
    else{
        if(flg == SerialInterruptMode::OnFinishedRecived){
            SERIAL_UCSRnB &= ~( 1 << SERIAL_UCSRnB_RXCIEn );
        }
        else if(flg == SerialInterruptMode::OnFinishedTranmit){
            SERIAL_UCSRnB &= ~( 1 << SERIAL_UCSRnB_TXCIEn );
        }
        else if(flg == SerialInterruptMode::OnEmptyTransmitDataRegister){
            SERIAL_UCSRnB &= ~(1<< SERIAL_UCSRnB_UDRIEn);
        }
    }
}

void SerialController::setParityMode(SerialParityMode flg){
    if(flg == SerialParityMode::Disable){
        SERIAL_UCSRnC &= ~(( 1 << SERIAL_UCSRnC_UPMn0 ) |(1 << SERIAL_UCSRnC_UPMn1));
    }
    else if(flg == SerialParityMode::Even){
        SERIAL_UCSRnC &= ~( 1 << SERIAL_UCSRnC_UPMn0 );
        SERIAL_UCSRnC |= (1 << SERIAL_UCSRnC_UPMn1);
    }
    else if(flg == SerialParityMode::Odd){
        SERIAL_UCSRnC |= (( 1 << SERIAL_UCSRnC_UPMn0 ) |(1 << SERIAL_UCSRnC_UPMn1));
    }
}
void SerialController::setStopBitMode(SerialStopBitMode flg){
    if(flg == SerialStopBitMode::OneBit){
        SERIAL_UCSRnC &= ~(1<<SERIAL_UCSRnC_USBSn);
    }
    else if(flg == SerialStopBitMode::TwoBit){
        SERIAL_UCSRnC |= (1<<SERIAL_UCSRnC_USBSn);
    }
}
void SerialController::setTranmitMode(SerialTransmitMode flg){
    if(flg == SerialTransmitMode::Async){
        SERIAL_UCSRnC &= ~((1<<SERIAL_UCSRnC_UMSELn1) | (1<<SERIAL_UCSRnC_UMSELn0));
    }
    else if(flg == SerialTransmitMode::Sync){
        SERIAL_UCSRnC &= ~(1<<SERIAL_UCSRnC_UMSELn1);
        SERIAL_UCSRnC |= (1<<SERIAL_UCSRnC_UMSELn0);

    }
    else if(flg == SerialTransmitMode::MasterSPI){
        SERIAL_UCSRnC |= ((1<<SERIAL_UCSRnC_UMSELn1) | (1<<SERIAL_UCSRnC_UMSELn0));
    }
}
void SerialController::setFrameMode(SerialFrameMode flg){
    if(flg == SerialFrameMode::FiveBit){
        SERIAL_UCSRnB &= (1<<SERIAL_UCSRnB_UCSZn2);
        SERIAL_UCSRnC &= ~((1<<SERIAL_UCSRnC_UCSZn1) | (1<<SERIAL_UCSRnC_UCSZn0));
    }
    else if(flg == SerialFrameMode::SixBit){
        SERIAL_UCSRnB &= (1<<SERIAL_UCSRnB_UCSZn2);
        SERIAL_UCSRnC &= ~(1<<SERIAL_UCSRnC_UCSZn1);
        SERIAL_UCSRnC |=  (1<<SERIAL_UCSRnC_UCSZn0);
    }
    else if(flg == SerialFrameMode::SevenBit){
        SERIAL_UCSRnB &= (1<<SERIAL_UCSRnB_UCSZn2);
        SERIAL_UCSRnC |= (1<<SERIAL_UCSRnC_UCSZn1);
        SERIAL_UCSRnC &=  ~(1<<SERIAL_UCSRnC_UCSZn0);
    }
    else if(flg == SerialFrameMode::EightBit){
        SERIAL_UCSRnB &= (1<<SERIAL_UCSRnB_UCSZn2);
        SERIAL_UCSRnC |= (1<<SERIAL_UCSRnC_UCSZn1);
        SERIAL_UCSRnC |=  (1<<SERIAL_UCSRnC_UCSZn0);
    }
    else if(flg == SerialFrameMode::NineBit){
        SERIAL_UCSRnB |= (1<<SERIAL_UCSRnB_UCSZn2);
        SERIAL_UCSRnC |= (1<<SERIAL_UCSRnC_UCSZn1);
        SERIAL_UCSRnC |= (1<<SERIAL_UCSRnC_UCSZn0);
    }
}
void SerialController::setBaudRate(unsigned long ubrrl_value,unsigned long ubrrh_value,bool isUse2x){
    SERIAL_UBRRnH = ubrrh_value;
    SERIAL_UBRRnL = ubrrl_value;
    if(isUse2x){
        SERIAL_UCSRnA |= (1 << SERIAL_UCSRnA_U2Xn);
    }
    else{
        SERIAL_UCSRnA  &= ~(1 << SERIAL_UCSRnA_U2Xn);
    }
}
void SerialController::setClockPolarity(bool TxDnRising){
    if(TxDnRising){
        SERIAL_UCSRnC &= ~(1<<SERIAL_UCSRnC_UCPOLn);
    }
    else{ 
        SERIAL_UCSRnC |= (1<<SERIAL_UCSRnC_UCPOLn);
    }
}