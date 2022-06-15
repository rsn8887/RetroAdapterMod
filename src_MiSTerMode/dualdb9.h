void ReadDualDB9(report_t *reportBuffer);

void Read2ndDirect(report_t *reportBuffer);
void Read2ndFamicom(report_t *reportBuffer);
uchar Famicom2Read(void);

/*	B2	Data
	C2	Latch
	C1	Clock
*/

#define	FAM2_DAT	(1<<2)
#define	FAM2_LAT	(1<<2)
#define	FAM2_CLK	(1<<1)
#define FAMDELAY	24
