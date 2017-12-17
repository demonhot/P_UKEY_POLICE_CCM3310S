/*All rights reserved */
/*************************************************
  	  Copyright (C), 2010-2013, C*Core Technology Co., Ltd
  File name:uart.h                             // �ļ���
  Author:       Version:1.0     Date:          // ���ߡ��汾���������
  Description:                                 // ������ϸ˵���˳����ļ���ɵ���Ҫ����
  Others:                                      // �������ݵ�˵��
  History:                                     // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
                                               // �߼��޸����ݼ���  
    1. Date:         2013-03-13
       Author:       �쿭
       Modification: ���ա�����о�Ƽ����޹�˾��ƷӦ�ò�C���Ա�̹淶��V1.0��׼��
/*************************************************/
#ifdef  UART_SYMBOL
#define	UART_EXT
#else	
#define	UART_EXT extern
#endif

UART_EXT void UARTInit(unsigned long u32Fsys,unsigned long u32BaudRate,unsigned char u8FrameLen,unsigned char u8ParityEnable,unsigned char u8ParityType);
UART_EXT void SendOneByte(unsigned char u8OutByte);



