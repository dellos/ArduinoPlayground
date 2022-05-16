
#pragma region "Serial"
enum class SerialStopBitMode:char{OneBit,TwoBit};
enum class SerialParityMode:char{Event, Odd};
enum class SerialFrameMode:char{ EightBit,NineBit};
enum class SerialTranmitMode:char{Async,Sync};
enum class SerialStatus:char{Active,Pause,Stop,Full,Inited};
enum class SerialPart:char{Tx ,Rx, All};
enum class SerialInterruptMode:{OnFinishedRecived,OnFinishedTranmit,OnEmptyTranmitDataRegister};
#pragma endregion
