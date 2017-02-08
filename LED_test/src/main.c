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
unsigned char  workingstate=workingstate_stop;	//	Ĭ����ͣ״̬
unsigned char counter_buf=0;
unsigned char timeover_flag=true;
unsigned char timeover_buf=0;
unsigned char int1_flag=false;
unsigned char int1_deal=false;

uchar EEPROM_read(uint uiAddress);//��EEROM
void EEPROM_write(uint uiAddress, uchar ucData);//дEEROM
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
	dis_f_test();//�����ȫ��ʾ
	state_init();
	DIS_PARA(7,dis_delay_off);	//��ŷ�1,2ָʾ����
	
	CLEARBIT(EIFR,INTF0);//���ⲿ�ж�0���
	sei();//���ж�
	workingstate=workingstate_stop;
	dis_f_test();//�����ȫ��ʾ	
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
			Key_ISR();//��������
		}
		
	}
	/* Insert application code here, after the board has been initialized. */
}

void port_init(void)
{
	SETBIT(PINB_ddrd,E_delay2);	// ��1����Ϊ���
	SETBIT(PINB_ddrd,E_delay3);// ��1����Ϊ���
	SETBIT(PINB_ddrd,buzzer_pin);		// ��1����Ϊ���
	
	CLEARBIT(PINB_port,E_delay2);//��0  ��Ϊ���0
	CLEARBIT(PINB_port,E_delay3);//��0  ��Ϊ���0
	CLEARBIT(PINB_port,buzzer_pin);//��0  ��Ϊ���0
	
	SETBIT(PINB_port,buzzer_pin);//��0  ��Ϊ���0           //debug----------------------------------
	Delayms(100);//debug----------------------------------
	CLEARBIT(PINB_port,buzzer_pin);//��0  ��Ϊ���0   //debug----------------------------------
	
	
	ACSR  = 0x80;//�ر�ģ��Ƚ���
	ADCSRA= 0x00;//ADC����
	
	EICRA=0x02;//INT0�½����ж�
	EIMSK|=(1<<INT0);
}

ISR(INT0_vect)//�ⲿ�ж�0
{
	cli();//���жϣ���ֹ��ν�����ж�
	CLEARBIT(EIFR,INTF0);//���ⲿ�ж�0���
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
	
	fatigue_number =EEPROM_read(0x51);					//��ǰƣ�ʹ���
	fatigue_number =(fatigue_number<<8)+EEPROM_read(0x50);
	
	if(fatigue_number>=9999)fatigue_number=0;
	
	dis_buf[dis_bit00]=fatigue_number%10;
	dis_buf[dis_bit01]=(fatigue_number/10)%10;
	dis_buf[dis_bit02]=(fatigue_number/100)%10;
	dis_buf[dis_bit03]=(fatigue_number/1000)%10;
	
	display_some(0,4);

}




void EEPROM_write(uint uiAddress, uchar ucData)//дEEROM
{
	while(EECR & (1<<EEPE));/* �ȴ���һ��д�������� */
	EEAR = uiAddress;/* ���õ�ַ�����ݼĴ���*/
	EEDR = ucData;
	EECR |= (1<<EEMPE);/* ��λEEMWE */
	EECR |= (1<<EEPE);/* ��λEEWE ������д����*/
}
uchar EEPROM_read(uint uiAddress)//��EEROM
{
	while(EECR & (1<<EEPE)); /* �ȴ���һ��д�������� */
	EEAR = uiAddress;/* ���õ�ַ�Ĵ���*/
	EECR |= (1<<EERE);/* ����EERE ������������*/
	return EEDR;/* �����ݼĴ����������� */
}

void timer1_init(void)
{
	TCNT1  = 0x3CAF; //setup
	TCCR1A = 0x00;
	TCCR1B = 0x01;//�۶�˿8��Ƶ������������Ƶ�����ʱ��Ϊ5.22mS           0x05; // Ԥ��Ƶ ck/1024 ,��������
	TCCR1C = 0x00;
	TIMSK1 = 0x01;
	//sei();
}

void timer1_close(void)
{
	TCNT1  = 0x3CAF; //setup
	TCCR1A = 0x00;
	TCCR1B = 0x00; // Ԥ��Ƶ ck/1024 ,������ֹ
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