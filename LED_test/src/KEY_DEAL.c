/*
 * KEY_DEAL.c
 *
 * Created: 2017/1/20 9:08:19
 *  Author: jin
 */ 
#include <avr/io.h>			//���ݿ�ͷ�ļ�
#include <avr/interrupt.h>	//�жϺ���ͷ�ļ�
#include "Delay.h"
#include "define.h"			//�˿ڶ���	
#include "Display.h"			//��ʾģ��
#include "KEY_DEAL.h"			//��ʾģ��

void save_setting(void);		//�������ݱ���
unsigned char KeyValue=0;		//��������ֵ
unsigned char Keypermit_deal=true;//����������
unsigned char Key3=false;
unsigned char setting_dis_bit	=dis_bit00;//���ó�ʼλ
unsigned char setting_dis_bi_high	=dis_bit02;//�������λ
unsigned char setting_state		=0;//����״̬��־0�����ã�1���ó���ʱ�䣬2���÷���ʱ�䣬3����ƣ������
unsigned char work_hold_state	=0;//��ѹ״̬
unsigned char fatigue_step		=0;//ƣ�ʹ������ޣ�0  500�Σ�1 1000�Σ�2  2000�Σ�3  5000��    
unsigned char delay_state		=0;//0:��ŷ�1��������ŷ�2й���� 1����ŷ�2��������ŷ�1й��


	
/*
**-------------------------------------------------------------------------
**�ļ�˵��:

**    ���Ȱ������ĸ�,���߽���,û���þ���. 
   
**    ����ʹ��״̬��,�ĸ�״̬    
**    
**    ����������:     ��������|����ֵ  (�����)

**    ���԰���:KEY_DOWN, KEY_UP, ����������

**    ����ֵ   ���ǰ������ص�ֵ.
**        
**    ����λ��ʾ����,,����λ��ʾ����ֵ.
****************************************************************************/

/***********************************************************************
* ��������: GetKeyCode()
* ��������: ��ð����������

************************************************************************/

void key_init(void)
{
	SETBIT(KEY_port,KEYPIN1);//1
	CLEARBIT(KEY_ddrd,KEYPIN1);	// ��0����Ϊ����

	SETBIT(KEY_port,KEYPIN2);//1
	//CLEARBIT(KEY_port,KEYPIN2);//0
	CLEARBIT(KEY_ddrd,KEYPIN2);	//��0����Ϊ����
	SETBIT(KEY_port,KEYPIN3);//1
	CLEARBIT(KEY_ddrd,KEYPIN3);	//��0����Ϊ����
}
/********************************************************************
��ȡ�����š�
*********************************************************************/

unsigned char GetKeyCode(void)
{
	if(Key3)
	{
    if(0==CHECKBIT(KEY_dat,KEYPIN1)) return KEY_1;
    if(0==CHECKBIT(KEY_dat,KEYPIN2)) return KEY_2;
	}
	//if(1==CHECKBIT(KEY_dat,KEYPIN2)) return KEY_2;
    if(0==CHECKBIT(KEY_dat,KEYPIN3)) return KEY_3;	
    return NO_KEY;
	
}

/***********************************************************************
* ��������: unsigned char GetKey()
* ��������: ��ð�����
* ˵    ��: ʹ��״̬���ķ�����������
*           ֧�ֲ��ְ�������
************************************************************************/
unsigned char GetKey(void)
{
    static unsigned char KeyState   = 0;        //����״̬,״̬��ʹ��.         ��������������������������������������
    static unsigned char KeyPrev    = 0;        //������һ�ΰ���,��ֹ������������.
    static unsigned int KeyHoldDly = 0;        //��������ʱ��
    
    unsigned char KeyPress  = NO_KEY;           //����ֵ
    unsigned char KeyReturn = NO_KEY;           //��������ֵ

    KeyPress = GetKeyCode();					//��ȡ����ֵ
    switch(KeyState)
    {        
        case 0:  						//������ʼ̬00   
            if( KeyPress !=NO_KEY ) 	//�м�����
            {
                KeyState = 1;			//ת����һ��״̬������ȷ��
                KeyPrev  = KeyPress;	//�����ֵ��
			}
			break;      
        case 1:							//����ȷ��̬01
            if( KeyPress !=NO_KEY  ) 
            {                
                if(KeyPrev  != KeyPress)//���ϴΰ�����ͬ,������
                {
                    KeyState  = 0;
                }
                else
                {
                     //����Ϊ�������´���
                    KeyState  = 2;							//ȷ�ϰ������£�ת�밴�����̰��ж�
                    
                }
            }
            else	//����̧��,�Ƕ���,����Ӧ����
            {
				KeyState = 0;
            }

            break;
        case 2:                         //��������̬11    
            if( KeyPress !=NO_KEY )
            {
				KeyHoldDly++;
				KeyState=2;
				if(KeyHoldDly>KEY_BURST_FIRST)				//����״̬�ж�
				{
                    KeyHoldDly = 0;
                    KeyReturn  = KEY_HOLD | KeyPrev;       //���س������ֵ
                    KeyState=3;
					break;
				}
				//else
				//{
				//	KeyHoldDly = 0;
				//	KeyReturn = KEY_DOWN | KeyPrev;        //���ذ������¼�ֵ
				//	KeyState=3;   
       			//}
            } 
			else
			{
				KeyHoldDly = 0;
				KeyReturn = KEY_DOWN | KeyPrev;        //���ذ������¼�ֵ
				KeyState=3;
			}
			
       		break;
	    case 3:  //�����ͷ�̬10
            if(KeyPress == NO_KEY )KeyState=4; 
			break;
	    case 4:  //�����ͷ�̬10
            if(KeyPress == NO_KEY ) 
            {
                KeyState   = 0;
                KeyHoldDly = 0;
                KeyReturn  = KEY_UP | KeyPrev;              //���ذ���̧��ֵ
            }
            else	KeyState=3;             
            break;
		 default :
        	KeyState = 0;
        	KeyReturn = NO_KEY;
        	break;
    }    
    return KeyReturn;
}


/******************************************************************/
//�������
/*******************************************************************/
void Key_detect(void)
{	 
	unsigned char  keytemp;
	KeyValue=0;
	keytemp=GetKey();
/**/
	if(true==Keypermit_deal)									//����������
	{											
		if(null!=keytemp)
		{
			Keypermit_deal=false;						//�ڰ����ɿ�ǰ������ڶ��δ���			
			switch(keytemp)  
			{
			
				case KEY_DOWN|KEY_1:  	KeyValue=KEY_1_short; 	break;
				case KEY_DOWN|KEY_2:  	KeyValue=KEY_2_short; 	break;
				case KEY_DOWN|KEY_3:  	KeyValue=KEY_3_short; 	break;
				case KEY_HOLD|KEY_1:  	KeyValue=KEY_1_HOLD;	break;
				case KEY_HOLD|KEY_2:  	KeyValue=KEY_2_HOLD; 	break;
				case KEY_HOLD|KEY_3:  	KeyValue=KEY_3_HOLD; 	break;
				default :KeyValue=0; break;
			} 
		}
	}
	else
	{
		if(null!=keytemp)
		{
			switch(keytemp)  
			{
				case KEY_UP|KEY_1:  	KeyValue=KEY_1_UP;Keypermit_deal=true; 	break;//������һ�ΰ�������
				case KEY_UP|KEY_2:  	KeyValue=KEY_2_UP;Keypermit_deal=true; 	break;//������һ�ΰ�������
				case KEY_UP|KEY_3:  	KeyValue=KEY_3_UP;Keypermit_deal=true; 	break;//������һ�ΰ�������	 															
				default :KeyValue=0; break;
			} 
		}		
	}
}


/******************************************************************/
//�������
/*******************************************************************/
void Key_ISR(void)
{
	switch(KeyValue)
	{
		//case KEY_1_short:
		//if (Key3)
		//{
		//	KEY_1_short_isr();
		//}break;
		case KEY_2_short:
		if (Key3)
		{
			KEY_2_short_isr();
		}break;
		case KEY_3_short:	KEY_3_short_isr();
		break;
		case KEY_1_HOLD :	if (Key3)
		{
			KEY_1_short_isr();
		}break;
		case KEY_2_HOLD :	if (Key3)
		{
			KEY_2_short_isr();
		}break;
		//case KEY_3_HOLD :	KEY_3_HOLD_isr();
		//break;
		default:	KeyValue=null;break;
	}
	KeyValue=null;	
}

void KEY_1_short_isr(void)
{
		fatigue_number=0;

		dis_buf[dis_bit00]=fatigue_number%10;
		dis_buf[dis_bit01]=(fatigue_number/10)%10;
		dis_buf[dis_bit02]=(fatigue_number/100)%10;
		dis_buf[dis_bit03]=(fatigue_number/1000)%10;
		
		display_some(0,4);
}

void KEY_2_short_isr(void)
{

		if(fatigue_number>=9999)//���ƣ�ʹ����ѵ�������ͣ�����㣬
		{
			fatigue_number=0;//ƣ�ʹ�����������
		}
		else
		{
			fatigue_number+=1;//step+1
		}
		
		dis_buf[dis_bit00]=fatigue_number%10;
		dis_buf[dis_bit01]=(fatigue_number/10)%10;
		dis_buf[dis_bit02]=(fatigue_number/100)%10;
		dis_buf[dis_bit03]=(fatigue_number/1000)%10;
		
		display_some(0,4);
	
}

void KEY_3_short_isr(void)
{
	Key3=!Key3;
	if (Key3==false)
	{
		timer1_init();
	}
	else
	{
		timer1_close();
	}
	display_some(0,4);
	
}

/*������λ*/
void KEY_1_HOLD_isr(void)
{
	//if((workingstate==workingstate_stop)|(workingstate==workingstate_seting))
	//{
	//	workingstate=workingstate_seting;
		
		fatigue_number=0;

		//DIS_TEST(dis_null);
		dis_buf[dis_bit00]=fatigue_number%10;
		dis_buf[dis_bit01]=(fatigue_number/10)%10;
		dis_buf[dis_bit02]=(fatigue_number/100)%10;
		dis_buf[dis_bit03]=(fatigue_number/1000)%10;
		
		display_some(0,4);
	//}
}
