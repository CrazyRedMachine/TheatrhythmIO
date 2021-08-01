#include "Descriptors.h"

bool g_xinput_mode = false;

void desc_set_xinput_mode(bool value) {
  g_xinput_mode = value;
}

// HID Descriptors.

const USB_Descriptor_HIDReport_Datatype_t PROGMEM JoystickReport[] = {
  HID_RI_USAGE_PAGE(8, 1), /* Generic Desktop */
  HID_RI_USAGE(8, 5), /* Joystick */
  HID_RI_COLLECTION(8, 1), /* Application */
  // Buttons (2 bytes)
  HID_RI_LOGICAL_MINIMUM(8, 0),
  HID_RI_LOGICAL_MAXIMUM(8, 1),
  HID_RI_PHYSICAL_MINIMUM(8, 0),
  HID_RI_PHYSICAL_MAXIMUM(8, 1),
  // The Switch will allow us to expand the original HORI descriptors to a full 16 buttons.
  // The Switch will make use of 14 of those buttons.
  HID_RI_REPORT_SIZE(8, 1),
  HID_RI_REPORT_COUNT(8, 16),
  HID_RI_USAGE_PAGE(8, 9),
  HID_RI_USAGE_MINIMUM(8, 1),
  HID_RI_USAGE_MAXIMUM(8, 16),
  HID_RI_INPUT(8, 2),
  // HAT Switch (1 nibble)
  HID_RI_USAGE_PAGE(8, 1),
  HID_RI_LOGICAL_MAXIMUM(8, 7),
  HID_RI_PHYSICAL_MAXIMUM(16, 315),
  HID_RI_REPORT_SIZE(8, 4),
  HID_RI_REPORT_COUNT(8, 1),
  HID_RI_UNIT(8, 20),
  HID_RI_USAGE(8, 57),
  HID_RI_INPUT(8, 66),
  // There's an additional nibble here that's utilized as part of the Switch Pro Controller.
  // I believe this -might- be separate U/D/L/R bits on the Switch Pro Controller, as they're utilized as four button descriptors on the Switch Pro Controller.
  HID_RI_UNIT(8, 0),
  HID_RI_REPORT_COUNT(8, 1),
  HID_RI_INPUT(8, 1),
  // Joystick (4 bytes)
  HID_RI_LOGICAL_MAXIMUM(16, 255),
  HID_RI_PHYSICAL_MAXIMUM(16, 255),
  HID_RI_USAGE(8, 48),
  HID_RI_USAGE(8, 49),
  HID_RI_USAGE(8, 50),
  HID_RI_USAGE(8, 53),
  HID_RI_REPORT_SIZE(8, 8),
  HID_RI_REPORT_COUNT(8, 4),
  HID_RI_INPUT(8, 2),
  // ??? Vendor Specific (1 byte)
  // This byte requires additional investigation.
  HID_RI_USAGE_PAGE(16, 65280),
  HID_RI_USAGE(8, 32),
  HID_RI_REPORT_COUNT(8, 1),
  HID_RI_INPUT(8, 2),
  // Output (8 bytes)
  // Original observation of this suggests it to be a mirror of the inputs that we sent.
  // The Switch requires us to have these descriptors available.
  HID_RI_USAGE(16, 9761),
  HID_RI_REPORT_COUNT(8, 8),
  HID_RI_OUTPUT(8, 2),
  HID_RI_END_COLLECTION(0),
};

const USB_Descriptor_HIDReport_Datatype_t PROGMEM LEDReport[] =
{
  HID_RI_USAGE_PAGE(8, 0x01), /* Generic Desktop */
  HID_RI_USAGE(8, 0x00), /* Undefined */
  HID_RI_COLLECTION(8, 0x01), /* Application */

  /* Joystick RGB */

  /*Lights */
  0x85, 0x05,                    /*   REPORT_ID 5*/
  0x15, 0x00,                    /*     LOGICAL_MINIMUM (0) */
  0x26, 0xFF, 0x00,              /*     LOGICAL_MAXIMUM (255) */
  /* Joy Red */
  0x05, 0x0a,                    /*     USAGE_PAGE (Ordinals) */
  0x09, 0x01,                    /*     USAGE (Instance 1) */
  0xa1, 0x02,                    /*     COLLECTION (Logical) */
  0x05, 0x08,                    /*       USAGE_PAGE (LEDs) */
  0x09, 0x4b,                    /*       USAGE (Generic Indicator 1) */
  0x79, 0x04,                    /*       STRING INDEX (4) */
  0x75, 0x08,                    /*       REPORT_SIZE (8) */
  0x95, 0x01,                    /*       REPORT_COUNT (1) */
  0x91, 0x02,                    /*       OUTPUT (Data,Var,Abs) */
  0xc0,                          /*     END_COLLECTION */
  /* Joy Green */
  0x05, 0x0a,                    /*     USAGE_PAGE (Ordinals) */
  0x09, 0x02,                    /*     USAGE (Instance 2) */
  0xa1, 0x02,                    /*     COLLECTION (Logical) */
  0x05, 0x08,                    /*       USAGE_PAGE (LEDs) */
  0x09, 0x4b,                    /*       USAGE (Generic Indicator 1) */
  0x79, 0x05,                    /*       STRING INDEX (5) */
  0x75, 0x08,                    /*       REPORT_SIZE (8) */
  0x95, 0x01,                    /*       REPORT_COUNT (1) */
  0x91, 0x02,                    /*       OUTPUT (Data,Var,Abs) */
  0xc0,                          /*     END_COLLECTION */
  /* Joy Blue */
  0x05, 0x0a,                    /*     USAGE_PAGE (Ordinals) */
  0x09, 0x03,                    /*     USAGE (Instance 3) */
  0xa1, 0x02,                    /*     COLLECTION (Logical) */
  0x05, 0x08,                    /*       USAGE_PAGE (LEDs) */
  0x09, 0x4b,                    /*       USAGE (Generic Indicator 1) */
  0x79, 0x06,                    /*       STRING INDEX (6) */
  0x75, 0x08,                    /*       REPORT_SIZE (8) */
  0x95, 0x01,                    /*       REPORT_COUNT (1) */
  0x91, 0x02,                    /*       OUTPUT (Data,Var,Abs) */
  0xc0,                          /*     END_COLLECTION */
  /* RFID Red */
  0x05, 0x0a,                    /*     USAGE_PAGE (Ordinals) */
  0x09, 0x04,                    /*     USAGE (Instance 4) */
  0xa1, 0x02,                    /*     COLLECTION (Logical) */
  0x05, 0x08,                    /*       USAGE_PAGE (LEDs) */
  0x09, 0x4b,                    /*       USAGE (Generic Indicator 1) */
  0x79, 0x07,                    /*       STRING INDEX (7) */
  0x75, 0x08,                    /*       REPORT_SIZE (8) */
  0x95, 0x01,                    /*       REPORT_COUNT (1) */
  0x91, 0x02,                    /*       OUTPUT (Data,Var,Abs) */
  0xc0,                          /*     END_COLLECTION */
  /* RFID Green */
  0x05, 0x0a,                    /*     USAGE_PAGE (Ordinals) */
  0x09, 0x05,                    /*     USAGE (Instance 5) */
  0xa1, 0x02,                    /*     COLLECTION (Logical) */
  0x05, 0x08,                    /*       USAGE_PAGE (LEDs) */
  0x09, 0x4b,                    /*       USAGE (Generic Indicator 1) */
  0x79, 0x08,                    /*       STRING INDEX (8) */
  0x75, 0x08,                    /*       REPORT_SIZE (8) */
  0x95, 0x01,                    /*       REPORT_COUNT (1) */
  0x91, 0x02,                    /*       OUTPUT (Data,Var,Abs) */
  0xc0,                          /*     END_COLLECTION */
  /* RFID Blue */
  0x05, 0x0a,                    /*     USAGE_PAGE (Ordinals) */
  0x09, 0x06,                    /*     USAGE (Instance 6) */
  0xa1, 0x02,                    /*     COLLECTION (Logical) */
  0x05, 0x08,                    /*       USAGE_PAGE (LEDs) */
  0x09, 0x4b,                    /*       USAGE (Generic Indicator 1) */
  0x79, 0x09,                    /*       STRING INDEX (9) */
  0x75, 0x08,                    /*       REPORT_SIZE (8) */
  0x95, 0x01,                    /*       REPORT_COUNT (1) */
  0x91, 0x02,                    /*       OUTPUT (Data,Var,Abs) */
  0xc0,                          /*     END_COLLECTION */
  /* Left button Led */
  0x05, 0x0a,                    /*     USAGE_PAGE (Ordinals) */
  0x09, 0x07,                    /*     USAGE (Instance 7) */
  0xa1, 0x02,                    /*     COLLECTION (Logical) */
  0x05, 0x08,                    /*       USAGE_PAGE (LEDs) */
  0x09, 0x4b,                    /*       USAGE (Generic Indicator 1) */
  0x79, 0x0a,                    /*       STRING INDEX (10) */
  0x75, 0x08,                    /*       REPORT_SIZE (8) */
  0x95, 0x01,                    /*       REPORT_COUNT (1) */
  0x91, 0x02,                    /*       OUTPUT (Data,Var,Abs) */
  0xc0,                          /*     END_COLLECTION */
  /* Right button Led */
  0x05, 0x0a,                    /*     USAGE_PAGE (Ordinals) */
  0x09, 0x08,                    /*     USAGE (Instance 8) */
  0xa1, 0x02,                    /*     COLLECTION (Logical) */
  0x05, 0x08,                    /*       USAGE_PAGE (LEDs) */
  0x09, 0x4b,                    /*       USAGE (Generic Indicator 1) */
  0x79, 0x0b,                    /*       STRING INDEX (11) */
  0x75, 0x08,                    /*       REPORT_SIZE (1) */
  0x95, 0x01,                    /*       REPORT_COUNT (8) */
  0x91, 0x02,                    /*       OUTPUT (Data,Var,Abs) */
  0xc0,                          /*     END_COLLECTION */

  // Globals
  HID_RI_REPORT_ID(8, 6),
  HID_RI_REPORT_COUNT(8, 1),
  HID_RI_REPORT_SIZE(8, 8),
  HID_RI_LOGICAL_MINIMUM(16, 0),
  HID_RI_LOGICAL_MAXIMUM(16, 255),
  HID_RI_USAGE_PAGE(8, 0x09), // Ordinals
  // Locals
  HID_RI_USAGE_MINIMUM(8, 1), // LED 1
  HID_RI_USAGE_MAXIMUM(8, 1), // LED 8 + buttons
  HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE | HID_IOF_NON_VOLATILE),

  HID_RI_END_COLLECTION(0),
};

#ifdef WITH_KEYBOARD
const USB_Descriptor_HIDReport_Datatype_t PROGMEM KeyboardReport[] =
{
  HID_RI_USAGE_PAGE(8, 0x01), /* Generic Desktop */
  HID_RI_USAGE(8, 0x06), /* Keyboard */
  HID_RI_COLLECTION(8, 0x01), /* Application */
  HID_RI_USAGE_PAGE(8, 0x07), /* Key Codes */
  HID_RI_USAGE_MINIMUM(8, 0xE0), /* Keyboard Left Control */
  HID_RI_USAGE_MAXIMUM(8, 0xE7), /* Keyboard Right GUI */
  HID_RI_LOGICAL_MINIMUM(8, 0x00),
  HID_RI_LOGICAL_MAXIMUM(8, 0x01),
  HID_RI_REPORT_SIZE(8, 0x01),
  HID_RI_REPORT_COUNT(8, 0x08),
  HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),
  HID_RI_REPORT_COUNT(8, 0x01),
  HID_RI_REPORT_SIZE(8, 0x08),
  HID_RI_INPUT(8, HID_IOF_CONSTANT),
  HID_RI_USAGE_PAGE(8, 0x08), /* LEDs */
  HID_RI_USAGE_MINIMUM(8, 0x01), /* Num Lock */
  HID_RI_USAGE_MAXIMUM(8, 0x05), /* Kana */
  HID_RI_REPORT_COUNT(8, 0x05),
  HID_RI_REPORT_SIZE(8, 0x01),
  HID_RI_OUTPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE | HID_IOF_NON_VOLATILE),
  HID_RI_REPORT_COUNT(8, 0x01),
  HID_RI_REPORT_SIZE(8, 0x03),
  HID_RI_OUTPUT(8, HID_IOF_CONSTANT),
  HID_RI_LOGICAL_MINIMUM(8, 0x00),
  HID_RI_LOGICAL_MAXIMUM(8, 0x65),
  HID_RI_USAGE_PAGE(8, 0x07), /* Keyboard */
  HID_RI_USAGE_MINIMUM(8, 0x00), /* Reserved (no event indicated) */
  HID_RI_USAGE_MAXIMUM(8, 0x65), /* Keyboard Application */
  HID_RI_REPORT_COUNT(8, 0x06),
  HID_RI_REPORT_SIZE(8, 0x08),
  HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_ARRAY | HID_IOF_ABSOLUTE),
  HID_RI_END_COLLECTION(0),
};
#endif

// Device Descriptor Structure
const USB_Descriptor_Device_t PROGMEM DeviceDescriptor = {
  .Header                 = {.Size = sizeof(USB_Descriptor_Device_t), .Type = DTYPE_Device},

  .USBSpecification       = VERSION_BCD(2, 0, 0),
  .Class                  = USB_CSCP_NoDeviceClass,
  .SubClass               = USB_CSCP_NoDeviceSubclass,
  .Protocol               = USB_CSCP_NoDeviceProtocol,

  .Endpoint0Size          = FIXED_CONTROL_ENDPOINT_SIZE,

  .VendorID               = 0x0F0D,
  .ProductID              = 0x0092,
  .ReleaseNumber          = VERSION_BCD(1, 0, 0),

  .ManufacturerStrIndex   = STRING_ID_Manufacturer,
  .ProductStrIndex        = STRING_ID_Product,
  .SerialNumStrIndex      = NO_DESCRIPTOR,

  .NumberOfConfigurations = FIXED_NUM_CONFIGURATIONS
};

// Configuration Descriptor Structure
const USB_Descriptor_Configuration_t PROGMEM ConfigurationDescriptor = {
  .Config =
  {
    .Header                 = {.Size = sizeof(USB_Descriptor_Configuration_Header_t), .Type = DTYPE_Configuration},

    .TotalConfigurationSize = sizeof(USB_Descriptor_Configuration_t),
#ifdef WITH_KEYBOARD
    .TotalInterfaces        = 3,
#else
    .TotalInterfaces        = 2,
#endif
    .ConfigurationNumber    = 1,
    .ConfigurationStrIndex  = NO_DESCRIPTOR,

    .ConfigAttributes       = 0x80,

    .MaxPowerConsumption    = USB_CONFIG_POWER_MA(500)
  },

  .HID_Interface =
  {
    .Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

    .InterfaceNumber        = INTERFACE_ID_Joystick,
    .AlternateSetting       = 0x00,

    .TotalEndpoints         = 2,

    .Class                  = HID_CSCP_HIDClass,
    .SubClass               = HID_CSCP_NonBootSubclass,
    .Protocol               = HID_CSCP_NonBootProtocol,

    .InterfaceStrIndex      = NO_DESCRIPTOR
  },

  .HID_JoystickHID =
  {
    .Header                 = {.Size = sizeof(USB_HID_Descriptor_HID_t), .Type = HID_DTYPE_HID},

    .HIDSpec                = VERSION_BCD(1, 1, 1),
    .CountryCode            = 0x00,
    .TotalReportDescriptors = 1,
    .HIDReportType          = HID_DTYPE_Report,
    .HIDReportLength        = sizeof(JoystickReport)
  },

  .HID_ReportINEndpoint =
  {
    .Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

    .EndpointAddress        = JOYSTICK_IN_EPADDR,
    .Attributes             = (EP_TYPE_INTERRUPT | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
    .EndpointSize           = JOYSTICK_EPSIZE,
    .PollingIntervalMS      = 0x01
  },

  .HID_ReportOUTEndpoint =
  {
    .Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

    .EndpointAddress        = JOYSTICK_OUT_EPADDR,
    .Attributes             = (EP_TYPE_INTERRUPT | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
    .EndpointSize           = JOYSTICK_EPSIZE,
    .PollingIntervalMS      = 0x10
  },
  /* LIGHTS */
  .HID_LightInterface =
  {
    .Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

    .InterfaceNumber        = INTERFACE_ID_Light,
    .AlternateSetting       = 0x00,

    .TotalEndpoints         = 2,

    .Class                  = HID_CSCP_HIDClass,
    .SubClass               = HID_CSCP_NonBootSubclass,
    .Protocol               = HID_CSCP_NonBootProtocol,

    .InterfaceStrIndex      = NO_DESCRIPTOR
  },

  .HID_LightHID =
  {
    .Header                 = {.Size = sizeof(USB_HID_Descriptor_HID_t), .Type = HID_DTYPE_HID},

    .HIDSpec                = VERSION_BCD(1, 1, 1),
    .CountryCode            = 0x00,
    .TotalReportDescriptors = 1,
    .HIDReportType          = HID_DTYPE_Report,
    .HIDReportLength        = sizeof(LEDReport)
  },

  .HID_LightReportINEndpoint =
  {
    .Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

    .EndpointAddress        = LIGHT_IN_EPADDR,
    .Attributes             = (EP_TYPE_INTERRUPT | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
    .EndpointSize           = LIGHT_EPSIZE,
    .PollingIntervalMS      = 0x04
  },

  .HID_LightReportOUTEndpoint =
  {
    .Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

    .EndpointAddress        = LIGHT_OUT_EPADDR,
    .Attributes             = (EP_TYPE_INTERRUPT | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
    .EndpointSize           = LIGHT_EPSIZE,
    .PollingIntervalMS      = 0x10
  },
#ifdef WITH_KEYBOARD
  .HID1_KeyboardInterface =
  {
    .Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

    .InterfaceNumber        = INTERFACE_ID_Keyboard,
    .AlternateSetting       = 0x00,
    .TotalEndpoints         = 2,
    .Class                  = HID_CSCP_HIDClass,
    .SubClass               = HID_CSCP_BootSubclass,
    .Protocol               = HID_CSCP_KeyboardBootProtocol,
    .InterfaceStrIndex      = NO_DESCRIPTOR
  },
  .HID1_KeyboardHID =
  {
    .Header                 = {.Size = sizeof(USB_HID_Descriptor_HID_t), .Type = HID_DTYPE_HID},
    .HIDSpec                = VERSION_BCD(1, 1, 1),
    .CountryCode            = 0x00,
    .TotalReportDescriptors = 1,
    .HIDReportType          = HID_DTYPE_Report,
    .HIDReportLength        = sizeof(KeyboardReport)
  },
  .HID1_ReportINEndpoint =
  {
    .Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},
    .EndpointAddress        = KEYBOARD_IN_EPADDR,
    .Attributes             = (EP_TYPE_INTERRUPT | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
    .EndpointSize           = KEYBOARD_EPSIZE,
    .PollingIntervalMS      = 0x05
  },
  .HID1_ReportOUTEndpoint =
  {
    .Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},
    .EndpointAddress        = KEYBOARD_OUT_EPADDR,
    .Attributes             = (EP_TYPE_INTERRUPT | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
    .EndpointSize           = KEYBOARD_EPSIZE,
    .PollingIntervalMS      = 0x05
  }
#endif
};

/** Device descriptor structure. This descriptor, located in FLASH memory, describes the overall
    device characteristics, including the supported USB version, control endpoint size and the
    number of device configurations. The descriptor is read out by the USB host when the enumeration
    process begins.
*/
const uint8_t PROGMEM DeviceDescriptorX[] =
{
  0x12,        // bLength
  0x01,        // bDescriptorType (Device)
  0x00, 0x02,  // bcdUSB 2.00
  0xFF,        // bDeviceClass
  0xFF,        // bDeviceSubClass
  0xFF,        // bDeviceProtocol
  0x40,        // bMaxPacketSize0 64
  0x5E, 0x04,  // idVendor 0x045E
  0x8E, 0x02,  // idProduct 0x028E
  0x14, 0x01,  // bcdDevice 2.14
  0x01,        // iManufacturer (String Index)
  0x02,        // iProduct (String Index)
  0x03,        // iSerialNumber (String Index)
  0x01        // bNumConfigurations 1
};

/** Configuration descriptor structure. This descriptor, located in FLASH memory, describes the usage
    of the device in one of its supported configurations, including information about any device interfaces
    and endpoints. The descriptor is read out by the USB host during the enumeration process when selecting
    a configuration so that the host may correctly communicate with the USB device.
*/
const uint8_t PROGMEM ConfigurationDescriptorX[] =
{
  0x09,        // bLength
  0x02,        // bDescriptorType (Configuration)
  0x99, 0x00,  // wTotalLength 153
  0x04,        // bNumInterfaces 4
  0x01,        // bConfigurationValue
  0x00,        // iConfiguration (String Index)
  0xA0,        // bmAttributes Remote Wakeup
  0xFA,        // bMaxPower 500mA

  0x09,        // bLength
  0x04,        // bDescriptorType (Interface)
  0x00,        // bInterfaceNumber 0
  0x00,        // bAlternateSetting
  0x02,        // bNumEndpoints 2
  0xFF,        // bInterfaceClass
  0x5D,        // bInterfaceSubClass
  0x01,        // bInterfaceProtocol
  0x00,        // iInterface (String Index)

  0x11,        // bLength
  0x21,        // bDescriptorType (HID)
  0x00, 0x01,  // bcdHID 1.00
  0x01,        // bCountryCode
  0x25,        // bNumDescriptors
  0x81,        // bDescriptorType[0] (Unknown 0x81)
  0x14, 0x00,  // wDescriptorLength[0] 20
  0x00,        // bDescriptorType[1] (Unknown 0x00)
  0x00, 0x00,  // wDescriptorLength[1] 0
  0x13,        // bDescriptorType[2] (Unknown 0x13)
  0x01, 0x08,  // wDescriptorLength[2] 2049
  0x00,        // bDescriptorType[3] (Unknown 0x00)
  0x00,
  0x07,        // bLength
  0x05,        // bDescriptorType (Endpoint)
  0x81,        // bEndpointAddress (IN/D2H)
  0x03,        // bmAttributes (Interrupt)
  0x20, 0x00,  // wMaxPacketSize 32
  0x04,        // bInterval 4 (unit depends on device speed)

  0x07,        // bLength
  0x05,        // bDescriptorType (Endpoint)
  0x01,        // bEndpointAddress (OUT/H2D)
  0x03,        // bmAttributes (Interrupt)
  0x20, 0x00,  // wMaxPacketSize 32
  0x08,        // bInterval 8 (unit depends on device speed)

  0x09,        // bLength
  0x04,        // bDescriptorType (Interface)
  0x01,        // bInterfaceNumber 1
  0x00,        // bAlternateSetting
  0x04,        // bNumEndpoints 4
  0xFF,        // bInterfaceClass
  0x5D,        // bInterfaceSubClass
  0x03,        // bInterfaceProtocol
  0x00,        // iInterface (String Index)

  0x1B,        // bLength
  0x21,        // bDescriptorType (HID)
  0x00, 0x01,  // bcdHID 1.00
  0x01,        // bCountryCode
  0x01,        // bNumDescriptors
  0x82,        // bDescriptorType[0] (Unknown 0x82)
  0x40, 0x01,  // wDescriptorLength[0] 320
  0x02, 0x20, 0x16, 0x83, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x07,        // bLength
  0x05,        // bDescriptorType (Endpoint)
  0x82,        // bEndpointAddress (IN/D2H)
  0x03,        // bmAttributes (Interrupt)
  0x20, 0x00,  // wMaxPacketSize 32
  0x02,        // bInterval 2 (unit depends on device speed)

  0x07,        // bLength
  0x05,        // bDescriptorType (Endpoint)
  0x02,        // bEndpointAddress (OUT/H2D)
  0x03,        // bmAttributes (Interrupt)
  0x20, 0x00,  // wMaxPacketSize 32
  0x04,        // bInterval 4 (unit depends on device speed)

  0x07,        // bLength
  0x05,        // bDescriptorType (Endpoint)
  0x83,        // bEndpointAddress (IN/D2H)
  0x03,        // bmAttributes (Interrupt)
  0x20, 0x00,  // wMaxPacketSize 32
  0x40,        // bInterval 64 (unit depends on device speed)

  0x07,        // bLength
  0x05,        // bDescriptorType (Endpoint)
  0x03,        // bEndpointAddress (OUT/H2D)
  0x03,        // bmAttributes (Interrupt)
  0x20, 0x00,  // wMaxPacketSize 32
  0x10,        // bInterval 16 (unit depends on device speed)

  0x09,        // bLength
  0x04,        // bDescriptorType (Interface)
  0x02,        // bInterfaceNumber 2
  0x00,        // bAlternateSetting
  0x01,        // bNumEndpoints 1
  0xFF,        // bInterfaceClass
  0x5D,        // bInterfaceSubClass
  0x02,        // bInterfaceProtocol
  0x00,        // iInterface (String Index)

  0x09,        // bLength
  0x21,        // bDescriptorType (HID)
  0x00, 0x01,  // bcdHID 1.00
  0x01,        // bCountryCode
  0x22,        // bNumDescriptors
  0x84,        // bDescriptorType[0] (Unknown 0x84)
  0x07, 0x00,  // wDescriptorLength[0] 7

  0x07,        // bLength
  0x05,        // bDescriptorType (Endpoint)
  0x84,        // bEndpointAddress (IN/D2H)
  0x03,        // bmAttributes (Interrupt)
  0x20, 0x00,  // wMaxPacketSize 32
  0x10,        // bInterval 16 (unit depends on device speed)

  0x09,        // bLength
  0x04,        // bDescriptorType (Interface)
  0x03,        // bInterfaceNumber 3
  0x00,        // bAlternateSetting
  0x00,        // bNumEndpoints 0
  0xFF,        // bInterfaceClass
  0xFD,        // bInterfaceSubClass
  0x13,        // bInterfaceProtocol
  0x04,        // iInterface (String Index)

  0x06,        // bLength
  0x41,        // bDescriptorType (Unknown)
  0x00, 0x01, 0x01, 0x03
};

const USB_Descriptor_String_t PROGMEM LanguageString = USB_STRING_DESCRIPTOR_ARRAY(LANGUAGE_ID_ENG);
const USB_Descriptor_String_t PROGMEM VersionString = USB_STRING_DESCRIPTOR(L"1.0");

const USB_Descriptor_String_t PROGMEM ManufacturerString = USB_STRING_DESCRIPTOR(L"HORI CO.,LTD.");
const USB_Descriptor_String_t PROGMEM ProductString = USB_STRING_DESCRIPTOR(L"POKKEN CONTROLLER");
const USB_Descriptor_String_t PROGMEM ManufacturerStringX = USB_STRING_DESCRIPTOR(L"kadevice.net");
const USB_Descriptor_String_t PROGMEM ProductStringX = USB_STRING_DESCRIPTOR(L"KADE - Kick Ass Dynamic Encoder");

/* HID Light descriptor strings */
const USB_Descriptor_String_t PROGMEM LEDString_03 = USB_STRING_DESCRIPTOR(L"Joy RGB Red");
const USB_Descriptor_String_t PROGMEM LEDString_04 = USB_STRING_DESCRIPTOR(L"Joy RGB Green");
const USB_Descriptor_String_t PROGMEM LEDString_05 = USB_STRING_DESCRIPTOR(L"Joy RGB Blue");
const USB_Descriptor_String_t PROGMEM LEDString_06 = USB_STRING_DESCRIPTOR(L"RFID RGB Red");
const USB_Descriptor_String_t PROGMEM LEDString_07 = USB_STRING_DESCRIPTOR(L"RFID RGB Green");
const USB_Descriptor_String_t PROGMEM LEDString_08 = USB_STRING_DESCRIPTOR(L"RFID RGB Blue");
const USB_Descriptor_String_t PROGMEM LEDString_01 = USB_STRING_DESCRIPTOR(L"Left Button LED");
const USB_Descriptor_String_t PROGMEM LEDString_02 = USB_STRING_DESCRIPTOR(L"Right Button LED");

const USB_Descriptor_String_t *LEDString_indiv[] = {
  &LEDString_03,
  &LEDString_04,
  &LEDString_05,
  &LEDString_06,
  &LEDString_07,
  &LEDString_08,
  &LEDString_01,
  &LEDString_02,
};

// USB Device Callback - Get Descriptor
uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,
                                    const uint16_t wIndex,
                                    const void** const DescriptorAddress)
{
  const uint8_t  DescriptorType   = (wValue >> 8);
  const uint8_t  DescriptorNumber = (wValue & 0xFF);

  const void* Address = NULL;
  uint16_t    Size    = NO_DESCRIPTOR;

  switch (DescriptorType)
  {
    case DTYPE_Device:
      Address = g_xinput_mode ? &DeviceDescriptorX : &DeviceDescriptor;
      Size    = g_xinput_mode ? sizeof(DeviceDescriptorX) : sizeof(USB_Descriptor_Device_t);
      break;
    case DTYPE_Configuration:
      Address = g_xinput_mode ? &ConfigurationDescriptorX : &ConfigurationDescriptor;
      Size    = g_xinput_mode ? sizeof(ConfigurationDescriptorX) : sizeof(USB_Descriptor_Configuration_t);
      break;
    case DTYPE_String:
      switch (DescriptorNumber)
      {
        case STRING_ID_Language:
          Address = &LanguageString;
          Size    = pgm_read_byte(&LanguageString.Header.Size);
          break;
        case STRING_ID_Manufacturer:
          Address = g_xinput_mode ? &ManufacturerStringX : &ManufacturerString;
          Size    = g_xinput_mode ? pgm_read_byte(&ManufacturerStringX.Header.Size) : pgm_read_byte(&ManufacturerString.Header.Size);
          break;
        case STRING_ID_Product:
          Address = g_xinput_mode ? &ProductStringX : &ProductString;
          Size    = g_xinput_mode ? pgm_read_byte(&ProductStringX.Header.Size) : pgm_read_byte(&ProductString.Header.Size);
          break;
        case STRING_ID_Version:
          Address = &VersionString;
          Size    = pgm_read_byte(&VersionString.Header.Size);
          break;
        default:
          if (DescriptorNumber >= STRING_ID_LED_Base && DescriptorNumber < (STRING_ID_LED_Base + STRING_ID_LED_Count)) {
            Address = LEDString_indiv[DescriptorNumber - STRING_ID_LED_Base];
            Size    = pgm_read_byte(&(LEDString_indiv[DescriptorNumber - STRING_ID_LED_Base]->Header.Size));
          }
      }
      break;
    case HID_DTYPE_HID:
      switch (wIndex)
      {
#ifdef WITH_KEYBOARD
        case (INTERFACE_ID_Keyboard):
          Address = &ConfigurationDescriptor.HID1_KeyboardHID;
          Size    = sizeof(USB_HID_Descriptor_HID_t);
          break;
#endif
        case (INTERFACE_ID_Joystick):
          Address = &ConfigurationDescriptor.HID_JoystickHID;
          Size    = sizeof(USB_HID_Descriptor_HID_t);
          break;
        case (INTERFACE_ID_Light):
          Address = &ConfigurationDescriptor.HID_LightHID;
          Size    = sizeof(USB_HID_Descriptor_HID_t);
          break;
      }
      break;
    case HID_DTYPE_Report:
      switch (wIndex)
      {
#ifdef WITH_KEYBOARD
        case INTERFACE_ID_Keyboard:
          Address = &KeyboardReport;
          Size    = sizeof(KeyboardReport);
          break;
#endif
        case INTERFACE_ID_Light:
          Address = &LEDReport;
          Size    = sizeof(LEDReport);
          break;
        case INTERFACE_ID_Joystick:
          Address = &JoystickReport;
          Size    = sizeof(JoystickReport);
          break;
      }
      break;
  }
  *DescriptorAddress = Address;
  return Size;

}
