/*
  Example sketch for the PS4 Bluetooth library - developed by Kristian Lauszus
  For more information visit my blog: http://blog.tkjelectronics.dk/ or
  send me an e-mail:  kristianl@tkjelectronics.com
*/
//https://github.com/felis/USB_Host_Shield_2.0
#include <PS4BT.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

uint8_t ds4_packet[15] = {0};
USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside
BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so

/* You can create the instance of the PS4BT class in two ways */
// This will start an inquiry and then pair with the PS4 controller - you only have to do this once
// You will need to hold down the PS and Share button at the same time, the PS4 controller will then start to blink rapidly indicating that it is in pairing mode
PS4BT PS4(&Btd, PAIR);

// After that you can simply create the instance like so and then press the PS button on the device
//PS4BT PS4(&Btd);

//bool printAngle, printTouch;
//uint8_t oldL2Value, oldR2Value;

void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); // Halt
  }
  Serial.print(F("\r\nPS4 Bluetooth Library Started"));
}

 
void loop() {
  Usb.Task();

  if (PS4.connected()) {
    Serial.print(F("\r\nDS4"));
    ds4_packet[0] = PS4.getAnalogHat(LeftHatX);
    ds4_packet[1] = PS4.getAnalogHat(LeftHatY);
    ds4_packet[2] = PS4.getAnalogHat(RightHatX);
    ds4_packet[3] = PS4.getAnalogHat(RightHatY);
    ds4_packet[4] = PS4.getAnalogButton(L2);
    ds4_packet[5] = PS4.getAnalogButton(R2);


    bitWrite(ds4_packet[6], 0, PS4.getButtonPress(TRIANGLE));
    bitWrite(ds4_packet[6], 1, PS4.getButtonPress(CIRCLE));
    bitWrite(ds4_packet[6], 2, PS4.getButtonPress(CROSS));
    bitWrite(ds4_packet[6], 3, PS4.getButtonPress(SQUARE));
    bitWrite(ds4_packet[6], 4, PS4.getButtonPress(UP));
    bitWrite(ds4_packet[6], 5, PS4.getButtonPress(RIGHT));
    bitWrite(ds4_packet[6], 6, PS4.getButtonPress(DOWN));
    bitWrite(ds4_packet[6], 7, PS4.getButtonPress(LEFT));

    bitWrite(ds4_packet[7], 0, PS4.getButtonPress(L1));
    bitWrite(ds4_packet[7], 1, PS4.getButtonPress(L3));
    bitWrite(ds4_packet[7], 2, PS4.getButtonPress(R1));
    bitWrite(ds4_packet[7], 3, PS4.getButtonPress(R3));
    bitWrite(ds4_packet[7], 4, PS4.getButtonPress(SHARE));
    bitWrite(ds4_packet[7], 5, PS4.getButtonPress(OPTIONS));
    bitWrite(ds4_packet[7], 6, PS4.getButtonPress(TOUCHPAD));
    bitWrite(ds4_packet[7], 7, PS4.getButtonPress(PS));
    
    bitWrite(ds4_packet[8], 0, PS4.isTouching(0));
    bitWrite(ds4_packet[8], 1, PS4.isTouching(1));


    ds4_packet[9] = PS4.getAngle(Pitch);
    ds4_packet[10] = PS4.getAngle(Roll);


    ds4_packet[11] = PS4.getX(0);
    ds4_packet[12] = PS4.getY(0);

    ds4_packet[13] = PS4.getX(1);
    ds4_packet[14] = PS4.getY(1);

    Serial.write(ds4_packet, 15);
    /*
    if (PS4.getButtonPress(PS)) {

      PS4.disconnect();

    }
    */

  }
}
