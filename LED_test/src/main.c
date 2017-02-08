/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Delay.h"
#include "Display.h"
#include "define.h"
#include "KEY_DEAL.h"

#define BUFFEER IOPORT_CREATE_PIN(PORTB, 2)
#define uint unsigned int
#define uchar unsigned char
#define uint unsigned int
#define true 1
#define false 0

unsigned int fatigue_number=0;
unsigned char  workingstate=workingstate_stop;	//	默认暂停状态
unsigned char counter_buf=0;
unsigned char timeover_flag=true;
unsigned char timeover_buf=0;
unsigned char int1_flag=false;
unsigned char int1_deal=false;

uchar EEPROM_read(uint uiAddress);//读EEROM
void EEPROM_write(uint uiAddress, uchar ucData);//写EEROM
void timer1_init(void);
void timer1_close(void);
void state_init(void);
void port_init(void);

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	//unsigned int  fatigue_number =0;
	
	board_init();
	ioport_init();
	
	cli();
	Delayms(100);
	port_init();
	key_init();
	dis_f_test();//数码管全显示
	state_init();
	DIS_PARA(7,dis_delay_off);	//电磁阀1,2指示灯灭
	
	CLEARBIT(EIFR,INTF0);//清外部中断0标记
	sei();//开中断
	workingstate=workingstate_stop;
	dis_f_test();//数码管全显示	
	Delayms(500);
	
	timer1_init();
	//timer1_close();
	while (1)
	{
		//if (!Key3)
		//{
		//	display_some(0,4);
		//}
		
		
		Key_detect();
		if(null!=KeyValue)
		{
			Key_ISR();//按键处理
		}
		
	}
	/* Insert application code here, after the board has been initialized. */
}

void port_init(void)
{
	SETBIT(PINB_ddrd,E_delay2);	// 置1　设为输出
	SETBIT(PINB_ddrd,E_delay3);// 置1　设为输出
	SETBIT(PINB_ddrd,buzzer_pin);		// 置1　设为输出
	
	CLEARBIT(PINB_port,E_delay2);//置0  设为输出0
	CLEARBIT(PINB_port,E_delay3);//置0  设为输出0
	CLEARBIT(PINB_port,buzzer_pin);//置0  设为输出0
	
	SETBIT(PINB_port,buzzer_pin);//置0  设为输出0           //debug----------------------------------
	Delayms(100);//debug----------------------------------
	CLEARBIT(PINB_port,buzzer_pin);//置0  设为输出0   //debug----------------------------------
	
	
	ACSR  = 0x80;//关闭模拟比较器
	ADCSRA= 0x00;//ADC控制
	
	EICRA=0x02;//INT0下降沿中断
	EIMSK|=(1<<INT0);
}

ISR(INT0_vect)//外部中断0
{
	cli();//关中断，防止多次进入该中断
	CLEARBIT(EIFR,INTF0);//清外部中断0标记
	//if(int1_flag==false)
	//{	
	//	int1_flag=true;
		
		if(fatigue_number>=9999)
		{
			fatigue_number=0;
		}
		EEPROM_write(0x0050,fatigue_number&0xff);
		EEPROM_write(0x0051,(fatigue_number>>8)&0xff);
		
		
		//EEPROM_write(0x0050,5&0xff);
		//EEPROM_write(0x0051,(0>>8)&0xff);
		
	//}
	NOP();
}

void state_init(void)
{
	//EEPROM_write(0x0050,5&0xff);
	//EEPROM_write(0x0051,(0>>8)&0xff);
//
	//NOP();
	
	fatigue_number =EEPROM_read(0x51);					//当前疲劳次数
	fatigue_number =(fatigue_number<<8)+EEPROM_read(0x50);
	
	if(fatigue_number>=9999)fatigue_number=0;
	
	dis_buf[dis_bit00]=fatigue_number%10;
	dis_buf[dis_bit01]=(fatigue_number/10)%10;
	dis_buf[dis_bit02]=(fatigue_number/100)%10;
	dis_buf[dis_bit03]=(fatigue_number/1000)%10;
	
	display_some(0,4);

}




void EEPROM_write(uint uiAddress, uchar ucData)//写EEROM
{
	while(EECR & (1<<EEPE));/* 等待上一次写操作结束 */
	EEAR = uiAddress;/* 设置地址和数据寄存器*/
	EEDR = ucData;
	EECR |= (1<<EEMPE);/* 置位EEMWE */
	EECR |= (1<<EEPE);/* 置位EEWE 以启动写操作*/
}
uchar EEPROM_read(uint uiAddress)//读EEROM
{
	while(EECR & (1<<EEPE)); /* 等待上一次写操作结束 */
	EEAR = uiAddress;/* 设置地址寄存器*/
	EECR |= (1<<EERE);/* 设置EERE 以启动读操作*/
	return EEDR;/* 自数据寄存器返回数据 */
}

void timer1_init(void)
{
	TCNT1  = 0x3CAF; //setup
	TCCR1A = 0x00;
	TCCR1B = 0x01;//熔断丝8分频，计数器不分频，溢出时间为5.22mS           0x05; // 预分频 ck/1024 ,计数允许
	TCCR1C = 0x00;
	TIMSK1 = 0x01;
	//sei();
}

void timer1_close(void)
{
	TCNT1  = 0x3CAF; //setup
	TCCR1A = 0x00;
	TCCR1B = 0x00; // 预分频 ck/1024 ,计数禁止
	TCCR1C = 0x00;
	TIMSK1 = 0x00;
}

//ISR(TIMER1_OVF_vect)
ISR(TIMER1_OVF_vect)
{
	if (timeover_buf>=20)
	{
		
		timeover_buf=0;
		if (timeover_flag)
		{
			timeover_flag=false;
			DIS_PARA(0,dis_null);
			DIS_PARA(1,dis_null);
			DIS_PARA(2,dis_null);
			DIS_PARA(3,dis_null);
		} 
		else
		{
			timeover_flag=true;
			display_some(0,4);
		}
		
		
		
	} 
	else
	{
		timeover_buf++;
	}
	
	
	CLEARBIT(TIFR1,TOV1);
	TCNT1  = 0x3CAF; //setup
}