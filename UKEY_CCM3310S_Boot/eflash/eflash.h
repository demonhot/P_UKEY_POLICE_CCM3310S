/*All rights reserved */
/*************************************************
  	  Copyright (C), 2010-2013, C*Core Technology Co., Ltd
  File name:eflash.h                           // �ļ���
  Author:       Version:1.0     Date:          // ���ߡ��汾���������
  Description:                                 // ������ϸ˵���˳����ļ���ɵ���Ҫ����
  Others:                                      // �������ݵ�˵��
  History:                                     // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
                                               // �߼��޸����ݼ���  
    1. Date:         2013-03-13
       Author:       �쿭
       Modification: ���ա�����о�Ƽ����޹�˾��ƷӦ�ò�C���Ա�̹淶��V1.0��׼��
/*************************************************/
#ifdef  EFLASH_SYMBOL
#define	EFLASH_EXT
#else	
#define	EFLASH_EXT extern
#endif

EFLASH_EXT void set_clkd(UINT32 f_sys_khz);


extern unsigned char (*EFLASH_page_erase)(unsigned int);
extern void (*set_HighPowerMode)(unsigned char, unsigned char);
extern unsigned char (*EFLASH_program)(unsigned int, unsigned int);

