
 
//=============================================================================
// INA3221 Library
// by Sven Lack
//=============================================================================

#include "INA3221.h"

/**************************************************************************/
/*! 
    @brief  Instantiates a new INA3221 object
*/
/**************************************************************************/
INA3221::INA3221(PinName sda, PinName scl, uint8_t addr, float shuntresistor_1, float shuntresistor_2, float shuntresistor_3) {

    _INA3221_i2caddr = addr;
    _INA3221_shuntresistor_1 = shuntresistor_1;
    _INA3221_shuntresistor_2 = shuntresistor_2;
    _INA3221_shuntresistor_3 = shuntresistor_3;
    _i2c = new I2C(sda, scl);
    //100KHz, as specified by the datasheet.
    _i2c->frequency(100000);
}

/**************************************************************************/
/*! 
    @brief Set shunt resistor values (default 0.1 Ohm)
*/
/**************************************************************************/
void INA3221::SetShuntValues (float shuntresistor_1, float shuntresistor_2, float shuntresistor_3)
{
    _INA3221_shuntresistor_1 = shuntresistor_1;
    _INA3221_shuntresistor_2 = shuntresistor_2;
    _INA3221_shuntresistor_3 = shuntresistor_3; 
}

/**************************************************************************/
/*! 
    @brief Set the I2C Address (8 Bit) (default (0x80))
*/
/**************************************************************************/
void INA3221::SetI2CAdr (uint8_t addr)
{
    _INA3221_i2caddr = addr;
}

/**************************************************************************/
/*! 
    @brief  Write a 16 bit value over I2C to reg 
*/
/**************************************************************************/
void INA3221::WriteRegister (uint8_t reg, uint16_t value)
{
    char tx[3];
   
    tx[0]=reg;                              // Set register pointer to reg
    tx[1]=((value >> 8) & 0xFF);
    tx[2]=(value & 0xFF);
    _i2c->write(_INA3221_i2caddr,tx,3);     // Write word to pointer position 
  
}

/**************************************************************************/
/*! 
    @brief  Reads a 16 bit value over I2C
*/
/**************************************************************************/
uint16_t INA3221::ReadRegister(uint8_t reg)
{
    char tx[1];
    char rx[2];
    
    tx[0]=reg;
    _i2c->write(_INA3221_i2caddr,tx,1);     // Set register pointer to reg
    _i2c->read(_INA3221_i2caddr,rx,2);      // Read word from pointer positiom
    return (int)rx[0]<<8|(int)rx[1];
}

/**************************************************************************/
/*! 
    @brief  Get Manufacturer ID -> INA3221_REG_MANUFACTURER_ID_VALUE (0x5449)
*/
/**************************************************************************/
uint16_t INA3221::GetManufacturerID(void)
{
    return ReadRegister(INA3221_REG_MANUFACTURER_ID);
}

/**************************************************************************/
/*! 
    @brief  Get Die ID -> INA3221_REG_DIE_ID_VALUE (0x3220)
*/
/**************************************************************************/
uint16_t INA3221::GetDieID(void)
{
    return ReadRegister(INA3221_REG_DIE_ID);
}

/**************************************************************************/
/*! 
    @brief  Check connection
    Return: 1 - Connection OK 
            0 - no Connection
*/
/**************************************************************************/
uint8_t INA3221::CheckConnection(void)
{
    return ((GetManufacturerID() == INA3221_REG_MANUFACTURER_ID_VALUE) && (GetDieID() == INA3221_REG_DIE_ID_VALUE));
}

/**************************************************************************/
/*! 
    @brief  Get raw register value from shunt voltage register 
    Return: Raw Register Value 
*/
/**************************************************************************/
int16_t INA3221::GetRawShuntVoltage(uint8_t channel)
{
    switch(channel){
        case 1: return ((int16_t)ReadRegister(INA3221_REG_SHUNTVOLTAGE_1)>>3); 
        case 2: return ((int16_t)ReadRegister(INA3221_REG_SHUNTVOLTAGE_2)>>3); 
        case 3: return ((int16_t)ReadRegister(INA3221_REG_SHUNTVOLTAGE_3)>>3); 
        default: return 0;
    }
}

/**************************************************************************/
/*! 
    @brief  Get voltage from Shunt voltage register (Convert raw value to voltage) 
    Return: Voltage from Shunt voltage register 
*/
/**************************************************************************/
float INA3221::GetShuntVoltage(uint8_t channel)
{
    return ((float)GetRawShuntVoltage(channel)*INA3221_SHUNT_VOLTAGE_LSB);
}

/**************************************************************************/
/*! 
    @brief  Get current from Shunt voltage register (Convert voltage value to current) 
    Return: Current through shunt resistor
*/
/**************************************************************************/
float INA3221::GetCurrent(uint8_t channel)
{
    float voltage = GetShuntVoltage(channel);

    switch(channel){
        case 1: return (voltage / _INA3221_shuntresistor_1); 
        case 2: return (voltage / _INA3221_shuntresistor_2);
        case 3: return (voltage / _INA3221_shuntresistor_3); 
        default: return 0.0;
    }
}

/**************************************************************************/
/*! 
    @brief  Get raw register value from bus voltage register 
    Return: Raw Register Value 
*/
/**************************************************************************/
int16_t INA3221::GetRawBusVoltage(uint8_t channel)
{
    switch(channel){
        case 1: return ((int16_t)ReadRegister(INA3221_REG_BUSVOLTAGE_1)>>3);
        case 2: return ((int16_t)ReadRegister(INA3221_REG_BUSVOLTAGE_2)>>3);
        case 3: return ((int16_t)ReadRegister(INA3221_REG_BUSVOLTAGE_3)>>3);
        default: return 0;
    }
}

/**************************************************************************/
/*! 
    @brief  Get voltage from bus voltage register (Convert raw value to voltage) 
    Return: Voltage from Bus voltage register  
*/
/**************************************************************************/
float INA3221::GetBusVoltage(uint8_t channel)
{
    return ((float)GetRawBusVoltage(channel)*INA3221_BUS_VOLTAGE_LSB);
}

/**************************************************************************/
/*! 
    @brief  Get raw register value from critical alert limit register 
    Return: Raw Register Value 
*/
/**************************************************************************/
int16_t INA3221::GetRawCriticalAlertLimit(uint8_t channel)
{
    switch(channel){
        case 1: return ((int16_t)ReadRegister(INA3221_REG_CRITICAL_1)>>3);
        case 2: return ((int16_t)ReadRegister(INA3221_REG_CRITICAL_2)>>3);
        case 3: return ((int16_t)ReadRegister(INA3221_REG_CRITICAL_3)>>3);
        default: return 0;
    }
}

/**************************************************************************/
/*! 
    @brief  Get current setpoint from the critical alert limit register (Convert raw value to current) 
    Return: Current setpoint for critical alert limit
*/
/**************************************************************************/
float INA3221::GetCurrentCriticalAlertLimit(uint8_t channel)
{
    float voltage=((float)GetRawCriticalAlertLimit(channel))*INA3221_SHUNT_VOLTAGE_LSB; 
    switch(channel){
        case 1: return (voltage / _INA3221_shuntresistor_1);
        case 2: return (voltage / _INA3221_shuntresistor_2);
        case 3: return (voltage / _INA3221_shuntresistor_3);
        default: return 0;
    }
}

/**************************************************************************/
/*! 
    @brief  Set raw register value to critical alert limit register 
*/
/**************************************************************************/
void INA3221::SetRawCriticalAlertLimit(uint8_t channel, uint16_t value)
{
    value = value << 3;
    switch(channel){
        case 1: WriteRegister(INA3221_REG_CRITICAL_1,value); break;
        case 2: WriteRegister(INA3221_REG_CRITICAL_2,value); break;
        case 3: WriteRegister(INA3221_REG_CRITICAL_3,value); break;
    }
}

/**************************************************************************/
/*! 
    @brief  Set current value to critical alert register 
*/
/**************************************************************************/
void INA3221::SetCurrentCriticalAlertLimit(uint8_t channel, float current )
{
    float value;
    switch(channel){
        case 1: value = _INA3221_shuntresistor_1; break;
        case 2: value = _INA3221_shuntresistor_2; break;
        case 3: value = _INA3221_shuntresistor_3; break;
    }
    
    value = ((value*current) / (INA3221_SHUNT_VOLTAGE_LSB));
    SetRawCriticalAlertLimit(channel, (uint16_t)value);
}

/**************************************************************************/
/*! 
    @brief  Get raw register value from warning alert limit register 
    Return: Raw Register Value 
*/
/**************************************************************************/
int16_t INA3221::GetRawWarningAlertLimit(uint8_t channel)
{
    switch(channel){
        case 1: return ((int16_t)ReadRegister(INA3221_REG_WARNING_1)>>3); 
        case 2: return ((int16_t)ReadRegister(INA3221_REG_WARNING_2)>>3); 
        case 3: return ((int16_t)ReadRegister(INA3221_REG_WARNING_3)>>3); 
        default: return 0;
    }
}

/**************************************************************************/
/*! 
    @brief  Get current setpoint from the warning alert limit register (Convert raw value to current) 
    Return: Current setpoint for warning alert limit
*/
/**************************************************************************/
float INA3221::GetCurrentWarningAlertLimit(uint8_t channel)
{
    float voltage =((float)GetRawWarningAlertLimit(channel))*INA3221_SHUNT_VOLTAGE_LSB; 
    switch(channel){
        case 1: return (voltage / _INA3221_shuntresistor_1); 
        case 2: return (voltage / _INA3221_shuntresistor_2); 
        case 3: return (voltage / _INA3221_shuntresistor_3);
        default: return 0;
    }
}

/**************************************************************************/
/*! 
    @brief  Set raw register value to warning alert limit register 
*/
/**************************************************************************/
void INA3221::SetRawWarningAlertLimit(uint8_t channel, int16_t value)
{
    value = value << 3;
    switch(channel){
        case 1: WriteRegister(INA3221_REG_WARNING_1,value); break;
        case 2: WriteRegister(INA3221_REG_WARNING_2,value); break;
        case 3: WriteRegister(INA3221_REG_WARNING_3,value); break;
    }
}

/**************************************************************************/
/*! 
    @brief  Set current value to warning alert register 
*/
/**************************************************************************/
void INA3221::SetCurrentWarningAlertLimit(uint8_t channel, float current )
{
    float value;
    switch(channel){
        case 1: value = _INA3221_shuntresistor_1; break;
        case 2: value = _INA3221_shuntresistor_2; break;
        case 3: value = _INA3221_shuntresistor_3; break;
    }
    
    value = ((value*current) / (INA3221_SHUNT_VOLTAGE_LSB));
    SetRawWarningAlertLimit(channel, (int16_t)value);
}

/**************************************************************************/
/*! 
    @brief  Get raw register value from shunt voltage sum register 
    Return: Raw Register Value 
*/
/**************************************************************************/
int16_t INA3221::GetRawShuntVoltageSum(void)
{
    return ((int16_t)ReadRegister(INA3221_REG_SUM_SHUNTVOLTAGE)>>1);
}

/**************************************************************************/
/*! 
    @brief  Get voltage value from shunt voltage sum register (Convert raw value to voltage)
    Return: Voltage form shunt voltage sum register
*/
/**************************************************************************/
float INA3221::GetShuntVoltageSum(void)
{
    return ((float)GetRawShuntVoltageSum() * INA3221_SHUNT_VOLTAGE_LSB);
}

/**************************************************************************/
/*! 
    @brief  Get raw register value from shunt voltage sum limit register 
    Return: Raw Register Value 
*/
/**************************************************************************/
int16_t INA3221::GetRawShuntVoltageSumLimit(void)
{
    return ((int16_t)ReadRegister(INA3221_REG_SUM_LIMIT)>>1);
}

/**************************************************************************/
/*! 
    @brief  Get voltage value from shunt voltage sum limit register (Convert raw value to voltage)
    Return: Voltage setpoint for shunt voltage sum limit register
*/
/**************************************************************************/
float INA3221::GetShuntVoltageSumLimit(void)
{
    return ((float)GetRawShuntVoltageSumLimit() * INA3221_SHUNT_VOLTAGE_LSB);
}

/**************************************************************************/
/*! 
    @brief  Set raw register value to shunt voltage sum limit register 
*/
/**************************************************************************/
void INA3221::SetRawShuntVoltageSumLimit(uint16_t value )
{
        value = value << 1;
        WriteRegister(INA3221_REG_SUM_LIMIT,value);
}

/**************************************************************************/
/*! 
    @brief  Set voltage value to shunt voltage sum limit register 
*/
/**************************************************************************/
void INA3221::SetShuntVoltageSumLimit(float value)
{
    value = value / INA3221_SHUNT_VOLTAGE_LSB ;
    SetRawShuntVoltageSumLimit((uint16_t)value);
}

/**************************************************************************/
/*! 
    @brief  Get raw register value from power valid upper limit register 
    Return: Raw Register Value 
*/
/**************************************************************************/
int16_t INA3221::GetRawPowerValidUpperLimit(void)
{
    return ((int16_t)ReadRegister(INA3221_REG_POWERVALID_UPPER_LIMIT)>>3);
}

/**************************************************************************/
/*! 
    @brief  Get voltage value from power valid upper limit register (Convert raw value to voltage)
    Return: Voltage setpoint for power vaild upper limit register
*/
/**************************************************************************/
float INA3221::GetPowerValidUpperLimitVoltage(void)
{
    return ((float)GetRawPowerValidUpperLimit() * INA3221_BUS_VOLTAGE_LSB);
}

/**************************************************************************/
/*! 
    @brief  Set raw register value to power valid upper limit register 
*/
/**************************************************************************/
void INA3221::SetRawPowerValidUpperLimit(uint16_t value )
{
        value = value << 3;
        WriteRegister(INA3221_REG_POWERVALID_UPPER_LIMIT,value);
}

/**************************************************************************/
/*! 
    @brief  Set voltage value to power valid upper limit register 
*/
/**************************************************************************/
void INA3221::SetPowerValidUpperLimitVoltage(float value)
{
    value = value / INA3221_BUS_VOLTAGE_LSB ;
    SetRawPowerValidUpperLimit((uint16_t)value);
}

/**************************************************************************/
/*! 
    @brief  Get raw register value from power valid lower limit register 
    Return: Raw Register Value 
*/
/**************************************************************************/
int16_t INA3221::GetRawPowerValidLowerLimit(void)
{
    return ((int16_t)ReadRegister(INA3221_REG_POWERVALID_LOWER_LIMIT)>>3);
}

/**************************************************************************/
/*! 
    @brief  Get voltage value from power valid lower limit register (Convert raw value to voltage)
    Return: Voltage setpoint for power vaild lower limit register
*/
/**************************************************************************/
float INA3221::GetPowerValidLowerLimitVoltage(void)
{
    return ((float)GetRawPowerValidLowerLimit() * INA3221_BUS_VOLTAGE_LSB);
}

/**************************************************************************/
/*! 
    @brief  Set raw register value to power valid lower limit register 
*/
/**************************************************************************/
void INA3221::SetRawPowerValidLowerLimit(uint16_t value )
{
        value = value << 3;
        WriteRegister(INA3221_REG_POWERVALID_LOWER_LIMIT,value);
}

/**************************************************************************/
/*! 
    @brief  Set voltage value to power valid lower limit register 
*/
/**************************************************************************/
void INA3221::SetPowerValidLowerLimitVoltage(float value)
{
    value = value / INA3221_BUS_VOLTAGE_LSB ;
    SetRawPowerValidLowerLimit((uint16_t)value);
}

/**************************************************************************/
/*! 
    @brief  Get raw content of configuration register 
    Return: Raw Register Value 
*/
/**************************************************************************/
uint16_t INA3221::GetConfiguration(void)
{
    return (uint16_t)ReadRegister(INA3221_REG_CONFIG);
}

/**************************************************************************/
/*! 
    @brief  Set raw register value to configuration register 
*/
/**************************************************************************/
void INA3221::SetConfiguration(uint16_t value)
{
    WriteRegister(INA3221_REG_CONFIG , value);
}

/**************************************************************************/
/*! 
    @brief  Set mode bits in configuration register to: 
    INA3221_MODE_POWER_DOWN                 
    INA3221_MODE_SHUNT_SINGLE               
    INA3221_MODE_BUS_SINGLE                 
    INA3221_MODE_SHUNT_BUS_SINGLE           
    INA3221_MODE_SHUNT_CONTINUOUS           
    INA3221_MODE_BUS_CONTINUOUS             
    INA3221_MODE_SHUNT_BUS_CONTINUOUS        
*/
/**************************************************************************/
void INA3221::SetMode(uint16_t mode)
{   
    uint16_t mode_reg = GetConfiguration();
    mode_reg &= 0xFFF8;
    mode_reg |= mode;
    
    SetConfiguration(mode_reg);
}

/**************************************************************************/
/*! 
    @brief  Set conversion time bits for shunt voltage measurement in configuration register to: 
    INA3221_140_US                      
    INA3221_204_US                      
    INA3221_332_US                        
    INA3221_588_US                         
    INA3221_1_1_MS                         
    INA3221_2_116_MS                        
    INA3221_4_156_MS                        
    INA3221_8_244_MS      
*/
/**************************************************************************/
void INA3221::SetShuntConversionTime(uint16_t shunt_conversion_time)
{   
    uint16_t mode_reg = GetConfiguration();
    mode_reg &= 0xFFC7;
    mode_reg |= shunt_conversion_time<<3;
    
    SetConfiguration(mode_reg);
}

/**************************************************************************/
/*! 
    @brief  Set conversion time bits for bus voltage measurement in configuration register to: 
    INA3221_140_US                      
    INA3221_204_US                      
    INA3221_332_US                        
    INA3221_588_US                         
    INA3221_1_1_MS                         
    INA3221_2_116_MS                        
    INA3221_4_156_MS                        
    INA3221_8_244_MS      
*/
/**************************************************************************/
void INA3221::SetBusConversionTime(uint16_t bus_conversion_time)
{   
    uint16_t mode_reg = GetConfiguration();
    mode_reg &= 0xFE3F;
    mode_reg |= bus_conversion_time<<6;
    
    SetConfiguration(mode_reg);
}

/**************************************************************************/
/*! 
    @brief  Set averaging mode in configuration register to:
    INA3221_AVERAGE_1                     
    INA3221_AVERAGE_4                       
    INA3221_AVERAGE_16                      
    INA3221_AVERAGE_64                      
    INA3221_AVERAGE_128                     
    INA3221_AVERAGE_256                     
    INA3221_AVERAGE_512                     
    INA3221_AVERAGE_1024                   
*/
/**************************************************************************/
void INA3221::SetAveragingMode(uint16_t averaging_mode)
{   
    uint16_t mode_reg = GetConfiguration();
    mode_reg &= 0xF1FF;
    mode_reg |= (averaging_mode<<9);
    
    SetConfiguration(mode_reg);
}

/**************************************************************************/
/*! 
    @brief  Enable measurement for single channel
    INA3221_CONFIG_CH1            
    INA3221_CONFIG_CH2                   
    INA3221_CONFIG_CH3 
*/
/**************************************************************************/
void INA3221::EnableChannel(uint16_t channel)
{   
    uint16_t mode_reg = GetConfiguration();
    switch(channel){
        case 1: mode_reg |= INA3221_CONFIG_CH1; break;
        case 2: mode_reg |= INA3221_CONFIG_CH2; break;
        case 3: mode_reg |= INA3221_CONFIG_CH3; break;
    }
    
    SetConfiguration(mode_reg);
}

/**************************************************************************/
/*! 
    @brief  Disable measurement for single channel
    INA3221_CONFIG_CH1            
    INA3221_CONFIG_CH2                   
    INA3221_CONFIG_CH3 
*/
/**************************************************************************/
void INA3221::DisableChannel(uint16_t channel)
{   
    uint16_t mode_reg = GetConfiguration();
    switch(channel){
        case 1: mode_reg &= (~INA3221_CONFIG_CH1); break;
        case 2: mode_reg &= (~INA3221_CONFIG_CH2); break;
        case 3: mode_reg &= (~INA3221_CONFIG_CH3); break;
    }
    
    SetConfiguration(mode_reg);
}

/**************************************************************************/
/*! 
    @brief  Generate a system reset that is the same as a power-on reset (POR).
            This bit resets all registers to default values and self-clears.
*/
/**************************************************************************/
void INA3221::Rest(void)
{   
    uint16_t mode_reg = GetConfiguration();
    mode_reg &= 0xEFFF;
    mode_reg |= INA3221_CONFIG_RST;
    
    SetConfiguration(mode_reg);
}

/**************************************************************************/
/*! 
    @brief  Get raw register value from mask register 
    Return: Raw Register Value 
*/
/**************************************************************************/
uint16_t INA3221::GetMask(void)
{
    return (uint16_t)ReadRegister(INA3221_REG_MASK);
}

/**************************************************************************/
/*! 
    @brief  Set raw register value to mask register 
*/
/**************************************************************************/
void INA3221::SetMask(uint16_t value)
{
    WriteRegister (INA3221_REG_MASK , value);
}

/**************************************************************************/
/*! 
    @brief  Check if conversion has finished  
    Return: 1 after all conversions are complete else 0
*/
/**************************************************************************/
uint8_t INA3221::ConversionReady(void)
{
    uint16_t mask = GetMask();
    mask &= INA3221_REG_MASK_CVRF;
     
    return  (uint8_t)mask;
}

/**************************************************************************/
/*! 
    @brief  Get Timing-control-alert flag indicator 
*/
/**************************************************************************/
uint8_t INA3221::GetTimingAlertFlag(void)
{
    uint16_t mask = GetMask();
    mask &= INA3221_REG_MASK_TCF;
    mask = mask >>1;
     
    return  (uint8_t)mask;
}

/**************************************************************************/
/*! 
    @brief  Get Power-valid-alert flag indicator
*/
/**************************************************************************/
uint8_t INA3221::GetPowerVaildAlertFlag(void)
{
    uint16_t mask = GetMask();
    mask &= INA3221_REG_MASK_PVF;
    mask = mask >>2;
     
    return  (uint8_t)mask;
}

/**************************************************************************/
/*! 
    @brief  Get Warning-alert flag indicator for one channel
*/
/**************************************************************************/
uint8_t INA3221::GetWarningAlertFlag(uint8_t channel)
{
    uint16_t mask = GetMask();
    
    switch(channel){
        case 1: mask &= INA3221_REG_MASK_WF_1; mask= mask >> 5; break;
        case 2: mask &= INA3221_REG_MASK_WF_2; mask= mask >> 4; break;
        case 3: mask &= INA3221_REG_MASK_WF_3; mask= mask >> 3; break;
    }
     
    return  (uint8_t)mask;
}

/**************************************************************************/
/*! 
    @brief  Get Summation-alert flag indicator 
*/
/**************************************************************************/
uint8_t INA3221::GetSummationAlertFlag(void)
{
    uint16_t mask = GetMask();
    mask &= INA3221_REG_MASK_SF ;
    mask = mask >>6;
     
    return  (uint8_t)mask;
}

/**************************************************************************/
/*! 
    @brief  Get Critical-alert flag indicator for one channel
*/
/**************************************************************************/
uint8_t INA3221::GetCriticalAlertFlag(uint8_t channel)
{
    uint16_t mask = GetMask();
    
    switch(channel){
        case 1: mask &= INA3221_REG_MASK_CF_1; mask= mask >> 9; break;
        case 2: mask &= INA3221_REG_MASK_CF_2; mask= mask >> 8; break;
        case 3: mask &= INA3221_REG_MASK_CF_3; mask= mask >> 7; break;
    }
     
    return  (uint8_t)mask;
}

/**************************************************************************/
/*! 
    @brief  Get Critical alert latch enable bit 
    Return: 0 = Transparent (default)
            1 = Latch enabled
*/
/**************************************************************************/
uint8_t INA3221::GetCriticalAlertLachEnable(void)
{
    uint16_t mask = GetMask();
    mask &= INA3221_REG_MASK_CEN ;
    mask = mask >>10;
     
    return  (uint8_t)mask;
}

/**************************************************************************/
/*! 
    @brief  Set Critical alert latch enable bit 
*/
/**************************************************************************/
void INA3221::SetCriticalAlertLachEnable(uint8_t value)
{
    uint16_t mask = GetMask();
    mask &= (~INA3221_REG_MASK_CEN);
    mask |= (value << 10);
         
    SetMask(mask);
}

/**************************************************************************/
/*! 
    @brief  Get warning alert latch enable bit 
    Return: 0 = Transparent (default)
            1 = Latch enabled
*/
/**************************************************************************/
uint8_t INA3221::GetWarningAlertLachEnable(void)
{
    uint16_t mask = GetMask();
    mask &= INA3221_REG_MASK_WEN ;
    mask = mask >>11;
     
    return  (uint8_t)mask;
}

/**************************************************************************/
/*! 
    @brief  Set warning alert latch enable bit 
*/
/**************************************************************************/
void INA3221::SetWarningAlertLachEnable(uint8_t value)
{
    uint16_t mask = GetMask();
    mask &= (~INA3221_REG_MASK_WEN);
    mask |= (value << 11);
         
    SetMask(mask);
}

/**************************************************************************/
/*! 
    @brief  Enable channel for summation 
*/
/**************************************************************************/
void INA3221::EnableChannelSummation(uint16_t channel)
{   
    uint16_t mask = GetMask();
    switch(channel){
        case 1: mask |= INA3221_REG_MASK_SCC_1; break;
        case 2: mask |= INA3221_REG_MASK_SCC_2; break;
        case 3: mask |= INA3221_REG_MASK_SCC_3; break;
    }
    
    SetMask(mask);
}

/**************************************************************************/
/*! 
    @brief  Disable channel for summation 
 
*/
/**************************************************************************/
void INA3221::DisableChannelSummation(uint16_t channel)
{   
    uint16_t mask = GetMask();
    switch(channel){
        case 1: mask &= (~INA3221_REG_MASK_SCC_1 ); break;
        case 2: mask &= (~INA3221_REG_MASK_SCC_2 ); break;
        case 3: mask &= (~INA3221_REG_MASK_SCC_3); break;
    }
    
    SetMask(mask);
}
