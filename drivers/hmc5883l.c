#include "iic.h"
#define	HMC5883L 0x3C

void hmc5883l_init(void)
{
	unsigned char dat;
	
	dat = 0x14;
	i2c_write(HMC5883L,0x00,1,&dat);
	dat = 0x20;
	i2c_write(HMC5883L,0x01,1,&dat);
	dat = 0x00;
	i2c_write(HMC5883L,0x02,1,&dat);
}

void hmc5883l_read(short hmc[3])
{
	unsigned char  BUF[6];	

	i2c_read(HMC5883L,0x03,6,&BUF[0]);

	hmc[0] = (short)((BUF[0]<<8) + BUF[1]);
	hmc[1] = (short)((BUF[2]<<8) + BUF[3]);
	hmc[2] = (short)((BUF[4]<<8) + BUF[5]);
}

int hmc5883l_calc(void)
{
	return 1;
}
