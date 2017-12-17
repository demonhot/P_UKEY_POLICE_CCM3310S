/*All rights reserved */
/*************************************************
  	  Copyright (C), 2010-2013, C*Core Technology Co., Ltd
  File name:debug.h                            // �ļ���
  Author:       Version:1.0     Date:          // ���ߡ��汾���������
  Description:                                 // ������ϸ˵���˳����ļ���ɵ���Ҫ����
  Others:                                      // �������ݵ�˵��
  History:                                     // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
                                               // �߼��޸����ݼ���  
    1. Date:         2013-03-13
       Author:       �쿭
       Modification: ���ա�����о�Ƽ����޹�˾��ƷӦ�ò�C���Ա�̹淶��V1.0��׼��
/*************************************************/
#ifdef  DEBUG_SYMBOL
#define	DEBUG_EXT
#else	
#define	DEBUG_EXT extern
#endif




#define    ASSERT_ENABLE       // ��ʹ�ö��Բ���
#define    NOP     asm {mov r0,r0}


DEBUG_EXT   unsigned char * p_u8CurrentFileName;
DEBUG_EXT 	unsigned char * p_u8CurrentLineNumber;

DEBUG_EXT 	void ExitProgram (void);
DEBUG_EXT 	void SendString(unsigned char * p_u8String);
DEBUG_EXT 	void AssertReport( unsigned char * p_u8FileName,unsigned char * p_u8LineNumber );
DEBUG_EXT    void UART_printf(char *fmt,...);
DEBUG_EXT    void HAL_UART_SEND_ASC(unsigned char* pInData,unsigned short len);




#ifdef ASSERT_ENABLE       // ��ʹ�ö��Բ���	
	#define  ASSERT_REPORT       AssertReport( p_u8CurrentFileName,p_u8CurrentLineNumber )
#else // ����ʹ�ö��Բ���
	#define ASSERT_REPORT        NOP
#endif /* end of ASSERT */

