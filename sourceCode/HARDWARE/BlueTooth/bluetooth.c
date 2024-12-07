#include "stm32f10x.h"  
#include "bluetooth.h"  
#include "sys.h"
#include "led.h"
#include "delay.h"
#include "wheel.h"

u8 i;
int flag=0;  //�����־λ

void USART1_IRQHandler(void)  //�����жϷ�����
{  
     if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)      //USART2 ���ڽ����жϷ������
      { 	
			USART_ClearITPendingBit(USART1, USART_IT_RXNE);   //����жϴ�����λ
			i=  USART_ReceiveData(USART1);                    //���ݴ���
		  if(i==1)
				{ flag=1;}
			if(i==2)
				{ flag=2;}
			if(i==3)
			  { flag=3;}
			if(i==4)
			  { flag=4;}
			if(i==5)
			  { flag=5;}  			    //�ƶ�ָ��
				
		  }
}
	  
			
	void BluetoothInit(void)
	{
	//�����жϽ����������շ�
	  GPIO_InitTypeDef GPIO_InitStructure;
	  USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
    RCC_APB1PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE );//ʹ��USART1��GPIOAʱ��
		
  //USART1_TX   PA9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
   
  //USART1_RX	  PA10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10    
	
  //GPIO_PinRemapConfig(GPIO_PartialRemap_USART3,ENABLE);

		USART_InitStructure.USART_BaudRate = 9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

		USART_Init(USART1, &USART_InitStructure);
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�

  //Usart1 NVIC ����
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�3
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	  NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
     
		USART_Cmd(USART1, ENABLE);  //ʹ�ܴ��� 
	
	}
	
