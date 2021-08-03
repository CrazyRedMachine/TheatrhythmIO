[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/donate?hosted_button_id=WT735CX4UMZ9U)

# TheatrhythmIO

Final Fantasy Theatrhythm arcade panel interface using Arduino Leonardo

## CAUTION

This panel contains an RFID reader with what appears to be an USB connection. This is **NOT USB** and is actually meant for JVS I/O. 

**DO NOT** plug the RFID reader into a computer USB port.

## Acknowledgments

This firmware is basically a fork of my GrooveCoasterIO firmware, and thus it uses Arduino-LUFA as well.

Thanks to pikpop for testing and providing pinout.

Thanks to progmem for his work on reversing the pokken controller for Nintendo Switch.

FASTIO mode uses digitalWriteFast library by NicksonYap.

Button debouncing is done with Bounce2 library by Thomas O Fredericks.

## Demo

https://www.instagram.com/p/CSCh5MRosdg/

## Pre-requisites (How to flash)

- Install `Bounce2` from the Arduino IDE Library manager
- Install `Arduino-LUFA boards` from the Arduino IDE board manager following the instructions from https://github.com/CrazyRedMachine/Arduino-LUFA
- Select `Arduino Leonardo (LUFA)` as your board type and click upload

## Features

This firmware will make your panel recognized as a DirectInput controller, compatible with Nintendo Switch.

Additionally, a second interface will appear on PC for HID lights control.

It is possible to make the panel send keyboard inputs in parallel to the gamepad although not recommended.

`config.h` contains configurable user options.

### Light modes

The controller supports HID and two reactive modes.

Also it will display a light animation when idling (the idle animation fallback delay can be configured in `config.h`).

#### HID mode

Controller will automatically switch to HID upon receiving hid messages (and will fallback to reactive mode after 3 seconds without communication).

Joysticks RGB, RFID RGB and individual button leds can be controlled this way, and all outputs are named.

**Note:** Joysticks RGB is for both sticks at once (the panel connectors don't expose individual joysticks)

#### Reactive mode

When no HID messages are received, reactive lighting mode behaves according to 4 user-configurable colors as shown in the following color table :

|Component|Joysticks moved|Button pressed|Nothing pressed|
|:---|---|---|---|
| RFID |`RFID_COLOR`|`RFID_COLOR`|`RFID_COLOR`|
|Joysticks|`REACTIVE_PRIMARY`|`REACTIVE_SECONDARY`|`REACTIVE_TERNARY`|
|Button|`BUTTON_LIGHT_RELEASED_STATE`|`BUTTON_LIGHT_PRESSED_STATE`|`BUTTON_LIGHT_RELEASED_STATE`|

When `REACTIVE_FADE` is set to 1, there is a fade transition when releasing buttons instead of an immediate one.

Transition speed can be adjusted with the `REACTIVE_FADERATE` option.

The color palette can be changed in `config.h`, for example

`#define REACTIVE_TERNARY ((color_t) { 0xFF, 0xFF, 0x00 })`

or `#define REACTIVE_TERNARY YELLOW` would work too here since some colors are defined in `RGB.h`

#### Idle animation

A rainbow wave idle animation (also used as startup animation) will be displayed before touching a button on the controller, or after not touching anything for a while (10 seconds by default).

The delay can be configured in `config.h` :

`#define IDLE_ANIMATION_DELAY 10`

Setting this value to 0 will disable the idle animation

## Pinout

Inside the panel are 5 connectors, labeled N, V, U, K and K2.

N and V take care of lights. K and K2 take care of inputs. V takes care of the headphone jack and volume pot (irrelevant to this firmware)

### N connector

This is a 3x3 9 pin connector.

This one takes care of RGB lights for the panel.

RGB leds are common anode.

|Pin number|Wire color|Role|Arduino GPIO|
|:---|---|---|---:|
| 1 |Pink |RFID RGB Red - |5|
|2|Brown |RFID RGB Green -|6|
|3|Purple |RFID RGB Blue -|3|
|4|White/Red |Joysticks RGB Red -|9|
|5|White/Green |Joysticks RGB Green -|10|
|6|White/Blue |Joysticks RGB Blue -|11|
|7| |||
|8|Black |GND|GND|
|9|Red |+5V|+5V|

### U connector

This is a 2x4 8 pin connector.

This one takes care of the two button leds.

**Beware**: They are 24v leds. You have to replace them with 5v leds for this firmware.

|Pin number|Wire color|Role|Arduino GPIO|
|:---|---|---|---:|
| 1 |Brown| Left button led - |8|
|2|Pink| Right button led -|2|
|3||||
|4||||
|5||||
|6||||
|7|Orange| Button leds + | +5V |
|8||||

### K connector

This is a 3*5 15 pin connector.

This one takes care of the inputs.

|Pin number|Wire color|Role|Arduino GPIO|
|:---|---|---|---:|
| 1 |White/Pink| Left button |7|
|2|White/Brown| Left joy UP |4|
|3|White/Red| Left joy DOWN |A3|
|4|White/Orange| Left joy LEFT |12|
|5|White/Yellow| Left joy RIGHT |A2|
|6|White/Green| Right joy UP|0|
|7|White/Blue| Right joy DOWN | A5 |
|8|White/Purple| Right joy LEFT |1|
|9|White/Grey| Right joy RIGHT |A4|
|10||||
|11||||
|12|Black|GND|GND|
|13||||
|14||||
|15||||

### K2 connector

This is a single pin connector.

This one takes care of the lonely forgotten input.

|Pin number|Wire color|Role|Arduino GPIO|
|:---|---|---|---:|
| 1 |Blue| Right button |13|

## Donation

If this project helps you and you want to give back, you can help me with my future projects.

While not necessary, donations are much appreciated and will only go towards funding future github projects (arcade hardware ain't cheap :( ).

Of course you'll also receive my gratitude and I'll remember you if you post a feature request ;)

[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/donate?hosted_button_id=WT735CX4UMZ9U)
