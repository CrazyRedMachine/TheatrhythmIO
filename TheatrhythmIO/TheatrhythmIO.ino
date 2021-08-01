#include "pinout.h"
#include "LUFAConfig.h"
#include <LUFA.h>
#include "FFT_HID.h"
#define BOUNCE_WITH_PROMPT_DETECTION
#include <Bounce2.h>
#include <EEPROM.h>
#include <inttypes.h>

unsigned long startAndSelTime = 0;
unsigned long currTime = 0;

Bounce buttons[NUM_BUTTONS];

/* MODE DECLARATIONS */

/* mode selectors */
bool xinput;

void setupPins() {
  pinMode(PIN_LB_LED, OUTPUT);
  pinMode(PIN_RB_LED, OUTPUT);
  digitalWrite(PIN_LB_LED, HIGH);
  digitalWrite(PIN_RB_LED, HIGH);

  uint8_t pins[NUM_BUTTONS] = {PIN_LB_UP, PIN_LB_DOWN, PIN_LB_LEFT, PIN_LB_RIGHT, PIN_LB_BUTTON, PIN_RB_UP, PIN_RB_DOWN, PIN_RB_LEFT, PIN_RB_RIGHT, PIN_RB_BUTTON, PIN_TEST, PIN_SERVICE, PIN_COIN};
  for (int i = 0; i < NUM_BUTTONS; i++)
  {
    buttons[i] = Bounce();
    buttons[i].attach(pins[i], INPUT_PULLUP);
    buttons[i].interval(MILLIDEBOUNCE);
  }
}
void setup() {

  EEPROM.get(2, xinput);
  setupPins();
  delay(500);

  RGB_setup();

#ifdef DISABLE_NSWITCH
#warning "NSWITCH mode is disabled, will act only as an XInput controller"
  /* force Xinput */
  xinput = true;
#else
#ifdef DISABLE_XINPUT
#warning "XINPUT mode is disabled, will act only as a Nintendo switch controller"
  /* force nswitch */
  xinput = false;
#else
  /* set xinput mode according to held button */
  // if select is held on boot, NSWitch mode
  int value = digitalRead(PIN_COIN);
  if (value == LOW)
  {
    xinput = false;
    EEPROM.put(2, xinput);
  }
  // if start is held on boot, XInput mode
  else {
    value = digitalRead(PIN_TEST);
    if (value == LOW)
    {
      xinput = true;
      EEPROM.put(2, xinput);
    }
  }
#endif
#endif
  SetupHardware(xinput);
  GlobalInterruptEnable();
}


void loop() {
  uint16_t button_state = 0;
  currTime = millis();
  buttonRead(&button_state);
  send_pad_state(button_state);
  update_lamp(button_state);
}

void buttonRead(uint16_t *button_state)
{
  uint32_t buttonsState = 0;
  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttons[i].update();
    int value = buttons[i].read();
    if (value != HIGH) {
      *button_state |= (uint32_t)1 << i;
    } else {
      *button_state &= ~((uint32_t)1 << i);
    }
  }
}
