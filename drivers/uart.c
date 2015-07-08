#include "uart.h"
#include "Mymath.h"
#include "parameter.h"

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

typedef union 
{
	unsigned char byte[4];
	float num;
}t_floattobyte;
t_floattobyte floattobyte;

u8 Tx_Buf[4];
u8 Rx_Buf[2][256];	//����32�ֽڵĴ��ڽ��ջ���
u8 Rx_Act=0;		//����ʹ�õ�buf��
u8 Rx_Adr=0;		//���ڽ��յڼ��ֽ�
u8 Rx_Ok0 = 0;
u8 Rx_Ok1 = 0;

void uart_putchar(uint8_t DataToSend)
{
	USART_SendData(USART1,DataToSend);
    while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);
}

void uart_putshort(uint16_t DataToSend)
{
	Tx_Buf[0] = BYTE0(DataToSend);
	uart_putchar(Tx_Buf[0]);
	Tx_Buf[1] = BYTE1(DataToSend);
	uart_putchar(Tx_Buf[1]);
}
void uart_putfloat(float DataToSend)
{
	floattobyte.num=DataToSend;
	Tx_Buf[0] = floattobyte.byte[0];  
	Tx_Buf[1] = floattobyte.byte[1];  
	Tx_Buf[2] = floattobyte.byte[2];  
	Tx_Buf[3] = floattobyte.byte[3];  
	uart_putchar(Tx_Buf[0]);
	uart_putchar(Tx_Buf[1]);
	uart_putchar(Tx_Buf[2]);
	uart_putchar(Tx_Buf[3]);
}
void uart_putstr(unsigned char *Str)
{
	//�ж�Strָ��������Ƿ���Ч.
	while(*Str)
	{
	//�Ƿ��ǻس��ַ� �����,������Ӧ�Ļس� 0x0d 0x0a
	if(*Str=='\r')
		uart_putchar(0x0d);
	else if(*Str=='\n')
		uart_putchar(0x0a);
	else 
		uart_putchar(*Str);
	//ָ��++ ָ����һ���ֽ�.
	Str++;
	}
}

float uart_buffer[24] = {0.0};
void uart_send(unsigned char num)
{
	unsigned short temp;
	short tempp;
	unsigned char numm = num;
	
	uart_putchar(0xA5);	
	while(num--)
	{
		tempp = (short)uart_buffer[numm - num -1];
		temp = tempp;
		uart_putchar((unsigned char)(temp>>8));
		uart_putchar((unsigned char)(temp));
	}
	uart_putchar(0xAA);
}

int8_t Rx_num = -1;
void Uart1_IRQ(void)
{
	//�����ж� (���ռĴ����ǿ�) 
	if(USART1->SR & (1<<5))//if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)    
	{
		u8 com_data = USART1->DR;
		if(Rx_Adr==0)		//Ѱ��֡ͷ"$"
		{
			if(com_data=='$')	//�������������"$",��д�뻺��
			{
				Rx_Buf[Rx_Act][0] = com_data;
				Rx_Adr = 1;
			}
		}
		else if(Rx_Adr == 1)
		{
			Rx_num = com_data;
			Rx_Buf[Rx_Act][1] = Rx_num;
			Rx_Adr = 2;
		}
		else		//���ڽ�������
		{
			Rx_Buf[Rx_Act][Rx_Adr] = com_data;
			Rx_Adr ++;
		}
		if(Rx_Adr==Rx_num)		//���ݽ������
		{
			Rx_Adr = 0;
			Rx_num = -1;
			if(Rx_Act)	
			{ 
				if(Rx_Ok1 == 0)
				{
				  Rx_Act = 0; 			
				  Rx_Ok1 = 1;
				  Rx_Ok0 = 0;
				}
				else
				{
					Rx_Act = 1;
				  Rx_Ok0 = 1;
				  Rx_Ok1 = 0;
				}
			}
			else 				
			{
				if(Rx_Ok0 == 0)
				{
				  Rx_Act = 1;
				  Rx_Ok0 = 1;
				  Rx_Ok1 = 0;
				}
				else
				{
					Rx_Act = 0; 			
				  Rx_Ok1 = 1;
				  Rx_Ok0 = 0;
				}
			}
		}
	}
}

void doRxData(void)
{
	u8 BufferEnable;
	int i;
	u8 message[256]={0};

	u8 head,length,key;
	u16 crc_recived,crc_calc;
	u8 re_h,re_l,ca_h,ca_l;

	param_t *p = getParam();

	GPIO_ResetBits(GPIOC, GPIO_Pin_0);

	if(Rx_Ok0 == 1){BufferEnable = 0;Rx_Ok0 = 0;}
	else if(Rx_Ok1 == 1){BufferEnable = 1;Rx_Ok1 = 0;}
	else return;
	
	for(i=0;i<256;i++)
		message[i] = Rx_Buf[BufferEnable][i];

	//
	head = message[0];//֡ͷ
	if(head != '$')return;

	length = message[1];//����֡ͷ֡β���ܳ���
    if(length < 4)return;

    key = message[2];//�ؼ���
    //crc_recived = (message[length-2]<<8)+message[length-1];//У����
	//crc_calc = math_crc16(message,length-2);

	switch(key)
	{
		case 0:
			parameter_save();
		case 1:
			//uart_putstr(message);
			if(message[3] == 0)
			{
				parameter_send(0);
				parameter_send(1);
				parameter_send(2);
				parameter_send(3);
			}
			else
			{
				parameter_send(message[3]);
			}
			break;
		case 2:
			//uart_putstr(message);
			p->angle_pid[0].p = (*(float *)(&message[3]));
			p->angle_pid[0].i = (*(float *)(&message[7]));
			p->angle_pid[0].d = (*(float *)(&message[11]));
			p->angle_pid[0].imax = (*(float *)(&message[15]));
			p->angle_pid[0].imin = (*(float *)(&message[19]));
			p->flag = 1;
			break;
		case 3:
			//uart_putstr(message);
			p->angle_pid[1].p = (*(float *)(&message[3]));
			p->angle_pid[1].i = (*(float *)(&message[7]));
			p->angle_pid[1].d = (*(float *)(&message[11]));
			p->angle_pid[1].imax = (*(float *)(&message[15]));
			p->angle_pid[1].imin = (*(float *)(&message[19]));
			p->flag = 1;
			break;
		case 4:
			//uart_putstr(message);
			p->angle_pid[2].p = (*(float *)(&message[3]));
			p->angle_pid[2].i = (*(float *)(&message[7]));
			p->angle_pid[2].d = (*(float *)(&message[11]));
			p->angle_pid[2].imax = (*(float *)(&message[15]));
			p->angle_pid[2].imin = (*(float *)(&message[19]));
			p->flag = 1;
			break;
		case 5:
			//uart_putstr(message);
			p->speed_pid[0].p = (*(float *)(&message[3]));
			p->speed_pid[0].i = (*(float *)(&message[7]));
			p->speed_pid[0].d = (*(float *)(&message[11]));
			p->speed_pid[0].imax = (*(float *)(&message[15]));
			p->speed_pid[0].imin = (*(float *)(&message[19]));
			p->flag = 1;
			break;
		case 6:
			
			break;
		default:
			parameter_save();
			break;
	}

	for(i=0;i<256;i++)
	{
		Rx_Buf[BufferEnable][i] = 0;
	}
	
	GPIO_SetBits(GPIOC, GPIO_Pin_0);
	return;
}

int fputc(int ch, FILE *f)
{ //}
USART_SendData(USART1, (unsigned char) ch);// USART1 ???? USART2 ?
while (!(USART1->SR & USART_FLAG_TXE));
return (ch);
}
