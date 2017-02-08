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
//us级别的延时函数
/*************************************/
void Delayus(uint lus)
{
	while(lus--)
	{
		_delay_us(1);
	}
}
/******************************************/
//ms级别的延时函数
/******************************************/
void Delayms(uint lms)
{
	while(lms--)
	{
		_delay_ms(1);        //延时1ms
	}
}