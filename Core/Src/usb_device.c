
#include <usb_device.h>
#include "tusb.h"
#include "stm32f4xx_hal.h"

#define USB_VID 0xDEAD
#define USB_PID 0xBEEF
#define USB_BCD 0x0200

//https://learn.microsoft.com/en-us/windows/win32/xinput/xinput-and-controller-subtypes
// 0x01 = Gamepad
// 0x06 = Rock Band Guitar
// 0x07 = Guitar Hero Guitar
// 0x08 = Drums
#define SUB_TYPE 0x06

CFG_TUSB_MEM_SECTION CFG_TUSB_MEM_ALIGN uint8_t buf[255];

const XInputInputCapabilities_t XInputInputCapabilities = {
    rid : 0x00,
    rsize : sizeof(XInputInputCapabilities_t),
    buttons : 0xf73f,
    leftTrigger : 0xff,
	rightTrigger : 0xff,
    leftThumbX : USB_VID,
    leftThumbY : USB_PID,
    rightThumbX : USB_VERSION_BCD(6, 0, 0),
    rightThumbY : 0xffc0,
    reserved : {0x00, 0x00, 0x00, 0x00},
    flags : 0x000D
};


const uint8_t desc_msft_os_string[] = {
    0x12,                         // bLength = 18 bytes
    0x03,                         // bDescriptorType = STRING
    'M', 0x00,                    // "M"
    'S', 0x00,                    // "S"
    'F', 0x00,                    // "F"
    'T', 0x00,                    // "T"
    '1', 0x00,                    // "1"
    '0', 0x00,                    // "0"
    '0', 0x00,                    // "0"
    0x20,                         // bMS_VendorCode 0x20
    0x00                          // bPad
};

const uint8_t desc_msft_compatible_id[] = {
    // ---- Header ----
    0x70, 0x00, 0x00, 0x00,       // dwLength = 112 bytes
    0x00, 0x01,                   // bcdVersion = 1.00
    0x04, 0x00,                   // wIndex = 0x0004
    0x04,                         // bCount = 2 sections
    0x00, 0x00, 0x00, 0x00,       // Reserved[0..3]
    0x00, 0x00, 0x00,             // Reserved[4..6]

    // ---- Function Section #1 ---- (WINUSB)
	INTERFACE_ID_Config,          // bFirstInterfaceNumber //Config Interface
    0x04,                         // Reserved
    'W','I','N','U','S','B',0x00,0x00,   // CompatibleID "WINUSB"
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // SubCompatibleID
    0x00,0x00,0x00,0x00,0x00,0x00,        // Reserved2

    // ---- Function Section #2 ---- (XUSB10)
	INTERFACE_ID_Device,          // bFirstInterfaceNumber // Device interface
    0x04,                         // Reserved
    'X','U','S','B','1','0',0x00,0x00,   // CompatibleID "XUSB10"
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // SubCompatibleID
    0x00,0x00,0x00,0x00,0x00,0x00,         // Reserved2

	// ---- Function Section #2 ---- (XUSB10)
	INTERFACE_ID_Padding,          // bFirstInterfaceNumber // Padding Interface
	0x04,                         // Reserved
	'X','U','S','B','1','0',0x00,0x00,   // CompatibleID "XUSB10"
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // SubCompatibleID
	0x00,0x00,0x00,0x00,0x00,0x00,         // Reserved2

	// ---- Function Section #2 ---- (XUSB10)
	INTERFACE_ID_XBOX_Security,          // bFirstInterfaceNumber // Xbox Security Interface
	0x04,                         // Reserved
	'X','U','S','B','1','0',0x00,0x00,   // CompatibleID "XUSB10"
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // SubCompatibleID
	0x00,0x00,0x00,0x00,0x00,0x00         // Reserved2


};

//Device Descriptor

tusb_desc_device_t const desc_device =
{
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = USB_BCD,

    .bDeviceClass       = 0x00,
    .bDeviceSubClass    = 0x00,
    .bDeviceProtocol    = 0x00,
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,

    .idVendor           = USB_VID,
    .idProduct          = USB_PID,
    .bcdDevice          = 0x0000,

    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,

    .bNumConfigurations = 0x01
};

const uint8_t usb_config_descriptor[] = {
	// Configuration Descriptor
	0x09,        // bLength
	0x02,        // bDescriptorType (CONFIGURATION)
	0x99, 0x00,  // wTotalLength (153)
	0x04,        // bNumInterfaces
	0x01,        // bConfigurationValue
	0x00,        // iConfiguration
	0xA0,        // bmAttributes
	0xFA,        // bMaxPower

	/* ---------------------------------------------------- */
	// Interface 0: Control Data
	0x09,        // bLength
	0x04,        // bDescriptorType (INTERFACE)
	0x00,        // bInterfaceNumber
	0x00,        // bAlternateSetting
	0x02,        // bNumEndpoints
	0xFF,        // bInterfaceClass
	0x5D,        // bInterfaceSubClass
	0x01,        // bInterfaceProtocol
	0x00,        // iInterface

	// Unknown Descriptor (If0)
	0x11,        // bLength
	0x21,        // bDescriptorType
	0x10, 0x01, SUB_TYPE, 0x25,  // ???
	0x81,        // bEndpointAddress (IN, 1)
	0x14,        // bMaxDataSize
	0x03, 0x03, 0x03, 0x04, 0x13,  // ???
	0x02,        // bEndpointAddress (OUT, 2)
	0x08,        // bMaxDataSize
	0x03, 0x03,  // ???

	// Endpoint 1: Control Surface Send
	0x07,       		 // bLength
	0x05,       		 // bDescriptorType (ENDPOINT)
	DEVICE_EPIN_ADDR,    // bEndpointAddress (IN, 1)
	0x03,       		 // bmAttributes
	0x20, 0x00,  		 // wMaxPacketSize
	0x01,        		 // bInterval

	// Endpoint 1: Control Surface Receive
	0x07,        			  // bLength
	0x05,					  // bDescriptorType (ENDPOINT)
	DEVICE_EPOUT_ADDR,        // bEndpointAddress (OUT, 2)
	0x03,        			  // bmAttributes
	0x20, 0x00,  			  // wMaxPacketSize
	0x01,        			  // bInterval

	/* ---------------------------------------------------- */
	// Interface 1: Headset (and Expansion Port?)
	0x09,        // bLength
	0x04,        // bDescriptorType (INTERFACE)
	0x01,        // bInterfaceNumber
	0x00,        // bAlternateSetting
	0x04,        // bNumEndpoints
	0xFF,        // bInterfaceClass
	0x5D,        // bInterfaceSubClass
	0x03,        // bInterfaceProtocol
	0x00,        // iInterface

	// Headset Descriptor (If1)
	0x1B,        // bLength
	0x21,        // bDescriptorType
	0x00, 0x01, 0x01, 0x01,  // ???
	HEADSET_EPIN_ADDR,        // bEndpointAddress (IN, 3)
	0x40,        // bMaxDataSize
	0x01,        // ???
	HEADSET_EPOUT_ADDR,        // bEndpointAddress (OUT, 4)
	0x20,        // bMaxDataSize
	0x16,        // ???
	UNK_EPIN_ADDR,        // bEndpointAddress (IN, 5)
	0x00,        // bMaxDataSize
	0x00, 0x00, 0x00, 0x00, 0x00, 0x16,  // ???
	UNK_EPOUT_ADDR,        // bEndpointAddress (OUT, 5)
	0x00,        // bMaxDataSize
	0x00, 0x00, 0x00, 0x00, 0x00,  // ???

	// Endpoint 2: Microphone Data Send
	0x07,        // bLength
	0x05,        // bDescriptorType (ENDPOINT)
	HEADSET_EPIN_ADDR,        // bEndpointAddress (IN, 3)
	0x03,        // bmAttributes
	0x20, 0x00,  // wMaxPacketSize
	0x02,        // bInterval

	// Endpoint 2: Headset Audio Receive
	0x07,        // bLength
	0x05,        // bDescriptorType (ENDPOINT)
	HEADSET_EPOUT_ADDR,        // bEndpointAddress (OUT, 4)
	0x03,        // bmAttributes
	0x20, 0x00,  // wMaxPacketSize
	0x04,        // bInterval

	// Endpoint 3: Unknown, Send
	0x07,        // bLength
	0x05,        // bDescriptorType (ENDPOINT)
	UNK_EPIN_ADDR,        // bEndpointAddress (IN, 5)
	0x03,        // bmAttributes
	0x20, 0x00,  // wMaxPacketSize
	0x40,        // bInterval

	// Endpoint 3: Unknown, Receive
	0x07,        // bLength
	0x05,        // bDescriptorType (ENDPOINT)
	UNK_EPOUT_ADDR,        // bEndpointAddress (OUT, 5)
	0x03,        // bmAttributes
	0x20, 0x00,  // wMaxPacketSize
	0x10,        // bInterval

	/* ---------------------------------------------------- */
	// Interface 2: Unknown
	0x09,        // bLength
	0x04,        // bDescriptorType (INTERFACE)
	0x02,        // bInterfaceNumber
	0x00,        // bAlternateSetting
	0x01,        // bNumEndpoints
	0xFF,        // bInterfaceClass
	0x5D,        // bInterfaceSubClass
	0x02,        // bInterfaceProtocol
	0x00,        // iInterface

	// Unknown Descriptor (If2)
	0x09,        // bLength
	0x21,        // bDescriptorType
	0x00, 0x01, 0x01, 0x22,  // ???
	PLUGIN_EPIN_ADDR,        // bEndpointAddress (IN, 6)
	0x07,        // bMaxDataSize
	0x00,        // ???

	// Endpoint 4: Unknown, Send
	0x07,        // bLength
	0x05,        // bDescriptorType (ENDPOINT)
	PLUGIN_EPOUT_ADDR,        // bEndpointAddress (IN, 6)
	0x03,        // bmAttributes
	0x20, 0x00,  // wMaxPacketSize
	0x10,        // bInterval

	/* ---------------------------------------------------- */
	// Interface 3: Security Method
	0x09,        // bLength
	0x04,        // bDescriptorType (INTERFACE)
	0x03,        // bInterfaceNumber
	0x00,        // bAlternateSetting
	0x00,        // bNumEndpoints
	0xFF,        // bInterfaceClass
	0xFD,        // bInterfaceSubClass
	0x13,        // bInterfaceProtocol
	0x04,        // iInterface

	// Security Descriptor (If3)
	0x06,        // bLength
	0x41,        // bDescriptorType
	0x00, 0x01, 0x01, 0x03,  // ???
};

uint8_t const *tud_descriptor_device_cb(void) {
    return (uint8_t const *)&desc_device;
}

uint8_t const *tud_descriptor_configuration_cb(uint8_t index) {
  (void)index; // for multiple configurations
    return usb_config_descriptor;
  return 0;
}


// array of pointer to string descriptors
char const *string_desc_arr[] =
{
  (const char[]) { 0x09, 0x04 }, 	// 0: is supported language is English (0x0409)
  "jimmyh998",                      // 1: Manufacturer
  "Rock Band Guitar",               // 2: Product
  "123456789ABCDE",          		// 3: Serials will use unique ID if possible
  //"Xbox Security Method 3, Version 1.00, \xA9 2005 Microsoft Corporation. All rights reserved.",
};

static uint16_t _desc_str[173 + 1];

// Invoked when received GET STRING DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
  (void) langid;
  size_t chr_count;

  switch ( index ) {
    case STRID_LANGID:
      memcpy(&_desc_str[1], string_desc_arr[0], 2);
      chr_count = 1;
      break;

    case STRID_MSOS_STRING:
    	memcpy(_desc_str, (uint8_t *)&desc_msft_os_string, sizeof(desc_msft_os_string));
    	return  _desc_str;
    default:
      // Note: the 0xEE index string is a Microsoft OS 1.0 Descriptors.
      // https://docs.microsoft.com/en-us/windows-hardware/drivers/usbcon/microsoft-defined-usb-descriptors

      if ( !(index < sizeof(string_desc_arr) / sizeof(string_desc_arr[0])) ) return NULL;

      const char *str = string_desc_arr[index];

      // Cap at max char
      chr_count = strlen(str);
      size_t const max_count = sizeof(_desc_str) / sizeof(_desc_str[0]) - 1; // -1 for string type
      if ( chr_count > max_count ) chr_count = max_count;

      // Convert ASCII string into UTF-16
      for ( size_t i = 0; i < chr_count; i++ ) {
        _desc_str[1 + i] = str[i];
      }
      break;
  }

  // first byte is length (including header), second byte is string type
  _desc_str[0] = (uint16_t) ((TUSB_DESC_STRING << 8) | (2 * chr_count + 2));

  return _desc_str;
}

bool tud_vendor_control_xfer_cb(uint8_t rhport, uint8_t stage, tusb_control_request_t const *request)
{
	//printf("%02x %04x %04x %04x %04x\r\n", request->bmRequestType, request->bRequest, request->wValue, request->wIndex, request->wLength);
    if (request->bmRequestType_bit.type == TUSB_REQ_TYPE_STANDARD && request->bRequest == TUSB_REQ_GET_DESCRIPTOR)
    {
        //------------- STD Request -------------//
        if (stage == CONTROL_STAGE_SETUP)
        {
        	return false;
        }
    }

    if(request->bmRequestType == 0xC0 && request->bRequest == 0x20 && request->wIndex == 0x04){
    	if(stage == CONTROL_STAGE_SETUP){
    		tud_control_xfer(rhport, request, (uint8_t *)desc_msft_compatible_id, sizeof(desc_msft_compatible_id));
    	}
    }

    if (request->bmRequestType_bit.direction == TUSB_DIR_IN)
           {
               if (stage == CONTROL_STAGE_SETUP)
               {
            	   if(request->bmRequestType == 0xC1 && request->bRequest == 0x81){
            		   tud_control_xfer(rhport, request, buf, 0);
            	   }
            	   if(request->bmRequestType == 0xC0 && request->bRequest == 0x01 && request->wIndex == 0x0000 && request->wValue == 0x0000){
            		   //tud_control_xfer(rhport, request, (uint8_t *)test, sizeof(test));
            		   uint32_t serial = HAL_GetTick();
            		   tud_control_xfer(rhport, request, &serial, sizeof(serial));
            	   }
            	   if(request->bmRequestType == 0xC1 && request->bRequest == 0x01 && request->wValue == 0x0100){
            		   memcpy(buf, &XInputInputCapabilities, sizeof(XInputInputCapabilities));
            		   tud_control_xfer(rhport, request, buf, sizeof(buf));
            	   }

               }
           }
    return true;
}
