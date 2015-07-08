#ifndef __MAIN_H__
#define __MAIN_H__
 
#include "rc.h"
#include "bsp.h"
#include "enviroment.h"
#include "ahrs.h"
#include "ekf.h"
#include "uart.h"
#include "controller.h"
#include "stm32f10x.h"
#include "sensors.h"
#include "pwm.h"
#include "eeprom.h"
#include "Mymath.h"
#include "parameter.h"

typedef enum
{
	Hand_Rate = 0x00,
	Hand_Angle = 0x01,
	Hand_Speed = 0x02,
	Auto_Hover = 0x03,
	Auto_Speed = 0x04,
	Auto_Path = 0x05
}FLY_MODE;

#endif
