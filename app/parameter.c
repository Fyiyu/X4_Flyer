#include "parameter.h"


static param_t parameter;
param_t* getParam()
{
	param_t* p = &parameter;
	return p;
}

u8 parameter_init(void)
{
	int resault;
	unsigned char * data;
	resault = EE_test(TIM1->CNT);
	if(resault == 0)
		return 0;
	
	data = (unsigned char *)(&(parameter.rate_pid[0]));
	EE_readstr(0x0080,data,80);
	delay(11);
	data = (unsigned char *)(&parameter);
	EE_readstr(0x0100,data,124);
	
	parameter.flag = 0;
	
	return 1;
}

void parameter_save(void)
{
	unsigned char * data;
	if(parameter.flag != 0)
	{
		data = (unsigned char *)(&(parameter.rate_pid[0]));
		EE_put(0x0080,data,80);
		delay(11);
		data = (unsigned char *)(&parameter);
		EE_put(0x0100,data,124);
		
		parameter.flag = 0;
	}
	
}

void parameter_send(u8 who)
{
	switch(who)
	{
	case 0:
		uart_putchar('$');
		uart_putchar('$');
		uart_putchar(28);
		uart_putchar(0xC8);
		uart_putchar(0);
		uart_putfloat(parameter.angle_pid[0].p);
		uart_putfloat(parameter.angle_pid[0].i);
		uart_putfloat(parameter.angle_pid[0].d);
		uart_putfloat(parameter.angle_pid[0].imax);
		uart_putfloat(parameter.angle_pid[0].imin);
		uart_putshort(0);

		uart_putchar('$');
		uart_putchar('$');
		uart_putchar(28);
		uart_putchar(0xC8);
		uart_putchar(1);
		uart_putfloat(parameter.angle_pid[1].p);
		uart_putfloat(parameter.angle_pid[1].i);
		uart_putfloat(parameter.angle_pid[1].d);
		uart_putfloat(parameter.angle_pid[1].imax);
		uart_putfloat(parameter.angle_pid[1].imin);
		uart_putshort(0);

		uart_putchar('$');
		uart_putchar('$');
		uart_putchar(28);
		uart_putchar(0xC8);
		uart_putchar(2);
		uart_putfloat(parameter.angle_pid[2].p);
		uart_putfloat(parameter.angle_pid[2].i);
		uart_putfloat(parameter.angle_pid[2].d);
		uart_putfloat(parameter.angle_pid[2].imax);
		uart_putfloat(parameter.angle_pid[2].imin);
		uart_putshort(0);

		uart_putchar('$');
		uart_putchar('$');
		uart_putchar(28);
		uart_putchar(0xC8);
		uart_putchar(3);
		uart_putfloat(parameter.speed_pid[0].p);
		uart_putfloat(parameter.speed_pid[0].i);
		uart_putfloat(parameter.speed_pid[0].d);
		uart_putfloat(parameter.speed_pid[0].imax);
		uart_putfloat(parameter.speed_pid[0].imin);
		uart_putshort(0);
		break;
	case 1:
		uart_putchar('$');
		uart_putchar('$');
		uart_putchar(28);
		uart_putchar(201);
		uart_putchar(0);
		uart_putfloat(parameter.rate_pid[0].p);
		uart_putfloat(parameter.rate_pid[0].i);
		uart_putfloat(parameter.rate_pid[0].d);
		uart_putfloat(parameter.rate_pid[0].imax);
		uart_putfloat(parameter.rate_pid[0].imin);
		uart_putshort(0);
		break;

	default:
		break;
	}
	
}
