/*
----------------------------------------------------------                                                                                                                                                  /* 
I2Steve note:
the design purpose ina3221 seems  to be measure the voltage output of POWER SUPPLY
However, it is a cost-effective non-chip level solution for ADC(more than 5V)
YET, due to the design of the chip only 1 channel can be use for one chip(if u plan to use it as ADC) 
*/
 
//=============================================================================
// INA3221 Library
// by Sven Lack
// copy by on9 steve
//=============================================================================


#ifndef INA3221_H
#define INA3221_H

/**
I2Steve onn99 EIE time:bus voltage vs shunt voltage
The bus voltage is the total voltage between power and GND. It is the sum of the load voltage and the shunt voltage.
The load voltage is the voltage going to the load.
The shunt voltage is the voltage drop across the shunt resistor that is in series with the load.. That is how the sensor measures the current.
 */
#include "mbed.h"

    #define INA3221_SHUNT_VOLTAGE_LSB               0.00004f   // 40 uV per LSB
    #define INA3221_BUS_VOLTAGE_LSB                 0.008f     // 8 mV per LSB
/*=========================================================================
    I2C ADDRESS/BITS
-----------------------------------------------------------------------*/
    #define INA3221_ADDRESS                         (0x80)    // 1000000 
/*=========================================================================*/

/*=========================================================================
    Configuration Register (RW) - See table 5 spec
---------------------------------------------------------------------*/
    #define INA3221_CONFIG_RST                      (0x8000)  // Reset Bit
    
    #define INA3221_CONFIG_CH1                      (0x4000)  // Enable Channel 1
    #define INA3221_CONFIG_CH2                      (0x2000)  // Enable Channel 2
    #define INA3221_CONFIG_CH3                      (0x1000)  // Enable Channel 3
    
    #define INA3221_CONFIG_AVG2                     (0x0800)  // Averaging mode Bit 2 
    #define INA3221_CONFIG_AVG1                     (0x0400)  // Averaging mode Bit 1 
    #define INA3221_CONFIG_AVG0                     (0x0200)  // Averaging mode Bit 0 

    #define INA3221_CONFIG_VBUS_CT2                 (0x0100)  // Bus-voltage conversion time bit 0
    #define INA3221_CONFIG_VBUS_CT1                 (0x0080)  // Bus-voltage conversion time bit 0
    #define INA3221_CONFIG_VBUS_CT0                 (0x0040)  // Bus-voltage conversion time bit 0

    #define INA3221_CONFIG_VSH_CT2                  (0x0020)  // Shunt-voltage conversion time bit 2
    #define INA3221_CONFIG_VSH_CT1                  (0x0010)  // Shunt-voltage conversion time bit 1
    #define INA3221_CONFIG_VSH_CT0                  (0x0008)  // Shunt-voltage conversion time bit 0
    
    #define INA3221_CONFIG_MODE_3                   (0x0004)  // Operating Mode bit 2
    #define INA3221_CONFIG_MODE_2                   (0x0002)  // Operating Mode bit 1
    #define INA3221_CONFIG_MODE_1                   (0x0001)  // Operating Mode bit 0
    
    #define INA3221_MODE_POWER_DOWN                 (0x0000)  // Power-down
    #define INA3221_MODE_SHUNT_SINGLE               (0x0001)  // Shunt voltage, single-shot (triggered)
    #define INA3221_MODE_BUS_SINGLE                 (0x0002)  // Bus voltage, single-shot (triggered)
    #define INA3221_MODE_SHUNT_BUS_SINGLE           (0x0003)  // Shunt and bus, single-shot (triggered)
    #define INA3221_MODE_SHUNT_CONTINUOUS           (0x0005)  // Shunt voltage, continuous
    #define INA3221_MODE_BUS_CONTINUOUS             (0x0006)  // Bus voltage, continuous
    #define INA3221_MODE_SHUNT_BUS_CONTINUOUS       (0x0007)  // Shunt and bus, continuous
    
    #define INA3221_140_US                          (0x0000)  // Conversion time 140 us
    #define INA3221_204_US                          (0x0001)  // Conversion time 204 us
    #define INA3221_332_US                          (0x0002)  // Conversion time 332 us
    #define INA3221_588_US                          (0x0003)  // Conversion time 588 us
    #define INA3221_1_1_MS                          (0x0004)  // Conversion time 1.1 ms
    #define INA3221_2_116_MS                        (0x0005)  // Conversion time 2.116 ms
    #define INA3221_4_156_MS                        (0x0006)  // Conversion time 4.156 ms
    #define INA3221_8_244_MS                        (0x0007)  // Conversion time 8.244 ms
    
    #define INA3221_AVERAGE_1                       (0x0000)  // Averaging mode 1
    #define INA3221_AVERAGE_4                       (0x0001)  // Averaging mode 4
    #define INA3221_AVERAGE_16                      (0x0002)  // Averaging mode 16
    #define INA3221_AVERAGE_64                      (0x0003)  // Averaging mode 64
    #define INA3221_AVERAGE_128                     (0x0004)  // Averaging mode 128
    #define INA3221_AVERAGE_256                     (0x0005)  // Averaging mode 256
    #define INA3221_AVERAGE_512                     (0x0006)  // Averaging mode 512
    #define INA3221_AVERAGE_1024                    (0x0007)  // Averaging mode 1024
/*=========================================================================    
    Mask/ Enable Register (RW) - See table 20 spec
---------------------------------------------------------------------*/

    #define INA3221_REG_MASK_SCC_1                  (0x4000)  // Enable Channel 1 for Summation
    #define INA3221_REG_MASK_SCC_2                  (0x2000)  // Enable Channel 2 for Summation
    #define INA3221_REG_MASK_SCC_3                  (0x1000)  // Enable Channel 3 for Summation
    
    #define INA3221_REG_MASK_WEN                    (0x0800)  // Warning-alert latch enable
    #define INA3221_REG_MASK_CEN                    (0x0400)  // Critical-alert latch enable
    
    #define INA3221_REG_MASK_CF_1                   (0x0200)  // Critical-alert flag indicator for Channel 1
    #define INA3221_REG_MASK_CF_2                   (0x0100)  // Critical-alert flag indicator for Channel 2
    #define INA3221_REG_MASK_CF_3                   (0x0080)  // Critical-alert flag indicator for Channel 3
    
    #define INA3221_REG_MASK_SF                     (0x0040)  // Summation-alert flag indicator

    #define INA3221_REG_MASK_WF_1                   (0x0020)  // Warning-alert flag indicator for Channel 1
    #define INA3221_REG_MASK_WF_2                   (0x0010)  // Warning-alert flag indicator for Channel 2
    #define INA3221_REG_MASK_WF_3                   (0x0008)  // Warning-alert flag indicator for Channel 3

    #define INA3221_REG_MASK_PVF                    (0x0004)  // Power-vaild-alert flag indicator
    #define INA3221_REG_MASK_TCF                    (0x0002)  // Timing-control-alert flag indicator
    #define INA3221_REG_MASK_CVRF                   (0x0001)  // Conversion-ready flag
/*=========================================================================    
    Manufacturer ID (R) - See table 23 spec
---------------------------------------------------------------------*/
    #define INA3221_REG_MANUFACTURER_ID_VALUE       (0x5449)  // Manufacturer ID bits
/*=========================================================================    
    Die ID (R) - See table 24 spec
---------------------------------------------------------------------*/
    #define INA3221_REG_DIE_ID_VALUE                (0x3220)  // Die ID bits
/*=========================================================================*/
    #define INA3221_REG_CONFIG                      (0x00)  // Configuration Register (RW)
    #define INA3221_REG_SHUNTVOLTAGE_1              (0x01)  // Channel-1 Shunt Voltage (R)
    #define INA3221_REG_BUSVOLTAGE_1                (0x02)  // Channel-1 Bus Voltage (R)
    #define INA3221_REG_SHUNTVOLTAGE_2              (0x03)  // Channel-2 Shunt Voltage (R)
    #define INA3221_REG_BUSVOLTAGE_2                (0x04)  // Channel-2 Bus Voltage (R)
    #define INA3221_REG_SHUNTVOLTAGE_3              (0x05)  // Channel-3 Shunt Voltage (R)
    #define INA3221_REG_BUSVOLTAGE_3                (0x06)  // Channel-3 Bus Voltage (R)
    #define INA3221_REG_CRITICAL_1                  (0x07)  // Channel-1 Critical Alert Limit (RW)
    #define INA3221_REG_WARNING_1                   (0x08)  // Channel-1 Warning-Alert Limit (RW) 
    #define INA3221_REG_CRITICAL_2                  (0x09)  // Channel-2 Critical Alert Limit (RW)
    #define INA3221_REG_WARNING_2                   (0x0A)  // Channel-2 Warning-Alert Limit (RW) 
    #define INA3221_REG_CRITICAL_3                  (0x0B)  // Channel-3 Critical Alert Limit (RW)
    #define INA3221_REG_WARNING_3                   (0x0C)  // Channel-3 Warning-Alert Limit (RW)  
    #define INA3221_REG_SUM_SHUNTVOLTAGE            (0x0D)  // Shunt Voltage Sum (R)
    #define INA3221_REG_SUM_LIMIT                   (0x0E)  // Shunt Voltage Sum Limit (RW)
    #define INA3221_REG_MASK                        (0x0F)  // Mask/ Enable (RW)
    #define INA3221_REG_POWERVALID_UPPER_LIMIT      (0x10)  // Power-Valid Upper Limit (RW)
    #define INA3221_REG_POWERVALID_LOWER_LIMIT      (0x11)  // Power-Valid Lower Limit (RW)
    #define INA3221_REG_MANUFACTURER_ID             (0xFE)  // Manufacturer ID (R)
    #define INA3221_REG_DIE_ID                      (0xFF)  // Die ID (R)

    #define VS             (0xFE)  // Manufacturer ID (R)
    #define INA3221_REG_DIE_ID                      (0xFF)  // Die ID (R)

    #define SHUNT_RESISTOR_VALUE  (0.1)   // default shunt resistor value of 0.1 Ohm

class INA3221{
 public:
    INA3221(PinName sda , PinName scl, uint8_t addr = INA3221_ADDRESS, float shuntresistor_1= SHUNT_RESISTOR_VALUE, float shuntresistor_2= SHUNT_RESISTOR_VALUE, float shuntresistor_3= SHUNT_RESISTOR_VALUE);
   
    void SetShuntValues (float shuntresistor_1, float shuntresistor_2, float shuntresistor_3);  
    void SetI2CAdr(uint8_t);                                                                    
    void WriteRegister (uint8_t reg, uint16_t value);                                          
    uint16_t ReadRegister(uint8_t reg);                                                        
    uint16_t GetManufacturerID(void);                                                        
    uint16_t GetDieID(void);                                                                 
    uint8_t CheckConnection(void);                                                           
    int16_t GetRawShuntVoltage(uint8_t channel);                                             
    float GetShuntVoltage(uint8_t channel);                                                  
    float GetCurrent(uint8_t channel);                                                         
    int16_t GetRawBusVoltage(uint8_t channel);                                                
    float GetBusVoltage(uint8_t channel);                                                     
    int16_t GetRawCriticalAlertLimit(uint8_t channel);                                         
    float GetCurrentCriticalAlertLimit(uint8_t channel);                                      
    void SetRawCriticalAlertLimit(uint8_t channel, uint16_t value);                         
    void SetCurrentCriticalAlertLimit(uint8_t channel, float current );                    
    int16_t GetRawWarningAlertLimit(uint8_t channel);                                          
    float GetCurrentWarningAlertLimit(uint8_t channel);                                       
    void SetRawWarningAlertLimit(uint8_t channel, int16_t value);                            
    void SetCurrentWarningAlertLimit(uint8_t channel, float current );                        
    int16_t GetRawShuntVoltageSum(void);                                                       
    float GetShuntVoltageSum(void);                                                          
    int16_t GetRawShuntVoltageSumLimit(void);                                                  
    float GetShuntVoltageSumLimit(void);                                                       
    void SetRawShuntVoltageSumLimit(uint16_t value );                                          
    void SetShuntVoltageSumLimit(float value);                                                  
    int16_t GetRawPowerValidUpperLimit(void);                                                   
    float GetPowerValidUpperLimitVoltage(void);                                                 
    void SetRawPowerValidUpperLimit(uint16_t value );                                           
    void SetPowerValidUpperLimitVoltage(float value);                                           
    int16_t GetRawPowerValidLowerLimit(void);                                                   
    float GetPowerValidLowerLimitVoltage(void);                                                 
    void SetRawPowerValidLowerLimit(uint16_t value );                                           
    void SetPowerValidLowerLimitVoltage(float value);                                           
    uint16_t GetConfiguration(void);                                                            
    void SetConfiguration(uint16_t value);                                                      
    void SetMode(uint16_t mode);                                                                
    void SetShuntConversionTime(uint16_t shunt_conversion_time);                                
    void SetBusConversionTime(uint16_t bus_conversion_time);                                    
    void SetAveragingMode(uint16_t averaging_mode);                                             
    void EnableChannel(uint16_t channel);                                                       
    void DisableChannel(uint16_t channel);                                                      
    void Rest(void);                                                                            
    uint16_t GetMask(void);                                                                     
    void SetMask(uint16_t value);                                                               
    uint8_t ConversionReady(void);                                                              
    uint8_t GetTimingAlertFlag(void);                                                           
    uint8_t GetPowerVaildAlertFlag(void);                                                       
    uint8_t GetWarningAlertFlag(uint8_t channel);                                               
    uint8_t GetSummationAlertFlag(void);                                                        
    uint8_t GetCriticalAlertFlag(uint8_t channel);                                              
    uint8_t GetCriticalAlertLachEnable(void);                                                   
    void SetCriticalAlertLachEnable(uint8_t value);                                                
    uint8_t GetWarningAlertLachEnable(void);                                                    
    void SetWarningAlertLachEnable(uint8_t value);                                              
    void EnableChannelSummation(uint16_t channel);                                              
    void DisableChannelSummation(uint16_t channel);                                             

 private:
    uint8_t _INA3221_i2caddr;
      
    float _INA3221_shuntresistor_1;
    float _INA3221_shuntresistor_2;
    float _INA3221_shuntresistor_3;
      
    float _Shunt_Resistor_Ch1;
    float _Shunt_Resistor_Ch2;
    float _Shunt_Resistor_Ch3;
      
    I2C* _i2c;
};

#endif /* INA3221_H */

