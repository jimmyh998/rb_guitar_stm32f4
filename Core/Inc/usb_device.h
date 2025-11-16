#ifndef INC_USB_DEVICE_H_
#define INC_USB_DEVICE_H_

#include "stdint.h"

#define USB_VERSION_BCD(major, minor, sub) (((major) << 8) | ((minor << 4) | (sub)))

#define DEVICE_EPIN_ADDR 0x81
#define DEVICE_EPOUT_ADDR 0x02
#define PLUGIN_EPIN_ADDR 0x82
#define PLUGIN_EPOUT_ADDR 0x03

//We placed the addresses that we will not use outside the limit,
//since the stm32f401cu6 only has 6 endpoints (including the control endpoint).
#define HEADSET_EPIN_ADDR 0x86
#define HEADSET_EPOUT_ADDR 0x06
#define UNK_EPIN_ADDR 0x87
#define UNK_EPOUT_ADDR 0x07



typedef struct {
    uint8_t rid;
    uint8_t rsize;
    uint16_t buttons;
    uint8_t leftTrigger;
    uint8_t rightTrigger;
    uint16_t leftThumbX;
    uint16_t leftThumbY;
    uint16_t rightThumbX;
    uint16_t rightThumbY;
    uint8_t reserved[4];
    uint16_t flags;
} __attribute__((packed)) XInputInputCapabilities_t;

enum {
  STRID_LANGID = 0,
  STRID_MANUFACTURER,
  STRID_PRODUCT,
  STRID_SERIAL,
  STRID_MSOS_STRING = 0xEE,
};

enum interfaces_t {
    INTERFACE_ID_Device = 0, /**< XInput interface descriptor ID */
    INTERFACE_ID_Padding = 1,
    INTERFACE_ID_Config = 2, /**< Config interface descriptor ID */
    INTERFACE_ID_XBOX_Security = 3,
};

#endif
