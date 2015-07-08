#include "main.h"


extern float halfT;
extern float mpu6050_OFFSET[6];
extern int isLock;
int main()
{
	FLY_MODE fly_mode = Hand_Angle;
	static unsigned int loop_count = 0;
	int i,resault;
	u16 debug;
	u16 debugdata[32];
	//
	static float basic[9];
	static float angle[3];
	static float angle_pre[3];
	static float angle_rate[3];
	static float acc_angle[2];
	static float bmp085[2];
	static float speed[3];
	static float q[4] = {1,0,0,0};
	static float realAltitude = 0;
	//
	int offset_flag = 0;
	float offset_buf[3] = {0};
	int offset_count = 0;
	//
	float tarAltitude;
	static unsigned short ch[6];
	static unsigned short pwm[4];
  static float tarRate[3];
	static float tarAngle[3];
	static float output[3];
	static float altitudeOutput;

	
	bsp_init();
	PWM_protect();
	sensor_init();
	environment_init();
	resault = parameter_init();
	if(resault != 1)
		goto error;

	//wait for ready to start
	while(!isReady())
		delay(100);
	
	//校准遥控器中立值
	calc_rc_mid();

	//lock
	lock:while(isLock)
	{ //}
		PWM_protect();
		doRcData();
		doRxData();
		sensor_read(basic, loop_count, bmp085);
		timeUpdate();
		//EKF(basic[3]*0.0174,basic[4]*0.0174,basic[5]*0.0174,basic[0],basic[1],basic[2],halfT,q);
		IMUupdate(basic[3]*0.0174,basic[4]*0.0174,basic[5]*0.0174,basic[0],basic[1],basic[2],q);
		//AHRSupdate(basic[3]*0.0174,basic[4]*0.0174,basic[5]*0.0174,basic[0],basic[1],basic[2],basic[6],basic[8],basic[7],q);
		angle[0] = Myatan2(2*(q[0]*q[1]+q[2]*q[3]),1-2*(q[1]*q[1]+q[2]*q[2]))/0.0174;
		angle[1] = Myasin(2*(q[0]*q[2]-q[3]*q[1]))/0.0174;
		angle[2] = Myatan2(2*(q[0]*q[3]+q[1]*q[2]),1-2*(q[2]*q[2]+q[3]*q[3]))/0.0174;
		acc_angle[0] = Myatan(Mysqrt(basic[0] * basic[0] + basic[2] * basic[2] ), basic[1])/0.0174;
		acc_angle[1] = -Myatan(Mysqrt(basic[1]* basic[1] + basic[2] * basic[2] ), basic[0])/0.0174;
		
		//offset_flag ++;
		
		if(offset_flag < 5000) //gyro offset init
		{
			offset_flag++;
			if(offset_flag > 2000)
			{
				for(i=0;i<3;i++)
				{
					offset_buf[i] += basic[i+3];
				}
				offset_count++;
				if(offset_count >= 2000)
				{
					offset_flag = 5001;
					mpu6050_OFFSET[3] = offset_buf[0]/2000;
					mpu6050_OFFSET[4] = offset_buf[1]/2000;
					mpu6050_OFFSET[5] = offset_buf[2]/2000;
					
					GPIO_SetBits(GPIOC, GPIO_Pin_0);
					GPIO_SetBits(GPIOC, GPIO_Pin_1);
					GPIO_SetBits(GPIOC, GPIO_Pin_2);
				}
				delay(1);
			}
		}
		
// 		uart_buffer[0] = halfT*20000;
// 		uart_buffer[1] = angle[0];
// 		uart_buffer[2] = acc_angle[0];
// 		uart_buffer[3] = basic[3];
// 		if(loop_count%200 == 0)
// 		uart_send(4);
		get_rc(ch);
		if((loop_count%100 == 0)&&(loop_count%200 != 0))
		{
		uart_putchar('$');
		uart_putchar('$');
		uart_putchar(37);
		uart_putchar(100);
		uart_putfloat(angle[0]);
		uart_putfloat(angle[1]);
		uart_putfloat(angle[2]);
		uart_putfloat(basic[3]);
		uart_putfloat(basic[4]);
		uart_putfloat(basic[5]);
		uart_putfloat(tarAngle[0]);
		uart_putfloat(tarAngle[1]);
		uart_putshort(0);
		}
		if(loop_count%200 == 0)
		{
			uart_putchar('$');
			uart_putchar('$');
			uart_putchar(25);
			uart_putchar(101);
			uart_putshort(ch[0]);
			uart_putshort(ch[1]);
			uart_putshort(ch[2]);
			uart_putshort(ch[3]);
			uart_putshort(ch[4]);
			uart_putshort(ch[5]);
			uart_putshort(pwm[0]);
			uart_putshort(pwm[1]);
			uart_putshort(pwm[2]);
			uart_putshort(pwm[3]);
			uart_putshort(0);
		}
		
		loop_count++;
	}
	
	//退出锁定
	loop_count = 0;
	
	while(1)
	{ //}
		GPIO_SetBits(GPIOC, GPIO_Pin_1);
		if(isLock != 0){
			parameter_save();
			loop_count=0;
			goto lock;
		}
		
		//environment_scan();
		
		sensor_read(basic, loop_count, bmp085);
		
		//姿态解算
		timeUpdate();
		IMUupdate(basic[3]*0.0174,basic[4]*0.0174,basic[5]*0.0174,basic[0],basic[1],basic[2],q);
		//AHRSupdate(basic[3]*0.0174,basic[4]*0.0174,basic[5]*0.0174,basic[0],basic[1],basic[2],basic[6],basic[8],basic[7],q);
		
		angle[0] = Myatan2(2*(q[0]*q[1]+q[2]*q[3]),1-2*(q[1]*q[1]+q[2]*q[2]))/0.0174;
		angle[1] = Myasin(2*(q[0]*q[2]-q[3]*q[1]))/0.0174;
		angle[2] = Myatan2(2*(q[0]*q[3]+q[1]*q[2]),1-2*(q[2]*q[2]+q[3]*q[3]))/0.0174;
		
		acc_angle[0] = Myatan(Mysqrt(basic[0] * basic[0] + basic[2] * basic[2] ), basic[1])/0.0174;
		acc_angle[1] = -Myatan(Mysqrt(basic[1]* basic[1] + basic[2] * basic[2] ), basic[0])/0.0174;
		
		GPIO_ResetBits(GPIOC, GPIO_Pin_1);
		
		//控制
		get_rc(ch);
		switch(fly_mode)
		{
			case Auto_Path:
				//nav(float point1[2],float point2[2],float speed[2],float output[3]);
			case Auto_Speed:
				//speed_loop(float tarSpeed[3],float realSpeed[3],float output[3]);
			case Hand_Angle:
				if(fly_mode == Hand_Angle)
				{
					tarAngle[0] = (ch[0]-1500)/25.0;
					tarAngle[1] = (ch[1]-1500)/25.0;
					tarAngle[2] = (ch[3]-1500)/25.0;
				}
				angle_loop(tarAngle,angle,&basic[3],output);
				break;
			case Hand_Rate:
				if(fly_mode == Hand_Rate)
				{
					tarRate[0] = (ch[0]-1500)/50.0;
					tarRate[1] = (ch[1]-1500)/50.0;
					tarRate[2] = (ch[3]-1500)/50.0;
				}
				rate_loop(tarRate,&basic[3],output);
			default:
				//tarAltitude = getTarAltitude();
				//altitudeOutput = altitude_loop(tarAltitude,realAltitude,speed[2],1);
				//float altitude_loop(float tarAltitude,float realAltitude,float speed,int speed_flag)
				break;
		}
		
		if(loop_count > 20)
		{
			PWM_output(output,altitudeOutput,angle,pwm);
		}
		
		/*
		if((my_abs(angle[0] - tarAngle[0]) < 1)&&(basic[0] < 2))
		{
			
		}
		*/
		
		//send message
// 		uart_buffer[0] = halfT*20000;
// 		uart_buffer[1] = angle[0];
// 		uart_buffer[2] = acc_angle[0];
// 		uart_buffer[3] = basic[3];
// 		uart_send(4);
		
		if((loop_count%100 == 0)&&(loop_count%200 != 0))
		{
		uart_putchar('$');
		uart_putchar('$');
		uart_putchar(37);
		uart_putchar(100);
		uart_putfloat(angle[0]);
		uart_putfloat(angle[1]);
		uart_putfloat(angle[2]);
		uart_putfloat(basic[3]);
		uart_putfloat(basic[4]);
		uart_putfloat(basic[5]);
		uart_putfloat(tarAngle[0]);
		uart_putfloat(tarAngle[1]);
		uart_putshort(0);
		}
		if(loop_count%200 == 0)
		{
			uart_putchar('$');
			uart_putchar('$');
			uart_putchar(25);
			uart_putchar(101);
			uart_putshort(ch[0]);
			uart_putshort(ch[1]);
			uart_putshort(ch[2]);
			uart_putshort(ch[3]);
			uart_putshort(ch[4]);
			uart_putshort(ch[5]);
			uart_putshort(pwm[0]);
			uart_putshort(pwm[1]);
			uart_putshort(pwm[2]);
			uart_putshort(pwm[3]);
			uart_putshort(0);
		}
		
		//
		doRxData();
		doRcData();
		loop_count++;
	}

	error:while(1)
	{ //}
		printf("On error, error code is %d.\n",resault);
		GPIO_SetBits(GPIOC, GPIO_Pin_2);
		delay(1000);
		GPIO_ResetBits(GPIOC, GPIO_Pin_2);
	}
	
}
