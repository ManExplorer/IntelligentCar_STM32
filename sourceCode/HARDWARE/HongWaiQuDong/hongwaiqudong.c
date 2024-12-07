#include "hongwaiqudong.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

//��ʼ������������ʱ�ӣ��������ģʽ����ʱ���������ж�
void hongwai_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //GPIO��ʼ���ṹ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; //��ʱ��4��ʼ���ṹ��
	TIM_ICInitTypeDef TIM_ICInitStructure; //��ʼ�����벶��ṹ��
	NVIC_InitTypeDef NVIC_InitStructure; //�жϳ�ʼ���ṹ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��GPIOB��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);	//ʹ�ܶ�ʱ��4��ʱ��
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9; //��PB9��������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD; //����PB9Ϊ��������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	GPIO_SetBits(GPIOB,GPIO_Pin_9); //�������ŵ������ƽ
	
	//��ʼ����ʱ��4
	TIM_TimeBaseStructure.TIM_Period=10000; //ʱ������10ms���������
	TIM_TimeBaseStructure.TIM_Prescaler=(72-1); //Ԥ��Ƶ����1M�ļ���Ƶ�ʣ�1us��1
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //����ʱ�ӷָTDTS=Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure); //����ָ��������ʼ����ʱ��4
	
	//���벶���ʼ��
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_4; //ѡ�������IC4ӳ�䵽TI4��
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising; //�����ز���
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1; //���������Ƶ������Ƶ
	TIM_ICInitStructure.TIM_ICFilter=0x03; //���������˲�����8����ʱ��ʱ�������˲�
	TIM_ICInit(TIM4,&TIM_ICInitStructure); //��ʼ����ʱ�����벶��ͨ��
	
	TIM_Cmd(TIM4,ENABLE); //ʹ�ܶ�ʱ��4
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3; //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE; //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure); //��ʼ������NVIC�Ĵ���
	
	TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_CC4,ENABLE); //��������жϣ�����CC4IE�����ж�
}


//ң��������״̬
//[7]:�յ����������־
//[6]:�õ���һ��������������Ϣ
//[5]:����	
//[4]:����������Ƿ��Ѿ�������								   
//[3:0]:�����ʱ��
u8 RmtSta=0;
u16 Dval; //�½���ʱ��������ֵ
u32 RmtRec=0; //������ܵ�������
u8 RmtCnt=0; //�������µĴ���

//��ʱ��4�жϷ������
void TIM4_IRQHandler(void)
{
	//�����˲����ж�
	if(TIM_GetITStatus(TIM4,TIM_IT_CC4)!=RESET)
	{
		if(RDATA)//�����������ز���
		{
			TIM_OC4PolarityConfig(TIM4,TIM_ICPolarity_Falling);
			TIM_SetCounter(TIM4,0); //��ն�ʱ����ֵ������Ժ��ٿ�ʼ����
			RmtSta|=0X10; //����������Ѿ�������
		}
		else //�½��ز���
		{
			Dval=TIM_GetCapture4(TIM4); //��ŵ�������
			TIM_OC4PolarityConfig(TIM4,TIM_ICPolarity_Rising); //����Ϊ�����ز���
			if(RmtSta&0X10) //�Ѿ�������������
			{
				if(RmtSta&0X80) //�Ѿ����ܵ���������
				{
					//��ʼ�жϽ��ܵ�������
					if(Dval>300&&Dval<800) //����560us�ĸߵ�ƽ��Ϊ�߼�0
					{
						RmtRec<<=1; //�ڳ�1λ������»�õ�����
						RmtRec|=0; //��������0
					}
					else if(Dval>1400&&Dval<1800) //����1680us�ĸߵ�ƽ��Ϊ�߼�0
					{
						RmtRec<<=1; //�ڳ�1λ������»�õ�����
						RmtRec|=1; //��������0
					}
					else if(Dval>2200&&Dval<2600) //����2.5ms�ĸߵ�ƽ��Ϊ�߼�0
					{
						RmtCnt++; //������������1
						RmtSta&=0XF0; //��������ʱ��
					}
				}
				else if(Dval>4200&&Dval<4700) //���ܵ���4.5ms�ĸߵ�ƽ
				{
					RmtSta|=1<<7; //������ڿ�ʼ���ܵ���������
					RmtCnt=0; //�����������������
				}
			}
			RmtSta&=~(1<<4); //����������Ѿ���������
		}
	}

	
	//�����˸����ж�
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET)
	{
		if(RmtSta&0x80) //����������
		{
			RmtSta&=~0x10; //ȡ���������Ѿ���������
			if((RmtSta&0X0F)==0X00) RmtSta|=1<<6; //��ǵõ���һ��������������Ϣ
			if((RmtSta&0X0F)<14) 
			{
				RmtSta++;
			}
			else
			{
				RmtSta&=~(1<<7); //����������ʶ
				RmtSta&=0XF0; //��ռ�����
			}
		}
	}	
	
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update|TIM_IT_CC4);
	
}

u8 hongwai_Scan(void)
	{
		u8 sta;
		u8 t1,t2;
		if(RmtSta&(1<<6)) //�õ�һ��������������Ϣ
		{
			t1=RmtRec>>24; //�õ���ַ��
			t2=(RmtRec>>16)&0xff; //�õ���ַ����
			if((t1==(u8)~t2)&&t1==REMOTE_ID)//����ң��ʶ����(ID)����ַ 
			{
				t1=RmtRec>>8;
				t2=RmtRec;
				if(t1==(u8)~t2) sta=t1; //��ֵ��ȷ
			}
			if((sta==0)||((RmtSta&0x80)==0)) //�������ݴ���ң���Ѿ�û�а���
			{
				RmtSta&=~(1<<6); //������յ���Ч������ʶ
				RmtCnt=0;
			}
		}
		return sta;
	}
