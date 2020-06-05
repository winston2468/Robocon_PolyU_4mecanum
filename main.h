#include "Motor.h"
#include "common.h"

Motor motor;

// Robot parameters
float lx = INVERSE_KINEMATICS_LX;
float ly = INVERSE_KINEMATICS_LY;
float wheelR = WHEEL_RADIUS; // RoboMaster Mecanum wheel
float gear = GEAR_RATIO;
//float radian_to_rpm_convert = (RAD_TO_DEG / 360) * 60 * gear;
float radian_to_rpm_convert = (1/((2*PI)/60)) * gear * 2;
double maximunSpeed = 1.0;

// Motor
int motor1 = 0;
int motor2 = 0;
int motor3 = 0;
int motor4 = 0;
