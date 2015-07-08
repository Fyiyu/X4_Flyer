#include "eeprom.h"

/*
The 512K is internally organized as 512 pages of 128-bytes each.
Random word addressing requires a 16-bit data word address.
The write cycle time tWR(10ms) is the time from a valid stop condition 
of a write sequence to the end of the internal clear/write cycle.
All inputs are disabled during this write cycle and the EEPROM will not
respond until the write is complete.
*/

//参数seed是检测eeprom时产生随机数使用的种子
uint8_t EE_test(uint16_t seed)
{	
	unsigned char buf[2] = {0};

    //检测eeprom是否可用 
	srand(seed);//随机数种子
	buf[0] = rand()%100;//产生随机数(0~99)
		
	ByteWrite24c512(0x00, 0x00, buf[0]);//写入随机数
	delay(50);
	
	buf[1] = RandomRead24c512(0x00,0x00);
	if(buf[0] != buf[1])
	{
		//eeprom不可用
		//printf("iic bus is wrong!buf[0]=%d,buf[1]=%d.\n",(int)buf[0],(int)buf[1]);
		return 0;
	}

	return 1;
}

void EE_putchar(uint16_t addr,uint8_t data)
{
	uint8_t faddr,saddr;
	faddr = (uint8_t)(addr>>8);
	saddr = (uint8_t)addr;
	ByteWrite24c512(faddr,saddr,data);
}

void EE_putshort(uint16_t addr,uint16_t data)
{
	uint8_t faddr,saddr;
	faddr = (uint8_t)(addr>>8);
	saddr = (uint8_t)addr;
	PageWrite24c512(faddr,saddr,(unsigned char*)&data,2);
}

void EE_putfloat(uint16_t addr,float data)
{
	uint8_t faddr,saddr;
	faddr = (uint8_t)(addr>>8);
	saddr = (uint8_t)addr;
	PageWrite24c512(faddr,saddr,(unsigned char*)&data,4);
}

void EE_putstr(uint16_t addr,unsigned char *Str)
{
	uint8_t * data;
	uint8_t faddr,saddr;
	uint8_t num = 0;
	
	faddr = (uint8_t)(addr>>8);
	saddr = (uint8_t)addr;
	data = Str;
	while(*Str)
	{
		num++;
		Str++;
	}
	PageWrite24c512(faddr,saddr,data,num);
}

void EE_put(uint16_t addr,unsigned char *data,uint8_t num)
{
	uint8_t faddr,saddr;
	faddr = (uint8_t)(addr>>8);
	saddr = (uint8_t)addr;
	PageWrite24c512(faddr,saddr,data,num);
}

uint8_t EE_readchar(uint16_t addr)
{
	uint8_t faddr,saddr,data;
	faddr = (uint8_t)(addr>>8);
	saddr = (uint8_t)addr;
	data = RandomRead24c512(faddr,saddr);
	return data;
}

uint16_t EE_readshort(uint16_t addr)
{
	uint8_t faddr,saddr,buf[2];
	uint16_t data;
	faddr = (uint8_t)(addr>>8);
	saddr = (uint8_t)addr;
	SequentialRead24c512(faddr,saddr,buf,2);
	data = (buf[0]<<8)+buf[1];
	return data;
}

float EE_readfloat(uint16_t addr)
{
	uint8_t faddr,saddr,buf[4];
	float data;
	faddr = (uint8_t)(addr>>8);
	saddr = (uint8_t)addr;
	SequentialRead24c512(faddr,saddr,buf,4);
	data = (buf[0]<<24)+(buf[1]<<16)+(buf[2]<<8)+buf[3];
	return data;
}

void EE_readstr(uint16_t addr, uint8_t* data, uint8_t num)
{
	uint8_t faddr,saddr;
	faddr = (uint8_t)(addr>>8);
	saddr = (uint8_t)addr;
	SequentialRead24c512(faddr,saddr,data,num);
}
