/*
 * KEY_DEAL.h
 *
 * Created: 2017/1/20 9:09:25
 *  Author: jin
 */ 


#ifndef KEY_DEAL_H_
#define KEY_DEAL_H_

#define     KEYPIN1             PIND5   //疲劳次数选择、疲劳次数清零
#define     KEYPIN2             PIND6   //两个电磁阀开、关
#define     KEYPIN3             PIND7	//暂停、开始疲劳
#define		KEY_dat				PIND
#define		KEY_ddrd			DDRD
#define		KEY_port			PORTD
//按键属性
#define     KEY_DOWN        0xA0     //按下
#define     KEY_HOLD        0xB0     //长按 HOLD
#define     KEY_BURST       0xC0     //连发BURST
#define     KEY_UP          0xD0     //抬起
#define		KEY_DOUBLE		0XE0     //双击
//-------------------------------------------------------------------------------

//按键连发设置
#define     KEY_BURST_FIRST    5000     //按键连发开始所需时间长度
//按键值
#define     KEY_1           0x01
#define     KEY_2           0x02
#define     KEY_3           0x03
//无键按下
#define     NO_KEY          0x00
//按键按下
#define     KEY_PRESS_DOWN	0x00

extern unsigned char GetKey(void);
extern void Key_detect(void);

extern void key_init(void);
extern void Key_ISR(void);
extern unsigned char GetKeyCode(void);

void KEY_1_short_isr(void);
void KEY_2_short_isr(void);
void KEY_3_short_isr(void);
void KEY_1_HOLD_isr(void);
void KEY_2_HOLD_isr(void);
void KEY_3_HOLD_isr(void);

extern unsigned char KeyValue;		//按键有效值
extern unsigned char Keypermit_deal;//允许按键处理
extern 	unsigned char setting_dis_bit;//设置初始位
//extern 	unsigned char setting_dis_bi_high;//设置最高位
extern 	unsigned char setting_state;//设置状态标志0无设置，1设置充气时间，2设置放气时间，3设置疲劳上限
extern 	unsigned char work_hold_state;//保压状态
extern	unsigned char delay_state;//0:电磁阀1进气，电磁阀2泄气； 1：电磁阀2进气，电磁阀1泄气
extern	unsigned char fatigue_step;//疲劳次数档位
extern unsigned char Key3;

#define KEY_1_short     1
#define KEY_2_short     2
#define KEY_3_short     3
#define KEY_1_HOLD      4
#define KEY_2_HOLD      5
#define KEY_3_HOLD      6
#define KEY_1_UP		7
#define KEY_2_UP		8
#define KEY_3_UP		9


#endif /* KEY_DEAL_H_ */