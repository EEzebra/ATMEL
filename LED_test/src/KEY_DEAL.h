/*
 * KEY_DEAL.h
 *
 * Created: 2017/1/20 9:09:25
 *  Author: jin
 */ 


#ifndef KEY_DEAL_H_
#define KEY_DEAL_H_

#define     KEYPIN1             PIND5   //ƣ�ʹ���ѡ��ƣ�ʹ�������
#define     KEYPIN2             PIND6   //������ŷ�������
#define     KEYPIN3             PIND7	//��ͣ����ʼƣ��
#define		KEY_dat				PIND
#define		KEY_ddrd			DDRD
#define		KEY_port			PORTD
//��������
#define     KEY_DOWN        0xA0     //����
#define     KEY_HOLD        0xB0     //���� HOLD
#define     KEY_BURST       0xC0     //����BURST
#define     KEY_UP          0xD0     //̧��
#define		KEY_DOUBLE		0XE0     //˫��
//-------------------------------------------------------------------------------

//������������
#define     KEY_BURST_FIRST    5000     //����������ʼ����ʱ�䳤��
//����ֵ
#define     KEY_1           0x01
#define     KEY_2           0x02
#define     KEY_3           0x03
//�޼�����
#define     NO_KEY          0x00
//��������
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

extern unsigned char KeyValue;		//������Чֵ
extern unsigned char Keypermit_deal;//����������
extern 	unsigned char setting_dis_bit;//���ó�ʼλ
//extern 	unsigned char setting_dis_bi_high;//�������λ
extern 	unsigned char setting_state;//����״̬��־0�����ã�1���ó���ʱ�䣬2���÷���ʱ�䣬3����ƣ������
extern 	unsigned char work_hold_state;//��ѹ״̬
extern	unsigned char delay_state;//0:��ŷ�1��������ŷ�2й���� 1����ŷ�2��������ŷ�1й��
extern	unsigned char fatigue_step;//ƣ�ʹ�����λ
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