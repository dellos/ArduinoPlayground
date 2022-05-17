

#pragma once
#include "model.h"
/*
    command: 
*/
struct SerialComunicationControlChar serialCtrChar = {
    .DataBlockStart = '\x02',//STX + data + ETX + checksum + ETB
    .DataBlockEnd = '\x03',//ETX 
    .HeaderStart = '\x01',//SOH + data + Stx + data + ETX + checksum + ETB
    .EndOfTransmission = '\x04'; //EOT -> GUI put Atmega to idle if i can
    .RequestReSendBlock = '@',//@ + blockID + ETB
    .CommandStart='\x0C',//ECD + data + SCD, support excute command immediately 
    .CommandEnd ='\x0B',//SCD
    .MessageStart ='\x9E',//PM + data + ETB, use to post message to GUI
    .TransmissionBlockEnd = '\x17'//ETB

}