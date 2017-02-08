/*
 * state_isr.c
 *
 * Created: 2017/1/20 9:11:37
 *  Author: jin
 */ 
	#include<avr/io.h>			//数据口头文件
	#include<util/delay.h>		//内部延时函数头文件
	#include<avr/interrupt.h>	//中断函数头文件
	#include"define.h"			//端口定义
	#include"Display.h"			//显示模块
	#include"KEY_DEAL.h"			//显示模块
	#include"state_isr.h"		//工作状态任务处理
	#define		true			1
	#define		false			0

	unsigned int gas_filled_time_seting	=1;		//充气设置时间，最大到99秒
	//	unsigned int gas_release_time_seting	=1;		//放弃设置时间	最大到99秒

	unsigned int  fatigue_number_max		=2000;
	unsigned int  fatigue_number			=0;		//当前疲劳次数

	unsigned int  fatigue_time				=0;		//
	unsigned int  fatigue_filled			=true;	//
	unsigned int state_stop_waittime   		= 0;   	//按键状态,状态机使用
	unsigned int fatigue_counter_flag   	= true; //按键状态,状态机使用
	
	void state_stop(void)
	{

		state_stop_waittime+=1;
		//state_stop_waittime=1;             //debug---------------------------------
		//CLEARBIT(led_port,led_pin_work);
		if(state_stop_waittime==1)
		{
			
			//DIS_TEST(dis_null);
			dis_buf[dis_bit00]=fatigue_number%10;
			dis_buf[dis_bit01]=(fatigue_number/10)%10;
			dis_buf[dis_bit02]=(fatigue_number/100)%10;
			dis_buf[dis_bit03]=(fatigue_number/1000)%10;

			display_some(0,4);

			if(fatigue_number>=fatigue_number_max)	SETBIT(PINB_port,buzzer_pin);//蜂鸣器响
			
		}
		
		if(state_stop_waittime==4*state_stop_waittime_break)
		{
			//DIS_TEST(dis_null);
			dis_buf[dis_bit00]=fatigue_number_max%10;
			dis_buf[dis_bit01]=(fatigue_number_max/10)%10;
			dis_buf[dis_bit02]=(fatigue_number_max/100)%10;
			dis_buf[dis_bit03]=(fatigue_number_max/1000)%10;

			display_some(0,4);

			CLEARBIT(PINB_port,buzzer_pin);//蜂鸣器停止
		}
		
		if(state_stop_waittime>8*state_stop_waittime_break)state_stop_waittime=0;
	}

	void state_run(void)
	{
		if(timeover_flag==true)
		{
			timeover_flag=false;
			fatigue_time+=1;
		}
		
		
		if(fatigue_time>=gas_filled_time_seting)
		{
			if(delay_state==false)//电磁阀1进气？
			{
				delay_state=true;
				CLEARBIT(PINB_port,E_delay2);		//关充气阀1
				SETBIT(PINB_port,E_delay3);		//开充气阀2
				DIS_PARA(7,dis_delay3);	//电磁阀2指示灯亮
				fatigue_number+=1;
				
				//DIS_TEST(dis_null);
				dis_buf[dis_bit00]=fatigue_number%10;
				dis_buf[dis_bit01]=(fatigue_number/10)%10;
				dis_buf[dis_bit02]=(fatigue_number/100)%10;
				dis_buf[dis_bit03]=(fatigue_number/1000)%10;
				display_some(0,4);
				
			}
			else
			{
				delay_state=false;
				CLEARBIT(PINB_port,E_delay3);		//关充气阀2
				SETBIT(PINB_port,E_delay2);		//开充气阀1
				DIS_PARA(7,dis_delay2);	//电磁阀1指示灯亮
			}
			fatigue_time=0;

			
			
		}

		
		
		if(fatigue_number>=fatigue_number_max)
		{
			//timer1_close();
			_delay_ms(900);//电磁阀充气延时
			
			workingstate=workingstate_stop;
			CLEARBIT(PINB_port,E_delay2);		//关充气阀
			CLEARBIT(PINB_port,E_delay3);		//关充气阀
			DIS_PARA(7,dis_delay_off);	//电磁阀1,2指示灯灭

			
		}
		
	}
	void state_seting(void)
	{
		//nop();
		display_some(0,4);
	}


