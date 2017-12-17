/*All rights reserved */
/*************************************************
  	  Copyright (C), 2010-2013, C*Core Technology Co., Ltd
  File name:clk.c                              // �ļ���
  Author:       Version:1.0     Date:          // ���ߡ��汾���������
  Description:                                 // ������ϸ˵���˳����ļ���ɵ���Ҫ����
  Others:                                      // �������ݵ�˵��
  History:                                     // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
                                               // �߼��޸����ݼ���  
    1. Date:         2013-03-13
       Author:       �쿭
       Modification: ���ա�����о�Ƽ����޹�˾��ƷӦ�ò�C���Ա�̹淶��V1.0��׼��
/*************************************************/
#define  CLK_SYMBOL
#include "includes.h"

UINT32 g_sys_clk = EFLASH_SYS_CLK_40M;

/*******************************************
*
* 
*
*******************************************/
void Delay(UINT32 times)
{
	while(times--);
}
/////////////////////////////////////////////



/*******************************************
*
* ����CCVR�Ĵ�����ֵ
*
*******************************************/
void UpdataCardReg()
{
	CLK_UCR |=0x00000002;
}
/////////////////////////////////////////////



/*******************************************
*
* ����GCVR�Ĵ�����ֵ 
*
******************************************/
void UpdataGlobalReg()
{
	CLK_UCR |=0x00000001;
}
/////////////////////////////////////////////



/*******************************************
*
* ����PCVR�Ĵ�����ֵ
*
******************************************/
void UpdataPLLReg()
{
	CLK_UCR |=0x00000004;
}


/*******************************************
*
* ѡ���ڲ�32Kʱ��Դ��
*
*******************************************/	
void InternalClk_32K()
{
	//internal clk 32k
	//1.set clock mode 1,�������� CLKMODE pin���� 1, ѡ���ڲ�ʱ��
	
	//2.set PLLBI_REG_32KSEL 1
	CLK_G1CR |=0x00080000;  					//ѡ��32k��Ϊϵͳ���ʱ��
	UpdataGlobalReg();							//����GCVR
	Delay(500);
	//3.set PLLBI_REG_CLKSEL 01
	CLK_CCVR =(CLK_CCVR&0xfffcffff)|0x00010000;  //ѡ���ڲ�32kʱ��Դ
	//4.set PLLBI_REG_CLKEXT 0
	CLK_CCVR &=0xfffbffff; 						//ѡ���ڲ�ʱ��Դ 
	UpdataCardReg();							//����CCVR
	Delay(500);
}

/*******************************************
*
* ѡ���ڲ�1Mʱ��Դ��
*
*******************************************/	
void InternalClk_1M()
{
	//internal clk 1M
	//1.set clock mode 1,�������� CLKMODE pin���� 1,ѡ���ڲ�ʱ��Դ
	
	//2.set PLLBI_REG_32KSEL 0
	CLK_G1CR &=0xfff7ffff;    						 //ѡ��1M��Ϊϵͳ���ʱ��  
	UpdataGlobalReg();                               //����GCVR
	//3.set PLLBI_REG_CLKSEL 01
	CLK_CCVR =(CLK_CCVR&0xfffcffff)|0x00010000;        // ѡ���ڲ�32K or 1Mʱ��Դ
	//4.set PLLBI_REG_CLKEXT 0
	CLK_CCVR &=0xfffbffff;                            // ѡ���ڲ�ʱ��Դ 
	UpdataCardReg();								  //����CCVR	
	Delay(500);
	
}


/*******************************************
*
* ѡ���ڲ�4Mʱ��Դ��
*
*******************************************/	
void InternalClk_4M()
{
	//internal clk 32M
	//1.set clock mode 1,�������� CLKMODE pin���� 1,ѡ���ڲ�ʱ��Դ
	
	//2.set PLLBI_REG_CLKSEL 00
	CLK_CCVR &=0xfffcffff;                  // clksel[1:0]=00  ѡ���ڲ�32M Divid ʱ��Դ
	//3.set PLLBI_REG_CLKEXT 0
	CLK_CCVR &=0xfffbffff;                  // ͨ������ѡ���ڲ�ʱ��Դ
	//div freq
	CLK_CCVR =(CLK_CCVR&0xffff0fff)|0x00003000; //���÷�Ƶֵ F = 32Mosc/2(PLLBI_REG_CLKDIV+1) �� F = 32/2*(3+1) = 4M
	UpdataCardReg();                        // ����CCVR
	
	Delay(500);

}


/*******************************************
*
* ѡ���ڲ�8Mʱ��Դ��
*
*******************************************/	
void InternalClk_8M()
{
	//internal clk 32M
	//1.set clock mode 1,�������� CLKMODE pin���� 1,ѡ���ڲ�ʱ��Դ
	
	//2.set PLLBI_REG_CLKSEL 00
	CLK_CCVR &=0xfffcffff;                  // clksel[1:0]=00  ѡ���ڲ�32M Divid ʱ��Դ
	//3.set PLLBI_REG_CLKEXT 0
	CLK_CCVR &=0xfffbffff;                  // ͨ������ѡ���ڲ�ʱ��Դ
	//div freq
	CLK_CCVR =(CLK_CCVR&0xffff0fff)|0x00001000; //���÷�Ƶֵ F = 32Mosc/2(PLLBI_REG_CLKDIV+1) �� F = 32/2*(1+1) = 8M
	UpdataCardReg();                        // ����CCVR
	
	Delay(500);

}


/*******************************************
*
* ѡ���ڲ�16Mʱ��Դ��
*
*******************************************/	
void InternalClk_16M()
{
	//internal clk 32M                       
	//1.set clock mode 1,�������� CLKMODE pin���� 1,ѡ���ڲ�ʱ��Դ
	
	//2.set PLLBI_REG_CLKSEL 00
	CLK_CCVR &=0xfffcffff;                  // clksel[1:0]=00  ѡ���ڲ�32M Divid ʱ��Դ
	//3.set PLLBI_REG_CLKEXT 0
	CLK_CCVR &=0xfffbffff;                  // ͨ������ѡ���ڲ�ʱ��Դ
	//div freq
	CLK_CCVR =(CLK_CCVR&0xffff0fff)|0x00000000; //���÷�Ƶֵ F = 32Mosc/2(PLLBI_REG_CLKDIV+1) �� F = 32/2*(0+1) = 16M
	UpdataCardReg();                        // ����CCVR
	
	Delay(500);

}


/*******************************************
*
* ѡ���ڲ�32Mʱ��Դ��
*
*******************************************/	
void InternalClk_32M()
{
	//internal clk 32M
	//1.set clock mode 1  �������� CLKMODE pin���� 1  ѡ���ڲ�32Mʱ��
	
	//2.set PLLBI_REG_CLKSEL 10
	CLK_CCVR =(CLK_CCVR&0xfffcffff)|0x00020000;	//clksel[1:0] =10b �����ڲ�ʱ��Ϊ32M
	//3.set PLLBI_REG_CLKEXT 0	
	CLK_CCVR &=0xfffbffff;						//ѡ���ڲ�ʱ��Դ
	UpdataCardReg();							//��CCVR���ݸ��µ�Clock module				
	Delay(500);
}


/*******************************************
*
* ѡ���ڲ�64Mʱ��Դ
*
*******************************************/	
void InternalClk_64M()
{
	//internal clk 32M
	//1.set clock mode 1  �������� CLKMODE pin���� 1  ѡ���ڲ�32Mʱ��
	
	//2.set PLLBI_REG_CLKSEL 11         
	CLK_CCVR =(CLK_CCVR&0xfffcffff)|0x00030000;  // clksel[1:0] =11b ѡ��32M*2 ʱ��Դ ����Ƶ F = 64M
	
	//3.set PLLBI_REG_CLKEXT 0
	CLK_CCVR &=0xfffbffff;                       // ѡ���ڲ�ʱ��Դ 
	UpdataCardReg();                             // ����CCVR
	
	Delay(500);

}
/*******************************************
*
*  ѡ���ⲿ12Mʱ��Դ
*
*******************************************/
void ExternalClk_12M()//xiang
{
	//Externaled clk	
	//set clock mode 0 �������� CLKMODE pin���� 0
	//1.set clock mode 0,�������� CLKMODE pin���� 0,ѡ���ⲿʱ��Դ
	
	//2.���PHY_BIT_DIR����λ����ʱ��ģ��Ĵ�������PLL
	CLEAR_PHY_BIT_DIR;
	
	//3.PLL en
	CLK_PCVR |=0x00200000;                        // PLLBI_RFG_PLLEN =1, pll enable     
	     
	//4.set PLLCLK_SEL 0,  PLL_DIV 9
	CLK_PCVR =(CLK_PCVR&0xffb0ffff)|0x00090000;   // PLLCLK_SEL = 0, select 240M clock, F = 240/2(9+1) = 12Mhz

	UpdataPLLReg();                               // ����PCVR
	Delay(500);
}

/*******************************************
*
*  ѡ���ⲿ20Mʱ��Դ
*
*******************************************/
void ExternalClk_20M()
{
	//1.set clock mode 0,�������� CLKMODE pin���� 0,ѡ���ⲿʱ��Դ
	
	//2.���PHY_BIT_DIR����λ����ʱ��ģ��Ĵ�������PLL
	CLEAR_PHY_BIT_DIR;
	
	//3.PLL en
	CLK_PCVR |=0x00200000;                        // PLLBI_RFG_PLLEN =1, pll enable     
	     
	//4.set PLLCLK_SEL 0,  PLL_DIV 5 
	CLK_PCVR =(CLK_PCVR&0xffb0ffff)|0x00050000;   // PLLCLK_SEL = 0, select 240M clock, F = 240/2(5+1) = 20Mhz

	UpdataPLLReg();                               // ����PCVR
	Delay(500);
}

/*******************************************
*
*  ѡ���ⲿ24Mʱ��Դ
*
*******************************************/
void ExternalClk_24M()
{
	//1.set clock mode 0,�������� CLKMODE pin���� 0,ѡ���ⲿʱ��Դ
	
	//2.���PHY_BIT_DIR����λ����ʱ��ģ��Ĵ�������PLL
	CLEAR_PHY_BIT_DIR;
	
	//3.PLL en
	CLK_PCVR |=0x00200000;                        // PLLBI_RFG_PLLEN =1, pll enable     
	     
	//4.set PLLCLK_SEL 0,  PLL_DIV 4
	CLK_PCVR =(CLK_PCVR&0xffb0ffff)|0x00040000;   // PLLCLK_SEL = 0, select 240M clock, F = 240/2(4+1) = 24Mhz

	UpdataPLLReg();                               // ����PCVR
	Delay(500);
}

/*******************************************
*
*  ѡ���ⲿ30Mʱ��Դ
*
*******************************************/
void ExternalClk_30M()
{
	//1.set clock mode 0,�������� CLKMODE pin���� 0,ѡ���ⲿʱ��Դ
	
	//2.���PHY_BIT_DIR����λ����ʱ��ģ��Ĵ�������PLL
	CLEAR_PHY_BIT_DIR;
	
	//3.PLL en
	CLK_PCVR |=0x00200000;                        // PLLBI_RFG_PLLEN =1, pll enable     
	     
	//4.set PLLCLK_SEL 0,  PLL_DIV 3 
	CLK_PCVR =(CLK_PCVR&0xffb0ffff)|0x00030000;   // PLLCLK_SEL = 0, select 240M clock, F = 240/2(3+1) = 30Mhz

	UpdataPLLReg();                               // ����PCVR
	Delay(500);
}

/*******************************************
*
*  ѡ���ⲿ40Mʱ��Դ
*
*******************************************/
void ExternalClk_40M()
{
	//1.set clock mode 0,�������� CLKMODE pin���� 0,ѡ���ⲿʱ��Դ
	
	//2.���PHY_BIT_DIR����λ����ʱ��ģ��Ĵ�������PLL
	CLEAR_PHY_BIT_DIR;
	
	//3.PLL en
	CLK_PCVR |=0x00200000;                        // PLLBI_RFG_PLLEN =1, pll enable     
	     
	//4.set PLLCLK_SEL 0,  PLL_DIV 2 
	CLK_PCVR =(CLK_PCVR&0xffb0ffff)|0x00020000;   // PLLCLK_SEL = 0, select 240M clock, F = 240/2(2+1) = 40Mhz

	UpdataPLLReg();                               // ����PCVR
	Delay(500);
}

/*******************************************
*
*  ѡ���ⲿ60Mʱ��Դ
*
*******************************************/
void ExternalClk_60M()
{
	//1.set clock mode 0,�������� CLKMODE pin���� 0,ѡ���ⲿʱ��Դ
	
	//2.���PHY_BIT_DIR����λ����ʱ��ģ��Ĵ�������PLL
	CLEAR_PHY_BIT_DIR;
	
	//3.PLL en
	CLK_PCVR |=0x00200000;                        // PLLBI_RFG_PLLEN =1, pll enable     
	     
	//4.set PLLCLK_SEL 0,  PLL_DIV 1 
	CLK_PCVR =(CLK_PCVR&0xffb0ffff)|0x00010000;   // PLLCLK_SEL = 0, select 240M clock, F = 240/2(1+1) = 60Mhz

	UpdataPLLReg();                               // ����PCVR
	Delay(500);
}



/*******************************************
*
*  ѡ���ⲿ80Mʱ��Դ
*
*******************************************/
void ExternalClk_80M()
{
//1.set clock mode 0,�������� CLKMODE pin���� 0,ѡ���ⲿʱ��Դ

//2.���PHY_BIT_DIR����λ����ʱ��ģ��Ĵ�������PLL
CLEAR_PHY_BIT_DIR;

//3.PLL en
CLK_PCVR |=0x00200000;                        // PLLBI_RFG_PLLEN =1, pll enable     
     
//4.set PLLCLK_SEL 0,  PLL_DIV 2 
CLK_PCVR =(CLK_PCVR&0xfff0ffff)|0x00420000;   // PLLCLK_SEL = 0, select 480M clock, F = 480/2(2+1) = 80Mhz

UpdataPLLReg();                               // ����PCVR
Delay(500);
}

/*******************************************
*
*  ��ĳ��ģ��ʱ��
*                                         
#define     MODULE_CLK_USI1               0x00000001	//Ĭ�ϴ�
#define     MODULE_CLK_SPI1               0x00000002	//Ĭ�ϴ�
#define     MODULE_CLK_USBC               0x00000004	//Ĭ�ϴ�
#define     MODULE_CLK_PIT1               0x00000008	//Ĭ�ϴ�
#define     MODULE_CLK_WDT                0x00000010	//Ĭ�ϴ�
#define     MODULE_CLK_EPORT              0x00000020	//Ĭ�ϴ�
#define     MODULE_CLK_CRYPTO             0x00000040	//Ĭ�Ϲر�
#define     MODULE_CLK_DES                0x00000080	//Ĭ�Ϲر�
#define     MODULE_CLK_SM1                0x00000100	//Ĭ�Ϲر�
#define     MODULE_CLK_TRNG               0x00000400	//Ĭ�Ϲر�
#define     MODULE_CLK_PIT2               0x00000800	//Ĭ�Ϲر�
#define     MODULE_CLK_SCI                0x00002000	//Ĭ�Ϲر�
#define     MODULE_CLK_USI2               0x00004000	//Ĭ�ϴ�
#define     MODULE_CLK_USI3               0x00008000	//Ĭ�ϴ�
#define     MODULE_CLK_I2C                0x00010000	//Ĭ�ϴ�
#define     MODULE_CLK_SSF33              0x00020000	//Ĭ�Ϲر�
#define     MODULE_CLK_TC                 0x00040000	//Ĭ�ϴ�
#define     MODULE_CLK_AES                0x00080000	//Ĭ�Ϲر�
#define     MODULE_CLK_SHA1               0x00100000	//Ĭ�Ϲر�
#define     MODULE_CLK_SPI2               0x01000000	//Ĭ�ϴ�
#define     MODULE_CLK_SMS4               0x08000000	//Ĭ�ϴ�

*******************************************/
void ModuleClk_On(UINT32 module)
{
	CLK_MSCR &= (~module);
}

/*******************************************
*
*  �ر�ĳ��ģ��ʱ��
*
*******************************************/
void ModuleClk_Off(UINT32 module)
{
	CLK_MSCR |= module;
}

/*******************************************
*
*  CLKOUT�������ϵͳʱ�ӵĿ��أ����ڲ��ԣ�Ĭ�ϴ�
*  u8Switch: 1�򿪣� 0�ر�
*******************************************/
void ClkoutPin_switch(UINT8 u8Switch)
{
	if (u8Switch == 1)
	{
		CLK_MSCR |= (1<<25);
	}
	else if (u8Switch == 0)
	{
		CLK_MSCR &= ~(1<<25);
	}
}

/////////////////////////////////////////////


/**********************************************************************
*
*  ϵͳʱ������
*
**********************************************************************/
void SetClk()
{
    if(PHYPA&CLKMODE)
    {
        //internal Ring-Oscillator
        InternalClk_32M();
        g_sys_clk = EFLASH_SYS_CLK_32M;
    }
    else
    {
        //external oscillator
        //Set PLL
        ExternalClk_40M();
        g_sys_clk = EFLASH_SYS_CLK_40M;
    }
}
///////////////////////////////////////////////////////////////////////