/*
Fork of https://os.mbed.com/users/okini3939/code/USBHostXpad/
 */
 
#ifndef USBHostXpad_H
#define USBHostXpad_H
 
#include "USBHostConf.h"
 
#if 1 or USBHOST_XPAD
 
#include "USBHost.h"
 
/** 
 * A class to communicate a USB flash disk
 */
class USBHostXpad : public IUSBEnumerator {
public:
    enum PAD {
        XPAD_HAT_UP    = 0x0001,
        XPAD_HAT_DOWN  = 0x0002,
        XPAD_HAT_LEFT  = 0x0004,
        XPAD_HAT_RIGHT = 0x0008,
        XPAD_START     = 0x0010,
        XPAD_BACK      = 0x0020,
        XPAD_STICK_L   = 0x0040,
        XPAD_STICK_R   = 0x0080,
        XPAD_PAD_LB    = 0x0100,
        XPAD_PAD_RB    = 0x0200,
        XPAD_XLOGO     = 0x0400,
        XPAD_PAD_A     = 0x1000,
        XPAD_PAD_B     = 0x2000,
        XPAD_PAD_X     = 0x4000,
        XPAD_PAD_Y     = 0x8000,
        XPAD_BUTTONS   = 0x10000,
        XPAD_STICK_LX,
        XPAD_STICK_LY,
        XPAD_STICK_RX,
        XPAD_STICK_RY,
        XPAD_TRIGGER_L,
        XPAD_TRIGGER_R,
        XPAD_BATTERY,
    };
 
    enum LED {
        LED_OFF    = 0x00,
        LED_BLINK  = 0x01,
        LED1_FLASH = 0x02,
        LED2_FLASH = 0x03,
        LED3_FLASH = 0x04,
        LED4_FLASH = 0x05,
        LED1_ON    = 0x06,
        LED2_ON    = 0x07,
        LED3_ON    = 0x08,
        LED4_ON    = 0x09,
        LED_ROTATE = 0x0a,
        LED_ALTERNATE = 0x0d,
    };
 
    enum TYPE {
        TYPE_UNKNOWN,
        TYPE_XBOX,
        TYPE_XBOX360,
        TYPE_XBOX360W,
        TYPE_DS4,
    };
 
    /**
    * Constructor
    *
    * @param rootdir mount name
    */
    USBHostXpad();
 
    /**
    * Check if a MSD device is connected
    *
    * @return true if a MSD device is connected
    */
    bool connected();
 
    /**
     * Try to connect to a MSD device
     *
     * @return true if connection was successful
     */
    bool connect();
 
    /**
     * Attach a callback called when a mouse event is received
     *
     * @param ptr function pointer
     */
    inline void attachEvent(void (*ptr)(int buttons,int buttons2, int stick_lx, int stick_ly, int stick_rx, int stick_ry, int trigger_l, int trigger_r)) {
        if (ptr != NULL) {
            onUpdate = ptr;
        }
    }
 
    int read (PAD pad);
    bool led (LED cmd);
    bool rumble (uint8_t large, uint8_t small);
    bool restart ();
    bool stop();
    bool send_data(uint8_t *output_data);
    bool send_data_bt(uint8_t *output_data);
    void lookup_table_hex(uint32_t num, char *s);
    //CRC32
    uint32_t calculate_crc32(unsigned char *p, unsigned int length);
    uint32_t update_crc32_refl(uint32_t crc, unsigned char c);
    uint32_t update_crc32_reflected(const uint32_t *table, uint32_t crc, unsigned char c);
 
protected:
    //From IUSBEnumerator
    virtual void setVidPid(uint16_t vid, uint16_t pid);
    virtual bool parseInterface(uint8_t intf_nb, uint8_t intf_class, uint8_t intf_subclass, uint8_t intf_protocol); //Must return true if the interface should be parsed
    virtual bool useEndpoint(uint8_t intf_nb, ENDPOINT_TYPE type, ENDPOINT_DIRECTION dir); //Must return true if the endpoint will be used
    
private:
    USBHost * host;
    USBDeviceConnected * dev;
    volatile bool dev_connected;
    TYPE dev_type;
    volatile bool dev_started;
 
    USBEndpoint * int_in;
    USBEndpoint * int_out;
    uint8_t nb_ep;
    int xpad_intf;
    bool xpad_device_found;
    uint8_t report[64];
 
    uint8_t buttons, buttons2;
    uint8_t stick_lx, stick_ly, stick_rx, stick_ry;
    uint8_t trigger_l, trigger_r;
    uint8_t battery;
 
    void rxHandler();
    void parseMessage();
    void (*onUpdate)(int buttons,int buttons2, int stick_lx, int stick_ly, int stick_rx, int stick_ry, int trigger_l, int trigger_r);
    void init();
    bool start();
    uint8_t output_data[32];
 
};
 
#endif
 
#endif
 
   