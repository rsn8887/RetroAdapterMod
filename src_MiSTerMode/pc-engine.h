void ReadPCE(report_t *reportBuffer);
void ReadPCFX(report_t *reportBuffer);

#define	PCE_DS	(1<<1)
#define PCE_UP	(1<<5)
#define	PCE_RT	(1<<4)
#define	PCE_DN	(1<<3)
#define	PCE_LF	(1<<2)
#define	PCE_OE	(1<<4)

/*	B1	DS	Data Select
	B5	UP	Up/I
	B4	RT	Right/II
	B3	DN	Down/Sel
	B2	LF	Left/Run
	D4	OE	Output Enable

	OE is also used by the auto-fire circuit to count the number of times that the controller
	is read, so that on the 4th or 8th read it can pulse the fire buttons.
*/
