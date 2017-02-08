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
	0x00,/*全灭*/
	0xFF,/*全亮*/
	0x05,/*电磁阀1亮*/
	0x03,/*电磁阀2亮*/
	0x01,/*电磁阀1、2全灭*/

};
const uchar  disnum[]={
	
	0xC3,/*第04位*/
	0xC2,/*第03位*/
	0xC1,/*第02位*/
	0xC0,/*第01位*/
	
	0xC7,/*第08位*/
	0xC6,/*第07位*/
	0xC5,/*第06位*/
	0xC4,/*第05位*/
	
	0xC8,/*第09位*/
	0xC9,/*第10位*/
	0xCA,/*第11位*/
	0xCB,/*第12位*/
	0xCC,/*第13位*/
	0xCD,/*第14位*/
	0xCE,/*第15位*/
	0xCF /*第16位*/
};
uchar dis_buf[]={null,null,null,null,null,null,null,null,null,null,null,
null,null,null,null,null,null,null,null,null,null,null};//数码管显示寄存器

uchar dis_bit_buf[]={null,null,null,null,null,null,null,null,null,null,null,
null,null,null,null,null,null,null,null,null,null,null};//数码管显示寄存器
uint dis_fbit_buf=0x00;// 位闪烁状态位，00为没有闪烁
uint dis_fbit_timewait=0x00;// 位闪烁状态位，00为没有闪烁

/*************************************************/
//6932引脚端口初始化
/*************************************************/
void TM1629_init(void)
{
	ACSR=0x80;//关闭模拟比较器
	CLEARBIT(dis_port,dis_6932cs);//输出0
	SETBIT(dis_ddr,dis_6932cs);	//dis_stb 置1　设为输出
	CLEARBIT(dis_port,dis_clk);//输出0
	SETBIT(dis_ddr,dis_clk);	//dis_clk 置1　设为输出
	CLEARBIT(dis_port,dis_din);//输出0
	SETBIT(dis_ddr,dis_din);	//dis_din 置1　设为输出
}
/****************显示模块区*****************
*********************************************
;文件名：_D_TEST
;功  能：LED全部显示
;输  入：DIS_BYT
;输  出：无
;调  用：_DIS_PARA
;*********************************************/
void DIS_TEST(uchar num)
{
	uchar n=0;
	for(n=0;n<4;n++)//4位数字数码管
	{
		DIS_PARA(n,num);
	}
}
/*********************************************
;文件名：_DIS_DISCON
;功  能：显示控制命令确定显示亮度
;输  入：
;输  出：无
;调  用：_DELAY2，_DIS_SEND
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
	dis_send(dis_con);//设置显示亮度
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
;文件名：_DIS_PARA
;功  能：显示字内容修改，修改显示内容
;输  入：DIS_ADD，DIS_BYT
;输  出：无
;调  用：_DIS_SEND
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
;文件名：_DIS_DISBET
;功  能：写入TM1618
;输  入：u
;输  出：无
;调  用：
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
;文件名：dis_f_test()
;功  能：数码管检测
;调  用：delays(),dis_con(),DIS_TEST()
;*******************************************/
void dis_f_test(void)
{
	TM1629_init();
	dis_con(0x9c);	//亮度设置
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
输入：dis_buf[n]
输出：无
功能：显示n位数据，有消隐功能
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
;文件名：dis_f_test()
;功  能：数码管单体闪烁函数,支持16位数码管
;调  用：DIS_PARA()
;*******************************************/
void dis_f_bit(uchar dis_bit)
{
	dis_fbit_timewait+=1;
	if(dis_fbit_timewait<dis_fbit_timebrake)DIS_PARA(dis_bit,dis_buf[dis_bit]);
	if(dis_fbit_timewait>dis_fbit_timebrake)DIS_PARA(dis_bit,dis_null);
	if(dis_fbit_timewait>2*dis_fbit_timebrake)dis_fbit_timewait=0;
}
