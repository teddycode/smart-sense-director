#include "sys.h"

const u8 gImage_jiantou[1800] = { /* 0X00,0X10,0X1E,0X00,0X1E,0X00,0X01,0X1B, */
0X9E,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XDF,0XFF,0XFF,0XFF,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0X9E,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XFF,0XFF,0X30,0X84,0X8E,0X73,0XFF,0XFF,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XFF,0XFF,
0XCB,0X5A,0X00,0X00,0XEF,0X7B,0XFF,0XFF,0XDF,0XFF,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XFF,0XFF,0X2C,0X63,0X00,0X00,
0X00,0X00,0XEF,0X7B,0XFF,0XFF,0XDF,0XFF,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XFF,0XFF,0X2C,0X63,0X00,0X00,0X00,0X00,0X00,0X00,
0XEF,0X7B,0XFF,0XFF,0XDF,0XFF,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XFF,0XFF,0X0C,0X63,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XEF,0X7B,
0XFF,0XFF,0XDF,0XFF,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0X9E,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XFF,0XFF,
0X0C,0X63,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XCF,0X7B,0XFF,0XFF,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0X4D,0X6B,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XEF,0X7B,0XFF,0XFF,0XDF,0XFF,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0X9E,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XDF,0XFF,
0XFF,0XFF,0XDF,0XFF,0X38,0XC6,0X30,0X84,0X24,0X21,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XEF,0X7B,0XFF,0XFF,0XDF,0XFF,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XFF,0XFF,0X7D,0XEF,0X71,0X8C,0X86,0X31,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0XF0,0X7B,0XFF,0XFF,0XDF,0XFF,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0X9E,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XFF,0XFF,0XFF,0XFF,0XD3,0X9C,0X04,0X21,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0XEF,0X7B,0XFF,0XFF,0XDF,0XFF,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XFF,0XFF,0X3C,0XE7,0X8A,0X52,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0XEF,0X7B,0XFF,0XFF,0XDF,0XFF,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XFF,0XFF,0XBA,0XD6,0X24,0X21,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X10,0X84,0XFF,0XFF,0XBE,0XF7,0XBE,0XF7,0X9E,0XF7,0XBE,0XF7,0XBE,0XF7,0XFF,0XFF,
0XFB,0XDE,0XE3,0X18,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XF7,0XBD,
0XFF,0XFF,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XDF,0XFF,0XBE,0XF7,0XA6,0X31,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X24,0X21,0XFB,0XDE,0XDF,0XFF,0XBE,0XF7,
0X9E,0XF7,0XBE,0XF7,0XFF,0XFF,0XAE,0X73,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X24,0X21,0XFB,0XDE,0XFF,0XFF,0X9E,0XF7,0XBE,0XF7,0XBE,0XF7,0XFF,0XFF,
0XBA,0XD6,0X20,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X24,0X21,0XFB,0XDE,
0XFF,0XFF,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0X9E,0XF7,0XFF,0XFF,0XEB,0X5A,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X04,0X21,0XFB,0XDE,0XFF,0XFF,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XDF,0XFF,0X1C,0XE7,0X61,0X08,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X21,0X00,0XC3,0X18,0XA6,0X31,0X08,0X42,0X28,0X42,
0XE4,0X18,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X04,0X21,0XFB,0XDE,0XFF,0XFF,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XFF,0XFF,0X14,0XA5,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X49,0X4A,
0XF3,0X9C,0X9A,0XD6,0X7D,0XEF,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0X4D,0X6B,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X04,0X21,0XDB,0XDE,0XFF,0XFF,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XFF,0XFF,0X0C,0X63,
0X00,0X00,0X00,0X00,0X00,0X00,0X65,0X29,0XF7,0XBD,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XDF,0XFF,0XBE,0XF7,0XBE,0XF7,0XFF,0XFF,0X2C,0X63,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X24,0X21,0XFB,0XDE,0XFF,0XFF,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XFF,0XFF,0X86,0X31,0X00,0X00,0X00,0X00,
0X8A,0X52,0X9E,0XF7,0XFF,0XFF,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XFF,0XFF,0X2C,0X63,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X24,0X21,
0XFB,0XDE,0XFF,0XFF,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0X5D,0XEF,0XA2,0X10,0X00,0X00,0XE7,0X39,0XDF,0XFF,0XFF,0XFF,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XFF,0XFF,
0X2C,0X63,0X00,0X00,0X00,0X00,0X00,0X00,0X04,0X21,0XFB,0XDE,0XFF,0XFF,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0X1C,0XE7,0X00,0X00,0X61,0X08,0X1C,0XE7,0XFF,0XFF,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XFF,0XFF,0X2C,0X63,0X00,0X00,
0X00,0X00,0X24,0X21,0XFB,0XDE,0XFF,0XFF,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0X9A,0XD6,0X00,0X00,
0X30,0X84,0XFF,0XFF,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XFF,0XFF,0X2C,0X63,0X00,0X00,0X24,0X21,0XDB,0XDE,
0XFF,0XFF,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0X9E,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0X79,0XCE,0X61,0X08,0X3C,0XE7,0XFF,0XFF,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XFF,0XFF,0XEB,0X5A,0XC3,0X18,0XFB,0XDE,0XFF,0XFF,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0X7D,0XEF,0X38,0XC6,0XDF,0XFF,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XDF,0XFF,
0X9A,0XD6,0XBA,0XD6,0XFF,0XFF,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XFF,0XFF,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XFF,0XFF,0XFF,0XFF,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0X9E,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,};

const u8 gImage_dianzan[1800] = { /* 0X00,0X10,0X1E,0X00,0X1E,0X00,0X01,0X1B, */
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBF,0XF7,0XBF,0XF7,0XBF,0XF7,0XBF,0XF7,0XBF,0XF7,0XBF,0XF7,0XBE,0XF7,0XBE,0XF7,
0X9E,0XFF,0X7D,0XFF,0X57,0XFE,0XAF,0XFC,0X0B,0XFC,0X6D,0XFC,0X37,0XFE,0X5D,0XFF,
0X9E,0XFF,0XDE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBF,0XF7,0XBF,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XFF,0XBE,0XFF,0X5B,0XFF,0X0C,0XC4,
0XC3,0XAA,0XC4,0XCB,0X25,0XE4,0X43,0XCB,0XA0,0X91,0X8F,0XDC,0X9E,0XFF,0XBE,0XFF,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBF,0XF7,0XBF,0XFF,0XBE,0XF7,0XBE,0XF7,0XBE,0XFF,0XBE,0XF7,
0XDE,0XF7,0XBE,0XF7,0X9E,0XFF,0X3B,0XFF,0XE6,0XBA,0X82,0XD3,0XE7,0XFE,0X45,0XFF,
0X64,0XFF,0X26,0XFF,0XE5,0XFC,0X60,0XA1,0XF6,0XFD,0X9F,0XFF,0XBE,0XF7,0XDE,0XF7,
0XBE,0XF7,0XBF,0XFF,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBF,0XF7,0XBE,0XF7,0XBE,0XFF,0XBE,0XF7,0XDF,0XEF,0XBE,0XFF,
0X7D,0XFF,0XCB,0XC3,0X00,0XC3,0XC7,0XFE,0X62,0XFF,0XC0,0XFF,0XE0,0XFF,0X82,0XFF,
0XA6,0XFE,0X60,0XCA,0X2E,0XD4,0X7F,0XFF,0XBE,0XF7,0XDE,0XEF,0XBE,0XF7,0X9F,0XFF,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XDF,0XF7,0XBF,0XF7,0X9E,0XFF,0X9E,0XF7,0XBF,0XEF,0XBF,0XFF,0XF7,0XFD,0XA0,0XA1,
0X65,0XFE,0XA2,0XFF,0XC1,0XFF,0XE0,0XFF,0XE0,0XF7,0XC2,0XFF,0XE6,0XFE,0XC0,0XBA,
0X6F,0XDC,0X5F,0XFF,0XBE,0XF7,0XDE,0XEF,0XBE,0XF7,0X9F,0XFF,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBF,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0X9D,0XF7,0X9E,0XF7,0XBF,0XF7,0X9E,0XFF,0XAC,0XE3,0XC0,0XCA,0X64,0XFF,0XE0,0XFF,
0XE0,0XFF,0XE1,0XFF,0XE0,0XFF,0X83,0XFF,0X26,0XFE,0XC0,0X99,0X16,0XFE,0X7F,0XFF,
0XBE,0XF7,0XDE,0XEF,0XBE,0XF7,0XBF,0XFF,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0X9E,0XF7,
0X9E,0XF7,0XBE,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XFF,0X9E,0XFF,0XDD,0XF7,0XDD,0XF7,
0XBF,0XFF,0X9C,0XFF,0XC3,0XB2,0XA3,0XF4,0X64,0XFF,0XC0,0XFF,0XE0,0XFF,0XC0,0XFF,
0XA0,0XFF,0X05,0XFF,0X40,0XCA,0XA7,0XCA,0X9D,0XFF,0XBF,0XFF,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBF,0XF7,0XBF,0XF7,0X9E,0XF7,0X9E,0XF7,0X9E,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0X9F,0XFF,0XBF,0XFF,0XDD,0XF7,0XBE,0XF7,0X9F,0XFF,0X3B,0XFF,
0X80,0XAA,0XE4,0XFD,0X83,0XFF,0XC0,0XFF,0XE0,0XFF,0XA1,0XFF,0X24,0XFF,0XE0,0XDB,
0X60,0XA1,0X39,0XFE,0XBE,0XFF,0XDE,0XEF,0XBF,0XF7,0XBE,0XFF,0XBE,0XF7,0XDE,0XF7,
0XBF,0XF7,0XBF,0XF7,0XBE,0XF7,0XBE,0XF7,0XBF,0XFF,0XBE,0XF7,0XDE,0XF7,0XDE,0XEF,
0XBF,0XF7,0XBF,0XF7,0XDF,0XEF,0XBF,0XF7,0X5F,0XFF,0X9B,0XFE,0X60,0XAA,0X45,0XFE,
0XA2,0XFF,0XE0,0XFF,0XE0,0XFF,0X83,0XFF,0X44,0XFD,0X80,0X99,0X94,0XF5,0X9F,0XFF,
0XBE,0XFF,0XDE,0XF7,0XBE,0XF7,0XBF,0XF7,0XDE,0XF7,0XDE,0XF7,0XBF,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBF,0XFF,0XBF,0XF7,0XDE,0XF7,0XBE,0XF7,0XBF,0XFF,0XBF,0XFF,
0XDE,0XFF,0XBE,0XFF,0X3F,0XFF,0XF6,0XFD,0X40,0XA2,0XE4,0XFE,0XC1,0XFF,0XE0,0XFF,
0XE0,0XFF,0X44,0XFF,0X80,0XBA,0X8E,0XFC,0X9E,0XFF,0XBF,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBF,0XF7,0XBF,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBF,0XF7,0XBE,0XF7,0XBF,0XFF,0X7E,0XFF,0XDA,0XFE,0X94,0XFD,0X0F,0XED,0XCE,0XEC,
0XAF,0XFC,0XA2,0XBA,0XC0,0XBB,0X63,0XFF,0XC1,0XFF,0XC0,0XFF,0XA1,0XFF,0X83,0XFE,
0X00,0XAA,0X78,0XFE,0XBE,0XFF,0XBF,0XF7,0XBE,0XFF,0XBE,0XFF,0XBF,0XF7,0XBF,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XFF,
0X5C,0XFF,0X4E,0XD4,0X62,0XAA,0XE1,0XBA,0XA2,0XCB,0X81,0XCB,0XC0,0XC2,0XC0,0XDB,
0XE4,0XFE,0XA1,0XFF,0XC0,0XFF,0XC0,0XFF,0X81,0XFF,0XC2,0XFD,0X01,0XAA,0X9A,0XFE,
0XBE,0XFF,0XDF,0XEF,0XBE,0XFF,0X9E,0XFF,0XBE,0XF7,0XBF,0XF7,0XBE,0XF7,0XBE,0XFF,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0X9F,0XFF,0X7D,0XFF,0X28,0XCB,0X00,0XAA,
0XC3,0XFD,0X45,0XFF,0X45,0XFF,0X26,0XFF,0XE7,0XFE,0X06,0XFF,0X63,0XFF,0XC1,0XFF,
0XE0,0XFF,0XE0,0XFF,0X62,0XFF,0X62,0XFD,0XC1,0XA1,0XBB,0XFE,0XDF,0XFF,0XFF,0XE7,
0XBE,0XF7,0X9E,0XFF,0XBE,0XF7,0XBF,0XF7,0XBE,0XF7,0XBE,0XFF,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XFF,0X9F,0XFF,0X94,0XF5,0XA0,0XA1,0X07,0XFE,0X63,0XFF,0XC0,0XFF,
0XA1,0XFF,0X00,0XFF,0X60,0XC4,0X20,0XFE,0XE2,0XFE,0X82,0XFF,0XC2,0XFF,0XC1,0XFF,
0X82,0XFF,0XA2,0XFD,0X80,0XA1,0X59,0XFE,0XBE,0XFF,0XFF,0XEF,0XBE,0XF7,0XBE,0XFF,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XFF,0XBE,0XFF,0XBE,0XF7,0XDE,0XF7,0XBE,0XFF,
0X7E,0XFF,0XEB,0XC3,0X01,0XDC,0X05,0XFF,0XC1,0XFF,0XE0,0XFF,0XC1,0XFF,0X20,0XFF,
0XE0,0XDD,0X80,0XE5,0XA0,0XC3,0XA0,0XCB,0XC4,0XFE,0X63,0XFF,0XA1,0XFF,0X63,0XFE,
0X40,0XB1,0X34,0XFD,0X5C,0XFF,0XBE,0XFF,0X9E,0XFF,0XBF,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XFF,0XBE,0XF7,0XDE,0XEF,0X9E,0XFF,0X5E,0XFF,0X89,0XC3,
0XE3,0XF4,0X24,0XFF,0XE1,0XFF,0XE0,0XF7,0XC0,0XFF,0XC1,0XFF,0XC1,0XFF,0X83,0XFF,
0XC5,0XFE,0X60,0XCB,0XE0,0XB2,0XE5,0XFE,0XA2,0XFF,0X23,0XFF,0X00,0XBA,0X23,0XD2,
0X08,0XC3,0X93,0XED,0XBE,0XFF,0X9F,0XFF,0XBE,0XF7,0XDE,0XF7,0XBE,0XF7,0XBF,0XFF,
0XBE,0XFF,0XBE,0XF7,0XDE,0XEF,0X9E,0XFF,0X5E,0XFF,0XEC,0XDB,0XA1,0XDB,0X05,0XFF,
0XC1,0XFF,0XE0,0XF7,0XE0,0XFF,0XC0,0XFF,0XE0,0XFF,0XC0,0XFF,0XA2,0XFF,0X44,0XFF,
0XA0,0XBA,0XE0,0XFC,0X63,0XFF,0X83,0XFF,0XA0,0XCB,0XC1,0XEB,0X46,0XFC,0X80,0X99,
0X78,0XFE,0X9F,0XFF,0XBE,0XF7,0XDE,0XEF,0XBE,0XF7,0XBF,0XFF,0XBE,0XFF,0XBE,0XF7,
0XDF,0XEF,0XBE,0XFF,0X5E,0XFF,0X8B,0XD3,0X80,0XA9,0X04,0XFE,0X83,0XFF,0XE1,0XFF,
0XC1,0XFF,0XC0,0XFF,0XE0,0XFF,0XE0,0XFF,0XC0,0XFF,0X63,0XFF,0X20,0XEC,0XC0,0XE3,
0X63,0XFF,0XA1,0XFF,0X40,0XF6,0X43,0XFE,0X88,0XFE,0X20,0XB2,0X0E,0XD4,0X5E,0XFF,
0XBE,0XF7,0XDE,0XEF,0XBE,0XF7,0X9F,0XFF,0XBE,0XFF,0XBE,0XF7,0XDF,0XEF,0X9E,0XFF,
0X9A,0XFE,0XA3,0XA9,0X21,0XEB,0X40,0XDB,0X83,0XFE,0X83,0XFF,0X82,0XFF,0XA1,0XFF,
0XC0,0XFF,0XC1,0XFF,0X82,0XFF,0X24,0XFF,0XA0,0XD3,0X00,0XF5,0X82,0XFF,0XC0,0XFF,
0XE1,0XFF,0XA2,0XFF,0X04,0XFF,0XE0,0XDB,0XE8,0XBA,0X3E,0XFF,0XBE,0XFF,0XFE,0XEF,
0XBE,0XF7,0X9E,0XFF,0XBE,0XFF,0XBE,0XF7,0XBE,0XEF,0X9E,0XFF,0X9A,0XFE,0X82,0XA9,
0XC7,0XFC,0X44,0XFD,0X60,0XB3,0X60,0XE5,0X03,0XFF,0X63,0XFF,0X82,0XFF,0X63,0XFF,
0XC3,0XFE,0XC0,0XE4,0XC0,0XCB,0XE5,0XFE,0XA1,0XFF,0XE0,0XFF,0XE0,0XF7,0XE0,0XFF,
0X43,0XFF,0XC1,0XEC,0X65,0XB2,0X1D,0XFF,0XBE,0XFF,0XDE,0XEF,0XBE,0XF7,0X9E,0XFF,
0XBE,0XFF,0XBE,0XF7,0XDF,0XEF,0X9E,0XFF,0X1C,0XFF,0X65,0XC2,0X00,0XE3,0X87,0XFE,
0X63,0XFE,0X60,0XC4,0X80,0XC3,0XA0,0XCB,0XE0,0XC3,0XA0,0XBB,0X20,0XC3,0X80,0XAA,
0X63,0XFE,0X83,0XFF,0XC1,0XFF,0XE0,0XFF,0XE0,0XF7,0XC1,0XFF,0X42,0XFF,0X22,0XF5,
0X64,0XAA,0X1D,0XFF,0XBE,0XFF,0XDF,0XEF,0XBE,0XF7,0X9E,0XFF,0XBE,0XFF,0XBE,0XF7,
0XDF,0XEF,0XBE,0XFF,0X79,0XFE,0X04,0XB2,0XA0,0XB1,0X21,0XFD,0X26,0XFF,0X45,0XFF,
0X84,0XFE,0XA2,0XFD,0X80,0XFD,0XC2,0XFD,0X48,0XFE,0X01,0XFD,0XC0,0XED,0XA2,0XFF,
0XC0,0XFF,0XE0,0XFF,0XE0,0XFF,0XC1,0XFF,0X43,0XFF,0XE2,0XEC,0XA6,0XB2,0X3E,0XFF,
0XBE,0XFF,0XDE,0XEF,0XBE,0XF7,0X9E,0XFF,0XBE,0XFF,0XBE,0XF7,0XDF,0XEF,0XBE,0XFF,
0X17,0XFE,0XC2,0XA9,0XC8,0XFC,0XC0,0XD2,0XC0,0XDC,0X03,0XFF,0X44,0XFF,0X43,0XFF,
0X43,0XFF,0X24,0XFF,0XC6,0XFE,0X80,0XE4,0X60,0XF6,0XA1,0XFF,0XC0,0XFF,0XE0,0XFF,
0XE0,0XFF,0XC1,0XFF,0X23,0XFF,0X20,0XDC,0X69,0XC3,0X5E,0XFF,0XBE,0XFF,0XDE,0XEF,
0XBE,0XF7,0X9F,0XFF,0XBE,0XFF,0XBE,0XF7,0XDF,0XEF,0X9E,0XFF,0X1C,0XFF,0X65,0XBA,
0XE4,0XFB,0X68,0XFE,0XC0,0XE4,0X80,0XC4,0XE0,0XD4,0X40,0XDD,0X60,0XDD,0X20,0XDD,
0XC0,0XCB,0X40,0XD4,0X83,0XFF,0XC1,0XFF,0XC1,0XFF,0XC1,0XFF,0XE0,0XFF,0XC1,0XFF,
0X05,0XFF,0X00,0XBB,0XB0,0XE4,0X7F,0XFF,0XBE,0XFF,0XDE,0XEF,0XBF,0XF7,0X9F,0XFF,
0XBE,0XFF,0XBE,0XF7,0XDF,0XF7,0X9E,0XFF,0X5D,0XFF,0XCB,0XD3,0X20,0XBA,0X80,0XFC,
0XE7,0XFE,0X07,0XFF,0X64,0XFE,0XE1,0XFD,0XE1,0XFD,0X24,0XFE,0X83,0XFD,0XC0,0XE4,
0XC2,0XFF,0XE1,0XFF,0X83,0XFF,0X20,0XE6,0XA0,0XFF,0XA2,0XFF,0X66,0XFE,0X20,0XAA,
0X78,0XFE,0X9F,0XFF,0XBE,0XF7,0XDE,0XF7,0XBF,0XF7,0X9F,0XFF,0XBE,0XFF,0XBE,0XF7,
0XDE,0XF7,0XBE,0XFF,0X7E,0XFF,0X4E,0XD4,0X25,0XF4,0X00,0XF4,0X40,0XBB,0X20,0XF5,
0X65,0XFE,0XE7,0XFE,0XC7,0XFE,0XE4,0XFD,0XC0,0XD3,0XE2,0XFD,0X83,0XFF,0X83,0XFF,
0XE1,0XFD,0XE0,0XC3,0X63,0XFF,0X45,0XFF,0XE2,0XE3,0X06,0XC3,0X9D,0XFF,0X9F,0XFF,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBF,0XFF,0X57,0XFE,0X62,0XB2,0X2B,0XFE,0X83,0XFD,0XA0,0XD3,0X20,0XB3,0X20,0XB3,
0X20,0XC3,0XE0,0XD3,0X20,0XD4,0XC3,0XFE,0X05,0XFF,0X24,0XFE,0XE0,0XC1,0XE6,0XFC,
0X87,0XFE,0X04,0XFD,0X60,0X99,0XF6,0XFD,0XBE,0XFF,0XDF,0XF7,0XBE,0XF7,0XBE,0XFF,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0X9F,0XFF,0X9D,0XFF,
0XCE,0XE4,0X40,0XAA,0X67,0XFD,0XA9,0XFE,0XC8,0XFE,0X87,0XFE,0X88,0XFE,0XA8,0XFE,
0XE0,0XEC,0XC8,0XFE,0X65,0XFD,0X20,0XB2,0X40,0XB1,0XC4,0XEA,0X21,0XE3,0X80,0X99,
0X90,0XEC,0X5E,0XFF,0XBE,0XFF,0XDE,0XF7,0XBF,0XFF,0XBE,0XFF,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBF,0XF7,0XBE,0XFF,0X9D,0XFF,0X72,0XE5,
0XC5,0XAA,0XA2,0XBA,0XA3,0XD3,0X05,0XE4,0XA5,0XDB,0X81,0XBA,0X60,0XB2,0X04,0XC3,
0X64,0XA2,0X10,0XF5,0XF5,0XFD,0X0A,0XBB,0XE9,0XBA,0X73,0XF5,0X9E,0XFF,0X9E,0XFF,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBF,0XF7,0XBF,0XF7,0X9F,0XFF,0X9E,0XFF,0X1A,0XFF,0XB2,0XFD,
0XCC,0XFC,0X6A,0XF4,0XCD,0XFC,0X13,0XFE,0X72,0XFD,0X53,0XFD,0XDA,0XFE,0X3D,0XFF,
0X5E,0XFF,0X5E,0XFF,0X3E,0XFF,0X5E,0XFF,0X9E,0XFF,0XBF,0XEF,0X9E,0XF7,0X9E,0XF7,
0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0XBE,0XFF,};


