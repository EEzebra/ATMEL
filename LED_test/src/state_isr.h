/*
 * state_isr.h
 *
 * Created: 2017/1/20 9:11:09
 *  Author: jin
 */ 


#ifndef STATE_ISR_H_
#define STATE_ISR_H_

#define	state_stop_waittime_break	1500
void state_stop(void);
void state_run(void);
void state_seting(void);

extern unsigned int gas_filled_time_seting;//����ʱ��
//extern unsigned int gas_release_time_seting;//����ʱ��
extern unsigned int  fatigue_number_max;
extern unsigned int  fatigue_number;	//��ǰƣ�ʹ���
extern unsigned int state_stop_waittime ;
extern unsigned int  fatigue_time;
extern unsigned int  fatigue_filled	;



#endif /* STATE_ISR_H_ */