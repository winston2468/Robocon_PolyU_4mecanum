#include "robot.h"

//**DEFINE MOTION FUNCTIONS
bool SET_OP_MODE(uint8_t node_num, OP_MODE mode)
{
    char opmode = mode;
    switch(mode)
    {
        case 0x07:
        {
            //PC_SEND("    OP-MODE = IPM\r\n");
            break;
        }
        case 0x06:
        {
            //PC_SEND("    OP-MODE = HM\r\n");
            break;
        }
        case 0x03:
        {
            //PC_SEND("    OP-MODE = PVM\r\n");
            break;
        }
        case 0x01:
        {
            //PC_SEND("    OP-MODE = PPM\r\n");
            break;
        }
        case 0xFF:
        {
            //PC_SEND("    OP-MODE = PM\r\n");
            break;
        }
        case 0xFE:
        {
            //PC_SEND("    OP-MODE = VM\r\n");
            break;
        }
        case 0xFD:
        {
            //PC_SEND("    OP-MODE = CM\r\n");
            break;
        }
        case 0xFC:
        {
            //PC_SEND("    OP-MODE = DM\r\n");
            break;
        }
        case 0xFB:
        {
            //PC_SEND("    OP-MODE = MEM\r\n");
            break;
        }
        case 0xFA:
        {
            //PC_SEND("    OP-MODE = SDM\r\n");
            break;
        }
        default:
        {
            //PC_SEND("    ERROR: UNDEFINED OP MODE\r\n");
            return 0;
        }
    }
    
    if(!SDO_EXE(node_num,0x60,0x60,0x00,opmode,1)){; return 0;}
    
    return 1;
}

//*FUNCTIONS USED IN P-VELOCITY MODE
bool SET_MAX_P_VELOCITY(uint8_t node_num, uint32_t data)
{
    if(!SDO_EXE(node_num,0x7F,0x60,0x00,data,4)){; return 0;}
    //PC_SEND("SET MAX PROFILE VELOCITY: %d\n", data);
    return 1;
}

bool SET_P_ACCELERATION(uint8_t node_num, uint32_t data)
{
    if(!SDO_EXE(node_num,0x83,0x60,0x00,data,4)){; return 0;}
    //PC_SEND("SET PROFILE ACCELERATION: %d\n", data);
    return 1;
}

bool SET_P_DECELERATION(uint8_t node_num, uint32_t data)
{
    if(!SDO_EXE(node_num,0x84,0x60,0x00,data,4)){; return 0;}
    //PC_SEND("SET PROFILE DECELERATION: %d\n", data);
    return 1;
}

bool SET_QUICK_STOP_DECELERATION(uint8_t node_num, uint32_t data)
{
    if(!SDO_EXE(node_num,0x85,0x60,0x00,data,4)){; return 0;}
    //PC_SEND("SET QUICK STOP DECELERATION: %d\n", data);
    return 1;
}

bool SET_MOTION_PROFILE_TYPE(uint8_t node_num, MOTION_PROFILE_TYPE type)
{
    int mpt = type;
    if(!SDO_EXE(node_num,0x86,0x60,0x00,mpt,2)){; return 0;}
    //if(mpt == 0){PC_SEND("SET MOTION PROFILE TYPE: LINEAR\r\n");}
    //else {PC_SEND("SET MOTION PROFILE TYPE: SIN\r\n");}
    return 1;
}

bool SET_TARGET_VELOCITY(uint8_t node_num, int32_t data)
{   
    if(!SDO_EXE(node_num,0xFF,0x60,0x00,data,4)){; return 0;}
    //PC_SEND("SET TARGET VELOCITY: %d\n", data);
    return 1;
}

bool STOP_P_VELOCITY(uint8_t node_num)
{
    if(!SDO_EXE(node_num,0x40,0x60,0x00,0x010F,2)){; return 0;}
    //PC_SEND("STOP PROFILE VELOCITY\r\n");
    return 1;
}

bool SYNC_STOP_P_VELOCITY(uint8_t node_num)
{
    if(!RPDO1_EXE(node_num, 0x010F)){; return 0;}
    //PC_SEND("STOP PROFILE VELOCITY\r\n");
    return 1;
}

bool QUICK_STOP_P_VELOCITY(uint8_t node_num)
{
    if(!SDO_EXE(node_num,0x40,0x60,0x00,0x000B,2)){; return 0;}
    //PC_SEND("QUICK STOP PROFILE VELOCITY\r\n");
    return 1;
}

bool SYNC_QUICK_STOP_P_VELOCITY(uint8_t node_num)
{
    if(!RPDO1_EXE(node_num, 0x000B)){; return 0;}
    //PC_SEND("QUICK STOP PROFILE VELOCITY\r\n");
    return 1;
}

//*FUNCTIONS USED IN CURRENT MODE
bool SET_CONTINOUS_CURRENT_LIMIT(uint8_t node_num, uint16_t data)
{
    if(!SDO_EXE(node_num,0x10,0x64,0x01,data,2)){; return 0;}
    if(!SDO_EXE(node_num,0x10,0x64,0x02,(2*data),2)){; return 0;}
    //PC_SEND("SET CONTINOUS CURRENT LIMIT: %d\n", data);
    //PC_SEND("SET OUTPUT CURRENT LIMIT: %d\n", 2*data);   
    return 1;
}

bool SET_MAX_CURRENT_MODE_SPEED(uint8_t node_num, uint32_t data)
{
    if(!SDO_EXE(node_num,0x10,0x64,0x04,data,4)){; return 0;}
    //PC_SEND("SET MAX CURRENT MODE SPEED: %d\n", data);    
    return 1;
}

bool SET_THERMAL_TIME_CONSTANT_WINDING(uint8_t node_num, uint16_t data)
{
    if(!SDO_EXE(node_num,0x10,0x64,0x05,data,2)){; return 0;}
    //PC_SEND("SET THERMAL TIME CONSTANT WINDING: %d\n", data);    
    return 1;
}

bool SET_CURRENT_VALUE(uint8_t node_num, int16_t data)
{
    if(!SDO_EXE(node_num,0x30,0x20,0x00,data,2)){; return 0;}
    //PC_SEND("SET CURRENT VALUE: %d\n", data);
    return 1;
}

bool STOP_CURRENT_VELOCITY(uint8_t node_num)
{
    if(!SDO_EXE(node_num,0x30,0x20,0x00,0x0000,2)){; return 0;}
    //PC_SEND("STOP CURRENT MODE VELOCITY\r\n");
    return 1;
}

bool QUICK_STOP_CURRENT_VELOCITY(uint8_t node_num)
{
    if(!SDO_EXE(node_num,0x40,0x60,0x00,0x0002,2)){; return 0;}
    //PC_SEND("QUICK STOP CURRENT MODE VELOCITY\r\n");
    return 1;
}

bool SYNC_QUICK_STOP_CURRENT_VELOCITY(uint8_t node_num)
{
    if(!RPDO1_EXE(node_num, 0x0002)){; return 0;}
    //PC_SEND("QUICK STOP CURRENT MODE VELOCITY\r\n");
    return 1;
}

//*FUNCTIONS USED IN POSITION MODE
bool SET_MAX_FOLLOWING_ERROR(uint8_t node_num, uint32_t data)
{
    if(!SDO_EXE(node_num,0x65,0x60,0x00,data,4)){; return 0;}
    //PC_SEND("SET MAXIMUM FOLLOWING ERROR: %d\n", data);    
    return 1;
}

bool SET_MIN_POSITION_LIMIT(uint8_t node_num, int32_t data)
{
    if(!SDO_EXE(node_num,0x7D,0x60,0x01,data,4)){; return 0;}
    //PC_SEND("SET MINIMUM POSITION LIMIT: %d\n", data);    
    return 1;
}

bool SET_MAX_POSITION_LIMIT(uint8_t node_num, int32_t data)
{
    if(!SDO_EXE(node_num,0x7D,0x60,0x02,data,4)){; return 0;}
    //PC_SEND("SET MAXIMUM POSITION LIMIT: %d\n", data);    
    return 1;   
}

bool SET_PROFILE_VELOCITY(uint8_t node_num, uint32_t data)
{
    if(!SDO_EXE(node_num,0x81,0x60,0x00,data,4)){; return 0;}
    //PC_SEND("SET PROFILE VELOCITY: %d\n", data);    
    return 1;   
}

bool SET_TARGET_POSITION(uint8_t node_num, int32_t data)
{
    if(!SDO_EXE(node_num,0x7A,0x60,0x00,data,4)){; return 0;}
    //PC_SEND("SET TARGET POSITION: %d\n", data);    
    return 1; 
}
        
bool START_ABS_P(uint8_t node_num)
{
    if(!SDO_EXE(node_num,0x40,0x60,0x00,0x001F,2)){; return 0;}
    //PC_SEND("START ABSOLUTE POSITION\r\n");
    return 1;    
}

bool SYNC_START_ABS_P(uint8_t node_num)
{
    if(!RPDO1_EXE(node_num, 0x001F)){; return 0;}
    //PC_SEND("START ABSOLUTE POSITION\r\n");
    return 1;
}

bool START_ABS_IP(uint8_t node_num)
{
    if(!SDO_EXE(node_num,0x40,0x60,0x00,0x003F,2)){; return 0;}
    //PC_SEND("START ABSOLUTE IMMIDIATE POSITION\r\n");
    return 1;    
}

bool SYNC_START_ABS_IP(uint8_t node_num)
{
    if(!RPDO1_EXE(node_num, 0x003F)){; return 0;}
    //PC_SEND("START ABSOLUTE IMMIDIATE POSITION\r\n");
    return 1;
}

bool START_RLT_P(uint8_t node_num)
{
    if(!SDO_EXE(node_num,0x40,0x60,0x00,0x005F,2)){; return 0;}
    //PC_SEND("START RELATIVE POSITION\r\n");
    return 1;
}

bool SYNC_START_RLT_P(uint8_t node_num)
{
    if(!RPDO1_EXE(node_num, 0x005F)){; return 0;}
    //PC_SEND("START RELATIVE POSITION\r\n");
    return 1;
}

bool START_RLT_IP(uint8_t node_num)
{
    if(!SDO_EXE(node_num,0x40,0x60,0x00,0x007F,2)){; return 0;}
    //PC_SEND("START RELATIVE IMMEDIATE POSITION\r\n");
    return 1;   
}

bool SYNC_START_RLT_IP(uint8_t node_num)
{
    if(!RPDO1_EXE(node_num, 0x007F)){; return 0;}
    //PC_SEND("START RELATIVE IMMEDIATE POSITION\r\n");
    return 1;
}

bool STOP_POSITIONING(uint8_t node_num)
{
    if(!SDO_EXE(node_num,0x40,0x60,0x00,0x010F,2)){; return 0;}
    //PC_SEND("STOP POSITIONING\r\n");
    return 1; 
}

bool SYNC_STOP_POSITIONING(uint8_t node_num)
{
    if(!RPDO1_EXE(node_num, 0x010F)){; return 0;}
    //PC_SEND("STOP POSITIONING\r\n");
    return 1;
}

bool QUICK_STOP_POSITIONING(uint8_t node_num)
{
    if(!SDO_EXE(node_num,0x40,0x60,0x00,0x000B,2)){; return 0;}
    //PC_SEND("QUICK STOP POSITIONING\r\n");
    return 1; 
}

bool SYNC_QUICK_STOP_POSITIONING(uint8_t node_num)
{
    if(!RPDO1_EXE(node_num, 0x000B)){; return 0;}
    //PC_SEND("QUICK STOP POSITIONING\r\n");
    return 1;
}

//*FUNCTIONS USED IN HOMING MODE
bool SET_HOMING_MODE(uint8_t node_num, HOMING_MODE mode)
{
    int8_t hmode = mode;
    switch(mode)
    {
        case 35:
        {
            //PC_SEND("    HOMING MODE = AP\r\n");
            break;
        }
        case 34:
        {
            //PC_SEND("    HOMING MODE = INS\r\n");
            break;
        }
        case 33:
        {
            //PC_SEND("    HOMING MODE = IPS\r\n");
            break;
        }
        case -1:
        {
            //PC_SEND("    HOMING MODE = CTPSI\r\n");
            break;
        }
        case -2:
        {
            //PC_SEND("    HOMING MODE = CTNSI\r\n");
            break;
        }
        case -3:
        {
            //PC_SEND("    HOMING MODE = CTPS\r\n");
            break;
        }
        case -4:
        {
            //PC_SEND("    HOMING MODE = CTNS\r\n");
            break;
        }
        default:
        {
            //PC_SEND("    ERROR: UNDEFINED HOMING MODE\r\n");
            return 0;
        }
    }
    
    if(!SDO_EXE(node_num,0x98,0x60,0x00,hmode,1)){; return 0;}
    
    return 1;
}

bool SET_HOME_OFFSET(uint8_t node_num, int32_t data)
{
    if(!SDO_EXE(node_num,0x7C,0x60,0x00,data,4)){; return 0;}
    //PC_SEND("SET HOME OFFSET: %d\n", data);    
    return 1;
}

bool SET_SPEED_FOR_ZERO_SEARCH(uint8_t node_num, uint32_t data)
{
    if(!SDO_EXE(node_num,0x99,0x60,0x02,data,4)){; return 0;}
    //PC_SEND("SET SPEED FOR ZERO SEARCH: %d\n", data);    
    return 1;
}

bool SET_HOMING_ACCELEARATION(uint8_t node_num, uint32_t data)
{
    if(!SDO_EXE(node_num,0x9A,0x60,0x00,data,4)){; return 0;}
    //PC_SEND("SET HOMING ACCELEARATION: %d\n", data);    
    return 1;
}

bool SET_HOMING_CURRENT_THRESHOLD(uint8_t node_num, uint16_t data)
{
    if(!SDO_EXE(node_num,0x80,0x20,0x00,data,2)){; return 0;}
    //PC_SEND("SET HOMING CURRENT THRESHOLD: %d\n", data);    
    return 1;
}

bool SET_HOME_POSITION(uint8_t node_num, int32_t data)
{
    if(!SDO_EXE(node_num,0x81,0x20,0x00,data,4)){; return 0;}
    //PC_SEND("SET HOME POSITION: %d\n", data);    
    return 1;
}

bool START_HOMING(uint8_t node_num)
{
    if(!SDO_EXE(node_num,0x40,0x60,0x00,0x001F,2)){; return 0;}
    //PC_SEND("START HOMING\r\n");
    return 1;   
}

bool SYNC_START_HOMING(uint8_t node_num)
{
    if(!RPDO1_EXE(node_num, 0x001F)){; return 0;}
    //PC_SEND("START HOMING\r\n");
    return 1;
}

bool STOP_HOMING(uint8_t node_num)
{
    if(!SDO_EXE(node_num,0x40,0x60,0x00,0x011F,2)){; return 0;}
    //PC_SEND("STOP HOMING\r\n");
    return 1; 
}

bool SYNC_STOP_HOMING(uint8_t node_num)
{
    if(!RPDO1_EXE(node_num, 0x011F)){; return 0;}
    //PC_SEND("STOP HOMING\r\n");
    return 1;
}

bool QUICK_STOP_HOMING(uint8_t node_num)
{
    if(!SDO_EXE(node_num,0x40,0x60,0x00,0x000B,2)){; return 0;}
    //PC_SEND("QUICK STOP HOMING\r\n");
    return 1; 
}

bool SYNC_QUICK_STOP_HOMING(uint8_t node_num)
{
    if(!RPDO1_EXE(node_num, 0x000B)){; return 0;}
    //PC_SEND("QUICK STOP HOMING\r\n");
    return 1;
}








