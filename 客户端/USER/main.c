/************************************************************************************
							�������ṩ�����µ��̣�
								Ilovemcu.taobao.com
								epic-mcu.taobao.com
							ʵ�������Χ��չģ����������ϵ���
							���ߣ����زر���							
*************************************************************************************/
#include "stm32f10x.h"
#include "delay.h"
#include "HX711.h"
#include "usart.h"
#include "adc.h"
#include "stm32f10x_adc.h"
#include "timer.h"
void delay(u32 nCount)//�Լ��������ʱ����
{
	for(;nCount!=0;nCount--);
}
int main(void)
{	
	u8 i;
	u8 len;	
	u16 times=0;
	u16 adcx,adcy;
	float temp;
	float temperate;
	float waterPercent;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	delay_init();//��ʱģ�庯����ʼ������ʱʱ��Ƚ�׼ȷ��
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	GPIO_SetBits(GPIOB,GPIO_Pin_13);//������Ϊ13
	Adc_Init();//adc��ʼ��
	
	//*******************8**********��һ�δ���Ϊ�豸����WiFi�Ĵ���******************************
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
	delay_ms(1000);
	printf("AT+CIPSTART=\"TCP\",\"192.168.191.1\",8080\r\n");
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	printf("AT+CIPMODE=1\r\n");//͸��ģʽ
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	//*******************8**********�������******************************
	//*******************8*****�����������ȷ����Ϣ***********************
	printf("AT+CIPSEND\r\n");
	delay_ms(1000);
	//******************************�������******************************
	//****************************��������״̬****************************
	//******************************�������******************************
	TIM3_Int_Init(4999,7199);//500ms
	while(1)
	{
		if(USART_RX_STA&0x8000)//�����ܵ�����������Ϣ
		{
//			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
//			printf("\r\n�����͵���ϢΪ:\r\n\r\n");
//			for(i=0;i<len;i++)
//			{
//				USART_SendData(USART1, USART_RX_BUF[i]);//�򴮿�1��������
//				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
//			}
			
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			
			
			if(USART_RX_BUF[len-1]=='1')
			{
				GPIO_ResetBits(GPIOB,GPIO_Pin_13);
				for(i=0;i<4;i++)
					delay_ms(1000); ;//4s
				GPIO_SetBits(GPIOB,GPIO_Pin_13);
				printf("�������\r\n");
			}
			adcx=Get_Adc_Average(ADC_Channel_1,10);		//ADC��ֵ
			adcy=Get_Adc_Average(ADC_Channel_2,10);
			temp=(float)adcx*(3.3/4095);
			temperate=-0.0435*adcx+82.5;		//����ͨ�����Ժ���������ϣ��ֶ����Եó�
			waterPercent = ((4095-adcy)*100)/4095.0;
			
//			if(temperate>35&&waterPercent<0.1)//�ж��¶Ⱥ�ʪ���Ƿ����
//			{
//				GPIO_ResetBits(GPIOB,GPIO_Pin_13);
//				for(i=0;i<4;i++)
//					delay_ms(1000); ;//4s
//				GPIO_SetBits(GPIOB,GPIO_Pin_13);
//			}
			
			if(USART_RX_BUF[len-1]=='0')
			{
					printf("%2.2f %.2f\r\n",temperate,waterPercent);
					delay_ms(1000);
					delay_ms(1000);
			}
			
			USART_RX_STA=0; 
			
		}
	}
}







