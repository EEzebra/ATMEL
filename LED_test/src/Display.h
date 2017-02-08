/*
 * Display.h
 *
 * Created: 2017/1/19 16:25:24
 *  Author: jin
 */ 


#ifndef DISPLAY_H_
#define DISPLAY_H_

#define uint unsigned int
#define uchar unsigned char
#define uint unsigned int
#define true 1
#define false 0

#define	dis_0		0x00/*0*/
#define dis_1		0x01/*1*/
#define dis_2		0x02/*2*/
#define dis_3		0x03/*3*/
#define dis_4		0x04/*4*/
#define dis_5		0x05/*5*/
#define	dis_6		0x06/*6*/
#define	dis_7		0x07/*7*/
#define	dis_8		0x08/*8*/
#define	dis_9		0x09/*9*/
#define	dis_A		0x0A/*A*/
#define	dis_B		0x0B/*b*/
#define	dis_C		0x0C/*C*/
#define	dis_D		0x0D/*d*/
#define	dis_E		0x0E/*E*/
#define	dis_F		0x0F/*F*/
#define	dis_null	0x10/*H*/
#define	dis_full	0x11/*h*/
#define dis_delay2	0x12
#define dis_delay3	0x13
#define dis_delay_off	0x14


#define	dis_bit00	0x00/*0*/
#define dis_bit01	0x01/*1*/
#define dis_bit02	0x02/*2*/
#define dis_bit03	0x03/*3*/
#define dis_bit04	0x04/*4*/
#define dis_bit05	0x05/*5*/
#define	dis_bit06	0x06/*6*/
#define	dis_bit07	0x07/*7*/
#define	dis_bit08	0x08/*8*/
#define	dis_bit09	0x09/*9*/
#define	dis_bit10	0x0A/*A*/
#define	dis_bit11	0x0B/*b*/
#define	dis_bit12	0x0C/*C*/
#define	dis_bit13	0x0D/*d*/
#define	dis_bit14	0x0E/*E*/
#define	dis_bit15	0x0F/*F*/


void dis_send(uchar u);						//数码管送数
void DIS_TEST(uchar num);					//数码管检测
void DIS_PARA(uchar DISADD,uchar DISBYT);	//数码管显示
void dis_con(uchar);						//数码管设置
void dis_f_test(void);							//数码管检测函数
void dis_f_bit(uchar dis_bit);	//数码管单体闪烁函数
void display_some(uchar dis_start,uchar dis_bit);
void TM1629_init(void);
extern	uchar  dis_buf[];
extern	uchar dis_bit_buf[];
extern	uint dis_fbit_buf;// 位闪烁状态位，00为没有闪烁
extern	uint dis_fbit_timewait;// 位闪烁状态位，00为没有闪烁


void display_pass(void);
void display_fail(void);


#endif /* DISPLAY_H_ */