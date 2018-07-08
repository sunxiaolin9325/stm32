#include "lcd.h"
#include "cfont.h"    	 
#include "bitband.h"
				 
//��ʼ��LCD�Ļ�����ɫ�ͱ���ɫ	   
u16 BRUSH_COLOR=BLACK;	//������ɫ
u16 BACK_COLOR=WHITE;  //����ɫ 

//����LCD������Ҫ����
  u16  lcd_id;          //LCD ID
  u16  lcd_width;       //LCD�Ŀ��
  u16  lcd_height;      //LCD�ĸ߶�
  u16 write_gramcmd=0X2C;
	u16 read_gramcmd = 0x2E;
	u16 setxcmd=0X2A;
	u16 setycmd=0X2B;
	  
/****************************************************************************
* ��    ��: void LCD_WriteReg(u16 LCD_Reg, u16 LCD_Value)
* ��    �ܣ�LCDд�Ĵ���
* ��ڲ�����LCD_Reg: �Ĵ�����ַ
*           LCD_RegValue: Ҫд�������
* ���ز�������
* ˵    ����       
****************************************************************************/				   
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_Value)
{	
	LCD_CMD = LCD_Reg;		 //д��Ҫд�ļĴ������	 
	LCD_DATA = LCD_Value;  //��Ĵ���д�������	    		 
}
/****************************************************************************
* ��    ��: u16 LCD_ReadReg(u16 LCD_Reg)
* ��    �ܣ�LCD���Ĵ���
* ��ڲ�����LCD_Reg:�Ĵ�����ַ
* ���ز����������üĴ���������ֵ
* ˵    ����       
****************************************************************************/	
u16 LCD_ReadReg(u16 LCD_Reg)
{										   
	LCD_CMD=LCD_Reg;		//д��Ҫ���ļĴ������
	delay_us(5);		  
	return LCD_DATA;		//���ض�����ֵ
}   
//��ʼдGRAM
void LCD_WriteRAM_Prepare(void)
{
 	LCD_CMD=write_gramcmd;	  
}
//lcd��ʱ����
void lcdm_delay(u8 i)
{
	while(i--);
}
/****************************************************************************
* ��    ��: u16 LCD_BGRtoRGB(u16 bgr)
* ��    �ܣ�ͨ���ú�����GBRת��RGB
* ��ڲ�����bgr:GBR��ʽ����ɫֵ
* ���ز�����RGB��ʽ����ɫֵ
* ˵    ������ILI93xx����������ΪGBR��ʽ��������д���ʱ��ΪRGB��ʽ       
****************************************************************************/	
u16 LCD_BGRtoRGB(u16 bgr)
{
	u16  r,g,b,rgb;   
	b=(bgr>>0)&0x1f;
	g=(bgr>>5)&0x3f;
	r=(bgr>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
} 
/****************************************************************************
* ��    ��: u16 LCD_GetPoint(u16 x,u16 y)
* ��    �ܣ���ȡĳ�����ɫֵ	
* ��ڲ�����x��x����
            y��y����
* ���ز������˵����ɫ
* ˵    ����     
****************************************************************************/
u16 LCD_GetPoint(u16 x,u16 y)
{
 	vu16 r=0,g=0,b=0;

	if(x>=lcd_width||y>=lcd_height)
		return 0;	 //�����˷�Χ,ֱ�ӷ���		   
	LCD_SetCursor(x,y);	    
	LCD_CMD = read_gramcmd;   //9341 ���Ͷ�GRAMָ��	 				 
	if(LCD_DATA)
		r=0;						
	lcdm_delay(2);	  
 	r=LCD_DATA;  		  						 //ʵ��������ɫ
	
	lcdm_delay(2);	  
	b=LCD_DATA; 
	g=r&0XFF;		 //����9341��һ�ζ�ȡ����RG��ֵ,R��ǰ,G�ں�,��ռ8λ
	g<<=8;

	return (((r>>11)<<11)|((g>>10)<<5)|(b>>11)); //ILI9341��Ҫ��ʽת��һ��		  	
}	

//LCD������ʾ
void LCD_DisplayOn(void)
{					   
		   LCD_CMD=0x29;
}	

//LCD�ر���ʾ
void LCD_DisplayOff(void)
{	
		   LCD_CMD=0x28;
}   
/****************************************************************************
* ��    ��: void LCD_SetCursor(u16 Xaddr, u16 Yaddr)
* ��    �ܣ����ù��λ��
* ��ڲ�����x��x����
            y��y����
* ���ز�������
* ˵    ����     
****************************************************************************/
void LCD_SetCursor(u16 Xaddr, u16 Yaddr)
{	     
		LCD_CMD=setxcmd; 
		LCD_DATA=(Xaddr>>8); 
		LCD_DATA=(Xaddr&0XFF);	 
		LCD_CMD=setycmd; 
		LCD_DATA=(Yaddr>>8); 
		LCD_DATA=(Yaddr&0XFF);
} 
/****************************************************************************
* ��    ��: void LCD_AUTOScan_Dir(u8 dir)
* ��    �ܣ�����LCD���Զ�ɨ�跽��
* ��ڲ�����dir��ɨ�跽��
* ���ز�������
* ˵    ����     
****************************************************************************/  	   
void LCD_AUTOScan_Dir(u8 dir)
{
	u16 regval=0;
	u16 dirreg=0;
	//u16 temp; 

	switch(dir)
	{
		case L2R_U2D://������,���ϵ���
			regval|=(0<<7)|(0<<6)|(0<<5); 
			break;
		case L2R_D2U://������,���µ���
			regval|=(1<<7)|(0<<6)|(0<<5); 
			break;
		case R2L_U2D://���ҵ���,���ϵ���
			regval|=(0<<7)|(1<<6)|(0<<5); 
			break;
		case R2L_D2U://���ҵ���,���µ���
			regval|=(1<<7)|(1<<6)|(0<<5); 
			break;	 
		case U2D_L2R://���ϵ���,������
			regval|=(0<<7)|(0<<6)|(1<<5); 
			break;
		case U2D_R2L://���ϵ���,���ҵ���
			regval|=(0<<7)|(1<<6)|(1<<5); 
			break;
		case D2U_L2R://���µ���,������
			regval|=(1<<7)|(0<<6)|(1<<5); 
			break;
		case D2U_R2L://���µ���,���ҵ���
			regval|=(1<<7)|(1<<6)|(1<<5); 
			break;	 
	}

	//����ɨ�跽��
	dirreg=0X36;  
	regval|=0X08;		
	LCD_WriteReg(dirreg,regval);
	
	LCD_CMD=setxcmd; 
	//x����Сֵ
	LCD_DATA=0;
	LCD_DATA=0;
	//x�����ֵ
	LCD_DATA=(lcd_width-1)>>8;
	LCD_DATA=(lcd_width-1)&0XFF;
	LCD_CMD=setycmd; 
	//y����Сֵ
	LCD_DATA=0;
	LCD_DATA=0;
	//y�����ֵ
	LCD_DATA=(lcd_height-1)>>8;
	LCD_DATA=(lcd_height-1)&0XFF;  
}
/****************************************************************************
* ��    ��: void LCD_Display_Dir(u8 dir)
* ��    �ܣ�����LCD��ʾ����
* ��ڲ�����dir: 0,����
                 1,����
* ���ز�������
* ˵    �����B
****************************************************************************/
void LCD_Display_Dir(u8 dir)
{ 
	switch (dir) {
		case L2R_U2D:
		case L2R_D2U:
		case R2L_U2D:
		case R2L_D2U:
			//�����Һ����µ�������
			lcd_width=240;
			lcd_height=320;	
			break;
		default:
			//�����º����ҵĺ�����ʾ
			lcd_width=320;
			lcd_height=240;
			break;
	}
	
	LCD_AUTOScan_Dir(dir);	//����ɨ�跽��
}	
/****************************************************************************
* ��    ��: void LCD_DrawPoint(u16 x,u16 y)
* ��    �ܣ����㣨�ڸõ�д�뻭�ʵ���ɫ��
* ��ڲ�����x��x����
            y��y����
* ���ز�������
* ˵    ���BRUSH_COLOR:�˵����ɫֵ
****************************************************************************/
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);		    //���ù��λ�� 
	LCD_WriteRAM_Prepare();	  //��ʼд��GRAM
	LCD_DATA=BRUSH_COLOR; 
}
/****************************************************************************
* ��    ��: void LCD_Color_DrawPoint(u16 x,u16 y,u16 color)
* ��    �ܣ������õ����괦����Ӧ��ɫ���ڸõ�д���Զ�����ɫ��
* ��ڲ�����x��x����
            y��y����
            color �˵����ɫֵ
* ���ز�������
* ˵    ����color:д��˵����ɫֵ   UCGUI���øú���
****************************************************************************/
void LCD_Color_DrawPoint(u16 x,u16 y,u16 color)
{	       
		LCD_CMD=setxcmd; 
		LCD_DATA=(x>>8); 
		LCD_DATA=(x&0XFF);	 
		LCD_CMD=setycmd; 
		LCD_DATA=(y>>8); 
		LCD_DATA=(y&0XFF);
		
	  LCD_CMD=write_gramcmd; 
	  LCD_DATA=color; 
}	 
/****************************************************************************
* ��    ��: void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height)
* ��    �ܣ����ô���,������û������굽�������Ͻ�(sx,sy)
* ��ڲ�����sx,sy:������ʼ����(���Ͻ�)
            width,height:���ڿ�Ⱥ͸߶�
* ���ز�������
* ˵    ���������С:width*height.�B
****************************************************************************/
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height)
{   
	width=sx+width-1;
	height=sy+height-1;
  LCD_CMD=setxcmd; 
	LCD_DATA=(sx>>8); 
	LCD_DATA=(sx&0XFF);	 
	LCD_DATA=(width>>8); 
	LCD_DATA=(width&0XFF);  
	LCD_CMD=setycmd; 
	LCD_DATA=(sy>>8); 
	LCD_DATA=(sy&0XFF); 
	LCD_DATA=(height>>8); 
	LCD_DATA=(height&0XFF); 
} 
//����FSMC
void LCD_FSMC_Config()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
	FSMC_NORSRAMTimingInitTypeDef  writeTiming;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOG, ENABLE);//ʹ��PD,PE,PF,PGʱ��  
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);//ʹ��FSMCʱ��  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;        //PF10 �������,���Ʊ���
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;     //��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;      //����
  GPIO_Init(GPIOD, &GPIO_InitStructure);            //��ʼ�� //PF10 �������,���Ʊ���
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;      //�������
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;      //����
  GPIO_Init(GPIOD, &GPIO_InitStructure);            //��ʼ��  
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;      //�������
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;      //����
  GPIO_Init(GPIOE, &GPIO_InitStructure);            //��ʼ��  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;         //PG2,FSMC_A12
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;      //�������
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;      //����
  GPIO_Init(GPIOG, &GPIO_InitStructure);            //��ʼ��  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;        //PG12
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;      //�������
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;      //����
  GPIO_Init(GPIOG, &GPIO_InitStructure);            //��ʼ�� 

  GPIO_PinAFConfig(GPIOD,GPIO_PinSource0,GPIO_AF_FSMC);//PD0,AF12
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource1,GPIO_AF_FSMC);//PD1,AF12
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource4,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_FSMC); 
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_FSMC); 
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource10,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_FSMC);//PD15,AF12
 
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_FSMC); //PE7,AF12
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource15,GPIO_AF_FSMC);//PE15,AF12
 
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource2,GPIO_AF_FSMC);//PF12,AF12
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource12,GPIO_AF_FSMC);

  readWriteTiming.FSMC_AddressSetupTime = 0x10;	 //��ַ����ʱ�䣨ADDSET��Ϊ16��HCLK 1/168M=6ns*16=96ns	> 90
  readWriteTiming.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ�䣨ADDHLD��ģʽAδ�õ�	
  readWriteTiming.FSMC_DataSetupTime = 60;			 //���ݱ���ʱ��Ϊ60��HCLK	=6*60=360ns > 355
  readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
  readWriteTiming.FSMC_CLKDivision = 0x00;
  readWriteTiming.FSMC_DataLatency = 0x00;
  readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //ģʽA 
    
	writeTiming.FSMC_AddressSetupTime =3;	     //��ַ����ʱ�䣨ADDSET��Ϊ3��HCLK =18ns > 15
  writeTiming.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ�䣨ADDHLD) ģʽA��ʹ��	
  writeTiming.FSMC_DataSetupTime = 2;		     //���ݱ���ʱ��Ϊ6ns*3��HCLK=18ns > 15
  writeTiming.FSMC_BusTurnAroundDuration = 0x00;
  writeTiming.FSMC_CLKDivision = 0x00;
  writeTiming.FSMC_DataLatency = 0x00;
  writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //ģʽA 

  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;//   ʹ��NE4 ��Ҳ�Ͷ�ӦBTCR[6],[7]��
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // ���������ݵ�ַ
  FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM   
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//�洢�����ݿ��Ϊ16bit   
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable; 
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  �洢��дʹ��
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // ��дʹ�ò�ͬ��ʱ��
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; //��дʱ��
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;  //дʱ��
  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //��ʼ��FSMC����
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  // ʹ��BANK1 
				 
  delay_ms(50); // delay 50 ms 
}
//��ʼ��lcd
void LCD_Init(void)
{ 			
		LCD_FSMC_Config();
		
 		//����9341 ID�Ķ�ȡ		
		LCD_CMD=0XD3;				   
		lcd_id=LCD_DATA;	//dummy read 	
 		lcd_id=LCD_DATA; 	//����0X00
  	lcd_id=LCD_DATA;   	//��ȡ93								   
 		lcd_id<<=8;
		lcd_id|=LCD_DATA;  	//��ȡ41 
	
		LCD_CMD=0xCF;  
		LCD_DATA=0x00;
		LCD_DATA=0xC1; 
		LCD_DATA=0X30; 
		LCD_CMD=0xED;  
		LCD_DATA=0x64; 
		LCD_DATA=0x03; 
		LCD_DATA=0X12; 
		LCD_DATA=0X81; 
		LCD_CMD=0xE8;  
		LCD_DATA=0x85; 
		LCD_DATA=0x10; 
		LCD_DATA=0x7A; 
		LCD_CMD=0xCB;  
		LCD_DATA=0x39; 
		LCD_DATA=0x2C; 
		LCD_DATA=0x00; 
		LCD_DATA=0x34; 
		LCD_DATA=0x02; 
		LCD_CMD=0xF7;  
		LCD_DATA=0x20; 
		LCD_CMD=0xEA;  
		LCD_DATA=0x00; 
		LCD_DATA=0x00; 
		LCD_CMD=0xC0;    //Power control 
		LCD_DATA=0x1B;   //VRH[5:0] 
		LCD_CMD=0xC1;    //Power control 
		LCD_DATA=0x01;   //SAP[2:0];BT[3:0] 
		LCD_CMD=0xC5;    //VCM control 
		LCD_DATA=0x30; 	 //3F
		LCD_DATA=0x30; 	 //3C
		LCD_CMD=0xC7;    //VCM control2 
		LCD_DATA=0XB7; 
		LCD_CMD=0x36;    // Memory Access Control 
		LCD_DATA=0x48; 
		LCD_CMD=0x3A;   
		LCD_DATA=0x55; 
		LCD_CMD=0xB1;   
		LCD_DATA=0x00;   
		LCD_DATA=0x1A; 
		LCD_CMD=0xB6;    // Display Function Control 
		LCD_DATA=0x0A; 
		LCD_DATA=0xA2; 
		LCD_CMD=0xF2;    // 3Gamma Function Disable 
		LCD_DATA=0x00; 
		LCD_CMD=0x26;    //Gamma curve selected 
		LCD_DATA=0x01; 
		LCD_CMD=0xE0;    //Set Gamma 
		LCD_DATA=0x0F; 
		LCD_DATA=0x2A; 
		LCD_DATA=0x28; 
		LCD_DATA=0x08; 
		LCD_DATA=0x0E; 
		LCD_DATA=0x08; 
		LCD_DATA=0x54; 
		LCD_DATA=0XA9; 
		LCD_DATA=0x43; 
		LCD_DATA=0x0A; 
		LCD_DATA=0x0F; 
		LCD_DATA=0x00; 
		LCD_DATA=0x00; 
		LCD_DATA=0x00; 
		LCD_DATA=0x00; 		 
		LCD_CMD=0XE1;    //Set Gamma 
		LCD_DATA=0x00; 
		LCD_DATA=0x15; 
		LCD_DATA=0x17; 
		LCD_DATA=0x07; 
		LCD_DATA=0x11; 
		LCD_DATA=0x06; 
		LCD_DATA=0x2B; 
		LCD_DATA=0x56; 
		LCD_DATA=0x3C; 
		LCD_DATA=0x05; 
		LCD_DATA=0x10; 
		LCD_DATA=0x0F; 
		LCD_DATA=0x3F; 
		LCD_DATA=0x3F; 
		LCD_DATA=0x0F; 
		LCD_CMD=0x2B; 
		LCD_DATA=0x00;
		LCD_DATA=0x00;
		LCD_DATA=0x01;
		LCD_DATA=0x3f;
		LCD_CMD=0x2A; 
		LCD_DATA=0x00;
		LCD_DATA=0x00;
		LCD_DATA=0x00;
		LCD_DATA=0xef;	 
		LCD_CMD=0x11; //Exit Sleep
		delay_ms(120);
		LCD_CMD=0x29; //display on	
		
		//LCD_Display_Dir(L2R_U2D);		 //��ʼ��Ϊ����
		LCD_Display_Dir(U2D_R2L);
		LCD_BACK=1;			   //��������
		LCD_Clear(WHITE);
}
/****************************************************************************
* ��    ��: void LCD_Clear(u16 color)
* ��    �ܣ���������
* ��ڲ�����color: Ҫ���������ɫ
* ���ز�������
* ˵    �����B
****************************************************************************/
void LCD_Clear(u16 color)
{
	u32 i=0;      
	u32 pointnum=0;
	
	pointnum=lcd_width*lcd_height; 	 //�õ�LCD�ܵ���
	LCD_SetCursor(0x00,0x00);	       //���ù��λ�� 
	LCD_WriteRAM_Prepare();     		 //��ʼд��GRAM	 	  
	for(i=0;i<pointnum;i++)
	{
		LCD_DATA=color;	   
	}
} 
/****************************************************************************
* ��    ��: void LCD_Fill_onecolor(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
* ��    �ܣ���ָ����������䵥����ɫ
* ��ڲ�����(sx,sy),(ex,ey):�����ζԽ�����
            color:Ҫ������ɫ
* ���ز�������
* ˵    ���������СΪ:(ex-sx+1)*(ey-sy+1)  �B
****************************************************************************/
void LCD_Fill_onecolor(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	u16 i,j;
	u16 nlen=0;

		nlen=ex-sx+1;	 
		for(i=sy;i<=ey;i++)
		{
		 	LCD_SetCursor(sx,i);      				  //���ù��λ�� 
			LCD_WriteRAM_Prepare();     			  //��ʼд��GRAM	  
			for(j=0;j<nlen;j++)LCD_DATA=color;	//���ù��λ�� 	    
		}
} 
/****************************************************************************
* ��    ��: void LCD_Draw_Picture(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
* ��    �ܣ���ָ�������ڻ���ͼƬ
* ��ڲ�����(sx,sy),(ex,ey):�����ζԽ�����
            color:Ҫ����ͼƬ������ɫ����
* ���ز�������
* ˵    ���������СΪ:(ex-sx+1)*(ey-sy+1)  �B
****************************************************************************/
void LCD_Draw_Picture(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
	u16 height,width;
	u16 i,j;
	width=ex-sx+1; 			    //�õ�ͼƬ�Ŀ��
	height=ey-sy+1;			    //�õ�ͼƬ�ĸ߶�
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);   	//���ù��λ�� 
		LCD_WriteRAM_Prepare();     //��ʼд��GRAM
		for(j=0;j<width;j++)LCD_DATA=color[i*height+j];//д����ɫֵ
	}	  
}  
/****************************************************************************
* ��    ��: void LCD_Draw_Line(u16 x1, u16 y1, u16 x2, u16 y2)
* ��    �ܣ�����
* ��ڲ�����x1,y1:�������
            x2,y2:�յ�����
* ���ز�������
* ˵    ��������������Ļ��ߣ�ˮƽ�ߡ���ֱ����б��(���ߡ��������뻭Բ�ο����ϴ���)  �B
************************2****************************************************/  
void LCD_Draw_Line(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 i; 
	int xm1=0,ym2=0,model_x,model_y,model,  mcx,mcy,mRow,mCol;  

	model_x=x2-x1;                  //����ֱ�ߵ�ģ ������������ 
	model_y=y2-y1; 
	mRow=x1; 
	mCol=y1; 
	if(model_x>0)mcx=1;       
	else if(model_x==0)mcx=0;      //��ֱ�� 
	else {mcx=-1;model_x=-model_x;} 
	if(model_y>0)mcy=1; 
	else if(model_y==0)mcy=0;      //ˮƽ�� 
	else{mcy=-1;model_y=-model_y;} 
	if( model_x>model_y)model=model_x;  
	else model=model_y; 
	for(i=0;i<=model+1;i++ )       //������� 
	{  
		LCD_DrawPoint(mRow,mCol);     
		xm1+=model_x ; 
		ym2+=model_y ; 
		if(xm1>model) 
		{ 
			xm1-=model; 
			mRow+=mcx; 
		} 
		if(ym2>model) 
		{ 
			ym2-=model; 
			mCol+=mcy; 
		} 
	}  
}
/****************************************************************************
* ��    ��: void LCD_Draw_Rectangle(u16 x1, u16 y1, u16 x2, u16 y2)
* ��    �ܣ�������	  
* ��ڲ�����(x1,y1),(x2,y2):���εĶԽ�����
* ���ز�������
* ˵    ����  �B
****************************************************************************/
void LCD_Draw_Rectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_Draw_Line(x1,y1,x2,y1);
	LCD_Draw_Line(x1,y1,x1,y2);
	LCD_Draw_Line(x1,y2,x2,y2);
	LCD_Draw_Line(x2,y1,x2,y2);
}
/****************************************************************************
* ��    ��: void LCD_Draw_Circle(u16 x0,u16 y0,u8 r)
* ��    �ܣ���ָ��λ�û�һ��ָ����С��Բ
* ��ڲ�����(x,y):���ĵ�
            r    :�뾶
* ���ز�������
* ˵    ����  �B
****************************************************************************/
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             
 		LCD_DrawPoint(x0+b,y0-a);                    
		LCD_DrawPoint(x0+b,y0+a);                       
		LCD_DrawPoint(x0+a,y0+b);             
		LCD_DrawPoint(x0-a,y0+b);             
 		LCD_DrawPoint(x0-b,y0+a);             
		LCD_DrawPoint(x0-a,y0-b);                      
  	LCD_DrawPoint(x0-b,y0-a);               	         
		a++;	
		if(di<0)di +=4*a+6;	  //ʹ��Bresenham�㷨��Բ     
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 
/****************************************************************************
* ��    ��: void LCD_DisplayChar(u16 x,u16 y,u8 num,u8 size)
* ��    �ܣ���ָ��λ����ʾһ���ַ�
* ��ڲ�����x,y:��ʼ����
            word:Ҫ��ʾ���ַ�:abcdefg1234567890...
            size:�����С 12/16/24
* ���ز�������
* ˵    ����ȡ��ģ�ο�����ȡ��ģ��ʽ������ģȡģ����Ϊ�ȴ��ϵ��£��ٴ�����  �B
****************************************************************************/
void LCD_DisplayChar(u16 x,u16 y,u8 word,u8 size)
{  							  
  u8  bytenum,bytedata, a,b;
	u16 ymid=y;   			     
	 
	if(size==12) bytenum=12;        // �жϸ����������ֿ�������ռ���ֽ���
	else if(size==16) bytenum=16;
	else if(size==24) bytenum=36;
	else return;
	
	word=word-' ';  //�õ�ƫ�ƺ��ֵ ��Ϊ�ո�֮ǰ���ַ�û��font.h�е���������

	    for(b=0;b<bytenum;b++)
	    {   
					if(size==12)bytedata=char_1206[word][b]; 	 	  //����1206����
					else if(size==16)bytedata=char_1608[word][b];	//����1608����
					else if(size==24)bytedata=char_2412[word][b];	//����2412����
					else return;								                  //û�е��ַ����飬û�ַ��ֿ�
					for(a=0;a<8;a++)
					{			    
						if(bytedata&0x80)LCD_Color_DrawPoint(x,y,BRUSH_COLOR);
						else LCD_Color_DrawPoint(x,y,BACK_COLOR);
						bytedata<<=1;
						y++;
						if(y>=lcd_height)return;		//�������˳�����
						if((y-ymid)==size)
						{
							y=ymid;
							x++;
							if(x>=lcd_width)return;	  //�������˳�����
							break;
						}
		      }   	 
	    }       	 	  
}   
//m^n����
//����ֵ:m^n�η�.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 mid=1;	 
	while(n--)mid*=m;    
	return mid;
}
/****************************************************************************
* ��    ��: void LCD_DisplayNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
* ��    �ܣ���ָ��λ����ʾһ������
* ��ڲ�����x,y:�������
            num:��ֵ(0~999999999);	 
            len:����(��Ҫ��ʾ��λ��)
            size:�����С
            mode: 0����λΪ0����ʾ
                  1����λΪ0��ʾ0
* ���ز�������
* ˵    ����  �B
****************************************************************************/
void LCD_DisplayNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
{  
	u8 t,numtemp;
	u8 end0=0;						   
	for(t=0;t<len;t++)
	{
		numtemp=(num/LCD_Pow(10,len-t-1))%10;
		if(end0==0&&t<(len-1))
		{
			if(numtemp==0)
			{
				if(mode)LCD_DisplayChar(x+(size/2)*t,y,'0',size);  
				else LCD_DisplayChar(x+(size/2)*t,y,' ',size);  
 				continue;
			}else end0=1; 
		 	 
		}
	 	LCD_DisplayChar(x+(size/2)*t,y,numtemp+'0',size); 
	}
} 
/****************************************************************************
* ��    ��: void LCD_DisplayNum_color(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
* ��    �ܣ���ָ��λ����ʾһ���Զ�����ɫ������
* ��ڲ�����x,y:�������
            num:��ֵ(0~999999999);	 
            len:����(��Ҫ��ʾ��λ��)
            size:�����С
            mode: 0����λΪ0����ʾ
                  1����λΪ0��ʾ0
            brushcolor���Զ��廭����ɫ
            backcolor�� �Զ��屳����ɫ
* ���ز�������
* ˵    ����  �B
****************************************************************************/
void LCD_DisplayNum_color(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode,u16 brushcolor,u16 backcolor)
{
 u16 bh_color,bk_color;
	
 	bh_color=BRUSH_COLOR;  //�ݴ滭����ɫ
	bk_color=BACK_COLOR;   //�ݴ汳����ɫ
	
	BRUSH_COLOR=brushcolor;
	BACK_COLOR=backcolor;
	
	LCD_DisplayNum(x,y,num,len,size,mode);
	
	BRUSH_COLOR=bh_color;   //���ı�ϵͳ��ɫ
	BACK_COLOR=bk_color;
}
/****************************************************************************
* ��    ��: void LCD_DisplayString(u16 x,u16 y,u8 size,u8 *p)
* ��    �ܣ���ʾ�ַ���
* ��ڲ�����x,y:�������
*           size:�����С
*           *p:�ַ�����ʼ��ַ	
* ���ز�������
* ˵    ����  �B
****************************************************************************/	  
void LCD_DisplayString(u16 x,u16 y,u8 size,u8 *p)
{         
    while((*p<='~')&&(*p>=' ')) //�ж��ǲ��ǷǷ��ַ�!
    {       
        LCD_DisplayChar(x,y,*p,size);
        x+=size/2;
			  if(x>=lcd_width) break;
        p++;
    }  
}
/****************************************************************************
* ��    ��: void LCD_DisplayString(u16 x,u16 y,u8 size,u8 *p)
* ��    �ܣ���ʾ�Զ����ַ���
* ��ڲ�����x,y:�������
*           width,height:�����С  
*           size:�����С
*           *p:�ַ�����ʼ��ַ	
*           brushcolor���Զ��廭����ɫ
*           backcolor�� �Զ��屳����ɫ
* ���ز�������
* ˵    ����  �B
****************************************************************************/	  
void LCD_DisplayString_color(u16 x,u16 y,u8 size,u8 *p,u16 brushcolor,u16 backcolor)
{
   u16 bh_color,bk_color;
	
 	bh_color=BRUSH_COLOR;  //�ݴ滭����ɫ
	bk_color=BACK_COLOR;   //�ݴ汳����ɫ
	
	BRUSH_COLOR=brushcolor;
	BACK_COLOR=backcolor;
	
	LCD_DisplayString(x,y,size,p);
	
	BRUSH_COLOR=bh_color;   //���ı�ϵͳ��ɫ
	BACK_COLOR=bk_color;
}

void LCD_DisplayChinese(u16 x, u16 y, u8 index, u8 size)
{  							  
	u8 bytenum, bytedata, a, b;
	u16 ymid = y;   			     
	 
	bytenum = 128;
	for (b = 0; b < bytenum; b++) {   
		bytedata = sxl[index][b];
		for (a = 0; a < 8; a++) {			    
			if (bytedata & 0x80)
				LCD_Color_DrawPoint(x, y, BRUSH_COLOR);
			else
				LCD_Color_DrawPoint(x, y, BACK_COLOR);
			
			bytedata <<= 1;
			y++;
			if (y >= lcd_height)
				return;		//�������˳�����
			
			if ((y - ymid) == size) {
				y = ymid;
				x++;
				if ( x >= lcd_width)
					return;	  //�������˳�����
				break;
			}
		}
	}
}
	
void LCD_Printf(u8 *p)
{
	LCD_DisplayString(0, 210, 24, p);
}



