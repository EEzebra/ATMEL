/*
 * Display.c
 *
 * Created: 2017/1/19 16:24:16
 *  Author: jin
 */ 

#include <avr/io.h>
#include "define.h"
#include "Display.h"
#include <util/delay.h>

const uint dis_fbit_timebrake=300;
const 	uchar  table[]={
	0x3F,/*0*/
	0x06,/*1*/
	0x5B,/*2*/
	0x4F,/*3*/
	0x66,/*4*/
	0x6D,/*5*/
	0x7D,/*6*/
	0x07,/*7*/
	0x7F,/*8*/
	0x6F,/*9*/
	0x77,/*A*/
	0x7C,/*b*/
	0x39,/*C*/
	0x5E,/*d*/
	0x79,/*E*/
	0x71,/*F*/
	0x00,/*ȫ��*/
	0xFF,/*ȫ��*/
	0x05,/*��ŷ�1��*/
	0x03,/*��ŷ�2��*/
	0x01,/*��ŷ�1��2ȫ��*/

};
const uchar  disnum[]={
	
	0xC3,/*��04λ*/
	0xC2,/*��03λ*/
	0xC1,/*��02λ*/
	0xC0,/*��01λ*/
	
	0xC7,/*��08λ*/
	0xC6,/*��07λ*/
	0xC5,/*��06λ*/
	0xC4,/*��05λ*/
	
	0xC8,/*��09λ*/
	0xC9,/*��10λ*/
	0xCA,/*��11λ*/
	0xCB,/*��12λ*/
	0xCC,/*��13λ*/
	0xCD,/*��14λ*/
	0xCE,/*��15λ*/
	0xCF /*��16λ*/
};
uchar dis_buf[]={null,null,null,null,null,null,null,null,null,null,null,
null,null,null,null,null,null,null,null,null,null,null};//�������ʾ�Ĵ���

uchar dis_bit_buf[]={null,null,null,null,null,null,null,null,null,null,null,
null,null,null,null,null,null,null,null,null,null,null};//�������ʾ�Ĵ���
uint dis_fbit_buf=0x00;// λ��˸״̬λ��00Ϊû����˸
uint dis_fbit_timewait=0x00;// λ��˸״̬λ��00Ϊû����˸

/*************************************************/
//6932���Ŷ˿ڳ�ʼ��
/*************************************************/
void TM1629_init(void)
{
	ACSR=0x80;//�ر�ģ��Ƚ���
	CLEARBIT(dis_port,dis_6932cs);//���0
	SETBIT(dis_ddr,dis_6932cs);	//dis_stb ��1����Ϊ���
	CLEARBIT(dis_port,dis_clk);//���0
	SETBIT(dis_ddr,dis_clk);	//dis_clk ��1����Ϊ���
	CLEARBIT(dis_port,dis_din);//���0
	SETBIT(dis_ddr,dis_din);	//dis_din ��1����Ϊ���
}
/****************��ʾģ����*****************
*********************************************
;�ļ�����_D_TEST
;��  �ܣ�LEDȫ����ʾ
;��  �룺DIS_BYT
;��  ������
;��  �ã�_DIS_PARA
;*********************************************/
void DIS_TEST(uchar num)
{
	uchar n=0;
	for(n=0;n<4;n++)//4λ���������
	{
		DIS_PARA(n,num);
	}
}
/*********************************************
;�ļ�����_DIS_DISCON
;��  �ܣ���ʾ��������ȷ����ʾ����
;��  �룺
;��  ������
;��  �ã�_DELAY2��_DIS_SEND
;********************************************/
void dis_con(uchar dis_con)
{
	SETBIT(dis_port,dis_6932cs);
	SETBIT(dis_port,dis_din);
	SETBIT(dis_port,dis_clk);
	_delay_us(100);
	CLEARBIT(dis_port,dis_6932cs);
	NOP();
	NOP();
	NOP();
	dis_send(dis_con);//������ʾ����
	NOP();
	NOP();
	SETBIT(dis_port,dis_clk);
	NOP();
	NOP();
	SETBIT(dis_port,dis_6932cs);
	NOP();
	NOP();
}
/*********************************************
;�ļ�����_DIS_PARA
;��  �ܣ���ʾ�������޸ģ��޸���ʾ����
;��  �룺DIS_ADD��DIS_BYT
;��  ������
;��  �ã�_DIS_SEND
;******************************************** */
void DIS_PARA(uchar DISADD , uchar DISBYT)
{

	SETBIT(dis_port,dis_6932cs);
	NOP();
	NOP();
	NOP();
	CLEARBIT(dis_port,dis_6932cs);
	NOP();
	NOP();
	NOP();
	dis_send(disnum[DISADD]);
	NOP();
	NOP();
	dis_send(table[DISBYT]);
	NOP();
	NOP();
	SETBIT(dis_port,dis_6932cs);

}
/*******************************************
;�ļ�����_DIS_DISBET
;��  �ܣ�д��TM1618
;��  �룺u
;��  ������
;��  �ã�
;*******************************************/
void dis_send(uchar  u)
{
	uchar	df;
	CLEARBIT(dis_port,dis_clk);
	for(df=0;df<8;df++)
	{
		CLEARBIT(dis_port,dis_clk);
		NOP();
		NOP();
		NOP();

		if((u>>df)&0x01)SETBIT(dis_port,dis_din);
		else CLEARBIT(dis_port,dis_din);
		NOP();
		NOP();

		SETBIT(dis_port,dis_clk);
		NOP();
		NOP();
		NOP();
		NOP();

	}
}
/*******************************************
;�ļ�����dis_f_test()
;��  �ܣ�����ܼ��
;��  �ã�delays(),dis_con(),DIS_TEST()
;*******************************************/
void dis_f_test(void)
{
	TM1629_init();
	dis_con(0x9c);	//��������
	_delay_ms(100);
	DIS_TEST(dis_full);
	_delay_ms(100);
	//DIS_TEST(dis_null);
	//Delayms(300);
	//DIS_TEST(dis_full);
	//Delayms(300);
	//DIS_TEST(dis_null);
	//Delayms(300);
}
/***************************************
���룺dis_buf[n]
�������
���ܣ���ʾnλ���ݣ�����������
***************************************/
void display_some(uchar dis_start,uchar dis_bit)
{
	uchar	dis_nc=null;
	uchar	dis_n=null;
	uchar	dis_m=null;
	uchar	dis_falg=false;
	for(dis_nc=dis_bit;dis_nc>0;dis_nc--)
	{
		dis_m=dis_start+dis_nc-1;
		if(null!=dis_buf[dis_m])
		{
			for(dis_n=dis_nc;dis_n>0;dis_n--)
			{
				dis_m=dis_start+dis_n-1;
				DIS_PARA(dis_m,dis_buf[dis_m]);
				dis_falg=true;
			}
			break;
		}
		else 	DIS_PARA(dis_m,dis_null);
	}
	if(false==dis_falg)//----------------------------------
	{
		DIS_PARA(dis_start,dis_buf[dis_start]);
	}
}
/*******************************************
;�ļ�����dis_f_test()
;��  �ܣ�����ܵ�����˸����,֧��16λ�����
;��  �ã�DIS_PARA()
;*******************************************/
void dis_f_bit(uchar dis_bit)
{
	dis_fbit_timewait+=1;
	if(dis_fbit_timewait<dis_fbit_timebrake)DIS_PARA(dis_bit,dis_buf[dis_bit]);
	if(dis_fbit_timewait>dis_fbit_timebrake)DIS_PARA(dis_bit,dis_null);
	if(dis_fbit_timewait>2*dis_fbit_timebrake)dis_fbit_timewait=0;
}
