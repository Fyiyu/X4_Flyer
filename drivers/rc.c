#include "stm32f10x.h"
#include "uart.h"
#include "bsp.h"

static unsigned char ppm_step = 0; 
static unsigned char ppm_c = 0;	
static unsigned short buffer = 0;
static unsigned short channel[6] = {1000};
static unsigned short mid[3] = {1500};


void calc_rc_mid(void)
{
	int i;
	int temp_mid[3] = {0};
	
	GPIO_SetBits(GPIOC, GPIO_Pin_0);
	GPIO_SetBits(GPIOC, GPIO_Pin_1);
	GPIO_SetBits(GPIOC, GPIO_Pin_2);
	delay(2000);
	GPIO_ResetBits(GPIOC, GPIO_Pin_0);
	GPIO_ResetBits(GPIOC, GPIO_Pin_1);
	GPIO_ResetBits(GPIOC, GPIO_Pin_2);
	delay(500);
	GPIO_SetBits(GPIOC, GPIO_Pin_0);
	GPIO_SetBits(GPIOC, GPIO_Pin_1);
	GPIO_SetBits(GPIOC, GPIO_Pin_2);
	delay(1000);
	GPIO_ResetBits(GPIOC, GPIO_Pin_0);
	GPIO_ResetBits(GPIOC, GPIO_Pin_1);
	GPIO_ResetBits(GPIOC, GPIO_Pin_2);
	delay(500);
	GPIO_SetBits(GPIOC, GPIO_Pin_0);
	GPIO_SetBits(GPIOC, GPIO_Pin_1);
	GPIO_SetBits(GPIOC, GPIO_Pin_2);
	delay(1000);
	GPIO_ResetBits(GPIOC, GPIO_Pin_0);
	GPIO_ResetBits(GPIOC, GPIO_Pin_1);
	GPIO_ResetBits(GPIOC, GPIO_Pin_2);
	
	for(i=0;i<100;i++)
	{
		temp_mid[0] += channel[0];
		temp_mid[1] += channel[1];
		temp_mid[2] += channel[3];
		delay(10);
	}
	
	mid[0] = temp_mid[0] / 100;
	mid[1] = temp_mid[1] / 100;
	mid[2] = temp_mid[2] / 100;
	
	GPIO_SetBits(GPIOC, GPIO_Pin_0);
	GPIO_SetBits(GPIOC, GPIO_Pin_1);
	GPIO_SetBits(GPIOC, GPIO_Pin_2);
	delay(500);
	GPIO_ResetBits(GPIOC, GPIO_Pin_0);
	GPIO_ResetBits(GPIOC, GPIO_Pin_1);
	GPIO_ResetBits(GPIOC, GPIO_Pin_2);
}

void get_rc(unsigned short rc[6])
{
	unsigned char i;
	//for(i=0;i<6;i++)
	//{
	//	rc[i] = channel[i];
	//}
	rc[0] = channel[0] - mid[0] + 1500;
	rc[1] = channel[1] - mid[1] + 1500;
	rc[2] = channel[2];
	rc[3] = channel[3] - mid[2] + 1500;
	rc[4] = channel[4];
	rc[5] = channel[5];
}

int isReady(void)
{
	if((channel[2]<1200)&&(channel[3]<1200))
		return 1;
	else
		return 0;
}

void ppm_isr(void)
{	
	unsigned char temp;
	switch (ppm_step)
	{
	case 0:
		TIM1->SR &= 0XFFFC; //clear flag
	    TIM1->CNT = 0;
	    TIM1->CCER = 0X0003; //change to falling
		ppm_step = 1;
		break;
	case 1:
		TIM1->SR &= 0XFFFC; //clear flag
	    buffer = TIM1->CNT; 
		TIM1->CNT = 0;	
	    TIM1->CCER = 0X0001; //change to rising
		(buffer<2500)?(ppm_step=0):(ppm_step=2);
		break;
	case 2:
	    TIM1->SR &= 0XFFFC; //clear flag
	    channel[ppm_c] = TIM1->CNT; 
        TIM1->CCER = 0X0003; //change to falling	
		ppm_step = 3;
		ppm_c++;
		if(ppm_c == 6)
		{
			ppm_c = 0;
			ppm_step = 0;
			goto end;
		}
		break;
	case 3:
	    TIM1->SR &= 0XFFFC; //clear flag
	    TIM1->CNT = 0;
	    TIM1->CCER = 0X0001; //change to rising
		ppm_step = 2;
		break;
	default:
		ppm_c = 0;
		ppm_step = 0;
	}
    end:temp=0;	
}
//channel[0]~channel[5]  yaw,pitch,altitude,roll,gate,rate


void getTarAngle(float tarAngle[3])//roll pitch yaw
{
	tarAngle[0] = (channel[3]-1500)/500*20.0;//���20�ȣ����Ը���
	tarAngle[1] = (channel[1]-1500)/500*20.0;
	tarAngle[2] = (channel[0]-1500)/500*20.0;
}

void getTarRate(float tarRate[3])
{
	tarRate[0] = (channel[3]-1500)/500*5.0;//���5��ÿ�룬���Ը���
	tarRate[1] = (channel[1]-1500)/500*5.0;
	tarRate[2] = (channel[0]-1500)/500*5.0;
}

float getTarAltitude(void)
{
	return (channel[2]-1000)/10.0;
}

uint8_t hand_val = 0;
uint8_t hand_bef = 0;
uint16_t lock = 0;
uint16_t free = 0;
uint8_t val(uint16_t buf)
{
	if(buf<1150)return 1;
	else if(buf>1350 && buf<1650)return 2;
	else if(buf>1850)return 3;
	else return 0;
}

int isLock = 1;
void doRcData(void)
{
	//ң�����ƻ�ȡ��ֻ��ǰ�ĸ�ͨ�����ź�
		hand_val = (val(channel[0])<<6) | (val(channel[1])<<4) | (val(channel[2])<<2) | (val(channel[3]));
		if(hand_val != hand_bef) //�ж�������û�б䣬����б䣬hand_val = 0 ��
		{
			hand_bef = hand_val;
			hand_val =0;
		}

		switch (hand_val) //�յ���Ӧ�����ƴ��룬����ʱ���1���յ����Ƹı��ź�0����������ʱ�����㡣
		{
			case 165:lock++; break;              //  �� ��
			case 167:free++; break;              //  �� ��
			case 150:break;//  �� ��
			case 182:break;//  �� ��
			case 181:break; //���� ��
			case 183:break; //���� ��
			case 149:break;//���� ��
			case 151:break;//���� ��
			default  :lock=0;free=0;break; 		
		}

		if(lock >= 500)//�������Ƴ�����3s
		{
			lock = 0;
			isLock = 1;
		}
		else if(free >= 500)//�������Ƴ�����3s
		{
			free = 0;
			isLock = 0;
		}
		
// 		if(channel[4]>1500)
// 		{
// 			isLock = 0;
// 		}
// 		else
// 		{
// 			isLock = 1;
// 		}
}
