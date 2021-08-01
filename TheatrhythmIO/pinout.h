#ifndef _PINOUT_H_
#define _PINOUT_H_

/* USER OPTIONS */

/* in case you want to disable one type of gamepad */
//#define DISABLE_NSWITCH
#define DISABLE_XINPUT

//make it so holding start+select triggers the HOME button
//#define HOME_HOTKEY
//delay in ms for start+select to become HOME in HOME_HOTKEY mode
#define HOME_DELAY 1000

#define MILLIDEBOUNCE 1 /* Debounce time in milliseconds */

/* LIGHT OPTIONS */

/* make reactive mode mimick the arcade cab (light blue joystick light, lit buttons... boring) */
#define REACTIVE_SIM 1

/* delay (in seconds) before fallback to idle light animation */
#define IDLE_ANIMATION_DELAY 3
/* button blink time for idle animation */
#define BUTTON_BLINK_TIME 0.8

/* PINS ASSIGNEMENT */

/* RGB pins (/!\ MUST USE PWM PINS /!\) */
#define PIN_JOY_RGB_R   9
#define PIN_JOY_RGB_G   10
#define PIN_JOY_RGB_B   11

#define PIN_RFID_RGB_R      5
#define PIN_RFID_RGB_G      6
#define PIN_RFID_RGB_B      13

/* Left joy pins */ /* switch : dpad lb */
#define PIN_LB_UP        4  /* dpad up */
#define PIN_LB_DOWN      A3 /* dpad down */
#define PIN_LB_LEFT      12 /* dpad left */
#define PIN_LB_RIGHT     A2  /* dpad right */
#define PIN_LB_BUTTON    7 /* L bumper */
#define PIN_LB_LED       8

/* Right joy pins */ /* switch : xbya rb */
#define PIN_RB_UP        0  /* X */
#define PIN_RB_DOWN      A5  /* B */
#define PIN_RB_LEFT      1  /* Y */
#define PIN_RB_RIGHT     A4 /* A */
#define PIN_RB_BUTTON    3 /* R bumper */
#define PIN_RB_LED       2

/* Additional button pins */
#define PIN_TEST         A0  /* plus */
#define PIN_SERVICE      A1  /* home */
#define PIN_COIN         MOSI /* minus */

/* LED board pins */

/* KEYS ASSIGNEMENT */
/* aditionnally simulate a keyboard in NSwitch mode */
//#define WITH_KEYBOARD

#define KEY_LB_UP          HID_KEYBOARD_SC_W
#define KEY_LB_DOWN        HID_KEYBOARD_SC_S
#define KEY_LB_LEFT        HID_KEYBOARD_SC_A
#define KEY_LB_RIGHT       HID_KEYBOARD_SC_D

/* CTRL, SHIFT and ALT keys are modifiers */
//#define LB_BUTTON_IS_MODIFIER
#define KEY_LB_BUTTON      HID_KEYBOARD_SC_SPACE

#define KEY_RB_UP          HID_KEYBOARD_SC_UP_ARROW
#define KEY_RB_DOWN        HID_KEYBOARD_SC_DOWN_ARROW
#define KEY_RB_LEFT        HID_KEYBOARD_SC_LEFT_ARROW
#define KEY_RB_RIGHT       HID_KEYBOARD_SC_RIGHT_ARROW

/* CTRL, SHIFT and ALT keys are modifiers */
#define RB_BUTTON_IS_MODIFIER
#define KEY_RB_BUTTON      HID_KEYBOARD_MODIFIER_LEFTCTRL

#define KEY_TEST           HID_KEYBOARD_SC_F3
#define KEY_SERVICE        HID_KEYBOARD_SC_F4
#define KEY_COIN           HID_KEYBOARD_SC_F5

/* END OF USER CONFIGURABLE OPTIONS */

/* use faster digitalWrite to speedup light processing (25 times faster) */
#define WITH_FASTIO

#define NUM_BUTTONS 13

enum button_mask {
  LB_UP     = 1 << 0,
  LB_DOWN   = 1 << 1,
  LB_LEFT   = 1 << 2,
  LB_RIGHT  = 1 << 3,
  LB_BUTTON = 1 << 4,
  RB_UP     = 1 << 5,
  RB_DOWN   = 1 << 6,
  RB_LEFT   = 1 << 7,
  RB_RIGHT  = 1 << 8,
  RB_BUTTON = 1 << 9,
  TEST      = 1 << 10,
  SERVICE   = 1 << 11,
  COIN      = 1 << 12,
};

#endif
