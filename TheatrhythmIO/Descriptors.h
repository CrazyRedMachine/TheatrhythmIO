#ifndef _DESCRIPTORS_H_
#define _DESCRIPTORS_H_

// Includes
#include "config.h"
#include "LUFAConfig.h"
#include <avr/pgmspace.h>
#include <LUFA/LUFA/Drivers/USB/USB.h>

void desc_set_xinput_mode(bool value);

// Type Defines
// Device Configuration Descriptor Structure
typedef struct
{
  USB_Descriptor_Configuration_Header_t Config;

  // Joystick HID Interface
  USB_Descriptor_Interface_t            HID_Interface;
  USB_HID_Descriptor_HID_t              HID_JoystickHID;
  USB_Descriptor_Endpoint_t             HID_ReportOUTEndpoint;
  USB_Descriptor_Endpoint_t             HID_ReportINEndpoint;
  // Light
  USB_Descriptor_Interface_t            HID_LightInterface;
  USB_HID_Descriptor_HID_t              HID_LightHID;
  USB_Descriptor_Endpoint_t             HID_LightReportOUTEndpoint;
  USB_Descriptor_Endpoint_t             HID_LightReportINEndpoint;

#ifdef WITH_KEYBOARD
  USB_Descriptor_Interface_t            HID1_KeyboardInterface;
  USB_HID_Descriptor_HID_t              HID1_KeyboardHID;
  USB_Descriptor_Endpoint_t             HID1_ReportOUTEndpoint;
  USB_Descriptor_Endpoint_t             HID1_ReportINEndpoint;
#endif
} USB_Descriptor_Configuration_t;

// Device Interface Descriptor IDs
enum InterfaceDescriptors_t
{
  INTERFACE_ID_Joystick = 0, /**< Joystick interface descriptor ID */
  INTERFACE_ID_Light = 1, /**< Lights interface descriptor ID */
#ifdef WITH_KEYBOARD
  INTERFACE_ID_Keyboard = 2,
#endif
};

// Device String Descriptor IDs
enum StringDescriptors_t
{
  STRING_ID_Language     = 0, // Supported Languages string descriptor ID (must be zero)
  STRING_ID_Manufacturer = 1, // Manufacturer string ID
  STRING_ID_Product      = 2, // Product string ID
  STRING_ID_Version      = 3, // Version string ID
  STRING_ID_LED_Base     = 4,
};

#define STRING_ID_LED_Count     8

// Macros
// Endpoint Addresses
#define JOYSTICK_IN_EPADDR  (ENDPOINT_DIR_IN  | 3)
#define JOYSTICK_OUT_EPADDR (ENDPOINT_DIR_OUT | 4)
#define LIGHT_IN_EPADDR  (ENDPOINT_DIR_IN  | 1)
#define LIGHT_OUT_EPADDR (ENDPOINT_DIR_OUT | 2)
#define LIGHT_EPSIZE           64
#ifdef WITH_KEYBOARD
#define KEYBOARD_IN_EPADDR  (ENDPOINT_DIR_IN  | 5)
#define KEYBOARD_OUT_EPADDR (ENDPOINT_DIR_OUT | 6)
#define KEYBOARD_EPSIZE           8
#endif
// HID Endpoint Size
// The Switch -needs- this to be 64.
// The Wii U is flexible, allowing us to use the default of 8 (which did not match the original Hori descriptors).
#define JOYSTICK_EPSIZE           64
#define JOYSTICK_EPSIZE_XINPUT           20

/* Function Prototypes: */
uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,
                                    const uint16_t wIndex,
                                    const void** const DescriptorAddress)
ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(3);

#endif
