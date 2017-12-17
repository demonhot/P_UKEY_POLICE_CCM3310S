/*All rights reserved */
/*************************************************
  	  Copyright (C), 2010-2013, C*Core Technology Co., Ltd
  File name:debug.c                            // �ļ���
  Author:       Version:1.0     Date:          // ���ߡ��汾���������
  Description:                                 // ������ϸ˵���˳����ļ���ɵ���Ҫ����
  Others:                                      // �������ݵ�˵��
  History:                                     // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
                                               // �߼��޸����ݼ���  
    1. Date:         2013-03-13
       Author:       �쿭
       Modification: ���ա�����о�Ƽ����޹�˾��ƷӦ�ò�C���Ա�̹淶��V1.0��׼��
/*************************************************/
#define  DEBUG_SYMBOL
#include "includes.h"

void ExitProgram (void)
{    
	while(1) 
	{}
}

void SendString(unsigned char * p_u8String)
{
	unsigned char * p_u8PresentChar;
	p_u8PresentChar  = p_u8String;
	while ( *p_u8PresentChar != '\0')
	{
		 SendOneByte(*p_u8PresentChar);
		 p_u8PresentChar++;
	}
}




void AssertReport( unsigned char * p_u8FileName,unsigned char * p_u8LineNumber )
{
    unsigned char a_u8ReportString[] = "[ASSERT]Error Report:\n";
    SendString(a_u8ReportString);
    SendString(p_u8FileName);
    SendOneByte(',');
    SendString(p_u8LineNumber);	
    SendOneByte('!');
    SendOneByte('\n');
    ExitProgram();
}

void HAL_UART_SendData(unsigned char *data, unsigned short len)
{
	unsigned int i;
	unsigned short rv=0;
	
	for(i=0; i<len; i++)
	{		
		 SendOneByte(*(data+i));			
	}
	
}

void UART_printf(char *fmt,...)
{
//	unsigned char temp[256];
//	va_list arg_ptr;
////	unsigned char *string=pUARTData;
//	unsigned char *string=temp;	
//	va_start(arg_ptr, fmt);  
//	vsprintf((char *)string,fmt,arg_ptr); 
//    	HAL_UART_SendData(string,strlen((char *)string));
//	va_end(arg_ptr);
}


void HAL_UART_SEND_ASC(unsigned char* pInData,unsigned short len)
{	
	unsigned char temp[4];
	unsigned char m;
	unsigned short i,len1;
	
	len1=2*len;
	for(i=0;i<len;i++)
	{
		m=(pInData[i]>>4)&0x0f;
		if(m>=0&&m<=9)
			m+=0x30;
		else
			m+=55;
		temp[0]=m;
		m=pInData[i]&0x0f;
		if(m>=0&&m<=9)
			m+=0x30;
		else
			m+=55;
		temp[1]=m;
		temp[2]=' ';
		HAL_UART_SendData(temp,3);
	}
	temp[0]=0x0a;
	HAL_UART_SendData(temp,1);
}




