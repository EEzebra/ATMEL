/*
 * Delay.h
 *
 * Created: 2017/1/22 14:26:57
 *  Author: jin
 */ 


#ifndef DELAY_H_
#define DELAY_H_

#define	uchar	unsigned	char
#define	uint	unsigned	int
void Delayus(uint lus);
void Delayms(uint lms);
/*Ƶ�ʶ���ĵ�λ��MHz! ������ʹ�ø�����! ���羧����12MHz��Ӧ��д��12.0000��12.0֮��*/
#ifndef  CPU_CRYSTAL
#define  CPU_CRYSTAL    (11.0592)
#endif //1us ��ʱ һ��_delay_loop_2(1)=4*(1/CPU) US	wait_us(X) =X*(CPU/4)*(4/CPU)=x us
#define  wait_us(us)    _delay_loop_2((uint)((us)*CPU_CRYSTAL/4))



#endif /* DELAY_H_ */