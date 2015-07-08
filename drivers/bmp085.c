#include "iic.h"
#define	BMP085 0xEE
#define OSS 3

static short ac1,ac2,ac3,b1,b2,mb,mc,md;
static unsigned short ac4,ac5,ac6;

void bmp085_init(void)
{
    unsigned char buf[22];
    i2c_read(BMP085,0XAA,22,buf);
	  ac1 = (buf[0]<<8)+buf[1];
	  ac2 = (buf[2]<<8)+buf[3];
	  ac3 = (buf[4]<<8)+buf[5];
	  ac4 = (buf[6]<<8)+buf[7];
	  ac5 = (buf[8]<<8)+buf[9];
	  ac6 = (buf[10]<<8)+buf[11];
	  b1 = (buf[12]<<8)+buf[13];
	  b2 = (buf[14]<<8)+buf[15];
	  mb = (buf[16]<<8)+buf[17];
	  mc = (buf[18]<<8)+buf[19];
	  md = (buf[20]<<8)+buf[21];
}

void bmp085_read(unsigned char step,float bmp085[2])
{
	static long ut,up;
	unsigned char dat,buf[3];
	long x1, x2, x3, b3, b5, b6, p;
	unsigned long b4, b7;

	switch (step)
	{
		case 0:
			dat = 0x2E;
			i2c_write(BMP085,0xF4,1,&dat);
			break;
		case 2:
			i2c_read(BMP085,0xF6,2,buf);
			ut = (long)((buf[0]<<8)+buf[1]);
			break;
		case 3:
			dat = 0xF4;
			i2c_write(BMP085,0xF4,1,&dat);
			break;
		case 9:
			i2c_read(BMP085,0xF6,3,buf);
			up = ((buf[0]<<16)+(buf[1]<<8)+buf[2])>>(8-OSS);

			x1 = (ut - ac6) * ac5 >> 15;
			x2 = ((long) mc << 11) / (x1 + md);
			b5 = x1 + x2;
			bmp085[0] = ((b5 + 8) >> 4)/10.0;//¦

			b6 = b5 - 4000;
			x1 = (b2 * (b6 * b6 >> 12)) >> 11;
			x2 = ac2 * b6 >> 11;
			x3 = x1 + x2;
			b3 = (((long)ac1 * 4 + x3) + 2)>>2;
			x1 = ac3 * b6 >> 13;
			x2 = (b1 * (b6 * b6 >> 12)) >> 16;
			x3 = ((x1 + x2) + 2) >> 2;
			b4 = (ac4 * (unsigned long) (x3 + 32768)) >> 15;
			b7 = ((unsigned long) up - b3) * (50000 >> OSS);
			if( b7 < 0x80000000)
				p = (b7 * 2) / b4 ;
			else  
		    p = (b7 / b4) * 2;
			x1 = (p >> 8) * (p >> 8);
			x1 = (x1 * 3038) >> 16;
			x2 = (-7357 * p) >> 16;
			bmp085[1] = (p + ((x1 + x2 + 3791) >> 4))/100.0;//hPa
		default:break;
	}
}
