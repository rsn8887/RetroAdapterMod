void ReadSaturn(report_t *reportBuffer);

void wait_tl_low(void);
void wait_tl_high(void);

/*	Pin 2 - B5 - D1 (Down)
	Pin 3 - B4 - D0 (Up)
	Pin 4 - B3 - D6 (TH)
	Pin 5 - B2 - D5 (TR)
	Pin 6 - B1 - D4 (TL)
	Pin 7 - D4 - D3 (Right)
	Pin 8 - D0 - D2 (Left)
*/

#define	SAT_D1	(1<<5)
#define	SAT_DN	(1<<5)

#define	SAT_D0	(1<<4)
#define	SAT_UP	(1<<4)

#define SAT_D6	(1<<3)
#define	SAT_TH	(1<<3)

#define	SAT_D5	(1<<2)
#define	SAT_TR	(1<<2)

#define	SAT_D4	(1<<1)
#define	SAT_TL	(1<<1)

#define	SAT_D3	(1<<4)
#define	SAT_RT	(1<<4)

#define	SAT_D2	(1<<0)
#define	SAT_LF	(1<<0)
