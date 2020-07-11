#include "./DT35.h"

    DT35::DT35(PinName sda1, PinName sci1, uint8_t address1)
    {
        this->vSenor1 = new INA3221(sci1,sda1,address1);
    }

    DT35::DT35(PinName sda1, PinName sci1, uint8_t address1, PinName sda2, PinName sci2, uint8_t address2)
    {
        this->vSenor1 = new INA3221(sci1,sda1,address1);
        this->vSenor2 = new INA3221(sci2,sda2,address2);
    }

    void DT35::DT35_initialization(int number_of_channel1, int number_of_channel2)
    { 
        this->vSenor1->SetMode(INA3221_MODE_BUS_CONTINUOUS);
        this->vSenor2->SetMode(INA3221_MODE_BUS_CONTINUOUS);
        if(number_of_channel1 == 1)
        {
            this->vSenor1->EnableChannel(1);//ch1
            this->vSenor1->DisableChannel(2);//ch2
            this->vSenor1->DisableChannel(3);//ch3
        }
        else if(number_of_channel1 == 2)
        {
            this->vSenor1->EnableChannel(1);//ch1
            this->vSenor1->EnableChannel(2);//ch2
            this->vSenor1->DisableChannel(3);//ch3
        }
        else if(number_of_channel1 == 3)
        {
            this->vSenor1->EnableChannel(1);//ch1
            this->vSenor1->EnableChannel(2);//ch2
            this->vSenor1->EnableChannel(3);//ch3
        }
        if(number_of_channel2 == 1)
        {
            this->vSenor2->EnableChannel(1);//ch1
            this->vSenor2->DisableChannel(2);//ch2
            this->vSenor2->DisableChannel(3);//ch3
        }
        else if(number_of_channel2 == 2)
        {
            this->vSenor2->EnableChannel(1);//ch1
            this->vSenor2->EnableChannel(2);//ch2
            this->vSenor2->DisableChannel(3);//ch3
        }
        else if(number_of_channel2 == 3)
        {
            this->vSenor2->EnableChannel(1);//ch1
            this->vSenor2->EnableChannel(2);//ch2
            this->vSenor2->EnableChannel(3);//ch3
        }
    }

    void DT35::DT35_initialization(int number_of_channel1)
    { 
        this->vSenor1->SetMode(INA3221_MODE_BUS_CONTINUOUS);
        if(number_of_channel1 == 1)
        {
            this->vSenor1->EnableChannel(1);//ch1
            this->vSenor1->DisableChannel(2);//ch2
            this->vSenor1->DisableChannel(3);//ch3
        }
        else if(number_of_channel1 == 2)
        {
            this->vSenor1->EnableChannel(1);//ch1
            this->vSenor1->EnableChannel(2);//ch2
            this->vSenor1->DisableChannel(3);//ch3
        }
        else if(number_of_channel1 == 3)
        {
            this->vSenor1->EnableChannel(1);//ch1
            this->vSenor1->EnableChannel(2);//ch2
            this->vSenor1->EnableChannel(3);//ch3
        }
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
        return temp;
    }

    int DT35::getBusVoltage(int INA3221_id, int channel_id)
    {
        int temp = 0;
        if(INA3221_id == 1)
        {
            temp = this->vSenor1->GetBusVoltage(channel_id) * 1000;
        }
        else if(INA3221_id == 2)
        {
            temp = this->vSenor2->GetBusVoltage(channel_id) * 1000;
        }
        return temp;
    }