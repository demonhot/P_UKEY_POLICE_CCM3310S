/*All rights reserved */
/*************************************************
  	  Copyright (C), 2010-2013, C*Core Technology Co., Ltd
  File name:main.c                             // �ļ���
  Author:       Version:1.0     Date:          // ���ߡ��汾���������
  Description:                                 // ������ϸ˵���˳����ļ���ɵ���Ҫ����
  Others:                                      // �������ݵ�˵��
  History:                                     // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
                                               // �߼��޸����ݼ���  
    1. Date:         2013-03-13
       Author:       �쿭
       Modification: ���ա�����о�Ƽ����޹�˾��ƷӦ�ò�C���Ա�̹淶��V1.0��׼��
/*************************************************/
#define  MAIN_SYMBOL
#include "includes.h"





void main()
{
	UINT8 i;
    //unsigned char a_u8FileNameBuffer[]   = "main.c";
    //unsigned char a_u8LineNumberBuffer[] = "0000";
    //unsigned char a_u8ReportPass[] = "SysClk = 40MHz";
	unsigned char buffer[512];
	ModuleClk_On(MODULE_CLK_SCI|MODULE_CLK_USBC);	//��SCI��USBģ���ʱ��
	SetClk();

	//return boot
	//EnBootMode();

	//eflash clock
    set_clkd(g_sys_clk);
	
	//uart init
//	UARTInit(g_sys_clk*1000,9600,TEN,ParityDIS,EVE);


	//des enable
	*(volatile unsigned int *)(0x00c30004) &= ~0x300200;  //enable
	DES_Enable();

	//usb init	
	USBInit();
    
	//var init
	MID_GVAR_Init();

	//boot init		
	BOOT_INIT();
	
	Disable_Interrupts;
	NIER_reg=0x0;
	ICR_reg=0;
	PLSR25_reg=PRIORITY_LEVEL_4;
	NIER_reg |= 0x10;
	Enable_Interrupts;

	while(1)
	{
		HAL_USB_BulkOut(1,g_b_Comm,&g_BulkOutlen);
		if(0!=g_BulkOutlen)
		{
			ccid_data_process();					
		}	
	}
						
}			



void init_cos()
{
	unsigned char uchFlag = 0;
	unsigned int	AppCode = 0;
	
	
	uchFlag = (*(volatile unsigned char*)BOOTLOADER_FLAG_ADDR);
	AppCode = (*(volatile unsigned int*)APP_FLASH_ADDR);

	if((uchFlag == 0x55) && (AppCode != 0xffffffff))
	{
		asm
		{
			lrw r1, APP_START;
			jmp r1;	
		}	
	}
	else
	{
		asm
		{
			bsr   main
		}
	}	
}
	
