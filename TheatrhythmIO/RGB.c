// avec fastio
// 40 µs pour une led (1µs par bit ecrit)
// 5 * 8 bit * 1µs = 40
// sans fastio
// 1ms pour écrire une led (3 couleurs)
// 5 * 8bit * 25µs = 1000

#include "RGB.h"

#ifdef WITH_FASTIO
#include "digitalWriteFast.h"
#define IO_WRITE(X, Y) digitalWriteFast(X, Y)
#define IO_MODE(X, Y) pinModeFast(X, Y)
#else
#define IO_WRITE(X, Y) digitalWrite(X, Y)
#define IO_MODE(X, Y) pinMode(X, Y)
#endif

uint8_t g_global_brightness = 0;

void RGB_update_hid_data(uint8_t *data)
{
  color_t hid_color;
  hid_color.red = data[0];
  hid_color.green = data[1];
  hid_color.blue = data[2];
  
  RGB_light(hid_color);

  hid_color.red = data[3];
  hid_color.green = data[4];
  hid_color.blue = data[5];

  RGB_rfid_light(hid_color);
  
  /* button leds */
  if (data[6])
  {
    IO_WRITE(PIN_LB_LED, LOW);
  }
  else
  {
    IO_WRITE(PIN_LB_LED, HIGH);
  }

  if (data[7])
  {
    IO_WRITE(PIN_RB_LED, LOW);
  }
  else
  {
    IO_WRITE(PIN_RB_LED, HIGH);
  }
  return;
}

#define INVERT 255 -
void RGB_light(color_t color)
{
  analogWrite(PIN_JOY_RGB_R, INVERT color.red);
  analogWrite(PIN_JOY_RGB_G, INVERT color.green);
  analogWrite(PIN_JOY_RGB_B, INVERT color.blue);
}

void RGB_rfid_light(color_t color)
{
  //RFID blue is much dimmer than red and green, therefore i'm scaling them down to restore color balance
  analogWrite(PIN_RFID_RGB_R, (INVERT color.red*0.2));
  analogWrite(PIN_RFID_RGB_G, (INVERT color.green*0.2));
  analogWrite(PIN_RFID_RGB_B, INVERT color.blue);
}
#undef INVERT

void RGB_setup()
{
  IO_MODE(PIN_JOY_RGB_R, OUTPUT);
  IO_MODE(PIN_JOY_RGB_G, OUTPUT);
  IO_MODE(PIN_JOY_RGB_B, OUTPUT);
  IO_MODE(PIN_RFID_RGB_R, OUTPUT);
  IO_MODE(PIN_RFID_RGB_G, OUTPUT);
  IO_MODE(PIN_RFID_RGB_B, OUTPUT);

  IO_WRITE(PIN_JOY_RGB_R, HIGH);
  IO_WRITE(PIN_JOY_RGB_G, HIGH);
  IO_WRITE(PIN_JOY_RGB_B, HIGH);
  IO_WRITE(PIN_RFID_RGB_R, HIGH);
  IO_WRITE(PIN_RFID_RGB_G, HIGH);
  IO_WRITE(PIN_RFID_RGB_B, HIGH);
}
