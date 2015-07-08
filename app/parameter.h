#ifndef __PARAMETER_H__
#define __PARAMETER_H__

#include "eeprom.h"
#include "uart.h"
#include "stm32f10x.h"

typedef struct
{
	struct
	{
		float R;
	}nav;
	struct
	{
		float p,i,d,imax,imin;
	}speed_pid[3];
	struct 
    {
        float p,i,d,imax,imin;
    }angle_pid[3];
	//
    struct
    {
        float p,i,d,imax,imin;
    }rate_pid[3];
	
	struct
	{
		float p,i,d,imax,imin;
	}altitude_pid;
	
	int flag;
} param_t;


u8 parameter_init(void);
void parameter_save(void);
void parameter_send(u8 who);
param_t* getParam(void);

#endif
