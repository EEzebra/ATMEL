/*
 * Delay.c
 *
 * Created: 2017/1/22 14:26:42
 *  Author: jin
 */ 
#include<avr/io.h>
#include<util/delay.h>
#include"Delay.h"

/*************************************/
//us�������ʱ����
/*************************************/
void Delayus(uint lus)
{
	while(lus--)
	{
		_delay_us(1);
	}
}
/******************************************/
//ms�������ʱ����
/******************************************/
void Delayms(uint lms)
{
	while(lms--)
	{
		_delay_ms(1);        //��ʱ1ms
	}
}