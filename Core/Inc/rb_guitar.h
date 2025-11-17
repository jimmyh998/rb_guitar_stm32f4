
#ifndef INC_RB_GUITAR_H_
#define INC_RB_GUITAR_H_
#include "stdint.h"

// XBOX 360 Controller Buttons Format
enum controllerOutputFormat
{
	XBOX_DPAD_UP,
	XBOX_DPAD_DOWN,
	XBOX_DPAD_LEFT,
	XBOX_DPAD_RIGHT,
	XBOX_START,
	XBOX_BACK,
	XBOX_LEFT_STICK,
	XBOX_RIGHT_STICK,
	XBOX_LB,
	XBOX_RB,
	XBOX_HOME,
	XBOX_UNUSED,
	XBOX_A,
	XBOX_B,
	XBOX_X,
	XBOX_Y,
};

static uint8_t xbox_players[] = {
    0,  // 0x00	 All off
    0,  // 0x01	 All blinking
    1,  // 0x02	 1 flashes, then on
    2,  // 0x03	 2 flashes, then on
    3,  // 0x04	 3 flashes, then on
    4,  // 0x05	 4 flashes, then on
    1,  // 0x06	 1 on
    2,  // 0x07	 2 on
    3,  // 0x08	 3 on
    4,  // 0x09	 4 on
    0,  // 0x0A	 Rotating (e.g. 1-2-4-3)
    0,  // 0x0B	 Blinking*
    0,  // 0x0C	 Slow blinking*
    0,  // 0x0D	 Alternating (e.g. 1+4-2+3), then back to previous*
};



#endif
