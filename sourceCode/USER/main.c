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
			uart_init(115200);	 	//串口初始化为115200
			LED_Init();			     //LED端口初始化
			//LCD_Init();
			hongwai_Init();	//红外接收初始化
	    BluetoothInit();
			TIM2_PWM_Init(9999,71);	 
			TIM3_PWM_Init(9999,71);	 //不分频PWM频率=72000000/1000/72=1Khz      周期1ms
	                             //不分频PWM频率=72000000/10000/72=10hz   周期0.01s

			while(1)
			{		
//				//红外控制
//					u8 key;
//					//u8 t=0;
//				key=hongwai_Scan();
//				if(key)
//				{
//					switch(key)
//					{
//						case 98:
//							      car_forward();  break;//小车前进 
//						case 194:
//										car_right();    break;//右转
//						case 34:
//										car_left();     break;//左转	
//						case 168:
//										car_backward(); break;//小车后退
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

			//蓝牙控制
			extern int flag;
			if(flag)   //蓝牙控制状态
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

