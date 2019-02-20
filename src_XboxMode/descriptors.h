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

PROGMEM const char usbDescriptorDeviceJoystick[] = {    /* USB device descriptor */
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

PROGMEM const char usbDescriptorDeviceMouse[] = {    /* USB device descriptor */
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
    (char)USB_CFG_DEVICE_ID+1,/* 2 bytes */
    USB_CFG_DEVICE_VERSION, /* 2 bytes */
    1,         /* manufacturer string index */
    2,        /* product string index */
    0,  /* serial number string index */
    1,          /* number of configurations */
};



/* ----- HID Descriptor 1P --------------------------------------------------------------------- */

#define	usbHidReportDescriptor1PLength	80

PROGMEM char usbHidReportDescriptor1P[80] = {
	0x05,0x01,		//Usage_Page (Generic Desktop)
	0x09,0x04,		//Usage (Joystick)
// 4

// Main joystick x/y
	0xA1,0x01,		//Collection (Application)
	0x85,0x01,			// Report ID (1)
	0x09,0x01,			//Usage (Pointer)
	0xA1,0x00,			//Collection (Physical)		
	0x09,0x30,				//Usage (X)
	0x09,0x31,				//Usage (Y)
	0x15,0x81,				//Logical_Minimum (-127)
	0x25,0x7F,				//Logical Maximum (127)
	0x75,0x08,				//Report_Size (8)
	0x95,0x02,				//Report_Count (4)
	0x81,0x02,				//Input (Data, Var, Abs)		
// (20)

// Second joystick Rx/Ry
	0x09,0x33,				//Usage (Rx)
	0x09,0x34,				//Usage (Ry)
	0x15,0x81,				//Logical_Minimum (-127)
	0x25,0x7F,				//Logical Maximum (127)
	0x75,0x08,				//Report_Size (8)
	0x95,0x02,				//Report_Count (4)
	0x81,0x02,				//Input (Data, Var, Abs)		
// (14)

// Hat switch
	0x09,0x39,				//Usage (Hat switch)
	0x15,0x00,				//Logical_Minimum (0)
	0x25,0x07,				//Logical_Maximum (7)
	0x75,0x05,				//Report_Size (4)
	0x95,0x01,				//Report_Count (1)
	0x81,0x03,				//Input (Data, Var, Abs)
// Round to one byte
	0x75,0x03,				//Report_Size (4)
	0x95,0x01,				//Report_Count (1)
	0x81,0x03,				//Input (Data, Var, Abs)
// (18)

// Digital fire buttons
	0x05,0x09,				//Usage_Page (Button)
	0x19,0x01,				//Usage_Minimum (Button 1)
	0x29,0x10,				//Usage_Maximum (Button 16)
	0x15,0x00,				//Logical_Minimum (0)
	0x25,0x01,				//Logical_Maximum (1)
	0x75,0x01,				//Report_Size (1)
	0x95,0x10,				//Report_Count (16)
	0x55,0x00,				//Unit_Exponent (0)
	0x65,0x00,				//Unit (None)
	0x81,0x02,				//Input (Data, Var, Abs)
	0xC0,				//End_Collection
	0xC0,			//End_Collection
// (22)
};



/* ----- HID Descriptor 2P --------------------------------------------------------------------- */

#define	usbHidReportDescriptor2PLength	160

PROGMEM char usbHidReportDescriptor2P[160] = {

	/* ----- Player 1 ----- */

	0x05,0x01,		//Usage_Page (Generic Desktop)
	0x09,0x04,		//Usage (Joystick)
// 4

// Main joystick x/y
	0xA1,0x01,		//Collection (Application)
	0x85,0x01,			// Report ID (1)
	0x09,0x01,			//Usage (Pointer)
	0xA1,0x00,			//Collection (Physical)		
	0x09,0x30,				//Usage (X)
	0x09,0x31,				//Usage (Y)
	0x15,0x81,				//Logical_Minimum (-127)
	0x25,0x7F,				//Logical Maximum (127)
	0x75,0x08,				//Report_Size (8)
	0x95,0x02,				//Report_Count (4)
	0x81,0x02,				//Input (Data, Var, Abs)		
// (20)

// Second joystick Rx/Ry
	0x09,0x33,				//Usage (Rx)
	0x09,0x34,				//Usage (Ry)
	0x15,0x81,				//Logical_Minimum (-127)
	0x25,0x7F,				//Logical Maximum (127)
	0x75,0x08,				//Report_Size (8)
	0x95,0x02,				//Report_Count (4)
	0x81,0x02,				//Input (Data, Var, Abs)		
// (14)

// Hat switch
	0x09,0x39,				//Usage (Hat switch)
	0x15,0x00,				//Logical_Minimum (0)
	0x25,0x07,				//Logical_Maximum (7)
	0x75,0x05,				//Report_Size (4)
	0x95,0x01,				//Report_Count (1)
	0x81,0x03,				//Input (Data, Var, Abs)
// Round to one byte
	0x75,0x03,				//Report_Size (4)
	0x95,0x01,				//Report_Count (1)
	0x81,0x03,				//Input (Data, Var, Abs)
// (18)

// Digital fire buttons
	0x05,0x09,				//Usage_Page (Button)
	0x19,0x01,				//Usage_Minimum (Button 1)
	0x29,0x10,				//Usage_Maximum (Button 16)
	0x15,0x00,				//Logical_Minimum (0)
	0x25,0x01,				//Logical_Maximum (1)
	0x75,0x01,				//Report_Size (1)
	0x95,0x10,				//Report_Count (16)
	0x55,0x00,				//Unit_Exponent (0)
	0x65,0x00,				//Unit (None)
	0x81,0x02,				//Input (Data, Var, Abs)
	0xC0,				//End_Collection
	0xC0,			//End_Collection
// (22)

	/* ----- Player 2 ----- */

	0x05,0x01,		//Usage_Page (Generic Desktop)
	0x09,0x04,		//Usage (Joystick)
// Main joystick x/y
	0xA1,0x01,		//Collection (Application)
	0x85,0x02,			// Report ID (2)
	0x09,0x01,			//Usage (Pointer)
	0xA1,0x00,			//Collection (Physical)		
	0x09,0x30,				//Usage (X)
	0x09,0x31,				//Usage (Y)
	0x15,0x81,				//Logical_Minimum (-127)
	0x25,0x7F,				//Logical Maximum (127)
	0x75,0x08,				//Report_Size (8)
	0x95,0x02,				//Report_Count (4)
	0x81,0x02,				//Input (Data, Var, Abs)		
// Second joystick Rx/Ry
	0x09,0x33,				//Usage (Rx)
	0x09,0x34,				//Usage (Ry)
	0x15,0x81,				//Logical_Minimum (-127)
	0x25,0x7F,				//Logical Maximum (127)
	0x75,0x08,				//Report_Size (8)
	0x95,0x02,				//Report_Count (4)
	0x81,0x02,				//Input (Data, Var, Abs)		
// Hat switch
	0x09,0x39,				//Usage (Hat switch)
	0x15,0x00,				//Logical_Minimum (0)
	0x25,0x07,				//Logical_Maximum (7)
	0x75,0x05,				//Report_Size (4)
	0x95,0x01,				//Report_Count (1)
	0x81,0x03,				//Input (Data, Var, Abs)
// Round to one byte
	0x75,0x03,				//Report_Size (4)
	0x95,0x01,				//Report_Count (1)
	0x81,0x03,				//Input (Data, Var, Abs)
// Digital fire buttons
	0x05,0x09,				//Usage_Page (Button)
	0x19,0x01,				//Usage_Minimum (Button 1)
	0x29,0x10,				//Usage_Maximum (Button 16)
	0x15,0x00,				//Logical_Minimum (0)
	0x25,0x01,				//Logical_Maximum (1)
	0x75,0x01,				//Report_Size (1)
	0x95,0x10,				//Report_Count (16)
	0x55,0x00,				//Unit_Exponent (0)
	0x65,0x00,				//Unit (None)
	0x81,0x02,				//Input (Data, Var, Abs)
	0xC0,				//End_Collection
	0xC0,			//End_Collection
};



/* ----- HID Descriptor Mouse ------------------------------------------------------------------ */

#define	usbHidReportDescriptorMouseLength	100

PROGMEM char usbHidReportDescriptorMouse[100] = {
    0x05, 0x01,			// USAGE_PAGE (Generic Desktop)
    0x09, 0x02,			// USAGE (Mouse)
    0xa1, 0x01,			// COLLECTION (Application)
	0x85, 0x01,				// Report ID (1)
    0x09, 0x01,         	// USAGE (Pointer)
    0xA1, 0x00,         	// COLLECTION (Physical)
    0x05, 0x09,         		// USAGE_PAGE (Button)
    0x19, 0x01,         		// USAGE_MINIMUM
    0x29, 0x03,					// USAGE_MAXIMUM
    0x15, 0x00,					// LOGICAL_MINIMUM (0)
    0x25, 0x01,             	// LOGICAL_MAXIMUM (1)
    0x95, 0x04,             	// REPORT_COUNT (4)
    0x75, 0x01,             	// REPORT_SIZE (1)
    0x81, 0x02,             	// INPUT (Data,Var,Abs)
    0x95, 0x01,             	// REPORT_COUNT (1)
    0x75, 0x04,             	// REPORT_SIZE (4)
    0x81, 0x03,             	// INPUT (Const,Var,Abs)
    0x05, 0x01,             	// USAGE_PAGE (Generic Desktop)
    0x09, 0x30,             	// USAGE (X)
    0x09, 0x31,             	// USAGE (Y)
    0x09, 0x38,					// USAGE (Wheel)
    0x15, 0x81, 	            // LOGICAL_MINIMUM (-127)
    0x25, 0x7F,     	        // LOGICAL_MAXIMUM (127)
    0x75, 0x08,         	    // REPORT_SIZE (8)
    0x95, 0x03,             	// REPORT_COUNT (3)
    0x81, 0x06,             	// INPUT (Data,Var,Rel)
    0xC0,					// END_COLLECTION
    0xC0,				// END COLLECTION
//54
	0x09,0x04,			// Usage (Joystick)
	0xA1,0x01,			// Collection (Application)
	0x85,0x02,				// Report ID (2)
// Main joystick x/y
	0x09,0x01,				// Usage (Pointer)
	0xA1,0x00,				// Collection (Physical)
	0x09,0x30,				// Usage (X)
	0x09,0x31,				// Usage (Y)
	0x15,0x81,				// Logical_Minimum (-127)
	0x25,0x7F,				// Logical Maximum (127)
	0x75,0x08,				// Report_Size (8)
	0x95,0x02,				// Report_Count (2)
	0x81,0x02,				// Input (Data, Var, Abs)
	0xC0,				// End_Collection
// Digital fire buttons
	0x05,0x09,			// Usage_Page (Button)
	0x19,0x01,			// Usage_Minimum (Button 1)
	0x29,0x10,			// Usage_Maximum (Button 16)
	0x15,0x00,			// Logical_Minimum (0)
	0x25,0x01,			// Logical_Maximum (1)
	0x75,0x01,			// Report_Size (1)
	0x95,0x10,			// Report_Count (16)
	0x55,0x00,			// Unit_Exponent (0)
	0x65,0x00,			// Unit (None)
	0x81,0x02,			// Input (Data, Var, Abs)
// Padding to make report 5 bytes
	//0x75,0x08,			// Report_Size (8)
	//0x95,0x01,			// Report_Count (1)
	//0x81,0x03,			// Input (Data, Var, Abs)
	0xC0,			// End_Collection
};
