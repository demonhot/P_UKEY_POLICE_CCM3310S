/*All rights reserved */
/*************************************************
  	  Copyright (C), 2010-2013, C*Core Technology Co., Ltd
  File name:clk.h                              // �ļ���
  Author:       Version:1.0     Date:          // ���ߡ��汾���������
  Description:                                 // ������ϸ˵���˳����ļ���ɵ���Ҫ����
  Others:                                      // �������ݵ�˵��
  History:                                     // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
                                               // �߼��޸����ݼ���  
    1. Date:         2013-03-13
       Author:       �쿭
       Modification: ���ա�����о�Ƽ����޹�˾��ƷӦ�ò�C���Ա�̹淶��V1.0��׼��
/*************************************************/
#ifdef  CLK_SYMBOL
#define	CLK_EXT
#else	
#define	CLK_EXT extern
#endif

extern UINT32 g_sys_clk;

CLK_EXT void SetClk();
