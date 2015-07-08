#include "stm32f10x.h"
#include "pwm.h"
#include "rc.h"

#define CH1	1
#define CH2 2
#define CH3 3
#define CH4 4
#define CH5 5
#define CH6 6
#define CH7 7
#define CH8 8

static FLIGHT_TYPE flight_type = xMODE;
void setFlightType(FLIGHT_TYPE type)
{
	flight_type = type;
}

void update_pwm(unsigned int channel, unsigned int pulse)
{

	if(pulse > 0xffff)pulse = 0xffff;

	switch(channel)
	{
		case 1:	TIM3->CCR1 = pulse;break;	
		case 2:	TIM3->CCR2 = pulse;break;
		case 3:	TIM3->CCR3 = pulse;break;
		case 4:	TIM3->CCR4 = pulse;break;
		case 8:	TIM4->CCR1 = pulse;break;	
		case 7:	TIM4->CCR2 = pulse;break;
		case 6:	TIM4->CCR3 = pulse;break;
		case 5:	TIM4->CCR4 = pulse;break;				
		default:break;	
	}
}


void PWM_protect(void)
{
	int i;
	update_pwm(CH1,1000);
	update_pwm(CH2,1000);
	update_pwm(CH3,1000);
	update_pwm(CH4,1000);
	update_pwm(CH5,1000);
	update_pwm(CH6,1000);
	update_pwm(CH7,1000);
	update_pwm(CH8,1000);
}
/*
	1  2
	4  3
*/
void PWM_output(float angleRateOutput[3],float altitudeOutput,float angle[3],unsigned short pwm[6])
{
	float temp[4];
	int i,tempp[4];
	unsigned short rc[6];
	
	get_rc(rc);
	
	temp[0] = 1000 + (rc[2]-1000)*1.2631-8.0631 + angleRateOutput[0] - angleRateOutput[1] + angleRateOutput[2];
	temp[1] = 		  rc[2] 					- angleRateOutput[0] - angleRateOutput[1] - angleRateOutput[2];
	temp[2] = 		  rc[2] 					- angleRateOutput[0] + angleRateOutput[1] + angleRateOutput[2];
	temp[3] = 1000 + (rc[2]-1000)*1.2631-8.0631 + angleRateOutput[0] + angleRateOutput[1] - angleRateOutput[2];
	
	for(i=0;i<4;i++)
	{
		tempp[i] = (int)temp[i];
		if(tempp[i]>2000)tempp[i] = 2000;
		else if(tempp[i]<1000)tempp[i] = 1000;
	}
	
	update_pwm(CH1,tempp[0]);
	update_pwm(CH2,tempp[1]);
	update_pwm(CH3,tempp[2]);
	update_pwm(CH4,tempp[3]);
	pwm[0] = (u16)tempp[0];
	pwm[1] = (u16)tempp[1];
	pwm[2] = (u16)tempp[2];
	pwm[3] = (u16)tempp[3];
}
/*
void PWM_output(float angleRateOutput[3],float altitudeOutput,float angle[3])
{
	float temp[4];
	int i,tempp[4];
	
	//
	if(flight_type == xMODE)
	{
		temp[0] = altitudeOutput + angleRateOutput[0] + angleRateOutput[1] + angleRateOutput[2];
		temp[1] = altitudeOutput - angleRateOutput[0] + angleRateOutput[1] - angleRateOutput[2];
		temp[2] = altitudeOutput - angleRateOutput[0] - angleRateOutput[1] + angleRateOutput[2];
		temp[3] = altitudeOutput + angleRateOutput[0] - angleRateOutput[1] - angleRateOutput[2];
	}
	else if(flight_type == crossMODE)
	{
		
	}
	
	for(i=0;i<4;i++)
	{
		tempp[i] = 1000 + (int)temp[i];
		if(tempp[i]>2000)tempp[i] = 2000;
		else if(tempp[i]<1000)tempp[i] = 1000;
	}
	
	update_pwm(CH1,tempp[0]);
	update_pwm(CH2,tempp[1]);
	update_pwm(CH3,tempp[2]);
	update_pwm(CH4,tempp[3]);
	
	//
	for(i=0;i<3;i++)
	{
		tempp[i] = 1500 - (int)angle[i]*500/45; //中立值 - 角度*500/舵机最大角度
		if(tempp[i]>2000)tempp[i] = 2000;
		else if(tempp[i]<1000)tempp[i] = 1000;
	}
	
	update_pwm(CH5,tempp[0]);
	update_pwm(CH6,tempp[1]);
	update_pwm(CH7,tempp[2]);
}
*/
