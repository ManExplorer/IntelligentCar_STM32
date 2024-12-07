#include "led.h"
#include "lcd.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "hongwaiqudong.h"
#include "wheel.h"
#include "pwm.h"
#include "bluetooth.h"

 int main(void)
 {		
			delay_init();
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
			uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
			LED_Init();			     //LED�˿ڳ�ʼ��
			//LCD_Init();
			hongwai_Init();	//������ճ�ʼ��
	    BluetoothInit();
			TIM2_PWM_Init(9999,71);	 
			TIM3_PWM_Init(9999,71);	 //����ƵPWMƵ��=72000000/1000/72=1Khz      ����1ms
	                             //����ƵPWMƵ��=72000000/10000/72=10hz   ����0.01s

			while(1)
			{		
//				//�������
//					u8 key;
//					//u8 t=0;
//				key=hongwai_Scan();
//				if(key)
//				{
//					switch(key)
//					{
//						case 98:
//							      car_forward();  break;//С��ǰ�� 
//						case 194:
//										car_right();    break;//��ת
//						case 34:
//										car_left();     break;//��ת	
//						case 168:
//										car_backward(); break;//С������
//					}		
//				}
//				else delay_ms(10);
////				t++;
////				
////				if(t==20)
////				{
////					t=0;
////					LED0=!LED0;
////				}

			//��������
			extern int flag;
			if(flag)   //��������״̬
				{
					switch(flag)
           {
					case 1:  stop();           break  ;
					case 2:  car_forward(); 	 break  ;
					case 3:  car_right(); 	   break  ;
					case 4:  car_left();       break  ;
					case 5:  car_backward();   break  ;
          default: stop();           break  ;                                                                                                
	          }
				}
				
			}
 }

