#include "stm32f10x.h"  
#include "bluetooth.h"  
#include "sys.h"
#include "led.h"
#include "delay.h"
#include "wheel.h"

u8 i;
int flag=0;  //定义标志位

void USART1_IRQHandler(void)  //串口中断服务函数
{  
     if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)      //USART2 串口接受中断发生与否
      { 	
			USART_ClearITPendingBit(USART1, USART_IT_RXNE);   //清除中断待处理位
			i=  USART_ReceiveData(USART1);                    //数据处理
		  if(i==1)
				{ flag=1;}
			if(i==2)
				{ flag=2;}
			if(i==3)
			  { flag=3;}
			if(i==4)
			  { flag=4;}
			if(i==5)
			  { flag=5;}  			    //移动指令
				
		  }
}
	  
			
	void BluetoothInit(void)
	{
	//利用中断进行蓝牙的收发
	  GPIO_InitTypeDef GPIO_InitStructure;
	  USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
    RCC_APB1PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE );//使能USART1，GPIOA时钟
		
  //USART1_TX   PA9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
   
  //USART1_RX	  PA10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10    
	
  //GPIO_PinRemapConfig(GPIO_PartialRemap_USART3,ENABLE);

		USART_InitStructure.USART_BaudRate = 9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

		USART_Init(USART1, &USART_InitStructure);
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断

  //Usart1 NVIC 配置
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	  NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
     
		USART_Cmd(USART1, ENABLE);  //使能串口 
	
	}
	
