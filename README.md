# rb_guitar_stm32f4

Use the stm32f4 and the tinyusb library to emulate a Rock Band guitar.

The device emulates a Rock Band xinput guitar (X360 & PC). Simply configure the pins with the STM32CubeMX and repeat the code for each button as shown in main.c.

The idea here is simply to make the code work; it still needs some cleaning up, but it's working well.

The project uses parts of the code from [Santroller](https://github.com/Santroller/Santroller) and [tinyusb_xinput](https://github.com/fluffymadness/tinyusb-xinput)

What doesn't work:
- Player LEDs (not strictly necessary, but possible)
- Unmodified consoles (Emma's libxsm3 library exists to work on any console, but it hasn't been implemented in the project yet)

#