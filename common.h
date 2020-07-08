#pragma once

// Constants
#define PI 3.14159265359
#define RAD_TO_DEG float(57.2957795)
#define DEG_TO_RAD float(0.01745329251)

// Robot Constants
#define AE_TO_GLOBAL_X 0.18
#define AE_TO_GLOBAL_Y 0.13
#define AE_TO_GLOBAL_W 0
#define INVERSE_KINEMATICS_LX 0.28112
#define INVERSE_KINEMATICS_LY 0.28573
#define WHEEL_RADIUS 0.1524
#define GEAR_RATIO 21


// Functions
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

// Data Structure
struct position {
    float x;
    float y;
    float w;
};

enum Command {noAction, homing, readyPosition, fire, VESC, toggleVG};

struct pointInfo
{
   struct position targetPos;

};





