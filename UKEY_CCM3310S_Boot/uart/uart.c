/*All rights reserved */
/*************************************************
  	  Copyright (C), 2010-2013, C*Core Technology Co., Ltd
  File name:uart.c                             // �ļ���
  Author:       Version:1.0     Date:          // ���ߡ��汾���������
  Description:                                 // ������ϸ˵���˳����ļ���ɵ���Ҫ����
  Others:                                      // �������ݵ�˵��
  History:                                     // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
                                               // �߼��޸����ݼ���  
    1. Date:         2013-03-13
       Author:       �쿭
       Modification: ���ա�����о�Ƽ����޹�˾��ƷӦ�ò�C���Ա�̹淶��V1.0��׼��
/*************************************************/
#define  UART_SYMBOL
#include "includes.h"


//**********************************************************************/
void UARTInit(unsigned long u32Fsys,unsigned long u32BaudRate,unsigned char u8FrameLen,unsigned char u8ParityEnable,unsigned char u8ParityType)
{
    
    //4800,9600,19200,38400,115200 are baudrate typical values
    
    rcSCIBD = (unsigned short)(u32Fsys/(16*u32BaudRate));
    
    rcSCICR1  = 0x00;
    rcSCICR1 |= u8FrameLen;   	
    rcSCICR1 |= u8ParityEnable;	      //parity enable
    rcSCICR1 |= u8ParityType;	      //odd parity
    rcSCICR2  = 0; //��ͨ 			  	
    //rcSCICR2 = SCICR2_RIE_MASK|SCICR2_TIE_MASK|SCICR2_TCIE_MASK|SCICR2_ILIE_MASK;	
          
}
///////////////////////////////////////////////////////////////////////


/**********************************************************************
*
* 
*
**********************************************************************/
void SendOneByte(unsigned char u8OutByte)
{	
    SCI_ENABLE_TRANSMITTER;
     	
    while((rcSCISR1 & SCISR1_TDRE_MASK)==0);	
    rcSCIDRL = u8OutByte;
    while((rcSCISR1 & SCISR1_TC_MASK)==0);
}
///////////////////////////////////////////////////////////////////////

