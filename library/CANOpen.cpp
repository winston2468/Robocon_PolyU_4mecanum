#include "robot.h"

//GATE 0:ADMIT TO TRANSMIT NEXT CAN MESSAGE;  1:CAN NOT
volatile char GATES[9] = {0};


//**PIN DEFINATION
//*DEFINE CANOpen Rd/Td PINS
CAN             can1(PB_5, PB_6);


//**DEFINE COMMAND INDEX 
//*DEFINE NMT COMMAND
char NMT_CMD[10] = {0x02,0x00,0x80,0x00,0x82,0x00,0x81,0x00,0x01,0x00};
char NMT_ALL_STOP[2]              = {0x02,0x00};
char NMT_ALL_ENTER_PREOP[2]       = {0x80,0x00};
char NMT_ALL_RESET_COMMU[2]       = {0x82,0x00};
char NMT_ALL_RESET_NODE[2]        = {0x81,0x00};
char NMT_ALL_ENTER_OP[2]          = {0x01,0x00};  

//*DEFINE SDO FREQUENTLY USED COMMAND                                    **********WARNING: MAY NOT USABLE IN OPERATION-MODE-SPECIFIC!!!**********
//WRITE C->S : (1byte data)0x2F  (2byte data)0x2B  (4byte data)0x23
char SDO_CW_CMD[10] = {0x06,0x00,0x0F,0x00,0x00,0x00,0x02,0x00,0xFF,0x00};
char SDO_CW_SHUT_DOWN[8]               = {0x2B,0x40,0x60,0x00,0x06,0x00,0x00,0x00};
char SDO_CW_SWITCH_ON_ENABLE_OP[8]     = {0x2B,0x40,0x60,0x00,0x0F,0x00,0x00,0x00}; 
char SDO_CW_DISABLE_OP[8]              = {0x2B,0x40,0x60,0x00,0x00,0x00,0x00,0x00}; 
char SDO_CW_QUICK_STOP[8]              = {0x2B,0x40,0x60,0x00,0x02,0x00,0x00,0x00}; 
char SDO_CW_CLEAR_FAULT[8]             = {0x2B,0x40,0x60,0x00,0xFF,0x00,0x00,0x00}; 

char SDO_INIT_RPDO_COBID[8]            = {0x23,0x00,0x14,0x01,0x01,0x02,0x00,0x00}; 
char SDO_INIT_RPDO_TRANSTYPE[8]        = {0x2F,0x00,0x14,0x02,0xFF,0x00,0x00,0x00};


//**DEFINE CANOpen DATA FIELD ARRAY
char SYNC_DATA[1]           = {0x00};
char CAN_DATA[4]            = {0x00};
char SDO_DATA[8]            = {0x00};
char RPDO_DATA[8]           = {0x00};
CANMessage CAN_RECEIVE_BUFFER;


//**DEFINE CANOpen COMMUNICATION FUNCTIONS
//*SET CANOpen FREQUENCY
void SET_CANOpen_FREQ(int freq)
{
    can1.frequency(freq);   
}

//*SET NMT STATE FUNCTION
bool SET_NMT(NMT_COMMAND cmd)
{
    //while((GATES[1]+GATES[2]+GATES[3]+GATES[4]+GATES[5]+GATES[6]+GATES[7]+GATES[8]) != 0);
    uint8_t para = cmd;
    switch(para)
    {
        case 1:
        {
            if(can1.write(CANMessage(0x000, NMT_ALL_ENTER_PREOP, 2)))
            {
                //PC_SEND("NMT_ALL SET TO PRE-OP\r\n");
               // LED_1_TOGGLE();
                wait(0.01);
            }
            else
            {
                //PC_SEND("NMT ERROR : SET TO PRE-OP\r\n");
                return 0;
            }           
            break;
        }
        case 2:
        {
            if(can1.write(CANMessage(0x000, NMT_ALL_RESET_COMMU, 2)))
            {
                //PC_SEND("NMT_ALL RESET COMMUNICATION\r\n");
              //  LED_1_TOGGLE();
                wait(1);
            }
            else
            {
                //PC_SEND("NMT ERROR : ALL RESET COMMUNICATION\r\n");
                return 0;
            }           
            break;
        }
        case 3:
        {
            if(can1.write(CANMessage(0x000, NMT_ALL_RESET_NODE, 2)))
            {
                //PC_SEND("NMT_ALL RESET NODE\r\n");
             //   LED_1_TOGGLE();
                wait(1);
            }
            else
            {
                //PC_SEND("NMT ERROR : ALL RESET NODE\r\n");
                return 0;
            }           
            break;
        }
        case 4:
        {
            if(can1.write(CANMessage(0x000, NMT_ALL_ENTER_OP, 2)))
            {
                //PC_SEND("NMT_ALL SET TO OP\r\n");
              //  LED_1_TOGGLE();
                wait(0.01);
            }
            else
            {
                //PC_SEND("NMT ERROR : ALL SET TO OP\r\n");
                return 0;
            }           
            break;
        }
        default:
        {
            if(can1.write(CANMessage(0x000, NMT_ALL_STOP, 2)))
            {
                //PC_SEND("NMT_ALL STOP\r\n");
           //     LED_1_TOGGLE();
                wait(0.01);
            }
            else
            {
                //PC_SEND("NMT ERROR : ALL STOP\r\n");
                return 0;
            }           
            break;
        }
     }
     return 1;
}

//*DATA FORMATTING FUNCTION
void DATA2ARRAY(uint32_t data)
{
    for(char i = 0; i<4; i++)
    {
       CAN_DATA[i] = (data & (0b11111111<<(i*8))) / (1<<(i*8));
    }
}
//OVERLOADING OF PREVIOUS FUNCTION FOR INT32 DATA
void DATA2ARRAY(int32_t data)
{
    for(char i = 0; i<4; i++)
    {
       CAN_DATA[i] = (data & (0b11111111<<(i*8))) / (1<<(i*8));
    }
}

//*SEND SYNC FRAME
void SYNC_SEND()
{
    while(!can1.write(CANMessage(0x080, SYNC_DATA, 1)));//{PC_SEND("SYNC FAIL\r\n");}
}


    
//*SDO SEND FUNCTION
bool SDO_SEND(uint8_t node_num, const char* msg)
{
    int cob_id = 0x600 + node_num;
    //while(GATES[node_num]!= 0);   //WAIT UNTIL TRANSMISSION GATE OPEN
    while(!can1.write(CANMessage(cob_id, msg)));
    SYNC_SEND();
    return 1;
}

//*MORE USER FRIENDLY SDO SEND FUNCTION TO EXECUTE SPECIFIC COMMAND
bool SDO_EXE(uint8_t node_num, char index_lower, char index_higher, char subindex, uint32_t data, char valid_data_length_in_byte)
{
    SDO_DATA[0] = 0x20 + 19 - 4*valid_data_length_in_byte;
    SDO_DATA[1] = index_lower;
    SDO_DATA[2] = index_higher;
    SDO_DATA[3] = subindex;
    DATA2ARRAY(data);
    for(char i = 0; i<4; i++)
    {
        SDO_DATA[i+4] = CAN_DATA[i];
    }
    
    if(!SDO_SEND(node_num, SDO_DATA)){return 0;}
    
    return 1;
}
//OVERLOADING OF PREVIOUS FUNCTION FOR INT32 DATA
bool SDO_EXE(uint8_t node_num, char index_lower, char index_higher, char subindex, int32_t data, char valid_data_length_in_byte)
{
    SDO_DATA[0] = 0x20 + 19 - 4*valid_data_length_in_byte;
    SDO_DATA[1] = index_lower;
    SDO_DATA[2] = index_higher;
    SDO_DATA[3] = subindex;
    DATA2ARRAY(data);
    for(char i = 0; i<4; i++)
    {
        SDO_DATA[i+4] = CAN_DATA[i];
    }
    
    if(!SDO_SEND(node_num, SDO_DATA)){return 0;}
    
    return 1;
}

//*SDO SEND CONTROL-WORD                                    **********WARNING: MAY NOT USABLE IN OPERATION-MODE-SPECIFIC!!!**********
bool SDO_CW(uint8_t node_num, CW_COMMAND cmd)
{
    uint8_t para = cmd;
    switch(para)
    {
        case 0:
        {
            if(!SDO_SEND(node_num, SDO_CW_SHUT_DOWN)){return 0;}
            //PC_SEND("    SHUT_DOWN\r\n");
            break;
        }
        case 1:
        {
            if(!SDO_SEND(node_num, SDO_CW_SWITCH_ON_ENABLE_OP)){return 0;}
            //PC_SEND("    SWITCH ON & ENABLE OP\r\n");
            break;
        }
        case 2:
        {
            if(!SDO_SEND(node_num, SDO_CW_DISABLE_OP)){return 0;}
            //PC_SEND("    DISABLE OP\r\n");
            break;
        }
        case 3:
        {
            if(!SDO_SEND(node_num, SDO_CW_QUICK_STOP)){return 0;}
            //PC_SEND("    QUICK STOP\r\n");
            break;
        }
        case 4:
        {
            if(!SDO_SEND(node_num, SDO_CW_CLEAR_FAULT)){return 0;}
            //PC_SEND("    CLEAR_FAULT\r\n");
            break;
        }
        default:
        {
            if(!SDO_SEND(node_num, SDO_CW_DISABLE_OP)){return 0;}
            //PC_SEND("    DISABLE OP\r\n");
            break;
        }
     }
     return 1;
}

//*CONFIG A SINGLE RPDO PARAMETER
bool RPDO_CONFIG(uint8_t node_num, PDO_SEQUENCE seq, PDO_TRANSTYPE type)
{
    uint8_t sequence = seq;
    uint8_t pdotype = type;
    //STEP1:CONFIG COB-ID
    SDO_INIT_RPDO_COBID[1] = sequence;
    SDO_INIT_RPDO_COBID[4] = node_num;
    SDO_INIT_RPDO_COBID[5] = sequence + 2;
    if (!SDO_SEND(node_num, SDO_INIT_RPDO_COBID)){return 0;}
    //STEP2:SET TRANSMISSION TYPE
    SDO_INIT_RPDO_TRANSTYPE[1] = sequence;
    SDO_INIT_RPDO_TRANSTYPE[4] = pdotype;
    if (!SDO_SEND(node_num, SDO_INIT_RPDO_TRANSTYPE)){return 0;}
    //STEP3-5:PDO MAPPING, DONE ON EPOS STUDIO
              //STEP3:CLEAR "Number of Mapped Application Objects"
              //STEP4:MAPPING OBJECTS (HERE WE USE DEFAULT VALUE)
              //STEP5:RESET "Number of Mapped Application Objects" (HERE WE USE DEFAULT VALUE)
    //STEP6:ACTIVATE CHANGES (AUTOMATICALLY)
    
    return 1;
}

//bool TPDO_CONFIG(uint8_t node_num, PDO_TRANSTYPE type)

//*PDO SEND FUNCTION
bool PDO_SEND(uint8_t node_num, PDO_SEQUENCE seq, const char* msg, char length)
{
    uint8_t sequence = seq;
    int cob_id = sequence*256 + 512 + node_num;
    while(!can1.write(CANMessage(cob_id, msg, length)));

    return 1;

}





//#######################################################################
//#                      USER DEFINED FUNCTIONS                         #
//#######################################################################


//*SET UP CANOpen RATE, SYNC FREQUENCY AND OTHER SETTINGS
void CANOpen_SETUP(int freq)
{
    SET_CANOpen_FREQ(freq);
    //START_SYNC(sync_interval);
    can1.attach(&CAN_RECEIVE_INTERRUPT);
}

//*PDO INITIALIZE FUNCTION, MODIFIED BY USER
bool PDO_INIT()
{
    //SET ALL AXIS TO PRE-OP STATE
    if (!SET_NMT(SET_PREOP)){return 0;}
    wait(0.1);

    //MODIFIED BY USER, DEFINE PDO INITIALIZATION BEHAVIOUR
    //PC_SEND("-----NODE1 PDO INITIALIZING-----\r\n");
    if (!RPDO_CONFIG(1, RPDO1, SYNC)){return 0;}
    //PC_SEND("-----NODE2 PDO INITIALIZING-----\r\n");
    if (!RPDO_CONFIG(2, RPDO1, SYNC)){return 0;}
    //PC_SEND("-----NODE3 PDO INITIALIZING-----\r\n");
    if (!RPDO_CONFIG(3, RPDO1, SYNC)){return 0;}
    //MAY EXIST OTHER NODES
    
    //RESET COMMUNICATION
    if (!SET_NMT(RESET_COMMU)){return 0;}
    wait(2); 
    
    return 1;
}

//*RPDO FUNCTION, MODIFIED BY USER ACCORDING TO PDO MAPPING
bool RPDO1_EXE(uint8_t node_num, CW_COMMAND cmd)
{
    char command = cmd;
    int length = 0;
    
    //DATA1:CONTROL WORD
    for(;length<2;length++)
    {    
         RPDO_DATA[length] = SDO_CW_CMD[length+command*2];
    }
    
    if(!PDO_SEND(node_num, RPDO1, RPDO_DATA, length)){//PC_SEND("     RPDO1 MSG FAIL\r\n"); return 0;
    }
           
    //PC_SEND("     RPDO1 EXECUTE\r\n");
    
    return 1;
}

bool RPDO1_EXE(uint8_t node_num, uint16_t cwcommand)
{
    int length = 0;
    DATA2ARRAY(cwcommand);
    //DATA1:CONTROL WORD
    for(;length<2;length++)
    {    
         RPDO_DATA[length] = CAN_DATA[length];
    }
    if(!PDO_SEND(node_num, RPDO1, RPDO_DATA, length)){//PC_SEND("     RPDO1 MSG FAIL\r\n"); return 0;
    }
           
    //PC_SEND("     RPDO1 EXECUTE\r\n");
    
    return 1;
}



void CAN_RECEIVE_INTERRUPT()
{
    can1.read(CAN_RECEIVE_BUFFER);
    if(CAN_RECEIVE_BUFFER.id>0x580 && CAN_RECEIVE_BUFFER.id<=0x588){GATES[(CAN_RECEIVE_BUFFER.id-0x580)] = 0;}   //OPEN TRANSMISSION GATE
    
}

/////************VESC*************/////
/*
bool VESC::sendPacket(uint32_t id, uint8_t packet[], int32_t len)
{
   if (can1.write(CANMessage(id, (const char*)packet, sizeof(packet),CANData,CANExtended)))
   {
      return true;
   }
   else {
      return false;
   }
}

void VESC::crossbow(uint8_t controller_id, float pos)
{
  int32_t send_index = 0;
  uint8_t buffer[4];
  buffer_append_int32(buffer, (int32_t)(pos * 1e6), &send_index);
  sendPacket(controller_id | ((uint32_t)14 << 8), buffer, send_index);   //crossbow=14
}

void VESC::buffer_append_int32(uint8_t* buffer, int32_t number, int32_t *index)
{
   buffer[(*index)++] = number >> 24;
   buffer[(*index)++] = number >> 16;
   buffer[(*index)++] = number >> 8;
   buffer[(*index)++] = number;
}
*/







