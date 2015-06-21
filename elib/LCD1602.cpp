#include "lcd1602.h"
#include "stdio.h"
#include "lcd1602.h"

#define DATAOUT  GPIOB->ODR             //PD[0..7]--(LCD)D0~D7

LCD1602::LCD1602(uint8_t LEDPin,uint8_t ENPin,uint8_t RWPin,uint8_t RSPin,uint8_t DB0,uint8_t DB1,uint8_t DB2,uint8_t DB3,uint8_t DB4,uint8_t DB5,uint8_t DB6,uint8_t DB7)
{
		 _LEDPin = LEDPin;
		 _ENPin	= ENPin;
		 _RWPin	= RWPin;
		 _RSPin	= RSPin;
		 _DB0		= DB0;
		 _DB1		= DB1;
		 _DB2		= DB2;
		 _DB3		= DB3;
		 _DB4		= DB4;
		 _DB5		= DB5;
		 _DB6		= DB6;
		 _DB7		= DB7;
	pinMode(_LEDPin,OUTPUT);
	pinMode(_ENPin,OUTPUT);
	pinMode(_RWPin,OUTPUT);
	pinMode(_RSPin,OUTPUT);
	pinMode(_DB0,OUTPUT);
	pinMode(_DB1,OUTPUT);
	pinMode(_DB2,OUTPUT);
	pinMode(_DB3,OUTPUT);
	pinMode(_DB4,OUTPUT);
	pinMode(_DB5,OUTPUT);
	pinMode(_DB6,OUTPUT);
	pinMode(_DB7,OUTPUT);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	init();
	nDelay = 10;
}
LCD1602::LCD1602(uint8_t LEDPin,uint8_t ENPin,uint8_t RWPin,uint8_t RSPin,uint8_t DB0,uint8_t DB1,uint8_t DB2,uint8_t DB3)
{
		 _LEDPin = LEDPin;
		 _ENPin	= ENPin;
		 _RWPin	= RWPin;
		 _RSPin	= RSPin;
		 _DB0		= DB0;
		 _DB1		= DB1;
		 _DB2		= DB2;
		 _DB3		= DB3;
	pinMode(_LEDPin,OUTPUT);
	pinMode(_ENPin,OUTPUT);
	pinMode(_RWPin,OUTPUT);
	pinMode(_RSPin,OUTPUT);
	pinMode(_DB0,OUTPUT);
	pinMode(_DB1,OUTPUT);
	pinMode(_DB2,OUTPUT);
	pinMode(_DB3,OUTPUT);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

	nDelay = 10;
}

inline void LCD1602::LCDdelay(uint32_t nCount)
{
   for(; nCount != 0; nCount--);
}
inline uint8_t LCD1602::bz(void)
{                         
	uint8_t result;


	pinMode(_DB7,INPUT);
	digitalWrite(_RSPin,0);
	digitalWrite(_RWPin,1);
	digitalWrite(_ENPin,1);
	LCDdelay(nDelay);
	result = digitalRead(_DB7);
	digitalWrite(_ENPin,0);

	return result; 

}
/******************************д����*************************************************/	
/*********************************************************************
*��    �ƣ�Writecmd()
*��    �ܣ�дָ�LCD1602,ָ������ռһ���ֽ�
*��ڲ�����cmd:��д���ָ��
*���ڲ�������
*********************************************************************/
void LCD1602::wcmd(uint8_t cmd)
{                          
	while(bz())
	{
		;
	}

	pinMode(_DB7,OUTPUT);
	digitalWrite(_RSPin,0);	   //��ͬһ���Ĵ���������д�룬�м���ʱһ��
	digitalWrite(_RWPin,0);
	digitalWrite(_ENPin,0);
	
//	digitalWrite(_DB0,cmd&0x01);
//	digitalWrite(_DB1,cmd&0x02);
//	digitalWrite(_DB2,cmd&0x04);
//	digitalWrite(_DB3,cmd&0x08);
//	digitalWrite(_DB4,cmd&0x10);
//	digitalWrite(_DB5,cmd&0x20);
//	digitalWrite(_DB6,cmd&0x40);
//	digitalWrite(_DB7,cmd&0x80);
	
	DATAOUT &= 0XFF00;
	DATAOUT |= cmd;


	LCDdelay(nDelay);
	digitalWrite(_ENPin,1);
	LCDdelay(nDelay);
	digitalWrite(_ENPin,0);
	LCDdelay(nDelay);
}	
/*********************************************************************
*��    �ƣ�Writedata()
*��    �ܣ�дһ�ֽ����ݵ�LCD1602
*��ڲ�����dat���޷����ֽ����ͣ�0~255  ��������ASCII���ַ�
*���ڲ�������
*********************************************************************/
void LCD1602::wdat(uint8_t dat) 
{                         
	while(bz())//�ȴ�LCD1602����
	{
		;
	}

	pinMode(_DB7,OUTPUT);
	digitalWrite(_RSPin,1);
	digitalWrite(_RWPin,0);
	digitalWrite(_ENPin,0);
//	digitalWrite(_DB0,dat&0x01);
//	digitalWrite(_DB1,dat&0x02);
//	digitalWrite(_DB2,dat&0x04);
//	digitalWrite(_DB3,dat&0x08);
//	digitalWrite(_DB4,dat&0x10);
//	digitalWrite(_DB5,dat&0x20);
//	digitalWrite(_DB6,dat&0x40);
//	digitalWrite(_DB7,dat&0x80);
	
	DATAOUT &= 0XFF00;
	DATAOUT |= dat;

	LCDdelay(nDelay);
	digitalWrite(_ENPin,1);
	LCDdelay(nDelay);
	digitalWrite(_ENPin,0);
}
	
/*********************************************************************
*��    �ƣ�pos()
*��    �ܣ��趨��ʾλ��
*��ڲ�����pos:��ʾλ��,ֵ�ķ�Χ���£�
*    0x00----------------0x0f 0x10-------0x27 ��һ�У�һ����ʾ16���ַ���
*    0x40----------------0x4f 0x50-------0x67 �ڶ���
*���ڲ�������
*********************************************************************/
void LCD1602::pos(uint8_t pos)
{                         
	wcmd(pos | 0x80);
}

/*********************************************************************
*��    �ƣ�Setpos()
*��    �ܣ�����ϰ���趨��ʾλ��
*��ڲ�����row:�У�row=1��ʾ��һ�У�row=2��ʾ�ڶ���
*          col:�У�0~15������ָ����ʾ���У���Χ������0~40
*���ڲ�������
*********************************************************************/
void LCD1602::Setpos(uint8_t row,uint8_t col)
{
	if(row==1) wcmd(col | 0x80);
	else wcmd(col | 0xC0);
}
/*********************************************************************
*��  �ܣ���ʾһ���ַ�	  
*��  �ڣ�ch������ʾ���ַ�
*********************************************************************/
void LCD1602::DispChar(char ch)
{
	wdat(ch);
}
/*********************************************************************
*��  �ܣ���ָ��λ����ʾһ���ַ�	  
*��  �ڣ�row:�� 1��2        col:�У�0~15
*        ch������ʾ���ַ�
*********************************************************************/
void LCD1602::Setpos_DispChar(uint8_t row,uint8_t col,char ch)
{
	Setpos(row,col);
	wdat(ch);
}
/*********************************************************************
*��    �ƣ�DispString()
*��    �ܣ�ʹLCD1602��ʾһ���ַ�������ʾλ������ǰ�趨
*��ڲ�����str[]:����ʾ���ַ���
*���ڲ�������
*********************************************************************/
void LCD1602::DispString(char str[])
{
    uint8_t i=0;
	while(str[i] != '\0')
	{
	   wdat(str[i]);
	   ++i;
	}
}
/*********************************************************************
*��    �ƣ�Setpos_DispString()
*��    �ܣ�ʹLCD1602��ָ��λ�ÿ�ʼ��ʾһ���ַ���
*��ڲ�����row:��ʾ�У�1��2
*          col:��ʾ�У�0~15 ��0~40�� 
*          str[]:����ʾ���ַ���
*���ڲ�������
*˵    ����ָ��λ������ʾ�ĳ�ʼλ�ã���һ���ַ���ʾ��λ��
*********************************************************************/
void LCD1602::Setpos_DispString(uint8_t row,uint8_t col,char str[])
{


	Setpos(row,col);
	DispString(str);
}
void LCD1602::Setpos_Dispnum(uint8_t row,uint8_t col,uint32_t num)
{
   Setpos(row,col);	
   Dispnum(num);
}
void LCD1602::Dispnum(uint32_t num)
{
	char s[3];
	sprintf(s,"%05d",num);
  DispString(s);//?????
}
/*********************************************************************
*��    �ƣ�DispDateTime()
*��    �ܣ���ʾ��������ʾ������ʱ��
*��ڲ�����year:��,����������ֵ
*          month:�£�1~12
*          day:�գ�1~31
*          hour:ʱ��1~24 ��  1~12
*          min:�֣�0~59
*          sec:�룬0~59
*���ڲ�������
*˵    ����ʹ��ǰ�����Ⱥ궨��USE_DISP_DATE���ڰ����ļ�֮ǰ������Ч
*��ʾ��ʽ��Date:yyyy-mm-dd  *
*          Time:hh:mm:ss    *
*********************************************************************/

void LCD1602::DispDateTime(uint32_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t min,uint8_t sec)
{
	char s[8];
	Setpos(1,0);
	DispString("Date:");
	
	sprintf(s,"%04d-%02d-%02d",year,month,day);
	DispString(s);
	

	Setpos(1,15);
	DispChar('*'); //��һ�н�������ʾ
	Setpos(2,0);
	DispString("Time:  ");

	sprintf(s,"%02d:%02d:%02d",hour,min,sec);
	DispString(s);

	Setpos(2,15);
	DispChar('*'); //�ڶ��н�������ʾ
}

void LCD1602::DispTimeHMS(uint8_t hour,uint8_t min,uint8_t sec)
{
	char s[8];

	Setpos(2,0);
	DispString("Time:  ");

	sprintf(s,"%02d:%02d:%02d",hour,min,sec);
	DispString(s);

	Setpos(2,15);
	DispChar('*'); //�ڶ��н�������ʾ
}
//��Ļ��������һ�����ڹ�����ʾ
void LCD1602::ShiftLeft()
{
	wcmd(0x18);
}
//��Ļ��������һ�����ڹ�����ʾ
void LCD1602::ShiftRight(void)
{
	wcmd(0x1C);
}
//�����������ʾ
void LCD1602::Clear(void)
{
	wcmd(0x01);
}
//ʹ��껹��ԭ��λ��
void LCD1602::Return(void)
{
	wcmd(0x02);
}
//����ʾ
void LCD1602::Close(void)
{
	wcmd(0x08);
}
//����ʾ
void LCD1602::Open(void)
{
	wcmd(0x0C);
}
/*********************************************************************
*��    �ƣ�FlickerChar()
*��    �ܣ�ʹָ��λ���ַ���˸������ʾ���
*��ڲ�����row:�� 1,2  
*          col:�� 0~15
*���ڲ�������
*˵    ����д����0x0D����ʾ������˸��д����0x0F����ʾ������˸
*          һ���趨��˸�󣬻����λ�ñ仯��˸������˸д����0x0C
*********************************************************************/
void LCD1602::Flicker(void)
{
	wcmd(0x0D);
}
void LCD1602::FlickerChar(uint8_t row,uint8_t col)
{
	wcmd(0x0D);
	Setpos(row,col);
}
//�ر��ַ���˸
void LCD1602::CloseFlicker(void)
{
	wcmd(0x0C);
}
//��Ļ����˸һ��
void LCD1602::FlickerScreen(void)
{
	wcmd(0x08);//����ʾ
	LCDdelay(500000);
	wcmd(0x0C);//����ʾ
	LCDdelay(500000);
}
void LCD1602::BackLight(u8 i)
{
	if(i == 1)
		digitalWrite(_LEDPin,1);
	else
		digitalWrite(_LEDPin,0);
}

/*********************************************************************
*��    �ƣ�Init()
*��    �ܣ���ʼ��LCD1602
*��ڲ�������
*���ڲ�������
*�������0x38:16*2��ʾ��5*&������ʾ�ַ���8λ����;ָ��ִ��ʱ��40us
*          0x0C:����ʾ���ع��	  40us
*          0x08:����ʾ���ع��	  40us
*          0x0D:�ַ���˸���ع�꣬�����趨λ�ã���˸����0.4ms����
*          0x0F:�ַ���˸�������
*          0x06:д�������Զ����ƹ�꣬��ͨ���Σ�����������ʾ  40us
*          0x04:д�������Զ����ƹ�꣬���Դ���������ʾ	 40us
*          0x01:�����ʾ���ݣ�������  1.64ms
*          0x02:ʹ��껹����ʼλ��	  1.64ms
*          0x18:��Ļ�������ַ�ͬʱ����һ���ʺϹ�����ʾ 40us
*          0x1C:��Ļ�������ַ�ͬʱ����һ���ʺϹ�����ʾ 40us
*********************************************************************/
void LCD1602::init(void)
{   
	wcmd(0x38);  //16*2��ʾ��5*7����8λ����
	LCDdelay(100);
	wcmd(0x38);  //16*2��ʾ��5*7����8λ����
	LCDdelay(100);
	wcmd(0x38);  //16*2��ʾ��5*7����8λ����
	LCDdelay(100);
	wcmd(0x08);  //�ȹ���ʾ������ʾ
	LCDdelay(100);
	wcmd(0x01);  //���LCD����ʾ����
	LCDdelay(100);
	wcmd(0x06);  //�Զ����ƹ��,0x04Ϊ���ƹ��
	LCDdelay(100);
	wcmd(0x0c);  //��ʾ�����ع��;0x08Ϊ����ʾ
	LCDdelay(100);
	BackLight(1);
}
void LCD1602::TEST(void)
{
	char str[]={"SHENTQLF!"};
	Setpos_DispString(1,3,str);
//	DispFreq(123456);
	FlickerChar(2,3);
	FlickerScreen();
	FlickerScreen();
	FlickerScreen();
	LCDdelay(100);
	CloseFlicker();	          
}
