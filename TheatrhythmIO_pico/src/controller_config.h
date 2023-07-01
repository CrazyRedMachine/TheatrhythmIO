#ifndef controller_config_h
#define controller_config_h
#include <stdint.h>
#include <stdbool.h>
#include "tusb.h"

#define REACTIVE_TIMEOUT_MS 3000

#define REACTIVE_FALLBACK(x) (!x || ((board_millis() - x) >= REACTIVE_TIMEOUT_MS))

/* USB CONFIGURATION */
#define REPORTID_JOY                 0x01
#define REPORTID_LIGHT_INPUT         0x03
#define REPORTID_LIGHT_OUTPUT        0x04

#pragma pack(1)
typedef enum hid_mode_e {
	HID_MODE_IGNORE,
	HID_MODE_SWAP,
	HID_MODE_COMBINE
} hid_mode_t;

#endif