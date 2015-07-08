#include "iic.h"
#include "bsp.h"

#define	Slave_MPU6050	0xD0
#define	SMPLRT_DIV		0x19	
#define	CONFIG			  0x1A	
#define	GYRO_CONFIG		0x1B	
#define	ACCEL_CONFIG	0x1C	
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B
#define	WHO_AM_I	    0x75

void mpu6050_init(void)
{
	  unsigned char dat;
	
	  dat = 0x88 ;
    i2c_write(Slave_MPU6050, PWR_MGMT_1,    1,&dat) ;
    delay(60);
    dat = 0x08 ;
    i2c_write(Slave_MPU6050, PWR_MGMT_1,    1,&dat) ;
    delay(60);
    i2c_write(Slave_MPU6050, PWR_MGMT_1,    1,&dat) ;
    dat = 0x09;
    i2c_write(Slave_MPU6050, SMPLRT_DIV,    1,&dat) ;
    dat = 0x04;
    i2c_write(Slave_MPU6050, CONFIG,        1,&dat) ;
    dat = 0x00; //± 250 °/s
    i2c_write(Slave_MPU6050, GYRO_CONFIG,   1,&dat) ;
    dat = 0x08; // ± 4g
    i2c_write(Slave_MPU6050, ACCEL_CONFIG,  1,&dat) ;
}

void mpu6050_read(short mpu[6])
{
	unsigned char  BUF[14];	

    i2c_read(Slave_MPU6050,ACCEL_XOUT_H,6,&BUF[0]) ;
    i2c_read(Slave_MPU6050,TEMP_OUT_H,  2,&BUF[6]) ;
    i2c_read(Slave_MPU6050,GYRO_XOUT_H, 6,&BUF[8]) ;
		
	mpu[0] = (short)((BUF[0]<<8) + BUF[1]); //ax
	mpu[1] = (short)((BUF[2]<<8) + BUF[3]);//ay
	mpu[2] = (short)((BUF[4]<<8) + BUF[5]);//az
	mpu[3] = (short)((BUF[8]<<8) + BUF[9]);//gx
	mpu[4] = (short)((BUF[10]<<8) + BUF[11]);//gy
	mpu[5] = (short)((BUF[12]<<8) + BUF[13]);//gz
}
