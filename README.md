#rb_guitar_stm32f4

Use the STM32F4 and the tinyusb library to emulate a Rock Band guitar.

The device emulates a Rock Band xinput guitar (X360 and PC). Simply configure the pins with STM32CubeMX and repeat the code for each button, as shown in main.c.

The idea here is simply to get the code working; it still needs some adjustments, but it's working well, especially on the X360. Just use the [UsbdSecPatch](https://github.com/InvoxiPlayGames/UsbdSecPatch) plugin.

The project uses parts of the code from [Santroller](https://github.com/Santroller/Santroller) and [tinyusb_xinput](https://github.com/fluffymadness/tinyusb-xinput)

What doesn't work:

- Unmodified consoles (Emma's [libxsm3](https://github.com/InvoxiPlayGames/libxsm3) library is used in Santroller so that the peripheral works on any console, but it hasn't been implemented in this project)
#