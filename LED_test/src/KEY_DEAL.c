/*
 * KEY_DEAL.c
 *
 * Created: 2017/1/20 9:08:19
 *  Author: jin
 */ 
#include <avr/io.h>			//数据口头文件
#include <avr/interrupt.h>	//中断函数头文件
#include "Delay.h"
#include "define.h"			//端口定义	
#include "Display.h"			//显示模块
#include "KEY_DEAL.h"			//显示模块

void save_setting(void);		//设置数据保存
unsigned char KeyValue=0;		//按键返回值
unsigned char Keypermit_deal=true;//允许按键处理
unsigned char Key3=false;
unsigned char setting_dis_bit	=dis_bit00;//设置初始位
unsigned char setting_dis_bi_high	=dis_bit02;//设置最高位
unsigned char setting_state		=0;//设置状态标志0无设置，1设置充气时间，2设置放气时间，3设置疲劳上限
unsigned char work_hold_state	=0;//保压状态
unsigned char fatigue_step		=0;//疲劳次数上限：0  500次，1 1000次，2  2000次，3  5000次    
unsigned char delay_state		=0;//0:电磁阀1进气，电磁阀2泄气； 1：电磁阀2进气，电磁阀1泄气


	
/*
**-------------------------------------------------------------------------
**文件说明:

**    首先按键有四个,单线接入,没有用矩阵. 
   
**    按键使用状态机,四个状态    
**    
**    按键返回码:     按键属性|按键值  (或操作)

**    属性包括:KEY_DOWN, KEY_UP, 长按键返回

**    按键值   就是按键返回的值.
**        
**    高四位表示属性,,低四位表示按键值.
****************************************************************************/

/***********************************************************************
* 函数名称: GetKeyCode()
* 函数功能: 获得按键物理编码

************************************************************************/

void key_init(void)
{
	SETBIT(KEY_port,KEYPIN1);//1
	CLEARBIT(KEY_ddrd,KEYPIN1);	// 置0　设为输入

	SETBIT(KEY_port,KEYPIN2);//1
	//CLEARBIT(KEY_port,KEYPIN2);//0
	CLEARBIT(KEY_ddrd,KEYPIN2);	//置0　设为输入
	SETBIT(KEY_port,KEYPIN3);//1
	CLEARBIT(KEY_ddrd,KEYPIN3);	//置0　设为输入
}
/********************************************************************
获取按键号　
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
* 函数名称: unsigned char GetKey()
* 函数功能: 获得按键码
* 说    明: 使用状态机的方法来处理按键
*           支持部分按键连发
************************************************************************/
unsigned char GetKey(void)
{
    static unsigned char KeyState   = 0;        //按键状态,状态机使用.         ？？？？？？？？？？？？？？？？？？？
    static unsigned char KeyPrev    = 0;        //保存上一次按键,防止出现乱码现象.
    static unsigned int KeyHoldDly = 0;        //按键连发时间
    
    unsigned char KeyPress  = NO_KEY;           //按键值
    unsigned char KeyReturn = NO_KEY;           //按键返回值

    KeyPress = GetKeyCode();					//获取按键值
    switch(KeyState)
    {        
        case 0:  						//按键初始态00   
            if( KeyPress !=NO_KEY ) 	//有键按下
            {
                KeyState = 1;			//转入下一个状态，按键确认
                KeyPrev  = KeyPress;	//保存键值　
			}
			break;      
        case 1:							//按键确认态01
            if( KeyPress !=NO_KEY  ) 
            {                
                if(KeyPrev  != KeyPress)//与上次按键不同,是乱码
                {
                    KeyState  = 0;
                }
                else
                {
                     //以下为按键按下处理
                    KeyState  = 2;							//确认按键按下，转入按键长短按判断
                    
                }
            }
            else	//按键抬起,是抖动,不响应按键
            {
				KeyState = 0;
            }

            break;
        case 2:                         //按键连续态11    
            if( KeyPress !=NO_KEY )
            {
				KeyHoldDly++;
				KeyState=2;
				if(KeyHoldDly>KEY_BURST_FIRST)				//长按状态判断
				{
                    KeyHoldDly = 0;
                    KeyReturn  = KEY_HOLD | KeyPrev;       //返回长按后的值
                    KeyState=3;
					break;
				}
				//else
				//{
				//	KeyHoldDly = 0;
				//	KeyReturn = KEY_DOWN | KeyPrev;        //返回按键按下键值
				//	KeyState=3;   
       			//}
            } 
			else
			{
				KeyHoldDly = 0;
				KeyReturn = KEY_DOWN | KeyPrev;        //返回按键按下键值
				KeyState=3;
			}
			
       		break;
	    case 3:  //按键释放态10
            if(KeyPress == NO_KEY )KeyState=4; 
			break;
	    case 4:  //按键释放态10
            if(KeyPress == NO_KEY ) 
            {
                KeyState   = 0;
                KeyHoldDly = 0;
                KeyReturn  = KEY_UP | KeyPrev;              //返回按键抬起值
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
//按键检测
/*******************************************************************/
void Key_detect(void)
{	 
	unsigned char  keytemp;
	KeyValue=0;
	keytemp=GetKey();
/**/
	if(true==Keypermit_deal)									//允许按键处理
	{											
		if(null!=keytemp)
		{
			Keypermit_deal=false;						//在按键松开前不允许第二次处理			
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
				case KEY_UP|KEY_1:  	KeyValue=KEY_1_UP;Keypermit_deal=true; 	break;//允许下一次按键处理
				case KEY_UP|KEY_2:  	KeyValue=KEY_2_UP;Keypermit_deal=true; 	break;//允许下一次按键处理
				case KEY_UP|KEY_3:  	KeyValue=KEY_3_UP;Keypermit_deal=true; 	break;//允许下一次按键处理	 															
				default :KeyValue=0; break;
			} 
		}		
	}
}


/******************************************************************/
//按键检测
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

		if(fatigue_number>=9999)//如果疲劳次数已到，按暂停键清零，
		{
			fatigue_number=0;//疲劳次数计数清零
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

/*长按复位*/
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
