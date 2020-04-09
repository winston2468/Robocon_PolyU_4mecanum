/*
README
*/

//**DEFINE MOTION FUNCTIONS
//*SET OP-MODE
bool SET_OP_MODE(uint8_t node_num, OP_MODE mode);

//*FUNCTIONS USED IN P-VELOCITY MODE
bool SET_MAX_P_VELOCITY(uint8_t node_num, uint32_t data);
bool SET_P_ACCELERATION(uint8_t node_num, uint32_t data);
bool SET_P_DECELERATION(uint8_t node_num, uint32_t data);
bool SET_QUICK_STOP_DECELERATION(uint8_t node_num, uint32_t data);
bool SET_MOTION_PROFILE_TYPE(uint8_t node_num, MOTION_PROFILE_TYPE type);
bool SET_TARGET_VELOCITY(uint8_t node_num, int32_t data);
bool STOP_P_VELOCITY(uint8_t node_num);
bool SYNC_STOP_P_VELOCITY(uint8_t node_num);
bool QUICK_STOP_P_VELOCITY(uint8_t node_num);
bool SYNC_QUICK_STOP_P_VELOCITY(uint8_t node_num);

//*FUNCTIONS USED IN CURRENT MODE
bool SET_CONTINOUS_CURRENT_LIMIT(uint8_t node_num, uint16_t data);
bool SET_MAX_CURRENT_MODE_SPEED(uint8_t node_num, uint32_t data);
bool SET_THERMAL_TIME_CONSTANT_WINDING(uint8_t node_num, uint16_t data);
bool SET_CURRENT_VALUE(uint8_t node_num, int16_t data);
bool STOP_CURRENT_VELOCITY(uint8_t node_num);
bool QUICK_STOP_CURRENT_VELOCITY(uint8_t node_num);
bool SYNC_QUICK_STOP_CURRENT_VELOCITY(uint8_t node_num);

//*FUNCTIONS USED IN POSITION MODE
bool SET_MAX_FOLLOWING_ERROR(uint8_t node_num, uint32_t data);
bool SET_MIN_POSITION_LIMIT(uint8_t node_num, int32_t data);
bool SET_MAX_POSITION_LIMIT(uint8_t node_num, int32_t data);
//ALREADY HAVE: bool SET_MAX_P_VELOCITY(uint8_t node_num, uint32_t data);
bool SET_PROFILE_VELOCITY(uint8_t node_num, uint32_t data);
//ALREADY HAVE: bool SET_P_ACCELERATION(uint8_t node_num, uint32_t data);
//ALREADY HAVE: bool SET_P_DECELERATION(uint8_t node_num, uint32_t data);
//ALREADY HAVE: bool SET_QUICK_STOP_DECELERATION(uint8_t node_num, uint32_t data);
//ALREADY HAVE: bool SET_MOTION_PROFILE_TYPE(uint8_t node_num, MOTION_PROFILE_TYPE type);
bool SET_TARGET_POSITION(uint8_t node_num, int32_t data);
bool START_ABS_P(uint8_t node_num);
bool SYNC_START_ABS_P(uint8_t node_num);
bool START_ABS_IP(uint8_t node_num);
bool SYNC_START_ABS_IP(uint8_t node_num);
bool START_RLT_P(uint8_t node_num);
bool SYNC_START_RLT_P(uint8_t node_num);
bool START_RLT_IP(uint8_t node_num);
bool SYNC_START_RLT_IP(uint8_t node_num);
bool STOP_POSITIONING(uint8_t node_num);
bool SYNC_STOP_POSITIONING(uint8_t node_num);
bool QUICK_STOP_POSITIONING(uint8_t node_num);
bool SYNC_QUICK_STOP_POSITIONING(uint8_t node_num);

//*FUNCTIONS USED IN HOMING MODE
bool SET_HOMING_MODE(uint8_t node_num, HOMING_MODE mode);
//ALREADY HAVE: bool SET_MAX_FOLLOWING_ERROR(uint8_t node_num, uint32_t data);
bool SET_HOME_OFFSET(uint8_t node_num, int32_t data);
//ALREADY HAVE: bool SET_MAX_P_VELOCITY(uint8_t node_num, uint32_t data);
//ALREADY HAVE: bool SET_QUICK_STOP_DECELERATION(uint8_t node_num, uint32_t data);
//NOT AVALIABLE NOW: SET SPEED FOR SWITCH SEARCH
bool SET_SPEED_FOR_ZERO_SEARCH(uint8_t node_num, uint32_t data);
bool SET_HOMING_ACCELEARATION(uint8_t node_num, uint32_t data);
bool SET_HOMING_CURRENT_THRESHOLD(uint8_t node_num, uint16_t data);
bool SET_HOME_POSITION(uint8_t node_num, int32_t data);
bool START_HOMING(uint8_t node_num);
bool SYNC_START_HOMING(uint8_t node_num);
bool STOP_HOMING(uint8_t node_num);
bool SYNC_STOP_HOMING(uint8_t node_num);
bool QUICK_STOP_HOMING(uint8_t node_num);
bool SYNC_QUICK_STOP_HOMING(uint8_t node_num);

















