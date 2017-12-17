							/*All rights reserved */
/*********************************************************************
Copyright (C), 2010-2013, C*Core Technology Co., Ltd
File name: usbdev_isr.c
Author:        Version:        Date: 20121230
Description:  usb�豸�жϴ���
Others: 
History:                 
  1. Date:
      Author:
      Modification:
  2. ...
**********************************************************************/

#define	USBDEV_ISR_SYMBOL
#include "includes.h"


/*************************************************
Function: USBDev_ISR
Description: USB�жϴ���
Calls:  
Called By:  
Input: ��
Output: ��
Return: ��
Others: ��
*************************************************/
void USBDev_ISR(void)
{
	UINT8  ucISR, index;
	
	index = gUSBC_ComReg->EINDEX;	//���USB�ж�ģʽ�����⡣�ȱ���˵㣬ִ�����ָ��˵㡣
	
	gpram_msgflags |= gUSBC_ComReg->INTRUSB;                   //ȡ��USB�жϼĴ����е�ֵ

#ifdef USB_USEINT
	gpram_msgflags1 |= gUSBC_ComReg->INTRTX_L;                 //ȡ��USB�ķ����жϼĴ����е�ֵ
	ucISR = gUSBC_ComReg->INTRRX_L;                        //ȡ��USB�Ľ����жϼĴ����е�ֵ

	//EP1~EP7�����ж�
	g_uchUSBRcvStatus = (ucISR&0xFE); 
	
	//EP0�ж�
	if(gpram_msgflags1 & 0x1)
	{
		if (gpram_USBAddrFlag == 1)                        //�����ַ��д��
		{
			//���õ�ַ������SetupEnd�ж�?
			gUSBC_ComReg->FADDRR = gpram_USBNewAddr;   //���µ�ַ�Ž�ȥ���в���
			gpram_USBAddrFlag  = 0;
		}
		gpram_msgflags1 &= 0xFE;                           //��EP0�жϱ�־		
		USBDev_EP0_ISR();
	}
	
	//Clear Last Completed Command		
	if(gpram_bulkPhase == 0xFF)
	{
		gpram_msgflags1 &= (~(1<<INDEX_EP1));
	}
	//EP1~EP7�����ж�
	if (gpram_msgflags1 & (1<<INDEX_EP1) )  
	{
		gpram_msgflags1 &= ( ~(1<<INDEX_EP1) );   //��EP1�����жϱ�־
		g_uchUSBSendStatus |= (1<<INDEX_EP1);					//�������Ͷ����ʱ�������жϷ��ͽ���
	}
	
#endif

	//bus reset
	if (gpram_msgflags & 0x04)  
	{
		gpram_msgflags &= 0xFB; 		
		//SendOneByte(0x01);		                      
		USBDev_BusReset(0);	    
	}

	//suspend
	if(gpram_msgflags & 0x1)     
	{
		gpram_msgflags &= 0xFE;  		
		//SendOneByte(0x02);		                           
	}

	//sof
	/*
	if(((gpram_msgflags & 0x08) == 0x08) && (gpram_msgflags1 == 0 ))    //�����֡��ʼ�ҷ����ж϶�û����λ(���ǵ�SOF̫�������ˣ�����ȥ���˷�֧)
	{
	    gpram_msgflags &= 0xF7;  
		
		SendOneByte(0x03);
		
		if(gpram_bDozeSupport)
		{}
	}
	 */		
	gUSBC_ComReg->EINDEX = index; 	//�ָ��˵�
}

/*************************************************
Function: USBDev_EP0_ISR
Description: USB�˵�0���жϴ���
Calls:  
Called By:  
Input: ��
Output: ��
Return: ��
Others: ��
*************************************************/
void USBDev_EP0_ISR(void)
{
	//INT8U ucReqType=0,ucCount=0;
	UINT8 ucReg=0;

	gUSBC_ComReg->EINDEX  = CONTROL_EP;     
	ucReg= gUSBC_IdxReg->E0CSR_L;
	
	//clear SentStall bit
	if(ucReg &0x04)      //���SendStall������ɣ������־
	{
		ucReg &=0xFB;	
		//printf_debug((INT8U*)"SendStall_Finish!", 17, 1, 1);
	}

	if(ucReg & 0x10)     //	���setup end��������ô��setup end��־λ
	{
		ucReg |=0x80;
		//printf_debug((INT8U*)"SetupEnd", 8, 1, 1);
	}
  
	gUSBC_IdxReg->E0CSR_L  = ucReg;

	if(ucReg & DEV_CSR0_RXPKTRDY)    //������յ����ݰ�
	{
		USBDev_Ep0Handler();
	} 
}

/*
Description:Receive data sent from host.(BULK-OUT)
*/
void USBDev_EP1_RX_ISR(void)
{
	unsigned char ucRegLow;
	
	//access DATA_OUT_EP register map
	gUSBC_ComReg->EINDEX = INDEX_EP1;
	ucRegLow = gUSBC_IdxReg->RXCSR_L;
	//Clear sentstall and restart data toggle.
	if(ucRegLow & DEV_RXCSR_SENTSTALL)
	{
		ucRegLow &=0xBF;//clear SendStall bit
		ucRegLow |=DEV_RXCSR_CLR_DATA_TOG;//set ClrDataTog
		gUSBC_IdxReg->RXCSR_L = ucRegLow;
	}

	//Start receive data packet
	if(ucRegLow & DEV_RXCSR_RXPKTRDY)	 //RxPktRdy=1
	{
		USBDev_Data_BulkOut();
	}
	
}


/*
Description:Send data to host.(BULK-IN)
*/
void USBDev_EP1_TX_ISR(void)
{
	//access DATA_OUT_EP register map
	gUSBC_ComReg->EINDEX = INDEX_EP1;
	USBDev_Data_BulkIn();
}

