#pragma once

#ifndef MODEL
#define MODEL
#endif

#pragma region "Serial"
enum class SerialStopBitMode:char{OneBit,TwoBit};
enum class SerialParityMode:char{Even, Odd};
enum class SerialFrameMode:char{ EightBit,NineBit};
enum class SerialTranmitMode:char{Async,Sync};
enum class SerialStatus:char{Active,Pause,Stop,Full,Inited};
enum class SerialPart:char{Tx ,Rx, All};
enum class SerialInterruptMode:char{OnFinishedRecived,OnFinishedTranmit,OnEmptyTranmitDataRegister};
#pragma endregion

#pragma region "data"
struct SerialComunicationControlChar{
    char DataBlockStart;
    char DataBlockEnd;
    char HeaderStart;
    char EndOfTransmission;
    char RequestReSendBlock;
    char CommandStart;
    char CommandEnd;
    char MessageStart;
    char TransmissionBlockEnd;
    SerialComunicationControlChar();

};
#pragma endregion