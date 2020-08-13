#include "DS4_BT_Serial_Host_Shield.h"

DS4_BT_Serial_Host_Shield::DS4_BT_Serial_Host_Shield(BufferedSerial *_DS4_BT_BufferedSerial) {
  DS4_BT_BufferedSerial = _DS4_BT_BufferedSerial;
}

void DS4_BT_Serial_Host_Shield::getPacket() {
  while (!DS4_BT_BufferedSerial->readable())
    ;
  if (DS4_BT_BufferedSerial->getc() == 'D') {
    while (!DS4_BT_BufferedSerial->readable())
      ;
    if (DS4_BT_BufferedSerial->getc() == 'S') {

      while (!DS4_BT_BufferedSerial->readable())
        ;
      if (DS4_BT_BufferedSerial->getc() == '4') {

        for (int i = 0; i < 15; i++) {
          while (!DS4_BT_BufferedSerial->readable())
            ;
          DS4_BT_Packet[i] = DS4_BT_BufferedSerial->getc();
        }

        parsePacket();
      }
    }
  }
}

void DS4_BT_Serial_Host_Shield::parsePacket() {

  DS4_Input.Triangle = DS4_BT_Packet[6] & (1 << 0);
  DS4_Input.Circle = DS4_BT_Packet[6] & (1 << 1);
  DS4_Input.Cross = DS4_BT_Packet[6] & (1 << 2);
  DS4_Input.Square = DS4_BT_Packet[6] & (1 << 3);

  DS4_Input.DPAD_N = DS4_BT_Packet[6] & (1 << 4);
  DS4_Input.DPAD_NW =
      DS4_BT_Packet[6] & (1 << 5) && DS4_BT_Packet[6] & (1 << 4);
  DS4_Input.DPAD_W = DS4_BT_Packet[6] & (1 << 5);
  DS4_Input.DPAD_SW =
      (DS4_BT_Packet[6] & (1 << 5) && DS4_BT_Packet[6] & (1 << 6));
  DS4_Input.DPAD_S = DS4_BT_Packet[6] & (1 << 6);
  DS4_Input.DPAD_SE =
      (DS4_BT_Packet[6] & (1 << 6) && DS4_BT_Packet[6] & (1 << 7));
  DS4_Input.DPAD_E = DS4_BT_Packet[6] & (1 << 7);
  DS4_Input.DPAD_NE =
      (DS4_BT_Packet[6] & (1 << 7) && DS4_BT_Packet[6] & (1 << 4));

  DS4_Input.L1 = DS4_BT_Packet[7] & (1 << 0);
  DS4_Input.L3 = DS4_BT_Packet[7] & (1 << 1);
  DS4_Input.R1 = DS4_BT_Packet[7] & (1 << 2);
  DS4_Input.R3 = DS4_BT_Packet[7] & (1 << 3);
  DS4_Input.Share = DS4_BT_Packet[7] & (1 << 4);
  DS4_Input.Options = DS4_BT_Packet[7] & (1 << 5);
  DS4_Input.Touchpad = DS4_BT_Packet[7] & (1 << 6);
  DS4_Input.PS = DS4_BT_Packet[7] & (1 << 7);

  // Deadzone
  if (!(DS4_BT_Packet[0] > 117 && DS4_BT_Packet[0] < 137)) {
    DS4_Input.Lstick_x = DS4_BT_Packet[0] - 127;
  } else {
    DS4_Input.Lstick_x = 0;
  }
  if (!(DS4_BT_Packet[1] > 117 && DS4_BT_Packet[1] < 137)) {
    DS4_Input.Lstick_y = -1 * (DS4_BT_Packet[1] - 127);
  } else {
    DS4_Input.Lstick_y = 0;
  }
  if (!(DS4_BT_Packet[2] > 117 && DS4_BT_Packet[2] < 137)) {
    DS4_Input.Rstick_x = DS4_BT_Packet[2] - 127;
  } else {
    DS4_Input.Rstick_x = 0;
  }
  if (!(DS4_BT_Packet[3] > 117 && DS4_BT_Packet[3] < 137)) {
    DS4_Input.Rstick_y = -1 * (DS4_BT_Packet[3] - 127);
  } else {
    DS4_Input.Rstick_y = 0;
  }

  DS4_Input.L2_Trigger = DS4_BT_Packet[4];
  DS4_Input.R2_Trigger = DS4_BT_Packet[5];
  DS4_Input.L2_Trigger >= 0
      ? DS4_Input.L2 = DS4_Input.L2_Trigger
      : DS4_Input.L2 = 0;
  DS4_Input.R2_Trigger >= 0
      ? DS4_Input.R2 = DS4_Input.R2_Trigger
      : DS4_Input.R2 = 0;

    #ifdef DS4_BT_Debug
  if (DS4_BT_Packet[6]& (1 << 0)) {
  printf("Triangle\r\n");
}
if (DS4_BT_Packet[6]& (1 << 1)) {
  printf("Circle\r\n");
}
if (DS4_BT_Packet[6]& (1 << 2)) {
  printf("Cross\r\n");
}
if (DS4_BT_Packet[6]& (1 << 3)) {
  printf("Square\r\n");
}
if (DS4_BT_Packet[6]& (1 << 4)) {
  printf("Up\r\n");
}
if (DS4_BT_Packet[6]& (1 << 5)) {
  printf("Right\r\n");
}
if (DS4_BT_Packet[6]& (1 << 6)) {
  printf("Down\r\n");
}
if (DS4_BT_Packet[6]& (1 << 7)) {
  printf("Left\r\n");
}
if (DS4_BT_Packet[7]& (1 << 0)) {
  printf("L1\r\n");
}
if (DS4_BT_Packet[7]& (1 << 1)) {
  printf("L3\r\n");
}
if (DS4_BT_Packet[7]& (1 << 2)) {
  printf("R1\r\n");
}
if (DS4_BT_Packet[7]& (1 << 3)) {
  printf("R3\r\n");
}
if (DS4_BT_Packet[7]& (1 << 4)) {
  printf("Share\r\n");
}
if (DS4_BT_Packet[7]& (1 << 5)) {
  printf("Options\r\n");
}
if (DS4_BT_Packet[7]& (1 << 6)) {
  printf("Touchpad\r\n");
}
if (DS4_BT_Packet[7]& (1 << 7)) {
  printf("PS\r\n");
}



  printf("L2 %d\r\n", DS4_BT_Packet[4]);

  printf("R2 %d\r\n", DS4_BT_Packet[5]);

printf("lstick_x %d\r\n", DS4_BT_Packet[0]);
printf("lstick_y %d\r\n", DS4_BT_Packet[1]);
printf("rstick_x %d\r\n", DS4_BT_Packet[2]);
printf("rstick_y %d\r\n", DS4_BT_Packet[3]);

if (DS4_BT_Packet[8]& (1 << 0)) {
  printf("Touch 0\r\n");
}
if (DS4_BT_Packet[8]& (1 << 1)) {
  printf("Touch 1\r\n");
}
printf("Angle Pitch %d\r\n", DS4_BT_Packet[9]);
printf("Angle Roll %d\r\n", DS4_BT_Packet[10]);
printf("Touch 0X %d\r\n", DS4_BT_Packet[11]);
printf("Touch 0Y %d\r\n", DS4_BT_Packet[12]);
printf("Touch 1X %d\r\n", DS4_BT_Packet[13]);
printf("Touch 1Y %d\r\n", DS4_BT_Packet[14]);


printf("--------------------------------------------\r\n");

    #endif
}

