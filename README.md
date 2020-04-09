Robocon_PolyU_4mecanum



Use with mbed-os 5.14.2
https://github.com/ARMmbed/mbed-os/#679d24833acf0a0b5b0d528576bb37c70863bc4e
OR
https://github.com/ARMmbed/mbed-os/releases/tag/mbed-os-5.14.2
OR
https://github.com/ARMmbed/mbed-os/tree/mbed-os-5.14.2


Comment out mutex locks in mbed-os/drivers/source/CAN.cpp for both CAN::read and CAN::write, TEMPORARY FIX

EX. 

//lock();
and
//unlock();