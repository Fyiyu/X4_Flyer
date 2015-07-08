#ifndef __EEPROM_H__
#define __EEPROM_H__

#include "iic.h"
#include "bsp.h"
#include "stm32f10x.h"
#include <stdlib.h>
#include <stdio.h>

uint8_t EE_test(uint16_t seed);
void EE_putchar(uint16_t addr,uint8_t data);
void EE_putshort(uint16_t addr,uint16_t data);
void EE_putfloat(uint16_t addr,float data);
void EE_putstr(uint16_t addr,unsigned char *Str);
void EE_put(uint16_t addr,unsigned char *data,uint8_t num);
uint8_t EE_readchar(uint16_t addr);
uint16_t EE_readshort(uint16_t addr);
float EE_readfloat(uint16_t addr);
void EE_readstr(uint16_t addr, uint8_t* data, uint8_t num);

#endif
