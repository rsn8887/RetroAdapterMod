void ReadPSX(report_t *reportBuffer, reportNegCon_t *reportNegCon, reportAnalogButtons_t *reportBufferAnalogButtons);
uchar PSXCommand(uchar cmd);
void PSXSendCommandString(uchar *string, uchar len);

#define	PSX_ID_DIGITAL	0x41
#define PSX_ID_A_RED	0x73
#define	PSX_ID_A_GREEN	0x53
#define PSX_ID_NEGCON	0x23
#define PSX_ID_PS2_PRESSURE		0x79

#define DAT	(1<<5)
#define	CMD	(1<<4)
#define	ATT	(1<<3)
#define	CLK	(1<<2)
#define	ACK	(1<<1)
