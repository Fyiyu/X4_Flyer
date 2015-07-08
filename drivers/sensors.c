#include "mpu6050.h"
#include "hmc5883l.h"
#include "bmp085.h"
#include "bsp.h"
#include "Mymath.h"


#define FILTER_NUM 20
short ACC_X_BUF[FILTER_NUM]={0};
short ACC_Y_BUF[FILTER_NUM]={0};
short ACC_Z_BUF[FILTER_NUM]={0};	//加速度滑动窗口滤波数组
short GYR_X_BUF[FILTER_NUM]={0};
short GYR_Y_BUF[FILTER_NUM]={0};
short GYR_Z_BUF[FILTER_NUM]={0};	//陀螺仪滑动窗口滤波数组
short filter_full = 0;

void sensor_init(void)
{
	mpu6050_init();
	hmc5883l_init();
	bmp085_init();
}
float mpu6050_OFFSET[6] = {0};
void sensor_read(float basic[9], unsigned int loop_count, float bmp085[2])
{
	short temp_mpu[6];
	short temp_acc[3] = {0};
	short temp_gyr[3] = {0};
	short temp_hmc[3];
	int i;
	unsigned char filter_cnt=0;
	unsigned char bmp085_state = loop_count % 10;
	mpu6050_read(temp_mpu);
	hmc5883l_read(temp_hmc);
	bmp085_read(bmp085_state,bmp085);
	
	ACC_X_BUF[filter_cnt] = temp_mpu[0];//更新滑动窗口数组
	ACC_Y_BUF[filter_cnt] = temp_mpu[1];
	ACC_Z_BUF[filter_cnt] = temp_mpu[2];	
	GYR_X_BUF[filter_cnt] = temp_mpu[3];
	GYR_Y_BUF[filter_cnt] = temp_mpu[4];
	GYR_Z_BUF[filter_cnt] = temp_mpu[5];
	if((filter_cnt++) == FILTER_NUM)	
	{
		filter_cnt=0;
		filter_full = 1;
	}

	if(filter_full == 1)
	{
		for(i=0;i<FILTER_NUM;i++)
		{
			temp_acc[0] += ACC_X_BUF[i];
			temp_acc[1] += ACC_Y_BUF[i];
			temp_acc[2] += ACC_Z_BUF[i];
			temp_gyr[0] += GYR_X_BUF[i];
			temp_gyr[1] += GYR_Y_BUF[i];
			temp_gyr[2] += GYR_Z_BUF[i];
		}
		for(i=0;i<3;i++)
		{
			temp_mpu[i] = temp_acc[i] / FILTER_NUM;
			temp_mpu[i+3] = temp_gyr[i] / FILTER_NUM;
		}
	}
	
	for(i=0;i<3;i++)
		basic[i] = (temp_mpu[i])*0.0001220703125;
	for(i=3;i<6;i++)
		basic[i] = (temp_mpu[i])*0.00762939453125 - mpu6050_OFFSET[i];
	for(i=6;i<9;i++)
		basic[i] = (temp_hmc[i-6])*1.0;
	
	if(my_abs(basic[5])<1)basic[5] = 0.0;
}
