 
 
 
 /*
Fork of https://os.mbed.com/users/okini3939/code/USBHostXpad/
 */

#include "DS4_BT.h"





//#define DS4_BT_Debug 0

DS4_BT::DS4_BT(BufferedSerial* _DS4_BT_BufferedSerial) {
    DS4_BT_BufferedSerial = _DS4_BT_BufferedSerial;

}

void DS4_BT::GetPacket(){
    while (!DS4_BT_BufferedSerial->readable())
      ;
    if (DS4_BT_BufferedSerial->getc() == 'D') {
      while (!DS4_BT_BufferedSerial->readable())
        ;
      if (DS4_BT_BufferedSerial->getc() == 'S') {

        while (!DS4_BT_BufferedSerial->readable())
          ;
        if (DS4_BT_BufferedSerial->getc() == '4') {
                ready=1;
          for (int i = 0; i < 15; i++) {
            while (!DS4_BT_BufferedSerial->readable())
            ;
              DS4_BT_Packet[i] = DS4_BT_BufferedSerial->getc();
            
          }




        }
         else{ ready =0;}
      }
       else{ ready =0;}
    }
    else{ ready =0;}
     if(ready){
 Parse_Packet();
 }



 }

DS4_BT::Parse_Packet(){

    
  triangle = DS4BT_packet[6]& (1 << 0);
  circle =DS4BT_packet[6]& (1 << 1);
  cross = DS4BT_packet[6]& (1 << 2);
  square = DS4BT_packet[6]& (1 << 3);

  DPAD_NW = DS4BT_packet[6]& (1 << 5) &&DS4BT_packet[6]& (1 << 4);
  DPAD_W = DS4BT_packet[6]& (1 << 5);
  DPAD_SW = (DS4BT_packet[6]& (1 << 5) && DS4BT_packet[6]& (1 << 6));
  DPAD_S = DS4BT_packet[6]& (1 << 6);
  DPAD_SE = (DS4BT_packet[6]& (1 << 6) && DS4BT_packet[6]& (1 << 7));
  DPAD_E = DS4BT_packet[6]& (1 << 7);
  DPAD_NE = (DS4BT_packet[6]& (1 << 7) && DS4BT_packet[6]& (1 << 4));
  DPAD_N = DS4BT_packet[6]& (1 << 4);
  r3 = DS4BT_packet[7]& (1 << 3);
  l3 = DS4BT_packet[7]& (1 << 1);
  options = DS4BT_packet[7]& (1 << 5);
  share = DS4BT_packet[7]& (1 << 4);
  touchpad = DS4BT_packet[7]& (1 << 6);
  PS = DS4BT_packet[7]& (1 << 7);
  r1 = DS4BT_packet[7]& (1 << 2);
  l1 = DS4BT_packet[7]& (1 << 0);


  //Deadzone
  if (!(DS4BT_packet[0] > 118 && DS4BT_packet[0] < 136)) {
    lstick_x = DS4BT_packet[0] - 127;
  } else {
    lstick_x = 0;
  }
  if (!(DS4BT_packet[1] > 118 && DS4BT_packet[1] < 136)) {
    lstick_y = -1*(DS4BT_packet[1] - 127);
  } else {
    lstick_y = 0;
  }
  if (!(DS4BT_packet[2] > 118 && DS4BT_packet[2] < 136)) {
    rstick_x = DS4BT_packet[2] - 127;
  } else {
    rstick_x = 0;
  }
  if (!(DS4BT_packet[3] > 118 && DS4BT_packet[3] < 136)) {
    rstick_y =  -1*(DS4BT_packet[3] - 127);
  } else {
    rstick_y = 0;
  }
    
  l2_trig = DS4BT_packet[4];
  r2_trig = DS4BT_packet[5];
    l2_trig >=0 ? l2=l2_trig  : l2=0;
    r2_trig >=0 ? r2=r2_trig  : r2=0;
}


  /*
    if (DS4BT_packet[6]& (1 << 0)) {
    pc.printf("Triangle\r\n");
  }
  if (DS4BT_packet[6]& (1 << 1)) {
    pc.printf("Circle\r\n");
  }
  if (DS4BT_packet[6]& (1 << 2)) {
    pc.printf("Cross\r\n");
  }
  if (DS4BT_packet[6]& (1 << 3)) {
    pc.printf("Square\r\n");
  }
  if (DS4BT_packet[6]& (1 << 4)) {
    pc.printf("Up\r\n");
  }
  if (DS4BT_packet[6]& (1 << 5)) {
    pc.printf("Right\r\n");
  }
  if (DS4BT_packet[6]& (1 << 6)) {
    pc.printf("Down\r\n");
  }
  if (DS4BT_packet[6]& (1 << 7)) {
    pc.printf("Left\r\n");
  }
  if (DS4BT_packet[7]& (1 << 0)) {
    pc.printf("L1\r\n");
  }
  if (DS4BT_packet[7]& (1 << 1)) {
    pc.printf("L3\r\n");
  }
  if (DS4BT_packet[7]& (1 << 2)) {
    pc.printf("R1\r\n");
  }
  if (DS4BT_packet[7]& (1 << 3)) {
    pc.printf("R3\r\n");
  }
  if (DS4BT_packet[7]& (1 << 4)) {
    pc.printf("Share\r\n");
  }
  if (DS4BT_packet[7]& (1 << 5)) {
    pc.printf("Options\r\n");
  }
  if (DS4BT_packet[7]& (1 << 6)) {
    pc.printf("Touchpad\r\n");
  }
  if (DS4BT_packet[7]& (1 << 7)) {
    pc.printf("PS\r\n");
  }



    pc.printf("L2 %d\r\n", DS4BT_packet[4]);

    pc.printf("R2 %d\r\n", DS4BT_packet[5]);
  
  pc.printf("lstick_x %d\r\n", DS4BT_packet[0]);
  pc.printf("lstick_y %d\r\n", DS4BT_packet[1]);
  pc.printf("rstick_x %d\r\n", DS4BT_packet[2]);
  pc.printf("rstick_y %d\r\n", DS4BT_packet[3]);

if (DS4BT_packet[8]& (1 << 0)) {
    pc.printf("Touch 0\r\n");
  }
  if (DS4BT_packet[8]& (1 << 1)) {
    pc.printf("Touch 1\r\n");
  }
  pc.printf("Angle Pitch %d\r\n", DS4BT_packet[9]);
  pc.printf("Angle Roll %d\r\n", DS4BT_packet[10]);
  pc.printf("Touch 0X %d\r\n", DS4BT_packet[11]);
  pc.printf("Touch 0Y %d\r\n", DS4BT_packet[12]);
  pc.printf("Touch 1X %d\r\n", DS4BT_packet[13]);
  pc.printf("Touch 1Y %d\r\n", DS4BT_packet[14]);
  

  pc.printf("--------------------------------------------\r\n");
*/