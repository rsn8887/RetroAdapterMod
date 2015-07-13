/*	D5	Data
	D7	Latch
	B0	Clock
*/

#define	FAM_DAT	(1<<5)
#define	FAM_LAT	(1<<7)
#define	FAM_CLK	(1<<0)

void ReadFamicom(report_t *reportBuffer, reportMouse_t *reportBufferMouse);
uchar FamicomRead(void);
uchar FamicomReadByte(void);
