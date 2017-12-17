/*All rights reserved */
/*************************************************
  	  Copyright (C), 2010-2013, C*Core Technology Co., Ltd
  File name:eflash.c                           // �ļ���
  Author:       Version:1.0     Date:          // ���ߡ��汾���������
  Description:                                 // ������ϸ˵���˳����ļ���ɵ���Ҫ����
  Others:                                      // �������ݵ�˵��
  History:                                     // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
                                               // �߼��޸����ݼ���  
    1. Date:         2013-03-13
       Author:       �쿭
       Modification: ���ա�����о�Ƽ����޹�˾��ƷӦ�ò�C���Ա�̹淶��V1.0��׼��
/*************************************************/
#define  EFLASH_SYMBOL
#include "includes.h"

/**********************************************************************
*
*  ʱ������
*
**********************************************************************/
void set_clkd(UINT32 f_sys_khz)
{
	UINT8 ctmp = 0;
	
	if(f_sys_khz > 64000)
	{
		ctmp = (unsigned char)((f_sys_khz / 8) / 1000);
		ctmp |= EFLASH_CLKD_PRDIV8;
	}
	else
	{
		ctmp = (unsigned char)( f_sys_khz / 1000);
	}
	
	EFLASHCLKD = 0x4C;
	//write EFMTIM
	EFLASHTIM = 0x040805E8;
}
///////////////////////////////////////////////////////////////////////


/**********************************************************************
*
*  EFLASH ����ʾ��
*
**********************************************************************/
void EFLASHDemo()
{
	UINT32 i;
	UINT32 data;
	UINT32 addr = 0x00000000; //master mode,eflash baseAddr
	

	//Enter High Power Mode
	set_HighPowerMode(0,10);  

	for(i=0x00030000;i<CHIP_BYTE_SIZE;i+=PAGE_BYTE_SIZE)
	{
		EFLASH_page_erase(addr+i);
	}

	for(i=0x00030000;i<CHIP_BYTE_SIZE;i += 4)
	{
		EFLASH_program(addr+i,i*0x12345677);
	}

	for(i=0x00030000;i<CHIP_BYTE_SIZE;i += 4)
	{
		data = *(unsigned int *)(addr + i);
		if(data != i*0x12345677)
		{
			asm{bkpt} //���У�����
			break;
		}
	}

	//Exit High Power Mode
	set_HighPowerMode(1,10);  // 

}

