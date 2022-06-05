
#pragma region "Serial"
enum class SerialStopBitMode:char{OneBit,TwoBit};
enum class SerialParityMode:char{Disable, Even, Odd};
enum class SerialFrameMode:char{ FiveBit,SixBit,SevenBit,EightBit,NineBit};
enum class SerialTransmitMode:char{Async,Sync,MasterSPI};
enum class SerialStatus:char{Active,Pause,Stop,Full,Inited};
enum class SerialPart:char{Tx ,Rx, All};
enum class SerialInterruptMode:char{OnFinishedRecived,OnFinishedTranmit,OnEmptyTransmitDataRegister};
#pragma endregion
