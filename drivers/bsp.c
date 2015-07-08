#include "stm32f10x.h"

void APP_RCC_ENABLE()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC
						  |RCC_APB2Periph_AFIO|RCC_APB2Periph_USART1|RCC_APB2Periph_TIM1,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3
												|RCC_APB1Periph_TIM4, ENABLE);
}

void APP_NVIC_Init(void)
{
	NVIC_InitTypeDef         NVIC_InitStructure;
	
	/*设置TIM1的输入捕获中断 ppm解码*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; 	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
	
	/*设置TIM2的全局中断 系统周期计时*/
	NVIC_InitStructure.NVIC_IRQChannel =TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; 	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void APP_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*ppm解码通道 PA8*/
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*超声波输入通道 TRIG->PA12,IN->PA11
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
    GPIO_Init(GPIOA, &GPIO_InitStructure);*/
	
	/*iic总线 SCL->PB10,SDA->PB11*/
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*pwm输出通道 最简四通道PB6~PB9 普通8通道*/
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*串口IO USART1 Tx->PA9 Rx->PA10 USART2 Tx->PA2 Rx->PA3*/	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*普通IO LED*/
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
    GPIO_Init(GPIOC, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
    GPIO_Init(GPIOC, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
    GPIO_Init(GPIOC, &GPIO_InitStructure);	
}

void APP_USART_Init(void)
{
	USART_InitTypeDef USART_InitStructure;
	//USART_ClockInitTypeDef USART_ClockInitStructure;
	
	/*串口1配置*/
	USART_InitStructure.USART_BaudRate = 57600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	//USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	//USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	//USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
	USART_Init(USART1, &USART_InitStructure);
	//USART_ClockInit(USART1, &USART_ClockInitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    USART_Cmd(USART1,ENABLE);
	
	/*串口2配置,DMA待配置*/
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	//USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	//USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	//USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
	USART_Init(USART2, &USART_InitStructure);
	//USART_ClockInit(USART2, &USART_ClockInitStructure);
}

void APP_TIM_Init(void)
{
	TIM_ICInitTypeDef        TIM_ICInitStruct;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef  TIM_OCInitStructure_pwm;
	
	/*systick*/
	SysTick->CTRL&=0xfffffffb;//选择内部时钟 HCLK/8
	
	
	/*tim1 ppm解码*/
	TIM_DeInit(TIM1);
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInitStruct.TIM_Prescaler = 72-1; //可以尝试设置0，不分频
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 0xFFFF;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct );

	TIM_ICStructInit(&TIM_ICInitStruct);
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStruct.TIM_ICFilter = 0x00;
	TIM_ICInit(TIM1, &TIM_ICInitStruct );

	/*tim2 定时器计时*/
	TIM_DeInit(TIM2);
	TIM_TimeBaseInitStruct.TIM_Period =0xffff;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 71;//1 = 1us	
	TIM_TimeBaseInitStruct.TIM_ClockDivision =  0x00;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

	/*ppm输出*/
	TIM_DeInit(TIM3);
	TIM_DeInit(TIM4);
	TIM_TimeBaseInitStruct.TIM_Period = 20* 1000 -1;		//4ms 
	TIM_TimeBaseInitStruct.TIM_Prescaler = 72-1;	
	TIM_TimeBaseInitStruct.TIM_ClockDivision =  0x00;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	//TIM_TimeBaseInitStruct.TIM_Period = 20 * 1000 -1;		//20ms 
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);

	TIM_OCInitStructure_pwm.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure_pwm.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure_pwm.TIM_Pulse = 0;
	TIM_OCInitStructure_pwm.TIM_OCPolarity = TIM_OCPolarity_High;

	//timer3 
	TIM_OC1Init(TIM3, &TIM_OCInitStructure_pwm);
	TIM_OC2Init(TIM3, &TIM_OCInitStructure_pwm);
	TIM_OC3Init(TIM3, &TIM_OCInitStructure_pwm);
	TIM_OC4Init(TIM3, &TIM_OCInitStructure_pwm);
	//timer4 
	TIM_OC1Init(TIM4, &TIM_OCInitStructure_pwm);
	TIM_OC2Init(TIM4, &TIM_OCInitStructure_pwm);
	TIM_OC3Init(TIM4, &TIM_OCInitStructure_pwm);
	TIM_OC4Init(TIM4, &TIM_OCInitStructure_pwm);

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

	/*启动定时器*/
	TIM_ClearFlag(TIM1,TIM_FLAG_CC1 );
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);	
	TIM_ITConfig(TIM1, TIM_IT_Update, DISABLE);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM1, ENABLE);
	//TIM_Cmd(TIM2, ENABLE);
	TIM2->CNT = 0;
	TIM_Cmd(TIM3, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
}
	
void bsp_init(void)
{
	//SystemInit(); //startup_stm32f10x_hd.s中已经定义
	APP_RCC_ENABLE();
	APP_NVIC_Init();
	APP_IO_Init();
	APP_USART_Init();
	APP_TIM_Init();
}
  
//使用SysTick的普通计数模式对延迟进行管理 
//包括delay_us,delay_ms   
//修正了中断中调用出现死循环的错误 
//防止延时不准确,采用do while结构!    
static unsigned char   fac_us=9;//us延时倍乘数 
static unsigned short fac_ms=9000;//ms延时倍乘数 
void delay(unsigned short nms) 
{          
    u32 temp;      
    SysTick->LOAD=(u32)nms*fac_ms;//时间加载 
    //SysTick_SetReload((u32)nms*fac_ms); 
    SysTick->VAL =0x00;            //清空计数器 
    //SysTick_CounterCmd(SysTick_Counter_Clear); 
    SysTick->CTRL=0x01 ;           //开始倒数 
    //SysTick_CounterCmd(SysTick_Counter_Enable); 
    do 
    { 
       temp=SysTick->CTRL; 
    } 
    while(temp&0x01&&!(temp&(1<<16)));//等待时间到达    
    SysTick->CTRL=0x00;        //关闭计数器 
    SysTick->VAL =0x00;        //清空计数器       
} 

void udelay(unsigned short nus) 
{          
    u32 temp;      
    SysTick->LOAD=(u32)nus*fac_us;//时间加载 
    //SysTick_SetReload((u32)nms*fac_ms); 
    SysTick->VAL =0x00;            //清空计数器 
    //SysTick_CounterCmd(SysTick_Counter_Clear); 
    SysTick->CTRL=0x01 ;           //开始倒数 
    //SysTick_CounterCmd(SysTick_Counter_Enable); 
    do 
    { 
       temp=SysTick->CTRL; 
    } 
    while(temp&0x01&&!(temp&(1<<16)));//等待时间到达    
    SysTick->CTRL=0x00;        //关闭计数器 
    SysTick->VAL =0x00;        //清空计数器       
} 
