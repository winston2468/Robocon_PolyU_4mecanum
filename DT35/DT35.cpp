#include "./DT35.h"

    DT35::DT35(PinName sda, PinName sci, uint8_t address1)
    {
        this->vSenor1 = new INA3221(sci, sda, address1);
    }

    DT35::DT35(PinName sda, PinName sci, uint8_t address1, uint8_t address2)
    {
        this->vSenor1 = new INA3221(sci, sda, address1);
        this->vSenor2 = new INA3221(sci, sda, address2);
    }

    DT35::DT35(PinName sda, PinName sci, uint8_t address1, uint8_t address2, uint8_t address3)
    {
        this->vSenor1 = new INA3221(sci, sda, address1);
        this->vSenor2 = new INA3221(sci, sda, address2);
        this->vSenor3 = new INA3221(sci, sda, address3);
    }

    void DT35::DT35_initialization(int channel_number1, int channel_number2, int channel_number3)
    { 
        this->vSenor1->SetMode(INA3221_MODE_BUS_CONTINUOUS);
        this->vSenor2->SetMode(INA3221_MODE_BUS_CONTINUOUS);
        this->vSenor3->SetMode(INA3221_MODE_BUS_CONTINUOUS);
        this->vSenor1->EnableChannel(channel_number1);
        this->vSenor2->EnableChannel(channel_number2);
        this->vSenor3->EnableChannel(channel_number3);
    }

    void DT35::DT35_initialization(int channel_number1, int channel_number2)
    { 
        this->vSenor1->SetMode(INA3221_MODE_BUS_CONTINUOUS);
        this->vSenor2->SetMode(INA3221_MODE_BUS_CONTINUOUS);
        this->vSenor1->EnableChannel(channel_number1);
        this->vSenor2->EnableChannel(channel_number2);
    }

    void DT35::DT35_initialization(int channel_number1)
    { 
        this->vSenor1->SetMode(INA3221_MODE_BUS_CONTINUOUS);
        this->vSenor1->EnableChannel(1);//ch1
    }

    int DT35::getManufacturerID(int INA3221_id)
    {
        int temp = 0;
        if(INA3221_id == 1)
        {
            temp = this->vSenor1->GetManufacturerID();
        }
        else if(INA3221_id == 2)
        {
            temp = this->vSenor2->GetManufacturerID();
        }
        else if(INA3221_id == 3)
        {
            temp = this->vSenor3->GetManufacturerID();
        }
        return temp;
    }

    int DT35::getDieID(int INA3221_id)
    {
        int temp = 0;
        if(INA3221_id == 1)
        {
            temp = this->vSenor1->GetDieID();
        }
        else if(INA3221_id == 2)
        {
            temp = this->vSenor2->GetDieID();
        }
        else if(INA3221_id == 3)
        {
            temp = this->vSenor3->GetDieID();
        }
        return temp;
    }

    int DT35::getConfiguration(int INA3221_id)
    {
        int temp = 0;
        if(INA3221_id == 1)
        {
            temp = this->vSenor1->GetConfiguration();
        }
        else if(INA3221_id == 2)
        {
            temp = this->vSenor2->GetConfiguration();
        }
        else if(INA3221_id == 3)
        {
            temp = this->vSenor2->GetConfiguration();
        }
        return temp;
    }

    int DT35::getBusVoltage(int INA3221_id, int channel_id)
    {
        int temp = 0;
        if(INA3221_id == 1)
        {
            temp = this->vSenor1->GetBusVoltage(channel_id) * 1000; //V to mV
        }
        else if(INA3221_id == 2)
        {
            temp = this->vSenor2->GetBusVoltage(channel_id) * 1000; //V to mV
        }
        else if(INA3221_id == 3)
        {
            temp = this->vSenor3->GetBusVoltage(channel_id) * 1000; //V to mV
        }
        return temp;
    }