/************************************************************************************
							本例程提供自以下店铺：
								Ilovemcu.taobao.com
								epic-mcu.taobao.com
							实验相关外围扩展模块均来自以上店铺
							作者：神秘藏宝室							
*************************************************************************************/
#include "stm32f10x.h"
#include "delay.h"
#include "HX711.h"
#include "usart.h"
#include "adc.h"
#include "stm32f10x_adc.h"
#include "timer.h"
void delay(u32 nCount)//自己定义的延时函数
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
	
	delay_init();//延时模板函数初始化（延时时间比较准确）
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
	GPIO_SetBits(GPIOB,GPIO_Pin_13);//引脚设为13
	Adc_Init();//adc初始化
	
	//*******************8**********这一段代码为设备连接WiFi的代码******************************
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
	printf("AT+CIPMODE=1\r\n");//透传模式
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	//*******************8**********连接完成******************************
	//*******************8*****向服务器发送确认信息***********************
	printf("AT+CIPSEND\r\n");
	delay_ms(1000);
	//******************************发送完毕******************************
	//****************************发送盆栽状态****************************
	//******************************发送完毕******************************
	TIM3_Int_Init(4999,7199);//500ms
	while(1)
	{
		if(USART_RX_STA&0x8000)//若接受到服务器的消息
		{
//			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
//			printf("\r\n您发送的消息为:\r\n\r\n");
//			for(i=0;i<len;i++)
//			{
//				USART_SendData(USART1, USART_RX_BUF[i]);//向串口1发送数据
//				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
//			}
			
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			
			
			if(USART_RX_BUF[len-1]=='1')
			{
				GPIO_ResetBits(GPIOB,GPIO_Pin_13);
				for(i=0;i<4;i++)
					delay_ms(1000); ;//4s
				GPIO_SetBits(GPIOB,GPIO_Pin_13);
				printf("浇花完成\r\n");
			}
			adcx=Get_Adc_Average(ADC_Channel_1,10);		//ADC的值
			adcy=Get_Adc_Average(ADC_Channel_2,10);
			temp=(float)adcx*(3.3/4095);
			temperate=-0.0435*adcx+82.5;		//这是通过线性函数近似拟合，手动测试得出
			waterPercent = ((4095-adcy)*100)/4095.0;
			
//			if(temperate>35&&waterPercent<0.1)//判断温度和湿度是否合适
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







