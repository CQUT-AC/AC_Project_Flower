#include "timer.h"
#include "usart.h"
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "delay.h"
u32 count=0;
void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrue;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_TimeBaseInitStrue.TIM_Period=arr;
	TIM_TimeBaseInitStrue.TIM_Prescaler=psc;
	TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStrue);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;//TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM3,ENABLE);
	
	delay_init();//��ʱģ�庯����ʼ������ʱʱ��Ƚ�׼ȷ��
}


void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)
	{
	
		count++;
		if(count==86400)
		{
			printf("+++");
			delay_ms(1000);
			printf("AT+RST\r\n");
	delay_ms(1000);
	delay_ms(1000);
	printf("AT+CWMODE=3\r\n");
	delay_ms(1000);
	delay_ms(1000);
	printf("AT+CWJAP=\"AC\",\"66666666\"\r\n");
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	printf("AT+CIPSTART=\"TCP\",\"192.168.191.1\",8080\r\n");
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	printf("AT+CIPMODE=1\r\n");//͸��ģʽ
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	printf("AT+CIPSEND\r\n");
	delay_ms(1000);
	//*******************8**********�������******************************
	//*******************8*****�����������ȷ����Ϣ***********************
//			printf("AT+RST\r\n");
//			delay_ms(1000);
//			printf("AT+CIPMODE=1\r\n");//͸��ģʽ
//			delay_ms(1000);
//			printf("AT+CIPSEND\r\n");
//			delay_ms(1000);
//					delay_ms(1000);
//					printf("test\r\n");
//			printf("AT+CIPSTART=1,\"TCP\",\"192.168.191.1\",8080\r\n\r\n");
//			printf("AT+CIPSEND=18\r\n");
//			delay_ms(1000);
//			printf("���ڿ��Է���ָ��\r\n");

//			if(USART_RX_STA&0x8000)//���յ�����
//				{
					//GPIO_ResetBits(GPIOB,GPIO_Pin_13);
//					u8 i;
//					u8	len=USART_RX_STA&0x3fff;
//					char * c;
//					for(i=0;i<len;i++){
//						c[i]=USART_RX_BUF[i];
//					}
//					printf("AT+CIPSEND=%d\r\n",len+2);
//					delay_ms(1000);
//					printf("%s\r\n",c);
//					printf("AT+CIPSEND=7\r\n");
//					delay_ms(1000);
//					printf("test1\r\n");
//					
//			}
			count=0;
		}
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}
}