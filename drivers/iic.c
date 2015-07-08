#include "stm32f10x.h"

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n) 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n) 

#define SDA_OUT		{GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;}
#define SDA_IN		{GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}

#define SDA_HIGH	{PBout(11) = 1;}
#define SDA_LOW		{PBout(11) = 0;}

#define SCL_HIGH	{PBout(10) = 1;}
#define SCL_LOW		{PBout(10) = 0;}

#define READ_SDA	PBin(11)

#define _ACK     0
#define _NACK    1

unsigned char ack;

void i2c_delay(void)
{
	__NOP();__NOP();
}

void i2c_start(void)
{
    SDA_HIGH ;
    SCL_HIGH ;  
    i2c_delay() ;  
    SDA_LOW ;   
    i2c_delay() ; 
    SCL_LOW ;  
}

void i2c_stop(void)
{
    SDA_LOW ;      
    SCL_HIGH ;    
    i2c_delay() ;  
    SDA_HIGH ;     
    i2c_delay() ;  
}

void i2c_send_ack(unsigned char ack)
{
    if(ack)
    {
        SDA_HIGH ;
    }
    else
    {
        SDA_LOW ;
    }          
    SCL_HIGH ;     
    i2c_delay() ;   
    SCL_LOW ;      
    i2c_delay() ;  
}

unsigned char i2c_recv_ack(void)
{
    SDA_IN ;          
    SCL_HIGH ;          
    i2c_delay() ;                
    if(READ_SDA)  
    {
       ack = 1;
    }
    else
    {
        ack = 0;
    }
    SCL_LOW ;   
    SDA_OUT ;  
    i2c_delay() ;                   
    return ack ;

}

void i2c_send_byte(unsigned char dat)
{
    unsigned char i;
    for(i = 0; i < 8; i++)
    {
      if((dat << i) & 0x80)
      {    
				SDA_HIGH ; 
      }
			else
			{
		    SDA_LOW ;
      }
			i2c_delay() ;
      SCL_HIGH ;  
      i2c_delay() ; 
      SCL_LOW ;   
    }
    i2c_recv_ack();
}

unsigned char i2c_recv_byte(void)
{
    unsigned char i,dat;
    SDA_HIGH;                 
    i2c_delay();             
    SDA_IN;                  
    i2c_delay();            
    for(i = 0; i < 8; i++)
    {
        dat <<= 1;           
        SCL_HIGH ;       
        i2c_delay();    
        if(READ_SDA)  
            dat ++;
        SCL_LOW;  
        i2c_delay();  
    }
    SDA_OUT ;   
    return dat ;
}

unsigned char i2c_read(unsigned char dev,
               unsigned char reg,
               unsigned char length,
               unsigned char * data)
{
    unsigned char i ;
    i2c_start() ;
    i2c_send_byte(dev) ;
    i2c_send_byte(reg) ;
    i2c_start() ;
    i2c_send_byte(dev + 1) ;
    for(i = 0; i < length; i ++)
    {
        data[i] = i2c_recv_byte() ;
        if(i == length-1)
            i2c_send_ack(_NACK) ;
        else
            i2c_send_ack(_ACK) ;
    }
    i2c_stop() ;
    return 0 ;
}

unsigned char i2c_write(unsigned char dev,
                        unsigned char reg,
                        unsigned char len,
                        unsigned char * dat)
{
    unsigned char i ;
    i2c_start() ;
    i2c_send_byte(dev) ;
    i2c_send_byte(reg) ;
    for(i = 0; i < len; i ++)
    {
        i2c_send_byte(dat[i]) ;
    }
    i2c_stop() ;
    return 0 ;
}

void ByteWrite24c512(unsigned char faddr,
										 unsigned char saddr,
										 unsigned char dat)
{
	i2c_start() ;
  i2c_send_byte(0xA0) ;
  i2c_send_byte(faddr) ;
	i2c_send_byte(saddr) ;
  i2c_send_byte(dat) ;
  i2c_stop() ;
}

void PageWrite24c512(unsigned char faddr,
										 unsigned char saddr, 
										 unsigned char* dat, 
										 unsigned char num)
{
	unsigned char i;
	
	i2c_start();
	i2c_send_byte(0xA0) ;
  i2c_send_byte(faddr) ;
	i2c_send_byte(saddr) ;
	for(i = 0; i < num; i ++)
  {
    i2c_send_byte(dat[i]) ;
  }
  i2c_stop() ;
}

unsigned char RandomRead24c512(unsigned char faddr,
											unsigned char saddr)
{
	unsigned char dat;
	
	i2c_start() ;
  i2c_send_byte(0xA0);
  i2c_send_byte(faddr);
	i2c_send_byte(saddr);
  
	i2c_start() ;
	i2c_send_byte(0xA1);
	dat = i2c_recv_byte();
  i2c_stop() ;
	
	return dat;
}

unsigned char CurrentRead24c512(void)
{
	unsigned char dat;
	
	i2c_start() ;
  i2c_send_byte(0xA1);
	dat = i2c_recv_byte();
	
	return dat;
}

void SequentialRead24c512(unsigned char faddr, 
													unsigned char saddr, 
													unsigned char *dat, 
													unsigned char num)
{
	unsigned char i;
	
	i2c_start();
	i2c_send_byte(0xA0);
  i2c_send_byte(faddr);
	i2c_send_byte(saddr);
  
	i2c_start() ;
	i2c_send_byte(0xA1);
	for(i=0;i<num;i++)
	{
		dat[i] = i2c_recv_byte();
    if(i == num-1)
      i2c_send_ack(_NACK) ;
    else
      i2c_send_ack(_ACK) ;
	}
	i2c_stop();
}
