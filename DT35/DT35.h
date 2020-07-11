#ifndef DT35_H
#define DT35_H
#endif
#include "../INA3221/INA3221.h"
#include "mbed.h"
#include <cmath>

class DT35
{
    private:
        INA3221 *vSenor1;
        INA3221 *vSenor2;

    public:
        DT35(PinName, PinName, uint8_t, PinName, PinName, uint8_t);
        DT35(PinName, PinName, uint8_t);
        void DT35_initialization(int, int);
        void DT35_initialization(int);
        int getManufacturerID(int);
        int getDieID(int);
        int getConfiguration(int);
        int getBusVoltage(int, int);
};
