#ifndef __BSP_H__
#define __BSP_H__

void bsp_init(void);
void updata_pwm(unsigned int channel, unsigned int pulse);
void delay(unsigned short nms);
void udelay(unsigned short nus);

#endif
