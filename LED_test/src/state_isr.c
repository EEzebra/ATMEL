/*
 * state_isr.c
 *
 * Created: 2017/1/20 9:11:37
 *  Author: jin
 */ 
	#include<avr/io.h>			//���ݿ�ͷ�ļ�
	#include<util/delay.h>		//�ڲ���ʱ����ͷ�ļ�
	#include<avr/interrupt.h>	//�жϺ���ͷ�ļ�
	#include"define.h"			//�˿ڶ���
	#include"Display.h"			//��ʾģ��
	#include"KEY_DEAL.h"			//��ʾģ��
	#include"state_isr.h"		//����״̬������
	#define		true			1
	#define		false			0

	unsigned int gas_filled_time_seting	=1;		//��������ʱ�䣬���99��
	//	unsigned int gas_release_time_seting	=1;		//��������ʱ��	���99��

	unsigned int  fatigue_number_max		=2000;
	unsigned int  fatigue_number			=0;		//��ǰƣ�ʹ���

	unsigned int  fatigue_time				=0;		//
	unsigned int  fatigue_filled			=true;	//
	unsigned int state_stop_waittime   		= 0;   	//����״̬,״̬��ʹ��
	unsigned int fatigue_counter_flag   	= true; //����״̬,״̬��ʹ��
	
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

			if(fatigue_number>=fatigue_number_max)	SETBIT(PINB_port,buzzer_pin);//��������
			
		}
		
		if(state_stop_waittime==4*state_stop_waittime_break)
		{
			//DIS_TEST(dis_null);
			dis_buf[dis_bit00]=fatigue_number_max%10;
			dis_buf[dis_bit01]=(fatigue_number_max/10)%10;
			dis_buf[dis_bit02]=(fatigue_number_max/100)%10;
			dis_buf[dis_bit03]=(fatigue_number_max/1000)%10;

			display_some(0,4);

			CLEARBIT(PINB_port,buzzer_pin);//������ֹͣ
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
			if(delay_state==false)//��ŷ�1������
			{
				delay_state=true;
				CLEARBIT(PINB_port,E_delay2);		//�س�����1
				SETBIT(PINB_port,E_delay3);		//��������2
				DIS_PARA(7,dis_delay3);	//��ŷ�2ָʾ����
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
				CLEARBIT(PINB_port,E_delay3);		//�س�����2
				SETBIT(PINB_port,E_delay2);		//��������1
				DIS_PARA(7,dis_delay2);	//��ŷ�1ָʾ����
			}
			fatigue_time=0;

			
			
		}

		
		
		if(fatigue_number>=fatigue_number_max)
		{
			//timer1_close();
			_delay_ms(900);//��ŷ�������ʱ
			
			workingstate=workingstate_stop;
			CLEARBIT(PINB_port,E_delay2);		//�س�����
			CLEARBIT(PINB_port,E_delay3);		//�س�����
			DIS_PARA(7,dis_delay_off);	//��ŷ�1,2ָʾ����

			
		}
		
	}
	void state_seting(void)
	{
		//nop();
		display_some(0,4);
	}


