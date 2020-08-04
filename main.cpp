
#include "USBHostXpad.h"
#include "mbed.h"
#include "BufferedSerial.h"
#include "main.h"


Thread DS4BT_thread;
uint8_t DS4BT_packet[15] = {0};

 BufferedSerial pc(USBTX, USBRX);
  BufferedSerial device(PA_9, PA_10);


Thread DS4_thread;
Thread quad_mecanum_thread;
volatile bool triangle, circle, cross, square;
volatile bool DPAD_NW, DPAD_W, DPAD_SW, DPAD_S, DPAD_SE, DPAD_E, DPAD_NE, DPAD_N;
volatile bool options, share, r2, l2, r1, l1;
volatile bool touchpad, touchpad_finger0, touchpad_finger1, PS;
volatile bool r3, l3;
volatile int lstick_x, lstick_y, rstick_x, rstick_y;
volatile int l2_trig, r2_trig;
bool ready=0;
float vx=0;
float vy=0;
float w=0;
int maxPVelocity = 0;
bool dir_mode = 0;
volatile int buttons_l =0;
DigitalOut pneumatic_Pick(PB_0,0);
DigitalOut pneumatic_Throw(PA_4,0);
DigitalOut pneumatic_Kick(PB_15,0);
bool throw_auto = 0;




void parseDS4(int buttons, int buttons2, int stick_lx, int stick_ly,
              int stick_rx, int stick_ry, int trigger_l, int trigger_r) {

  triangle = buttons & (1 << 7);
  circle = buttons & (1 << 6);
  cross = buttons & (1 << 5);
  square = buttons & (1 << 4);
  buttons_l = buttons & 0x0f;
  DPAD_NW = buttons_l == 0x07;
  DPAD_W = buttons_l == 0x06;
  DPAD_SW = buttons_l == 0x05;
  DPAD_S = buttons_l == 0x04;
  DPAD_SE = buttons_l == 0x03;
  DPAD_E = buttons_l == 0x02;
  DPAD_NE = buttons_l == 0x01;
  DPAD_N = buttons_l == 0x00;
  r3 = buttons2 & (1 << 7);
  l3 = buttons2 & (1 << 6);
  options = buttons2 & (1 << 5);
  share = buttons2 & (1 << 4);
  r2 = buttons2 & (1 << 3);
  l2 = buttons2 & (1 << 2);
  r1 = buttons2 & (1 << 1);
  l1 = buttons2 & (1 << 0);
  //Deadzone
  if (!(stick_lx > 118 && stick_lx < 136)) {
    lstick_x = stick_lx - 127;
  } else {
    lstick_x = 0;
  }
  if (!(stick_ly > 118 && stick_ly < 136)) {
    lstick_y = -1*(stick_ly - 127);
  } else {
    lstick_y = 0;
  }
  if (!(stick_rx > 118 && stick_rx < 136)) {
    rstick_x = stick_rx - 127;
  } else {
    rstick_x = 0;
  }
  if (!(stick_ry > 118 && stick_ry < 136)) {
    rstick_y =  -1*(stick_ry - 127);
  } else {
    rstick_y = 0;
  }

  l2_trig = trigger_l;
  r2_trig = trigger_r;

        if(throw_auto == 0){
        pneumatic_Throw=triangle;
        }

        pneumatic_Pick=circle;
        pneumatic_Kick=cross;
        if(options){
            if(dir_mode){
            lx =INVERSE_KINEMATICS_LX;
            ly = INVERSE_KINEMATICS_LY;
            dir_mode = 0;
            }
            else {
            lx =INVERSE_KINEMATICS_LY;
            ly = INVERSE_KINEMATICS_LX;
            dir_mode = 1;
            }
            
        }

}


void Parse_DS4_BT() {
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
        if(throw_auto == 0){
        pneumatic_Throw=triangle;
        }

        pneumatic_Pick=circle;
        pneumatic_Kick=cross;
        if(options){
            if(dir_mode){
            lx =INVERSE_KINEMATICS_LX;
            ly = INVERSE_KINEMATICS_LY;
            dir_mode = 0;
            }
            else {
            lx =INVERSE_KINEMATICS_LY;
            ly = INVERSE_KINEMATICS_LX;
            dir_mode = 1;
            }
            
        }

}

void DS4BT_task() {

  while (1) {
    if (!device.readable())
      ;
    if (device.getc() == 'D') {
      while (!device.readable())
        ;
      if (device.getc() == 'S') {

        while (!device.readable())
          ;
        if (device.getc() == '4') {
                ready=1;
          for (int i = 0; i < 15; i++) {
            while (!device.readable())
            ;
              DS4BT_packet[i] = device.getc();
            
          }




        }
      }
    }
     if(ready){
 Parse_DS4_BT();
 }
   // ThisThread::sleep_for(15);
 }

 
}
/* 
// functions:if button pressed is true -> print
void showbuttons() {

  if (triangle) {
    pc.printf("triangle\r\n");
  }
  if (circle) {
    pc.printf("circle\r\n");
  }
  if (cross) {
    pc.printf("cross\r\n");
  }
  if (square) {
    pc.printf("square\r\n");
  }
  if (DPAD_NW) {
    pc.printf("DPAD_NW\r\n");
  }
  if (DPAD_W) {
    pc.printf("DPAD_W\r\n");
  }
  if (DPAD_SW) {
    pc.printf("DPAD_SW\r\n");
  }
  if (DPAD_S) {
    pc.printf("DPAD_S\r\n");
  }
  if (DPAD_SE) {
    pc.printf("DPAD_SE\r\n");
  }
  if (DPAD_E) {
    pc.printf("DPAD_E\r\n");
  }
  if (DPAD_NE) {
    pc.printf("DPAD_NE\r\n");
  }
  if (DPAD_N) {
    pc.printf("DPAD_N\r\n");
  }
  if (r3) {
    pc.printf("r3\r\n");
  }
  if (l3) {
    pc.printf("l3\r\n");
  }
  if (options) {
    pc.printf("options\r\n");
  }
  if (share) {
    pc.printf("share\r\n");
  }

  if (l1) {
    pc.printf("l1\r\n");
  }
  if (r1) {
    pc.printf("r1\r\n");
  }

  if (r2) {
    pc.printf("r2 %d\r\n", r2_trig);
  }
  if (l2) {
    pc.printf("l2 %d\r\n", l2_trig);
  }
  pc.printf("lstick_x %d\r\n", lstick_x);
  pc.printf("lstick_y %d\r\n", lstick_y);
  pc.printf("rstick_x %d\r\n", rstick_x);
  pc.printf("rstick_y %d\r\n", rstick_y);
  pc.printf("--------------------------------------------\r\n");
}
*/


// attached handler, USBHostXpad onUpdate
void onXpadEvent(int buttons, int buttons2, int stick_lx, int stick_ly,
                 int stick_rx, int stick_ry, int trigger_l, int trigger_r) {
  // pc.printf("DS4: %02x %02x %-5d %-5d %-5d %-5d %02x %02x\r\n", buttons,
  // buttons2, stick_lx, stick_ly, stick_rx, stick_ry, trigger_l, trigger_r);
  parseDS4(buttons, buttons2, stick_lx, stick_ly, stick_rx, stick_ry, trigger_l,
           trigger_r);
}
void xpad_task() {
  USBHostXpad xpad;
  xpad.attachEvent(&onXpadEvent);
  while (1) {

    while (!xpad.connect()) {
      // This sleep_for can be removed
      //ThisThread::sleep_for(1000);
      //if(ready){
     //   Parse_DS4_BT();
   //     ThisThread::sleep_for(1000);
     // }
      
    }

    while (xpad.connected()) {

      // show what buttons are pressed every 0.5s
      //showbuttons();
      // This sleep_for can be removed
      
    }
  }
}


void inverse()
{ 
    while(1){
     // speed scalling for left/right(slower speed) jostick XY
if (DPAD_W){


     vy = 4;
     vx =0 ; 
    maxPVelocity = motor.getMaxPVelocity();
    motor1 = constrain(int((1 / wheelR) * (vx - vy - (lx + ly) * w) * radian_to_rpm_convert) , -maxPVelocity, maxPVelocity);
    motor2 = constrain(int((1 / wheelR) * (vx + vy + (lx + ly) * w) * radian_to_rpm_convert) , -maxPVelocity, maxPVelocity);
    motor3 = constrain(int((1 / wheelR) * (vx + vy - (lx + ly) * w) * radian_to_rpm_convert) , -maxPVelocity, maxPVelocity);
    motor4 = constrain(int((1 / wheelR) * (vx - vy + (lx + ly) * w) * radian_to_rpm_convert) , -maxPVelocity, maxPVelocity);
    motor.update(motor1, motor2, motor3, motor4);
    
     }
     else if (square) {
     vy = 4;
     vx =0 ; 
    maxPVelocity = motor.getMaxPVelocity();
    motor1 = constrain(int((1 / wheelR) * (vx - vy - (lx + ly) * w) * radian_to_rpm_convert) , -maxPVelocity, maxPVelocity);
    motor2 = constrain(int((1 / wheelR) * (vx + vy + (lx + ly) * w) * radian_to_rpm_convert) , -maxPVelocity, maxPVelocity);
    motor3 = constrain(int((1 / wheelR) * (vx + vy - (lx + ly) * w) * radian_to_rpm_convert) , -maxPVelocity, maxPVelocity);
    motor4 = constrain(int((1 / wheelR) * (vx - vy + (lx + ly) * w) * radian_to_rpm_convert) , -maxPVelocity, maxPVelocity);
    motor.update(motor1, motor2, motor3, motor4);

    wait_us(130000);
    throw_auto = 1;
    pneumatic_Throw= 1;
    
    wait_us(50000);
    motor1 = 0;
    motor2 = 0;
    motor3 = 0;
    motor4 = 0;
    motor.update(motor1, motor2, motor3, motor4);
    wait_us(500000);

    pneumatic_Throw= 0;
    throw_auto = 0;
     }
     else{
         if(dir_mode){
          vy = ((float)lstick_x / 100) + ((float)rstick_x / 500) ; 
     vx = ((float)lstick_y / 100 *-1) + ((float)rstick_y / 500 *-1);
         }
         else {
                       vy = ((float)lstick_y / 100) + ((float)rstick_y / 500) ; 
     vx = ((float)lstick_x / 100 *-1) + ((float)rstick_x / 500 *-1);
          }

     // rotation L1/R1 , L2/R2(slower speed)
     w=l1*1.3 - r1*1.3  + l2*0.3 -r2*0.3;



     

    //Limit the robot's velocity range
    maxPVelocity = motor.getMaxPVelocity();
    motor1 = constrain(int((1 / wheelR) * (vx - vy - (lx + ly) * w) * radian_to_rpm_convert) , -maxPVelocity, maxPVelocity);
    motor2 = constrain(int((1 / wheelR) * (vx + vy + (lx + ly) * w) * radian_to_rpm_convert) , -maxPVelocity, maxPVelocity);
    motor3 = constrain(int((1 / wheelR) * (vx + vy - (lx + ly) * w) * radian_to_rpm_convert) , -maxPVelocity, maxPVelocity);
    motor4 = constrain(int((1 / wheelR) * (vx - vy + (lx + ly) * w) * radian_to_rpm_convert) , -maxPVelocity, maxPVelocity);
    motor.update(motor1, motor2, motor3, motor4);
    //ThisThread::sleep_for(25);
    }
    }
}

//TODO REWRITE  + REPLAC1E 
//https://github.com/ARMmbed/mbed-os/issues/9495
//https://github.com/ARMmbed/mbed-os/issues/6714
int main() {
device.baud(115200);
pc.baud(115200);

    DS4BT_thread.start(callback(DS4BT_task));
  quad_mecanum_thread.start(callback(inverse));


  pc.printf("--------------------------------------------\r\n");
  
 // DS4_thread.start(callback(xpad_task));

  while (1) {
    //      if(ready){
       
        //ThisThread::sleep_for(1000);
      //}
  }
}
