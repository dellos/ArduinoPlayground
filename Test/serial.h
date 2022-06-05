
/*
    Note about avr uart


*/
#pragma once
#ifndef SERIAL_CONTROLLER
#define SERIAL_CONTROLLER
#endif
#include "queue.h"
#include "model.h"
#include <avr/io.h>
#include <avr/interrupt.h>
//mapping
#ifndef SERIAL_CONTROLLER
#define SERIAL_UDRn UDR0
#define SERIAL_UCSRnA UCSR0A
#define SERIAL_UCSRnB UCSR0B
#define SERIAL_UCSRnC UCSR0C
#define SERIAL_UBRRnL UBRR0L
#define SERIAL_UBRRnH UBRR0H

#define SERIAL_UCSRnA_RXCn RXC0
#define SERIAL_UCSRnA_TXCn TXC0
#define SERIAL_UCSRnA_UDREn UDRE0
#define SERIAL_UCSRnA_FEn FE0
#define SERIAL_UCSRnA_DORn DOR0
#define SERIAL_UCSRnA_UPEn UPE0
#define SERIAL_UCSRnA_U2Xn U2X0
#define SERIAL_UCSRnA_MPCMn MPCM0

#define SERIAL_UCSRnB_RXCIEn RXCIE0
#define SERIAL_UCSRnB_TXCIEn TXCIE0
#define SERIAL_UCSRnB_UDRIEn UDRIE0
#define SERIAL_UCSRnB_RXENn RXEN0
#define SERIAL_UCSRnB_TXENn TXEN0
#define SERIAL_UCSRnB_UCSZn2 UCSZ02
#define SERIAL_UCSRnB_RXB8n RXB80
#define SERIAL_UCSRnB_TXB8n TXB80

#define SERIAL_UCSRnC_UMSELn0 UMSEL00
#define SERIAL_UCSRnC_UMSELn1 UMSEL01
#define SERIAL_UCSRnC_UPMn1 UPM01
#define SERIAL_UCSRnC_UPMn0 UPM00
#define SERIAL_UCSRnC_USBSn USBS0
#define SERIAL_UCSRnC_UCSZn1 UCSZ01
#define SERIAL_UCSRnC_UCSZn0 UCSZ00
#define SERIAL_UCSRnC_UCPOLn UCPOL0
#endif
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
        void setInterrupt(SerialInterruptMode flg,bool isOn = true);
        void setParityMode(SerialParityMode flg);
        void setStopBitMode(SerialStopBitMode flg );
        void setTranmitMode(SerialTransmitMode flg);
        void setFrameMode(SerialFrameMode flg);
        void setBaudRate(unsigned long ubrrl_value,unsigned long ubrrh_value,bool isUse2x);
        void setClockPolarity(bool TxDnRising);
        void setEnableTransmit(bool isEnable);
        void setEnableRecive(bool isEnable);
        bool isFrameError();
        bool isParityError();
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