#ifndef uchar
#define uchar   unsigned char
#endif

#define HAT_UP      0x01
#define HAT_RIGHT   0x02
#define HAT_DOWN    0x04
#define HAT_LEFT    0x08

typedef struct {
	uchar	b1;			// 0
	uchar	b2;			// 1
	uchar	hat;		// 2
	char	x;			// 3
	char	y;			// 4
	char	rx;			// 5
	char	ry;			// 6
	char	padding;    // 7
} report_t;

//unused at the moment because Switch does not support analog buttons
typedef struct {
	uchar	a;
	uchar	b;
	uchar	x;
	uchar	y;
	uchar	l;
	uchar	r;
	uchar	black;
	uchar	white;
} reportAnalogButtons_t;
