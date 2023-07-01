#ifndef PINOUT_H_

#define PINOUT_H_

#include <stdint.h>

/* USER OPTIONS */

/* COLOR PALETTE */

/* rfid reader light color in reactive mode (see RGB.h for color definitions) */
#define RFID_COLOR         BLUE
/* Joystick RGB color used when moving joysticks in reactive mode (see RGB.h for color definitions) */
#define REACTIVE_PRIMARY   NOST_GREEN
/* Joystick RGB color used when pushing buttons in reactive mode (see RGB.h for color definitions) */
#define REACTIVE_SECONDARY WHITE
/* Joystick RGB color used when nothing is pushed in reactive mode (see RGB.h for color definitions) */
#define REACTIVE_TERNARY   BLACK

/* LIGHT OPTIONS */

/* button leds behavior in reactive mode */
#define BUTTON_LIGHT_RELEASED_STATE  OFF
#define BUTTON_LIGHT_PRESSED_STATE  ON

/* make reactive fade to ternary instead of an immediate transition */
#define REACTIVE_FADE 1
/* fade out speed (higher is slower) */
#define REACTIVE_FADERATE 60

/* delay (in seconds) before fallback to idle light animation (0 to disable idle animation) */
#define IDLE_ANIMATION_DELAY 10
/* button blink time (in seconds) for idle animation */
#define BUTTON_BLINK_TIME 1
/* rainbow animation parameters */
#define RAINBOW_NBSHADES 100
#define RAINBOW_WAVESPEED 10

/* PINS ASSIGNEMENT */

/* RGB pins (/!\ MUST USE PWM PINS /!\) */
#define PIN_JOY_RGB_R    3
#define PIN_JOY_RGB_G    4
#define PIN_JOY_RGB_B    5

#define PIN_RFID_RGB_R   18
#define PIN_RFID_RGB_G   19
#define PIN_RFID_RGB_B   20

/* Left joy pins */ /* switch : dpad lb */
#define PIN_LB_UP        6  /* dpad up */
#define PIN_LB_DOWN      7 /* dpad down */
#define PIN_LB_LEFT      8 /* dpad left */
#define PIN_LB_RIGHT     9  /* dpad right */
#define PIN_LB_BUTTON    14 /* L bumper */
#define PIN_LB_LED       17

/* Right joy pins */ /* switch : xbya rb */
#define PIN_RB_UP        10 /* X */
#define PIN_RB_DOWN      11  /* B */
#define PIN_RB_LEFT      12  /* Y */
#define PIN_RB_RIGHT     13 /* A */
#define PIN_RB_BUTTON    15 /* R bumper */
#define PIN_RB_LED       16

/* Additional button pins */
#define PIN_TEST         26  /* plus */
#define PIN_SERVICE      27  /* home */
#define PIN_COIN         28 /* minus */

/* g_button_state mapping */

#define NUM_BUTTONS 13
#define NUM_LIGHTS  8

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

// Switch buttons
#define DPAD_UP_MASK_ON 0x00
#define DPAD_UPRIGHT_MASK_ON 0x01
#define DPAD_RIGHT_MASK_ON 0x02
#define DPAD_DOWNRIGHT_MASK_ON 0x03
#define DPAD_DOWN_MASK_ON 0x04
#define DPAD_DOWNLEFT_MASK_ON 0x05
#define DPAD_LEFT_MASK_ON 0x06
#define DPAD_UPLEFT_MASK_ON 0x07
#define DPAD_NOTHING_MASK_ON 0x08
#define A_MASK_ON 0x04
#define B_MASK_ON 0x02
#define X_MASK_ON 0x08
#define Y_MASK_ON 0x01
#define LB_MASK_ON 0x10
#define RB_MASK_ON 0x20
#define ZL_MASK_ON 0x40
#define ZR_MASK_ON 0x80
#define START_MASK_ON 0x200
#define SELECT_MASK_ON 0x100
#define L3_MASK_ON 0x400
#define R3_MASK_ON 0x800
#define HOME_MASK_ON 0x1000
#define CAPTURE_MASK_ON 0x2000
// Generic XS pad status (follows nintendo switch convention (X = up / B = down))
#define BUTTONNONE -1
#define BUTTONUP 0
#define BUTTONDOWN 1
#define BUTTONLEFT 2
#define BUTTONRIGHT 3
#define BUTTONA 4
#define BUTTONB 5
#define BUTTONX 6
#define BUTTONY 7
#define BUTTONLB 8
#define BUTTONRB 9
#define BUTTONLT 10
#define BUTTONRT 11
#define BUTTONSTART 12
#define BUTTONSELECT 13
#define AXISLX 14
#define AXISLY 15
#define AXISRX 16
#define AXISRY 17
#define BUTTONL3 18
#define BUTTONR3 19
#define BUTTONHOME 20
#define BUTTONCAPTURE 21

/* manages the multitouch/hid lights layer */
/* takes care of managing a stack of fingers (using fingers.h) and convert it into hid reports */
extern uint32_t g_hid_last;

#define LED_POWER  25

#endif /* PINOUT_H_ */