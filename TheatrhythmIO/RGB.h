#ifndef _RGB_H_
#define _RGB_H_

#define RED         ((color_t) { 0xFF, 0x00, 0x00 })
#define GREEN       ((color_t) { 0x00, 0xFF, 0x00 })
#define BLUE        ((color_t) { 0x00, 0x00, 0xFF })
#define YELLOW      ((color_t) { 0xFF, 0xFF, 0x00 })
#define MAGENTA     ((color_t) { 0xFF, 0x00, 0xFF })
#define CYAN        ((color_t) { 0x00, 0xFF, 0xFF })
#define WHITE       ((color_t) { 0xFF, 0xFF, 0xFF })
#define BLACK       ((color_t) { 0x00, 0x00, 0x00 })
#define LIGHT_BLUE  ((color_t) { 0x00, 0x7F, 0xFF })
#define NOST_GREEN  ((color_t) { 0x00, 0x7F, 0x23 })

/* Includes: */
#include <Arduino.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <string.h>

#include "pinout.h"

typedef struct color_s {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
} color_t;

/* Function Prototypes: */
#ifdef __cplusplus
extern "C" {
#endif
void RGB_setup();

void RGB_light(color_t color);
void RGB_rfid_light(color_t color);

void RGB_update_hid_data(uint8_t *data);
#ifdef __cplusplus
}
#endif
#endif
