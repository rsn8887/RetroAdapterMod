PROGMEM char usbHidReportDescriptor[90] = {
	/* Controller and report_id 1 */
    0x05, 0x01,			// USAGE_PAGE (Generic Desktop)
    0x09, 0x04,			// USAGE (Joystick)
    0xa1, 0x01,			//	COLLECTION (Application)
	0x85, 0x01,			//			REPORT_ID (1)
    0xa1, 0x02,			//		COLLECTION (Physical)
    0x09, 0x01,			//		USAGE (Pointer)
	0x09, 0x30,			//			USAGE (X)
    0x09, 0x31,			//			USAGE (Y)
    0x15, 0x00,			//			LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,	//			LOGICAL_MAXIMUM (255)
    0x75, 0x08,			//			REPORT_SIZE (8)
    0x95, 0x02,			//			REPORT_COUNT (2)
    0x81, 0x02,			//			INPUT (Data,Var,Abs)

    0x05, 0x09,			//			USAGE_PAGE (Button)
    0x19, 1,			//   		USAGE_MINIMUM (Button 1)
    0x29, 8,			//   		USAGE_MAXIMUM (Button 8)
    0x15, 0x00,			//   		LOGICAL_MINIMUM (0)
    0x25, 0x01,			//   		LOGICAL_MAXIMUM (1)
    0x75, 1,			// 			REPORT_SIZE (1)
    0x95, 8,			//			REPORT_COUNT (8)
    0x81, 0x02,			//			INPUT (Data,Var,Abs)
	0xc0,				//		END_COLLECTION
    0xc0,				// END_COLLECTION

	/* Controller and report_id 2 */
    0x05, 0x01,			// USAGE_PAGE (Generic Desktop)
    0x09, 0x04,			// USAGE (Joystick)
    0xa1, 0x01,			//	COLLECTION (Application)
	0x85, 0x02,			//			REPORT_ID (2)
    0xa1, 0x02,			//		COLLECTION (Physical)
    0x09, 0x01,			//		USAGE (Pointer)
	0x09, 0x30,			//			USAGE (X)
    0x09, 0x31,			//			USAGE (Y)
    0x15, 0x00,			//			LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,	//			LOGICAL_MAXIMUM (255)
    0x75, 0x08,			//			REPORT_SIZE (8)
    0x95, 0x02,			//			REPORT_COUNT (2)
    0x81, 0x02,			//			INPUT (Data,Var,Abs)
    0x05, 0x09,			//			USAGE_PAGE (Button)
    0x19, 1,			//   		USAGE_MINIMUM (Button 1)
    0x29, 8,			//   		USAGE_MAXIMUM (Button 8)
    0x15, 0x00,			//   		LOGICAL_MINIMUM (0)
    0x25, 0x01,			//   		LOGICAL_MAXIMUM (1)
    0x75, 1,			// 			REPORT_SIZE (1)
    0x95, 8,			//			REPORT_COUNT (8)
    0x81, 0x02,			//			INPUT (Data,Var,Abs)
	0xc0,				//		END_COLLECTION
    0xc0,				// END_COLLECTION
};


/*
PROGMEM char usbHidReportDescriptor[142] = {
	0x05,0x01,		//Usage_Page (Generic Desktop)
	0x09,0x04,		//Usage (Joystick)
	0xA1,0x01,		//Collection (Application)
	0x85,0x01,			//Report ID (1)
//8
	0x05,0x09,			//Usage_Page (Button)
	0x19,0x01,			//Usage_Minimum (Button 1)
	0x29,0x10,			//Usage_Maximum (Button 16)
	0x15,0x00,			//Logical_Minimum (0)
	0x25,0x01,			//Logical_Maximum (1)
	0x35,0x00,			//Physical_Minimum (0)
	0x45,0x01,			//Physical_Maximum (1)
	0x75,0x01,			//Report_Size (1)
	0x95,0x10,			//Report_Count (16)
	0x81,0x02,			//Input (Data, Var, Abs)
//20, 28
	0x05,0x01,			//Usage_Page (Generic Desktop)
	0x09,0x30,			//Usage (X)
	0x09,0x31,			//Usage (Y)
	0x09,0x35,			//Usage (X)
	0x09,0x32,			//Usage (Y)
	0x26,0xff,0x00,		//Logical_Maximum (255)
	0x46,0xff,0x00,		//Physical_Maximum (255)
	0x65,0x00,0x00,		//Unit (None)
	0x75,0x01,			//Report_Size (1)
	0x95,0x04,			//Report_Count (4)
	0x81,0x02,			//Input (Data, Var, Abs)
//24, 52
	0x06,0x00,0xff,		//Usage_Page (Vendor Defined Page 1)
	0x09,0x04,			//Usage (NULL)
	0x81,0x02,			//Input (Data, Var, Abs)
	0x95,0x01,			//Report_Count (1)
	0x19,0x01,
	0x29,0x02,
	0x95,0x03,
	0x91,0x02,
	0xc0,				// End_Collection
//18, 70

// ---------- Stick 2 -----------

	0x05,0x01,		//Usage_Page (Generic Desktop)
	0x09,0x04,		//Usage (Joystick)
	0xA1,0x01,		//Collection (Application)
	0x85,0x02,			//Report ID (1)
//8
	0x05,0x09,			//Usage_Page (Button)
	0x19,0x01,			//Usage_Minimum (Button 1)
	0x29,0x10,			//Usage_Maximum (Button 16)
	0x15,0x00,			//Logical_Minimum (0)
	0x25,0x01,			//Logical_Maximum (1)
	0x35,0x00,			//Physical_Minimum (0)
	0x45,0x01,			//Physical_Maximum (1)
	0x75,0x01,			//Report_Size (1)
	0x95,0x10,			//Report_Count (16)
	0x81,0x02,			//Input (Data, Var, Abs)
//20, 28
	0x05,0x01,			//Usage_Page (Generic Desktop)
	0x09,0x30,			//Usage (X)
	0x09,0x31,			//Usage (Y)
	0x09,0x35,			//Usage (X)
	0x09,0x32,			//Usage (Y)
	0x26,0xff,0x00,		//Logical_Maximum (255)
	0x46,0xff,0x00,		//Physical_Maximum (255)
	0x65,0x00,0x00,		//Unit (None)
	0x75,0x01,			//Report_Size (1)
	0x95,0x04,			//Report_Count (4)
	0x81,0x02,			//Input (Data, Var, Abs)
//24, 52
	0x06,0x00,0xff,		//Usage_Page (Vendor Defined Page 1)
	0x09,0x04,			//Usage (NULL)
	0x81,0x02,			//Input (Data, Var, Abs)
	0x95,0x01,			//Report_Count (1)
	0x19,0x01,
	0x29,0x02,
	0x95,0x03,
	0x91,0x02,
	0xc0				// End_Collection
//18, 70
};
*/

PROGMEM char usbHidReportDescriptorZZZ[82] = { // 146
	0x05,0x01,		//Usage_Page (Generic Desktop)
	0x09,0x04,		//Usage (Joystick)
// (4)

// Main joystick x/y
	0xA1,0x01,		//Collection (Application)
	0x85,0x01,				//Report ID (1)
	0x09,0x01,			//Usage (Pointer)
	//0xA1,0x00,			//Collection (Physical)		
	0x09,0x30,				//Usage (X)
	0x09,0x31,				//Usage (Y)
	0x15,0x81,				//Logical_Minimum (-127)
	0x25,0x7F,				//Logical Maximum (127)
	0x75,0x08,				//Report_Size (8)
	0x95,0x02,				//Report_Count (2)
	0x81,0x02,				//Input (Data, Var, Abs)		
// (22)
/*
// Second joystick Rx/Ry
	0x09,0x33,				//Usage (Rx)
	0x09,0x34,				//Usage (Ry)
	0x15,0x81,				//Logical_Minimum (-127)
	0x25,0x7F,				//Logical Maximum (127)
	0x75,0x08,				//Report_Size (8)
	0x95,0x02,				//Report_Count (2)
	0x81,0x02,				//Input (Data, Var, Abs)		
// (14)

// Hat switch
	0x09,0x39,				//Usage (Hat switch)
	0x15,0x00,				//Logical_Minimum (0)
	0x25,0x07,				//Logical_Maximum (7)
	0x75,0x05,				//Report_Size (5)
	0x95,0x01,				//Report_Count (1)
	0x81,0x03,				//Input (Data, Var, Abs)
// Round to one byte
	0x75,0x03,				//Report_Size (3)
	0x95,0x01,				//Report_Count (1)
	0x81,0x03,				//Input (Data, Var, Abs)
// (18)
*/
// Digital fire buttons
	0x05,0x09,				//Usage_Page (Button)
	0x19,0x01,				//Usage_Minimum (Button 1)
	0x29,0x10,				//Usage_Maximum (Button 16)
	0x15,0x00,				//Logical_Minimum (0)
	0x25,0x01,				//Logical_Maximum (1)
	0x75,0x01,				//Report_Size (1)
	0x95,0x10,				//Report_Count (16)
	//0x55,0x00,				//Unit_Exponent (0)
	//0x65,0x00,				//Unit (None)
	0x81,0x02,				//Input (Data, Var, Abs)
	//0xC0,				//End_Collection
	0xC0,			//End_Collection
// (22)

// --- Report 2 -----------------------------------------------------------

	0x05,0x01,		//Usage_Page (Generic Desktop)
	0x09,0x04,		//Usage (Joystick)
// (4)

// Main joystick x/y
	0xA1,0x01,		//Collection (Application)
	0x85,0x02,				//Report ID (2)
	0x09,0x01,			//Usage (Pointer)
	//0xA1,0x00,			//Collection (Physical)		
	0x09,0x30,				//Usage (X)
	0x09,0x31,				//Usage (Y)
	0x15,0x81,				//Logical_Minimum (-127)
	0x25,0x7F,				//Logical Maximum (127)
	0x75,0x08,				//Report_Size (8)
	0x95,0x02,				//Report_Count (2)
	0x81,0x02,				//Input (Data, Var, Abs)		
// (22)
/*
// Second joystick Rx/Ry
	0x09,0x33,				//Usage (Rx)
	0x09,0x34,				//Usage (Ry)
	0x15,0x81,				//Logical_Minimum (-127)
	0x25,0x7F,				//Logical Maximum (127)
	0x75,0x08,				//Report_Size (8)
	0x95,0x02,				//Report_Count (2)
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
*/
// Digital fire buttons
	0x05,0x09,				//Usage_Page (Button)
	0x19,0x01,				//Usage_Minimum (Button 1)
	0x29,0x10,				//Usage_Maximum (Button 16)
	0x15,0x00,				//Logical_Minimum (0)
	0x25,0x01,				//Logical_Maximum (1)
	0x75,0x01,				//Report_Size (1)
	0x95,0x10,				//Report_Count (16)
	//0x55,0x00,				//Unit_Exponent (0)
	//0x65,0x00,				//Unit (None)
	0x81,0x02,				//Input (Data, Var, Abs)
	//0xC0,				//End_Collection
	0xC0,			//End_Collection
// (22)
};

//	0x09, 0x00		// Usage undefined (padding)
