/* ----- String Descriptors ------ */

PROGMEM int usbDescriptorStringDevice[] = {
    USB_STRING_DESCRIPTOR_HEADER(17),
    'R', 'e', 't', 'r', 'o', ' ', 'A', 'd', 'a', 'p', 't', 'e', 'r', ' ', 'M', 'o', 'd'
};

/* PROGMEM int usbDescriptorStringDeviceNegCon[] = {
    USB_STRING_DESCRIPTOR_HEADER(24),
    'R', 'e', 't', 'r', 'o', ' ', 'A', 'd', 'a', 'p', 't', 'e', 'r', ' ', 'M', 'o', 'd', ' ', 'N', 'e', 'g', 'C', 'o', 'n'
};
 */
 
/* ----- USB Configuration Descriptor ---------------------------------------------------------- */

char usbDescriptorConfiguration[] = {    /* USB configuration descriptor */
    9,          /* sizeof(usbDescriptorConfiguration): length of descriptor in bytes */
    USBDESCR_CONFIG,    /* descriptor type */
    18 + 7 * USB_CFG_HAVE_INTRIN_ENDPOINT + 9, 0,
                /* total length of data returned (including inlined descriptors) */
    1,          /* number of interfaces in this configuration */
    1,          /* index of this configuration */
    0,          /* configuration name string index */
    (1 << 7),   /* attributes */
    USB_CFG_MAX_BUS_POWER/2,            /* max USB current in 2mA units */

	/* interface descriptor follows inline: */
    9,          /* sizeof(usbDescrInterface): length of descriptor in bytes */
    USBDESCR_INTERFACE, /* descriptor type */
    0,          /* index of this interface */
    0,          /* alternate setting for this interface */
    USB_CFG_HAVE_INTRIN_ENDPOINT,   /* endpoints excl 0: number of endpoint descriptors to follow */
    USB_CFG_INTERFACE_CLASS,
    USB_CFG_INTERFACE_SUBCLASS,
    USB_CFG_INTERFACE_PROTOCOL,
    0,          /* string index for interface */

	/* HID descriptor */
    9,          /* sizeof(usbDescrHID): length of descriptor in bytes */
    USBDESCR_HID,   /* descriptor type: HID */
    0x01, 0x01, /* BCD representation of HID version */
    0x00,       /* target country code */
    0x01,       /* number of HID Report (or other HID class) Descriptor infos to follow */
    0x22,       /* descriptor type: report */
    USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH, 0,  /* total length of report descriptor */

#if USB_CFG_HAVE_INTRIN_ENDPOINT    /* endpoint descriptor for endpoint 1 */
    7,          /* sizeof(usbDescrEndpoint) */
    USBDESCR_ENDPOINT,  /* descriptor type = endpoint */
    0x81,       /* IN endpoint number 1 */
    0x03,       /* attrib: Interrupt endpoint */
    8, 0,       /* maximum packet size */
    USB_CFG_INTR_POLL_INTERVAL, /* in ms */
#endif
};



/* ----- USB Device Descriptor ----------------------------------------------------------------- */

PROGMEM char usbDescriptorDevice[] = {    /* USB device descriptor */
    18,         /* sizeof(usbDescriptorDevice): length of descriptor in bytes */
    USBDESCR_DEVICE,        /* descriptor type */
    0x10, 0x01,             /* USB version supported */
    USB_CFG_DEVICE_CLASS,
    USB_CFG_DEVICE_SUBCLASS,
    0,                      /* protocol */
    8,                      /* max packet size */
    /* the following two casts affect the first byte of the constant only, but
     * that's sufficient to avoid a warning with the default values.
     */
    (char)USB_CFG_VENDOR_ID,/* 2 bytes */
    (char)USB_CFG_DEVICE_ID,/* 2 bytes */
    USB_CFG_DEVICE_VERSION, /* 2 bytes */
    1,         /* manufacturer string index */
    2,        /* product string index */
    0,  /* serial number string index */
    1,          /* number of configurations */
};

PROGMEM char usbHidReportDescriptor[] = {
  0x05,0x01,		  //Usage_Page (Generic Desktop)
  0x09,0x05,		  //Usage (Game Pad)
  0xA1, 0x01,         //Collection (Application)
  0x15, 0x00,         //Logical Minimum (0)
  0x25, 0x01,         //Logical Maximum (1)
  0x35, 0x00,         //Physical Minimum (0)
  0x45, 0x01,         //Physical Maximum (1)
  0x75, 0x01,         //Report Size (1)
  0x95, 0x0E,         //Report Count (14)
  0x05, 0x09,         //Usage Page (Buttons)
  0x19, 0x01,         //Usage Minimum (1)
  0x29, 0x0E,         //Usage Maximum (14)
  0x81, 0x02,         //Input (Var)
  0x95, 0x02,         //Report Count (2)
  0x81, 0x01,         //Input (Const)
  0x05, 0x01,         //Usage Page (Generic Desktop Controls)
  0x25, 0x07,         //Logical Maximum (7)
  0x46, 0x3B, 0x01,   //Physical Maximum (315)
  0x75, 0x04,         //Report Size (4)
  0x95, 0x01,         //Report Count (1)
  0x65, 0x14,         //Unit (0x14)
  0x09, 0x39,         //Usage (Hat Switch)
  0x81, 0x42,         //Input (Var, NullState)
  0x65, 0x00,         //Unit (0x00)
  0x95, 0x01,         //Report Count (1)
  0x81, 0x01,         //Input (Const)
  0x26, 0xFF, 0x00,   //Logical Maximum (255)
  0x46, 0xFF, 0x00,   //Physical Maximum (255)
  0x09, 0x30,         //Usage (Direction-X)
  0x09, 0x31,         //Usage (Direction-Y)
  0x09, 0x32,         //Usage (Direction-Z)
  0x09, 0x35,         //Usage (Rotate-Z)
  0x75, 0x08,         //Report Size (8)
  0x95, 0x04,         //Report Count (4)
  0x81, 0x02,         //Input (Var)
  0x75, 0x08,         //Report Size (8)
  0x95, 0x01,         //Report Count (1)
  0x81, 0x01,         //Input (Const)
  0xC0                //End Collection
};