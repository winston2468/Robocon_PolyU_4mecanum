/*
Fork of https://os.mbed.com/users/okini3939/code/USBHostXpad/
 */

#include "USBHostXpad.h"

#if 3 or USBHOST_XPAD

#include "dbg.h"

#define DEVICE_TO_HOST 0x80
#define HOST_TO_DEVICE 0x00

USBHostXpad::USBHostXpad() {
  host = USBHost::getHostInst();
  init();
}

void USBHostXpad::init() {
  dev_connected = false;
  dev = NULL;
  int_in = NULL;
  int_out = NULL;
  xpad_intf = -1;
  xpad_device_found = false;
  nb_ep = 0;
  dev_type = TYPE_UNKNOWN;
  dev_started = false;
}

bool USBHostXpad::connected() { return dev_connected; }

bool USBHostXpad::connect() {

  if (dev_connected) {
    return true;
  }

  for (int i = 0; i < MAX_DEVICE_CONNECTED; i++) {
    if ((dev = host->getDevice(i)) != NULL) {

      USB_DBG("Trying to connect MSD device\r\n");

      if (host->enumerate(dev, this))
        break;

      if (xpad_device_found) {
        int_in = dev->getEndpoint(xpad_intf, INTERRUPT_ENDPOINT, IN);
        int_out = dev->getEndpoint(xpad_intf, INTERRUPT_ENDPOINT, OUT);

        if (!int_in || !int_out)
          continue;

        USB_INFO("New MSD device: VID:%04x PID:%04x [dev: %p - intf: %d]",
                 dev->getVid(), dev->getPid(), dev, xpad_intf);
        dev->setName("XPAD", xpad_intf);
        host->registerDriver(dev, xpad_intf, this, &USBHostXpad::init);
        int_in->attach(this, &USBHostXpad::rxHandler);
                // USB Control GET_REPORT REQUEST
        host->controlWrite(dev, 0xa1, 0x01, 0x0302, 0, 0, 0);
        uint8_t send_hid_report[32];
        send_hid_report[0] = 0x05; // report id
        send_hid_report[1] = 0xff;

        send_hid_report[4] = 0x00;  // small rumble
        send_hid_report[5] = 0x00;  // big rumble
        send_hid_report[6] = 0x00;  // red
        send_hid_report[7] = 0x00;  // green
        send_hid_report[8] = 0xff;  // blue
        send_hid_report[9] = 0x00;  // Time to flash bright (255 = 2.5 seconds)
        send_hid_report[10] = 0x00; // Time to flash dark (255 = 2.5 seconds)
        send_data(send_hid_report);


        host->interruptRead(dev, int_in, report, int_in->getSize(), false);

        ThisThread::sleep_for(100);
        // led(LED_OFF);
        dev_connected = true;
        return true;
      }
    }
  }
  init();
  return false;
}

void USBHostXpad::rxHandler() {
  int len_listen = int_in->getSize();
  int len = int_in->getLengthTransferred();

  /*
      for (int i = 0; i < len_listen; i ++) {
          std::printf(" %02x", report[i]);
      }
      std::printf("\r\n");
  */
  if (dev_type == TYPE_XBOX || TYPE_DS4) {
    if (report[0] == 0x01) {
      // USB_INFO("%x %x %x %x", report[4],report[5],report[6],report[7]);
      parseMessage();
    }
  }

  else if (report[0] == 0x08) {
    if (report[1] == 0x80) {
      // Connection Status Messages
      start();
    } else if (report[1] == 0x00) {
      // Disconnected Status Messages
      dev_started = false;
    }
  } else if (report[0] == 0x00) {
    if ((report[1] == 0x14) ||
        (report[1] == 0x01 && report[2] == 0x00 && report[3] == 0xf0)) {
      // Event data
      // parseMessage();
    } else if (report[1] == 0x0f) {
      // On connection
      led(LED1_ON);
      start();
      dev_started = true;
    } else if (report[1] == 0x00) {
      if (report[3] == 0x13) {
        // Battery charge level
        battery = report[4];
      } else if (report[3] == 0xf0) {
        // Device low power mode (HID updates will stop until controller inputs
        // change)
        //                restart();
      }
    } else if (report[1] == 0xf8) {
      // Status Messages ?
    }
  } else {
    // Unknown
    // USB_INFO("rxHandler len:%d reportid: %02x data:  LX: %02x  LY: %02x  RX:
    // %02x  RY: %02x  Button/DPAD: %02x  RL1-3+Op: %02x   L2TRIG: %02x R2TRIGG:
    // %02x", len, report[0], report[1], report[2], report[3], report[4],
    // report[5],  report[6], report[8], report[9]);
  }

  if (dev) {
      //ThisThread::sleep_for(1000);
    host->interruptRead(dev, int_in, report, len_listen, false);
  }
}

/*virtual*/ void USBHostXpad::setVidPid(uint16_t vid, uint16_t pid) {
  // we don't check VID/PID for MSD driver
  if (vid == 0x045e &&
      (pid == 0x0202 || pid == 0x0285 || pid == 0x0287 || pid == 0x0289)) {
    dev_type = TYPE_XBOX;
  } else if (vid == 0x045e && pid == 0x028e) {
    dev_type = TYPE_XBOX360;
  } else if (vid == 0x045e && pid == 0x0719) {
    dev_type = TYPE_XBOX360W;
  } else if (vid == 0x054C && (pid == 0x5C4 || pid == 0x09CC || pid == 0xBA0)) {
    dev_type = TYPE_DS4;
  }

  USB_INFO("setVidPid vid:%04x pid:%04x xbox:%d", vid, pid, dev_type);
}

/*virtual*/ bool USBHostXpad::parseInterface(
    uint8_t intf_nb, uint8_t intf_class, uint8_t intf_subclass,
    uint8_t intf_protocol) // Must return true if the interface should be parsed
{
  USB_INFO("parseInterface class:%02x subclass:%02x protocol:%02x [nb: %d - "
           "intf: %d]",
           intf_class, intf_subclass, intf_protocol, intf_nb, xpad_intf);
  if ((xpad_intf == -1) && (intf_class == 0x58) && (intf_subclass == 0x42) &&
      (intf_protocol == 0x00)) {
    xpad_intf = intf_nb; // XBOX ?
    return true;
  }
  if ((xpad_intf == -1) && (intf_class == 0xff) && (intf_subclass == 0x5d) &&
      (intf_protocol == 0x81)) {
    xpad_intf = intf_nb; // XBOX360W
    return true;
  }
  if ((xpad_intf == -1) && (intf_class == 0x03) && (intf_subclass == 0x00) &&
      (intf_protocol == 0x00)) {
    xpad_intf = intf_nb; // DS4
    return true;
  }

  return false;
}

/*virtual*/ bool USBHostXpad::useEndpoint(
    uint8_t intf_nb, ENDPOINT_TYPE type,
    ENDPOINT_DIRECTION dir) // Must return true if the endpoint will be used
{
  USB_INFO("useEndpoint nb:%d type:%d dir:%d", intf_nb, type, dir);
  if (intf_nb == xpad_intf) {
    if (type == INTERRUPT_ENDPOINT) {
      nb_ep++;
      if (nb_ep == 2) {
        xpad_device_found = true;
      }
      return true;
    }
  }
  return false;
}

void USBHostXpad::parseMessage() {
  uint8_t *data = report;

  switch (dev_type) {
  case TYPE_DS4:
    stick_lx = ((uint8_t)data[1]);
    stick_ly = ((uint8_t)data[2]);
    stick_rx = ((uint8_t)data[3]);
    stick_ry = ((uint8_t)data[4]);
    buttons = ((uint8_t)data[5]);
    buttons2 = ((uint8_t)data[6]);
    trigger_l = (uint8_t)data[8];
    trigger_r = (uint8_t)data[9];

    break;

  case TYPE_XBOX:
    buttons = ((uint32_t)report[3] << 8) | report[2];
    if (report[4])
      buttons |= XPAD_PAD_A;
    if (report[5])
      buttons |= XPAD_PAD_B;
    if (report[6])
      buttons |= XPAD_PAD_X;
    if (report[7])
      buttons |= XPAD_PAD_Y;
    trigger_l = data[10];
    trigger_r = data[11];

    stick_lx = ((int16_t)report[13] << 8) | report[12];
    stick_ly = ((int16_t)report[15] << 8) | report[14];
    stick_rx = ((int16_t)report[17] << 8) | report[16];
    stick_ry = ((int16_t)report[19] << 8) | report[18];
    break;
  case TYPE_XBOX360W:
    data += 4;
  case TYPE_XBOX360:
    buttons = ((uint32_t)data[3] << 8) | data[2];
    trigger_l = data[4];
    trigger_r = data[5];

    stick_lx = ((int16_t)data[7] << 8) | data[6];
    stick_ly = ((int16_t)data[9] << 8) | data[8];
    stick_rx = ((int16_t)data[11] << 8) | data[10];
    stick_ry = ((int16_t)data[13] << 8) | data[12];
    break;
  default:
    return;
  }

  if (onUpdate) {
    (*onUpdate)(buttons, buttons2, stick_lx, stick_ly, stick_rx, stick_ry,
                trigger_l, trigger_r);
  }
}

bool USBHostXpad::restart() {
  unsigned char odata[64];
  memset(odata, 0, sizeof(odata));
  odata[0] = 0x08;
  odata[2] = 0x0f;
  odata[3] = 0xc0;
  if (dev) {
    for (int i = 0; i < 4; i++) {
      host->interruptWrite(dev, int_out, odata, 12);
    }
  }
  return true;
}

int USBHostXpad::read(PAD pad) {
  switch (pad) {
  case XPAD_BUTTONS:
    return buttons;
  case XPAD_STICK_LX:
    return stick_lx;
  case XPAD_STICK_LY:
    return stick_ly;
  case XPAD_STICK_RX:
    return stick_rx;
  case XPAD_STICK_RY:
    return stick_ry;
  case XPAD_TRIGGER_L:
    return trigger_l;
  case XPAD_TRIGGER_R:
    return trigger_r;
  case XPAD_BATTERY:
    return battery;
  default:
    return (buttons & pad) ? 1 : 0;
  }
}

bool USBHostXpad::start() {
  // unsigned char odata[64];
  unsigned char odata[] = {
      0xa2, 0x11, 0xc0, 0x20, 0xf0, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x43,
      0x00, 0x4d, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0xd8, 0x8e, 0x94, 0xdd};
  memset(odata, 0, sizeof(odata));

  // odata[0] = 0x40;
  if (dev) {
    if (host->interruptWrite(dev, int_out, odata, 79) != USB_TYPE_OK) {
      return false;
    }
  }
  return true;
}

bool USBHostXpad::stop() {
  unsigned char odata[64];
  memset(odata, 0, sizeof(odata));
  odata[2] = 0x08;
  odata[3] = 0xc0;
  if (dev) {
    if (host->interruptWrite(dev, int_out, odata, 12) != USB_TYPE_OK) {
      return false;
    }
  }
  return true;
}

bool USBHostXpad::led(LED cmd) {
  unsigned char odata[32];
  memset(odata, 0, sizeof(odata));
  switch (dev_type) {
  case TYPE_DS4:
    break;
  case TYPE_XBOX:
    return true;
  case TYPE_XBOX360:
    odata[0] = 0x01;
    odata[1] = 0x03;
    odata[2] = cmd;
    break;
  case TYPE_XBOX360W:
    odata[2] = 0x08;
    odata[3] = 0x40 + (cmd % 0x0e);
    break;
  default:
    return false;
  }
  if (dev) {
    if (host->interruptWrite(dev, int_out, odata, 12) != USB_TYPE_OK) {
      return false;
    }
  }
  return true;
}

bool USBHostXpad::rumble(uint8_t large, uint8_t small) {
  unsigned char odata[32];
  memset(odata, 0, sizeof(odata));
  switch (dev_type) {
  case TYPE_DS4:

    break;
  case TYPE_XBOX:
    odata[1] = 0x06;
    odata[3] = small;
    odata[5] = large;
    break;
  case TYPE_XBOX360:
    odata[1] = 0x08;
    odata[3] = large;
    odata[4] = small;
    break;
  case TYPE_XBOX360W:
    odata[1] = 0x01;
    odata[2] = 0x0f;
    odata[3] = 0xc0;
    odata[5] = large;
    odata[6] = small;
    break;
  default:
    return false;
  }
  if (dev) {
    if (host->interruptWrite(dev, int_out, odata, 12) != USB_TYPE_OK) {
      return false;
    }
  }
  return true;
}

bool USBHostXpad::send_data(uint8_t *output_data) {
  uint8_t odata[32];
  memset(odata, 0, sizeof(odata));
  std::memcpy(odata, output_data, sizeof(odata));

  if (dev) {
    if (host->interruptWrite(dev, int_out, odata, sizeof(odata)) !=
        USB_TYPE_OK) {

      return false;
    }
  }
  return true;
}

bool USBHostXpad::send_data_bt(unsigned char *output_data) {
  unsigned char odata[79];
  memset(odata, 0, sizeof(odata));
  std::memcpy(odata, output_data, sizeof(odata));
  uint32_t crc32_checksum = calculate_crc32(output_data, 75);
  odata[75] = (crc32_checksum >> 24) & 0xff;
  odata[76] = (crc32_checksum >> 16) & 0xff;
  odata[77] = (crc32_checksum >> 8) & 0xff;
  odata[78] = (crc32_checksum)&0xff;

  if (dev) {

    if (host->interruptWrite(dev, int_out, odata, 79, true) != USB_TYPE_OK) {

      return false;
    }
    // USB_INFO("TESTESTESTSETSEt\r\n");
  }
  return true;
}

// CRC32

const uint32_t crc_tab32_reflected[] = {
    0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F,
    0xE963A535, 0x9E6495A3, 0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
    0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91, 0x1DB71064, 0x6AB020F2,
    0xF3B97148, 0x84BE41DE, 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
    0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9,
    0xFA0F3D63, 0x8D080DF5, 0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
    0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B, 0x35B5A8FA, 0x42B2986C,
    0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
    0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423,
    0xCFBA9599, 0xB8BDA50F, 0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
    0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,

    0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F,
    0x9FBFE4A5, 0xE8B8D433, 0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,
    0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01, 0x6B6B51F4, 0x1C6C6162,
    0x856530D8, 0xF262004E, 0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
    0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49,
    0x8CD37CF3, 0xFBD44C65, 0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
    0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB, 0x4369E96A, 0x346ED9FC,
    0xAD678846, 0xDA60B8D0, 0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
    0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3,
    0xB966D409, 0xCE61E49F, 0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,
    0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,

    0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A, 0xEAD54739, 0x9DD277AF,
    0x04DB2615, 0x73DC1683, 0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
    0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1, 0xF00F9344, 0x8708A3D2,
    0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
    0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC, 0xF9B9DF6F, 0x8EBEEFF9,
    0x17B7BE43, 0x60B08ED5, 0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,
    0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B, 0xD80D2BDA, 0xAF0A1B4C,
    0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
    0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236, 0xCC0C7795, 0xBB0B4703,
    0x220216B9, 0x5505262F, 0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,
    0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,

    0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, 0x9C0906A9, 0xEB0E363F,
    0x72076785, 0x05005713, 0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
    0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21, 0x86D3D2D4, 0xF1D4E242,
    0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
    0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69,
    0x616BFFD3, 0x166CCF45, 0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
    0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB, 0xAED16A4A, 0xD9D65ADC,
    0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
    0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693,
    0x54DE5729, 0x23D967BF, 0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
    0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D};

uint32_t USBHostXpad::calculate_crc32(unsigned char *p, unsigned int length) {
  uint32_t crc32;
  unsigned int i;

  crc32 = 0xFFFFFFFFL;

  for (i = 0; i < length; i++) {
    crc32 = update_crc32_refl(crc32, *p++);
  }

  /* One's complement = Xor with FFFFFFFF */
  return ~crc32;
}

uint32_t USBHostXpad::update_crc32_refl(uint32_t crc, unsigned char c) {
  return update_crc32_reflected(crc_tab32_reflected, crc, c);
}

uint32_t USBHostXpad::update_crc32_reflected(const uint32_t *table,
                                             uint32_t crc, unsigned char c) {
  uint32_t long_c;

  long_c = 0x000000ffL & (uint32_t)c;

  return (crc >> 8) ^ table[(crc ^ long_c) & 0xff];
}

#endif
