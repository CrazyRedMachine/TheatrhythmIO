/*
  This class handles all HID preprocessing and transfer, for both the XInput and the NSwitch+Keyboard mode.
*/

#ifndef _FFT_HID_H_
#define _FFT_HID_H_

/* Includes: */
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <string.h>

#include "Descriptors.h"
#include "RGB.h"
#include "pinout.h"

#include <LUFA/LUFA/Drivers/USB/USB.h>
#include <LUFA/LUFA/Drivers/Board/Joystick.h>
#include <LUFA/LUFA/Drivers/Board/LEDs.h>
#include <LUFA/LUFA/Drivers/Board/Buttons.h>
#include <LUFA/LUFA/Platform/Platform.h>

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

// XInput digital_buttons_1
#define XBOX_DPAD_UP    0x01
#define XBOX_DPAD_DOWN    0x02
#define XBOX_DPAD_LEFT    0x04
#define XBOX_DPAD_RIGHT   0x08
#define XBOX_START      0x10
#define XBOX_BACK     0x20
#define XBOX_LEFT_STICK   0x40
#define XBOX_RIGHT_STICK  0x80

// XInput digital_buttons_2
#define XBOX_LB   0x01
#define XBOX_RB   0x02
#define XBOX_HOME 0x04
#define XBOX_A    0x10
#define XBOX_B    0x20
#define XBOX_X    0x40
#define XBOX_Y    0x80

// Joystick HID report structure. We have an input and an output.
typedef struct {
  uint16_t Button; // 16 buttons; see JoystickButtons_t for bit mapping
  uint8_t  HAT;    // HAT switch; one nibble w/ unused nibble
  uint8_t  LX;     // Left  Stick X
  uint8_t  LY;     // Left  Stick Y
  uint8_t  RX;     // Right Stick X
  uint8_t  RY;     // Right Stick Y
  uint8_t  VendorSpec;
} USB_JoystickReport_Input_t;

/* Type Defines: */
/** Type define for the joystick HID report structure, for creating and sending HID reports to the host PC.
    This mirrors the layout described to the host in the HID report descriptor, in Descriptors.c.
*/
typedef struct {
  uint8_t rid;
  uint8_t rsize;
  uint8_t digital_buttons_1;
  uint8_t digital_buttons_2;
  uint8_t lt;
  uint8_t rt;
  int l_x;
  int l_y;
  int r_x;
  int r_y;
  uint8_t reserved_1[6];
} USB_JoystickReport_XInput_t;

// The output is structured as a mirror of the input.
// This is based on initial observations of the Pokken Controller.
typedef struct {
  uint16_t Button; // 16 buttons; see JoystickButtons_t for bit mapping
  uint8_t  HAT;    // HAT switch; one nibble w/ unused nibble
  uint8_t  LX;     // Left  Stick X
  uint8_t  LY;     // Left  Stick Y
  uint8_t  RX;     // Right Stick X
  uint8_t  RY;     // Right Stick Y
} USB_JoystickReport_Output_t;

typedef struct {
  uint8_t report_id;
  uint8_t data[30];
} USB_LightReport_Output_t;

/* Function Prototypes: */
#ifdef __cplusplus
extern "C" {
#endif
// Setup all necessary hardware, including USB initialization.
void SetupHardware(bool xinput_mode);
// Process and deliver data from IN and OUT endpoints.
void HID_Task(void);
// USB device event handlers.
void EVENT_USB_Device_Connect(void);
void EVENT_USB_Device_Disconnect(void);
void EVENT_USB_Device_ConfigurationChanged(void);
void EVENT_USB_Device_ControlRequest(void);

unsigned long long get_last_hid_update();
void update_lamp(uint16_t button_state);
void send_pad_state(uint16_t button_state);
static void generate_report(uint16_t button_state);

#ifdef __cplusplus
}
#endif

#endif
