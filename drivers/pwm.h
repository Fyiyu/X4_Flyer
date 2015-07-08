#ifndef __PWM_H__
#define __PWM_H__

typedef enum
{
	xMODE = 0x00,
	crossMODE = 0x01
}FLIGHT_TYPE;

void PWM_protect(void);
void PWM_output(float angleRateOutput[3],float altitudeOutput,float angle[3],unsigned short pwm[4]);

#endif
