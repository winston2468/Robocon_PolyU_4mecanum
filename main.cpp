
#include "./USBHostXpad/USBHostXpad.h"
#include "mbed.h"
#include "main.h"
#include "./INA3221/INA3221.h"
#include "./DT35/DT35.h"

#define TAKING1 1;
#define TAKING2 3;
#define TAKING3 5;
#define TAKING4 7;
#define TAKING5 9;
#define KICKING1 2;
#define KICKING2 4;
#define KICKING3 6;
#define KICKING4 8;
#define KICKING5 10;
#define STOP 0;

#define LEFT_GROUND 0;
#define RIGHT_GROUND 1;

Serial pc(USBTX, USBRX);
Thread DS4_thread;
Thread quad_mecanum_thread;
Thread DT35_thread;
Thread pneumatic_thread;
volatile bool triangle, circle, cross, square;
volatile bool DPAD_NW, DPAD_W, DPAD_SW, DPAD_S, DPAD_SE, DPAD_E, DPAD_NE, DPAD_N;
volatile bool options, share, r2, l2, r1, l1;
volatile int r3, l3;
volatile int lstick_x, lstick_y, rstick_x, rstick_y;
volatile int l2_trig, r2_trig;
float vx=0;
float vy=0;
float w=0;
int maxPVelocity = 0;

volatile int playground = LEFT_GROUND;
volatile int autoMode = 0;//
volatile int auto_stage = STOP;
volatile int safety_margine = 100;//
volatile int center_distance_x = 500;//
volatile int center_distance_y = 500;//
volatile int key_point_y = 6910;
volatile int pass_point1_center_y = 9790 - center_distance_y;
volatile int pass_point2_center_y = 9520 - center_distance_y;
volatile int pass_point3_center_y = 9250 - center_distance_y;
volatile int pass_point4_center_y = 8980 - center_distance_y;
volatile int pass_point5_center_y = 8710 - center_distance_y;
volatile int distance1 = 0; //y1
volatile int distance2 = 0; //y2
volatile int changing_range = 2900; // acceptable changing range of motor movement in mm/ms

volatile int buttons_l =0;
DigitalOut pneumatic_Pick(PC_5,0);
DigitalOut pneumatic_Throw(PB_9,0);
DigitalOut pneumatic_Kick(PB_10,0);






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
  
        pneumatic_Pick=square;
        pneumatic_Throw=circle;
        pneumatic_Kick=cross;
        /*
        if (triangle) {
            setAutoMode();
        }
        */

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
      ThisThread::sleep_for(1000);
    }

    while (xpad.connected()) {

      // show what buttons are pressed every 0.5s
      //showbuttons();
      // This sleep_for can be removed
      
    }
  }
}

void setAutoMode(){
    if(autoMode == 1){
        autoMode = 0;
    }
    else if(autoMode == 0){
        auto_stage++;
        autoMode = 1;
    }
}

void inverse()
{
    while(1){
     // speed scalling for left/right(slower speed) jostick XY
     vy = ((float)lstick_x / 100) + ((float)rstick_x / 500) ; 
     vx = ((float)lstick_y / 100 *-1) + ((float)rstick_y / 500 *-1);


     // rotation L1/R1 , L2/R2(slower speed)
     w=l1*1.3 - r1*1.3  + l2*0.3 -r2*0.3;

     

    //Limit the robot's velocity range
    maxPVelocity = motor.getMaxPVelocity();
    motor1 = constrain(int((1 / wheelR) * (vx - vy - (lx + ly) * w) * radian_to_rpm_convert) , -maxPVelocity, maxPVelocity);
    motor2 = constrain(int((1 / wheelR) * (vx + vy + (lx + ly) * w) * radian_to_rpm_convert) , -maxPVelocity, maxPVelocity);
    motor3 = constrain(int((1 / wheelR) * (vx + vy - (lx + ly) * w) * radian_to_rpm_convert) , -maxPVelocity, maxPVelocity);
    motor4 = constrain(int((1 / wheelR) * (vx - vy + (lx + ly) * w) * radian_to_rpm_convert) , -maxPVelocity, maxPVelocity);
    motor.update(motor1, motor2, motor3, motor4);
    ThisThread::sleep_for(25);
    }
}

void DT35_task(){
    //setup
    DT35 *DT35_class = new DT35(PB_9,PB_8,(0x82));;
    DT35_class->DT35_initialization(3);
    printf("INA3221:   FID:%d   UID:%d    Mode:%d\r\n",DT35_class->getManufacturerID(1),DT35_class->getDieID(1),DT35_class->getConfiguration(1));
    
    while (1){
        if(distance1 == 0 || ((distance1 - changing_range) <= DT35_class->getBusVoltage(1, 1) && (distance1 + changing_range) >= DT35_class->getBusVoltage(1, 1)))
        {
            distance1 = DT35_class->getBusVoltage(1, 1);
        }
        if(distance2 == 0 || ((distance2 - changing_range) <= DT35_class->getBusVoltage(1, 2) && (distance2 + changing_range) >= DT35_class->getBusVoltage(1, 2)))
        {
            distance2 = DT35_class->getBusVoltage(1, 2);
        }

        if (triangle) {
            setAutoMode();
        }

        if(DT35_class->getBusVoltage(1, 1) < DT35_class->getBusVoltage(1, 2)){
            w = 1;
        }
        else if(DT35_class->getBusVoltage(1, 2) < DT35_class->getBusVoltage(1, 1)){
            w = -1;
        }
        else {
            w = 0;
        }
        if(playground == 0){
            if(auto_stage == 1){
                if(distance1 < (10000 - pass_point1_center_y)){
                    vy = (float)1.28; 
                    vx = (float)0;
                }
                else{
                    vy = (float)-1.28; 
                    vx = (float)0;
                }
            }
            else if(auto_stage == 2){
                if(distance1 < (10000 - key_point_y)){
                    vy = (float)1.28; 
                    vx = (float)0.3;
                }
                else{
                    vy = (float)-1.28; 
                    vx = (float)-0.3;
                }
            }
            else if(auto_stage == 3){
                if(distance1 < (10000 - pass_point2_center_y)){
                    vy = (float)1.28; 
                    vx = (float)0;
                }
                else{
                    vy = (float)-1.28; 
                    vx = (float)0;
                }
            }
            else if(auto_stage == 4){
                if(distance1 < (10000 - key_point_y)){
                    vy = (float)1.28; 
                    vx = (float)0.3;
                }
                else{
                    vy = (float)-1.28; 
                    vx = (float)-0.3;
                }
            }
            else if(auto_stage == 5){
                if(distance1 < (10000 - pass_point3_center_y)){
                    vy = (float)1.28; 
                    vx = (float)0;
                }
                else{
                    vy = (float)-1.28; 
                    vx = (float)0;
                }
            }
            else if(auto_stage == 6){
                if(distance1 < (10000 - key_point_y)){
                    vy = (float)1.28; 
                    vx = (float)0.3;
                }
                else{
                    vy = (float)-1.28; 
                    vx = (float)-0.3;
                }
            }
            else if(auto_stage == 7){
                if(distance1 < (10000 - pass_point4_center_y)){
                    vy = (float)1.28; 
                    vx = (float)0;
                }
                else{
                    vy = (float)-1.28; 
                    vx = (float)0;
                }
            }
            else if(auto_stage == 8){
                if(distance1 < (10000 - key_point_y)){
                    vy = (float)1.28; 
                    vx = (float)0.3;
                }
                else{
                    vy = (float)-1.28; 
                    vx = (float)-0.3;
                }
            }
            else if(auto_stage == 9){
                if(distance1 < (10000 - pass_point5_center_y)){
                    vy = (float)1.28; 
                    vx = (float)0;
                }
                else{
                    vy = (float)-1.28; 
                    vx = (float)0;
                }
            }
            else if(auto_stage == 10){
                if(distance1 < (10000 - key_point_y)){
                    vy = (float)1.28; 
                    vx = (float)0.3;
                }
                else{
                    vy = (float)-1.28; 
                    vx = (float)-0.3;
                }
            }
        }
        else if(playground == 1){
            if(auto_stage == 1){
                if(distance1 < pass_point1_center_y){
                    vy = (float)1.28; 
                    vx = (float)0;
                }
                else{
                    vy = (float)-1.28; 
                    vx = (float)0;
                }
            }
            else if(auto_stage == 2){
                if(distance1 < key_point_y){
                    vy = (float)1.28; 
                    vx = (float)-0.3;
                }
                else{
                    vy = (float)-1.28; 
                    vx = (float)0.3;
                }
            }
            else if(auto_stage == 3){
                if(distance1 < pass_point2_center_y){
                    vy = (float)1.28; 
                    vx = (float)0;
                }
                else{
                    vy = (float)-1.28; 
                    vx = (float)0;
                }
            }
            else if(auto_stage == 4){
                if(distance1 < key_point_y){
                    vy = (float)1.28; 
                    vx = (float)-0.3;
                }
                else{
                    vy = (float)-1.28; 
                    vx = (float)0.3;
                }
            }
            else if(auto_stage == 5){
                if(distance1 < pass_point3_center_y){
                    vy = (float)1.28; 
                    vx = (float)0;
                }
                else{
                    vy = (float)-1.28; 
                    vx = (float)0;
                }
            }
            else if(auto_stage == 6){
                if(distance1 < key_point_y){
                    vy = (float)1.28; 
                    vx = (float)-0.3;
                }
                else{
                    vy = (float)-1.28; 
                    vx = (float)0.3;
                }
            }
            else if(auto_stage == 7){
                if(distance1 < pass_point4_center_y){
                    vy = (float)1.28; 
                    vx = (float)0;
                }
                else{
                    vy = (float)-1.28; 
                    vx = (float)0;
                }
            }
            else if(auto_stage == 8){
                if(distance1 < key_point_y){
                    vy = (float)1.28; 
                    vx = (float)-0.3;
                }
                else{
                    vy = (float)-1.28; 
                    vx = (float)0.3;
                }
            }
            else if(auto_stage == 9){
                if(distance1 < pass_point5_center_y){
                    vy = (float)1.28; 
                    vx = (float)0;
                }
                else{
                    vy = (float)-1.28; 
                    vx = (float)0;
                }
            }
            else if(auto_stage == 10){
                if(distance1 < key_point_y){
                    vy = (float)1.28; 
                    vx = (float)-0.3;
                }
                else{
                    vy = (float)-1.28; 
                    vx = (float)0.3;
                }
            }
        }
        printf("CH1:%dV   ", DT35_class->getBusVoltage(1, 1));
        printf("CH2:%dV   ", DT35_class->getBusVoltage(1, 2));

        maxPVelocity = motor.getMaxPVelocity();
        motor1 = constrain(int((1 / wheelR) * (vx - vy - (lx + ly) * w) * radian_to_rpm_convert) , -maxPVelocity, maxPVelocity);
        motor2 = constrain(int((1 / wheelR) * (vx + vy + (lx + ly) * w) * radian_to_rpm_convert) , -maxPVelocity, maxPVelocity);
        motor3 = constrain(int((1 / wheelR) * (vx + vy - (lx + ly) * w) * radian_to_rpm_convert) , -maxPVelocity, maxPVelocity);
        motor4 = constrain(int((1 / wheelR) * (vx - vy + (lx + ly) * w) * radian_to_rpm_convert) , -maxPVelocity, maxPVelocity);
        motor.update(motor1, motor2, motor3, motor4);

        ThisThread::sleep_for(100);
    }
}

//TODO REWRITE  + REPLACE 
//https://github.com/ARMmbed/mbed-os/issues/9495
//https://github.com/ARMmbed/mbed-os/issues/6714
int main() {
    // motorInitialization(); //Must be on first line in function due to some
    // wried timing problems with the motor controller
    pc.baud(115200);
    pc.printf("--------------------------------------------\r\n");
    if(autoMode == 0){
        quad_mecanum_thread.start(callback(inverse));
    }
    else if(autoMode == 1){
        DT35_thread.start(callback(DT35_task));
    }
    DS4_thread.start(callback(xpad_task));

    while (1) {
    }
}
