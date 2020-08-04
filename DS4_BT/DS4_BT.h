/*
Fork of https://os.mbed.com/users/okini3939/code/USBHostXpad/
 */
 
#ifndef DS4_BT_H
#define DS4_BT_H
 


#include "mbed.h"
#include "BufferedSerial.h"
 
//#define  DS4_BT_Debug 
 
/** 
 * A class to communicate a USB flash disk
 */
class DS4_BT {

DS4_BT(BufferedSerial* _DS4_BT_BufferedSerial);
void GetPacket();
void Parse_Packet();
    private:
Thread DS4_BT_Thread;
uint8_t DS4_BT_Packet[15] = {0};

bool ready =0;
    BufferedSerial* DS4_BT_BufferedSerial;

  //BufferedSerial device(PA_9, PA_10);

};
 
#endif

 