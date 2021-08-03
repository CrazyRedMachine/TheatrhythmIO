#include "FFT_HID.h"
static USB_JoystickReport_Input_t ReportData;
static USB_KeyboardReport_Data_t KeyboardReportData;
static USB_JoystickReport_XInput_t XInput_ReportData;
static unsigned long last_hid_update;
static unsigned long last_button_update;
static bool xs_xinput;

// Configures hardware and peripherals, such as the USB peripherals.
void SetupHardware(bool xinput_mode) {

  //set xinput_mode for descriptors.h/.c
  xs_xinput = xinput_mode;
  desc_set_xinput_mode(xinput_mode);
  // We need to disable watchdog if enabled by bootloader/fuses.
  MCUSR &= ~(1 << WDRF);
  wdt_disable();
  // We need to disable clock division before initializing the USB hardware.
  clock_prescale_set(clock_div_1);
  // We can then initialize our hardware and peripherals, including the USB stack.
  // The USB stack should be initialized last.
  USB_Init();
}

// Fired to indicate that the device is enumerating.
void EVENT_USB_Device_Connect(void) {
  // We can indicate that we're enumerating here (via status LEDs, sound, etc.).
}

// Fired to indicate that the device is no longer connected to a host.
void EVENT_USB_Device_Disconnect(void) {
  // We can indicate that our device is not ready (via status LEDs, sound, etc.).
}

// Fired when the host set the current configuration of the USB device after enumeration.
void EVENT_USB_Device_ConfigurationChanged(void) {
  bool ConfigSuccess = true;

  if (!xs_xinput) ConfigSuccess &= Endpoint_ConfigureEndpoint(JOYSTICK_OUT_EPADDR, EP_TYPE_INTERRUPT, JOYSTICK_EPSIZE, 1);
  else ConfigSuccess &= Endpoint_ConfigureEndpoint((ENDPOINT_DIR_IN | 3), EP_TYPE_INTERRUPT, 32, 1);

  ConfigSuccess &= Endpoint_ConfigureEndpoint(JOYSTICK_IN_EPADDR, EP_TYPE_INTERRUPT, xs_xinput ? JOYSTICK_EPSIZE_XINPUT : JOYSTICK_EPSIZE, 1);

  if (!xs_xinput)
  {
    ConfigSuccess &= Endpoint_ConfigureEndpoint(LIGHT_OUT_EPADDR, EP_TYPE_INTERRUPT, LIGHT_EPSIZE, 1);
    ConfigSuccess &= Endpoint_ConfigureEndpoint(LIGHT_IN_EPADDR, EP_TYPE_INTERRUPT, LIGHT_EPSIZE, 1);

#ifdef WITH_KEYBOARD
    ConfigSuccess &= Endpoint_ConfigureEndpoint(KEYBOARD_OUT_EPADDR, EP_TYPE_INTERRUPT, KEYBOARD_EPSIZE, 1);
    ConfigSuccess &= Endpoint_ConfigureEndpoint(KEYBOARD_IN_EPADDR, EP_TYPE_INTERRUPT, KEYBOARD_EPSIZE, 1);
#endif
  }

}

// Process control requests sent to the device from the USB host.
void EVENT_USB_Device_ControlRequest(void) {
  //No controlRequest received from the switch, so only handled in xinput mode
  if (xs_xinput) {
    /* Handle HID Class specific requests */
    switch (USB_ControlRequest.bRequest) {
      case HID_REQ_GetReport:
        if (USB_ControlRequest.bmRequestType
            == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE)) {
          Endpoint_ClearSETUP();

          /* Write the report data to the control endpoint */
          Endpoint_Write_Control_Stream_LE(&XInput_ReportData, 20);
          Endpoint_ClearOUT();
        }
        break;
    }
  }
}

// Process and deliver data from IN and OUT endpoints.
void HID_Task(void) {
  /* Device must be connected and configured for the task to run */
  if (USB_DeviceState != DEVICE_STATE_Configured)
    return;

  void* Address = &XInput_ReportData;
  uint16_t    Size    = 20;

  //no OUT endpoint for xinput in this firmware
  if (!xs_xinput) {
    Address = &ReportData;
    Size = sizeof(ReportData);

    /* HID Lamps data */
    // We'll start with the OUT endpoint.
    Endpoint_SelectEndpoint(LIGHT_OUT_EPADDR);
    // We'll check to see if we received something on the OUT endpoint.
    if (Endpoint_IsOUTReceived())
    {
      // If we did, and the packet has data, we'll react to it.
      if (Endpoint_IsReadWriteAllowed())
      {
        // We'll create a place to store our data received from the host.
        USB_LightReport_Output_t LightOutputData;
        // We'll then take in that data, setting it up in our storage.
        Endpoint_Read_Stream_LE(&LightOutputData, sizeof(LightOutputData), NULL);
        // At this point, we can react to this data.
        last_hid_update = millis();
        if (LightOutputData.report_id == 5)
        {
          RGB_update_hid_data(LightOutputData.data);
        }
      }
      // Regardless of whether we reacted to the data, we acknowledge an OUT packet on this endpoint.
      Endpoint_ClearOUT();
    }

    // We'll start with the OUT endpoint.
    Endpoint_SelectEndpoint(JOYSTICK_OUT_EPADDR);
    // We'll check to see if we received something on the OUT endpoint.
    if (Endpoint_IsOUTReceived())
    {
      // If we did, and the packet has data, we'll react to it.
      if (Endpoint_IsReadWriteAllowed())
      {
        // We'll create a place to store our data received from the host.
        USB_JoystickReport_Output_t JoystickOutputData;
        // We'll then take in that data, setting it up in our storage.
        Endpoint_Read_Stream_LE(&JoystickOutputData, sizeof(JoystickOutputData), NULL);
        // At this point, we can react to this data.
        // However, since we're not doing anything with this data, we abandon it.
      }
      // Regardless of whether we reacted to the data, we acknowledge an OUT packet on this endpoint.
      Endpoint_ClearOUT();
    }

  }
  /* Select the Joystick Report Endpoint */
  Endpoint_SelectEndpoint(JOYSTICK_IN_EPADDR);

  /* Check to see if the host is ready for another packet */
  if (Endpoint_IsINReady()) {
    /* Write Joystick Report Data */
    Endpoint_Write_Stream_LE(Address, Size, NULL);

    /* Finalize the stream transfer to send the last packet */
    Endpoint_ClearIN();
    /* Clear the report data afterwards */
    memset(Address, 0, Size);
    if (xs_xinput) (*((USB_JoystickReport_XInput_t *) Address)).rsize = 20;
  }
}

void send_pad_state(uint16_t button_state) {
  static uint16_t last_button_state = 0;
  if (button_state != last_button_state)
  {
    last_button_state = button_state;
    last_button_update = millis();
  }
  generate_report(button_state);
  HID_Task();
#ifdef WITH_KEYBOARD
  Keyboard_HID_Task();
#endif
  USB_USBTask();
}

void generate_report_xinput(uint16_t button_state) {

  // analogs
  XInput_ReportData.l_x = 128 * 257 + -32768;
  XInput_ReportData.l_y = 128 * -257 + 32767;
  XInput_ReportData.r_x = 128 * 257 + -32768;
  XInput_ReportData.r_y = 128 * -257 + 32767;

  // left booster
  if (button_state & LB_UP) {
    XInput_ReportData.digital_buttons_1 |= XBOX_DPAD_UP;
  }
  else if (button_state & LB_DOWN) {
    XInput_ReportData.digital_buttons_1 |= XBOX_DPAD_DOWN;
  }
  if (button_state & LB_LEFT) {
    XInput_ReportData.digital_buttons_1 |= XBOX_DPAD_LEFT;
  }
  else if (button_state & LB_RIGHT) {
    XInput_ReportData.digital_buttons_1 |= XBOX_DPAD_RIGHT;
  }

  if (button_state & LB_BUTTON) {
    XInput_ReportData.digital_buttons_2 |= XBOX_LB;
  }

  // right booster
  if (button_state & RB_UP) {
    XInput_ReportData.digital_buttons_2 |= XBOX_Y;
  }
  else if (button_state & RB_DOWN) {
    XInput_ReportData.digital_buttons_2 |= XBOX_A;
  }
  if (button_state & RB_LEFT) {
    XInput_ReportData.digital_buttons_2 |= XBOX_X;
  }
  else if (button_state & RB_RIGHT) {
    XInput_ReportData.digital_buttons_2 |= XBOX_B;
  }

  if (button_state & RB_BUTTON) {
    XInput_ReportData.digital_buttons_2 |= XBOX_RB;
  }

  // Buttons
  if (button_state & TEST) {
    XInput_ReportData.digital_buttons_1 |= XBOX_START;
  }
  if (button_state & SERVICE) {
    XInput_ReportData.digital_buttons_2 |= XBOX_HOME;
  }
  if (button_state & COIN) {
    XInput_ReportData.digital_buttons_1 |= XBOX_BACK;
  }
}

void generate_report_switch(uint16_t button_state) {

  // analogs
  ReportData.LX = 128;
  ReportData.LY = 128;
  ReportData.RX = 128;
  ReportData.RY = 128;

  // left booster
  if ((button_state & LB_UP) && (button_state & LB_RIGHT)) {
    ReportData.HAT = DPAD_UPRIGHT_MASK_ON;
  }
  else if ((button_state & LB_DOWN) && (button_state & LB_RIGHT)) {
    ReportData.HAT = DPAD_DOWNRIGHT_MASK_ON;
  }
  else if ((button_state & LB_DOWN) && (button_state & LB_LEFT)) {
    ReportData.HAT = DPAD_DOWNLEFT_MASK_ON;
  }
  else if ((button_state & LB_UP) && (button_state & LB_LEFT)) {
    ReportData.HAT = DPAD_UPLEFT_MASK_ON;
  }
  else if (button_state & LB_UP) {
    ReportData.HAT = DPAD_UP_MASK_ON;
  }
  else if (button_state & LB_DOWN) {
    ReportData.HAT = DPAD_DOWN_MASK_ON;
  }
  else if (button_state & LB_LEFT) {
    ReportData.HAT = DPAD_LEFT_MASK_ON;
  }
  else if (button_state & LB_RIGHT) {
    ReportData.HAT = DPAD_RIGHT_MASK_ON;
  }
  else {
    ReportData.HAT = DPAD_NOTHING_MASK_ON;
  }

  if (button_state & LB_BUTTON) {
    ReportData.Button |= LB_MASK_ON;
  }

  // right booster
  if (button_state & RB_UP) {
    ReportData.Button |= X_MASK_ON;
  }
  else if (button_state & RB_DOWN) {
    ReportData.Button |= B_MASK_ON;
  }
  if (button_state & RB_LEFT) {
    ReportData.Button |= Y_MASK_ON;
  }
  else if (button_state & RB_RIGHT) {
    ReportData.Button |= A_MASK_ON;
  }

  if (button_state & RB_BUTTON) {
    ReportData.Button |= RB_MASK_ON;
  }

  // Buttons
  if (button_state & TEST) {
    ReportData.Button |= START_MASK_ON;
  }
  if (button_state & SERVICE) {
    ReportData.Button |= HOME_MASK_ON;
  }
  if (button_state & COIN) {
    ReportData.Button |= SELECT_MASK_ON;
  }
}

#ifdef WITH_KEYBOARD
void generate_report_keyboard(uint16_t button_state) {
  // left booster
  if (button_state & LB_UP) {
    KeyboardReportData.KeyCode[0] = KEY_LB_UP;
  }
  else if (button_state & LB_DOWN) {
    KeyboardReportData.KeyCode[0] = KEY_LB_DOWN;
  }
  if (button_state & LB_LEFT) {
    KeyboardReportData.KeyCode[1] = KEY_LB_LEFT;
  }
  else if (button_state & LB_RIGHT) {
    KeyboardReportData.KeyCode[1] = KEY_LB_RIGHT;
  }

  if (button_state & LB_BUTTON) {
#ifdef LB_BUTTON_IS_MODIFIER
    KeyboardReportData.Modifier = KEY_LB_BUTTON;
#else
    KeyboardReportData.KeyCode[4] = KEY_LB_BUTTON;
#endif
  }

  // right booster
  if (button_state & RB_UP) {
    KeyboardReportData.KeyCode[3] = KEY_RB_UP;
  }
  else if (button_state & RB_DOWN) {
    KeyboardReportData.KeyCode[3] = KEY_RB_DOWN;
  }
  if (button_state & RB_LEFT) {
    KeyboardReportData.KeyCode[2] = KEY_RB_LEFT;
  }
  else if (button_state & RB_RIGHT) {
    KeyboardReportData.KeyCode[2] = KEY_RB_RIGHT;
  }

  if (button_state & RB_BUTTON) {
#ifdef RB_BUTTON_IS_MODIFIER
    KeyboardReportData.Modifier = KEY_RB_BUTTON;
#else
    KeyboardReportData.KeyCode[5] = KEY_RB_BUTTON;
#endif
  }

  // Buttons
  if (button_state & TEST) {
    KeyboardReportData.KeyCode[0] = KEY_TEST;
  }
  if (button_state & SERVICE) {
    KeyboardReportData.KeyCode[0] = KEY_SERVICE;
  }
  if (button_state & COIN) {
    KeyboardReportData.KeyCode[0] = KEY_COIN;
  }

}

/** Processes a given Keyboard LED report from the host, and sets the board LEDs to match. Since the Keyboard
    LED report can be sent through either the control endpoint (via a HID SetReport request) or the HID OUT
    endpoint, the processing code is placed here to avoid duplicating it and potentially having different
    behavior depending on the method used to sent it.
*/
void Keyboard_ProcessLEDReport(const uint8_t LEDStatus)
{
  uint8_t LEDMask = LEDS_LED2;

  if (LEDStatus & HID_KEYBOARD_LED_NUMLOCK)
    LEDMask |= LEDS_LED1;

  if (LEDStatus & HID_KEYBOARD_LED_CAPSLOCK)
    LEDMask |= LEDS_LED3;

  if (LEDStatus & HID_KEYBOARD_LED_SCROLLLOCK)
    LEDMask |= LEDS_LED4;

  /* Set the status LEDs to the current Keyboard LED status */
  LEDs_SetAllLEDs(LEDMask);
}

void Keyboard_HID_Task(void)
{

  /* Device must be connected and configured for the task to run */
  if (USB_DeviceState != DEVICE_STATE_Configured)
    return;

  /* Select the Keyboard Report Endpoint */
  Endpoint_SelectEndpoint(KEYBOARD_IN_EPADDR);

  /* Check if Keyboard Endpoint Ready for Read/Write */
  if (Endpoint_IsReadWriteAllowed())
  {
    /* Write Keyboard Report Data */
    Endpoint_Write_Stream_LE(&KeyboardReportData, sizeof(KeyboardReportData), NULL);

    /* Finalize the stream transfer to send the last packet */
    Endpoint_ClearIN();

    /* Clear the report data afterwards */
    memset(&KeyboardReportData, 0, sizeof(KeyboardReportData));
  }

  /* Select the Keyboard LED Report Endpoint */
  Endpoint_SelectEndpoint(KEYBOARD_OUT_EPADDR);

  /* Check if Keyboard LED Endpoint Ready for Read/Write */
  if (Endpoint_IsReadWriteAllowed())
  {
    /* Read in and process the LED report from the host */
    Keyboard_ProcessLEDReport(Endpoint_Read_8());

    /* Handshake the OUT Endpoint - clear endpoint and ready for next report */
    Endpoint_ClearOUT();
  }
}
#endif

static void generate_report(uint16_t button_state) {
  if (xs_xinput) generate_report_xinput(button_state);
  else
  {
    generate_report_switch(button_state);
#ifdef WITH_KEYBOARD
    generate_report_keyboard(button_state);
#endif
  }
}


const uint8_t HSVpower[121] =
{ 0, 2, 4, 6, 8, 11, 13, 15, 17, 19, 21, 23, 25, 28, 30, 32, 34, 36, 38, 40,
  42, 45, 47, 49, 51, 53, 55, 57, 59, 62, 64, 66, 68, 70, 72, 74, 76, 79, 81,
  83, 85, 87, 89, 91, 93, 96, 98, 100, 102, 104, 106, 108, 110, 113, 115, 117,
  119, 121, 123, 125, 127, 130, 132, 134, 136, 138, 140, 142, 144, 147, 149,
  151, 153, 155, 157, 159, 161, 164, 166, 168, 170, 172, 174, 176, 178, 181,
  183, 185, 187, 189, 191, 193, 195, 198, 200, 202, 204, 206, 208, 210, 212,
  215, 217, 219, 221, 223, 225, 227, 229, 232, 234, 236, 238, 240, 242, 244,
  246, 249, 251, 253, 255
};
#define NBSHADES 40
#define WAVESPEED 32
static color_t color_rainbow(uint16_t button_state)
{
  /* button position in ordering */
  static unsigned long startTime = 0;
  uint16_t angle;
  angle = ((((millis() - startTime) / (1000 / WAVESPEED))) % NBSHADES) * 360 / NBSHADES;
  byte r, g, b;
  if (angle < 120) {
    r = HSVpower[120 - angle];
    g = HSVpower[angle];
    b = 0;
  }
  else if (angle < 240) {
    r = 0;
    g = HSVpower[240 - angle];
    b = HSVpower[angle - 120];
  }
  else {
    r = HSVpower[angle - 240];
    g = 0;
    b = HSVpower[360 - angle];
  }

  color_t res;
  res.red = r;
  res.green = g;
  res.blue = b;

  return res;
}
#undef NBSHADES
#undef WAVESPEED

static color_t color_fade(uint16_t button_state)
{
  static byte fade_cooldown = 40;
  static uint16_t fade = 0;
  static color_t color = BLACK;
  color_t bgcolor = REACTIVE_TERNARY;
  
  if (fade > 0)
  {
    fade_cooldown--;
    if (fade_cooldown == 0)
    {
      if (fade > 0) fade--;
      fade_cooldown = 40                                                                                                                           ;
    }
  }

  if (button_state & (LB_BUTTON) || button_state & (RB_BUTTON))
  {
    fade = REACTIVE_FADERATE;
    color = REACTIVE_SECONDARY;
  }
  else if (button_state)
  {
    fade = REACTIVE_FADERATE;
    color = REACTIVE_PRIMARY;
  }

  float brit = fade * 255 / REACTIVE_FADERATE;
  float amount = brit / 255.;
  color_t res = {color.red* amount + bgcolor.red*(1-amount), color.green* amount + bgcolor.green*(1-amount), color.blue * amount + bgcolor.blue*(1-amount)};
  return res;
}

void update_lamp(uint16_t button_state)
{
  static bool startup = true;
  unsigned long now = millis();

  static unsigned long idle_but_led_update_time = 0;

  /* HID IN PROGRESS */
  if (now - last_hid_update <= 3000) {
    return;
  }

  color_t (*update_func)(uint16_t);
  /* STARTUP/IDLE ANIM */
  if (last_button_update != 0)
    startup = false;

#if IDLE_ANIMATION_DELAY != 0
  if (startup || (!button_state) && (now - last_button_update >= IDLE_ANIMATION_DELAY * 1000))
  {
    update_func = color_rainbow;
    RGB_rfid_light(update_func(button_state));

    if (now - idle_but_led_update_time <= BUTTON_BLINK_TIME * 1000) {
      digitalWrite(PIN_LB_LED, LOW);
      digitalWrite(PIN_RB_LED, HIGH);
    }
    else if (now - idle_but_led_update_time <= 2 * BUTTON_BLINK_TIME * 1000)
    {
      digitalWrite(PIN_RB_LED, LOW);
      digitalWrite(PIN_LB_LED, HIGH);
    }
    else
    {
      idle_but_led_update_time = now;
    }
  }
  else 
#endif /* IDLE ANIMATION */
    /* REACTIVE MODE */
  {
    
    RGB_rfid_light(RFID_COLOR);
    
#if REACTIVE_FADE == 0
    digitalWrite(PIN_LB_LED, LOW);
    digitalWrite(PIN_RB_LED, LOW);
    RGB_light(REACTIVE_PRIMARY);
    return;
#else
    if (button_state & LB_BUTTON) digitalWrite(PIN_LB_LED, LOW);
    else digitalWrite(PIN_LB_LED, HIGH);

    if (button_state & RB_BUTTON) digitalWrite(PIN_RB_LED, LOW);
    else digitalWrite(PIN_RB_LED, HIGH);

    update_func = color_fade;
#endif

  }

  /* UPDATE ACCORDINGLY */
  RGB_light(update_func(button_state));

}
