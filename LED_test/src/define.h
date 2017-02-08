/*
 * define.h
 *
 * Created: 2017/1/19 16:22:35
 *  Author: jin
 */ 


#ifndef DEFINE_H_
#define DEFINE_H_

#define BIT(x)	           (1 << (x))
#define SETBIT(x, y)   	  (x |= 1<<y)
#define CLEARBIT(x, y) 	  (x &=~(1<<y))
#define CHECKBIT(x, y) 	   (x &(1<<y))
#define COMFBIT(x, y) 	   (x ^ (1<<y)
#define true 1
#define True 1
#define false 0
#define False 0

#define	ulong	unsigned	long	int
#define uint  	unsigned 	int
#define	uchar	unsigned	char
#define	NOP()	asm("nop")
#define Nop()	asm("nop")
#define	null	0x00

//TM6932通讯引脚
/********TM6932通讯引脚********/
#define		dis_din		PIND0
#define		dis_clk		PIND1
#define		dis_6932cs	PIND3
//#define		dis_stb		PIND2
#define		dis_ddr		DDRD
#define		dis_port	PORTD


/********蜂鸣器引脚********/
#define		buzzer_pin		PINB2
#define		E_delay2		PINB7//
#define		E_delay3		PINB6//
#define		PINB_dat		PINB
#define		PINB_ddrd		DDRB
#define		PINB_port		PORTB


extern unsigned char workingstate ;//工作状态标志
extern 	unsigned char counter_buf;
extern 	unsigned char timeover_flag;
extern 	unsigned char timeover_buf;
extern 	unsigned int fatigue_number;

extern 	void timer1_init(void);
extern 	void timer1_close(void);


#define		workingstate_stop	0
#define		workingstate_run	1
#define		workingstate_seting	2




#endif /* DEFINE_H_ */