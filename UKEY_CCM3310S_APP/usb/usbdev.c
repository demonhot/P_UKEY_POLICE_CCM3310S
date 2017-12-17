							/*All rights reserved */
/*********************************************************************
Copyright (C), 2010-2013, C*Core Technology Co., Ltd
File name: usb.c
Author:        Version:        Date: 20121230
Description:  usbģ�����
Others: 
History:                 
  1. Date:
      Author:
      Modification:
  2. ...
**********************************************************************/

#define  USB_SYMBOL
#include "includes.h"


void USB_Init(void)
{
	PHYPA   |=  USB_WORDSWITCH;                                  //����usb PHY
	gUSBC_fifoReg  = (sUSBCFIFO_Reg*)(USBC_BASE + 0x1A);
	gUSBC_ComReg   = (sUSBCCommonReg*)USBC_BASE;
	gUSBC_IdxReg   = (sUSBCIndexedReg*)(USBC_BASE + 0x10);
	
	gpram_bSuspendSupport    = 0;                               //����Suspend����
	gUSBC_ComReg->INTRUSBE   =   USB_INTERRUPT_RESET            //ʹ��USB�ж�
	                            |USB_INTERRUPT_CONNECT
	                            |USB_INTERRUPT_DISCON
	                            |USB_INTERRUPT_SUSPEND
	                            |USB_INTERRUPT_RESUME;

#ifdef USB_USEINT
	gUSBC_ComReg->INTRTXE_L   =  USB_INTERRUPT_EP0              //ʹ��EP0�жϺ�EP1�����ж�             
	                            |(1<<INDEX_EP1);
	gUSBC_ComReg->INTRRXE_L   =  (1<<INDEX_EP1);       //ʹ��EP1�����ж�                         
#else		//#ifdef USB_USEINT
	gUSBC_ComReg->INTRTXE_L = 0;
	gUSBC_ComReg->INTRRXE_L = 0;
#endif		//#ifdef USB_USEINT
	gUSBC_ComReg->EINDEX   =  CONTROL_EP;                       //��EP0��Ϊ��ǰҪ�����Ķ˵�	
	
	if(gpram_usbVer == 1)
		gUSBC_ComReg->UCSR   = USB_POWER_SOFT_CONN | USB_POWER_HS_ENAB;    //�������Ӻ͸���ģʽ
	else
		gUSBC_ComReg->UCSR  = USB_POWER_SOFT_CONN;
			
	gpram_USBAddrFlag   = 0;
	gpram_USBNewAddr    = 0;

	gpram_cbwPacket=(unsigned char*)gpram_cbwPacket_l;
	gpram_cswPacket[0]=0x55;
	gpram_cswPacket[1]=0x53;
	gpram_cswPacket[2]=0x42;
	gpram_cswPacket[3]=0x53;
}

void USB_DisConnect(void)
{
	gUSBC_ComReg->UCSR  &= ~USB_POWER_SOFT_CONN;
}

void USB_Connect(void)
{
	gUSBC_ComReg->UCSR  |= USB_POWER_SOFT_CONN;
}


void USBDev_Init(void)
{
	gpram_usbVer   = 1;		//����USB
	//gpram_usbVer   = 0;		//ȫ��USB
	USB_Init();
}


void USBDev_BusReset(UINT8 flag)
{
	UINT8 ucMode;
	UINT8 tempL,tempH;
	UINT32 i=0;
	UINT16 EPxBufAddr;
	
	if (flag == 0)
	{
		while (i!= 0x3FFFF)
		{
			i++;			
		}
		gUSBC_ComReg->FADDRR   = 0 ;    //��λ��usb�豸��ַΪ0
	}
#ifndef USB_USEINT			//��USB_Init���Ѿ����ж��ˣ�Ϊʲô�����ִ���?�������¹��ж�
	gUSBC_ComReg->INTRTXE_L = 0;
	gUSBC_ComReg->INTRRXE_L = 0;
#endif

	gUSBC_ComReg->EINDEX  = INDEX_EP1;   //��EP1��Ϊ��ǰҪ�����Ķ˵� 
	if( gpram_usbVer == 1 )
	{
		gUSBC_fifoReg->TX_fifosz   = 0x06;     //512 Bytes
		gUSBC_fifoReg->RX_fifosz   = 0x06;
	}
	else
	{
		gUSBC_fifoReg->TX_fifosz   = 0x03;     //64Bytes
		gUSBC_fifoReg->RX_fifosz   = 0x03;
	}

	gUSBC_fifoReg->TX_fifoadd_L    = ((USB_FIFO_OFFSET>>3)&0x00FF);	   
	gUSBC_fifoReg->TX_fifoadd_H    = ((USB_FIFO_OFFSET>>11)&0x00FF);	
	gUSBC_fifoReg->RX_fifoadd_L    = ((USB_FIFO_OFFSET>>3)&0x00FF);	
	gUSBC_fifoReg->RX_fifoadd_H    = ((USB_FIFO_OFFSET>>11)&0x00FF);

	if(gpram_usbVer  == 1)
	{
		tempL  = USB_MAX_PACKET_SIZE_LOW;       
		tempH  = USB_MAX_PACKET_SIZE_HIGH;      //512
	}
	else
	{
		tempL  = USB_MAX_PACKET_SIZE_LOW_V11;	//64				 
		tempH  = USB_MAX_PACKET_SIZE_HIGH_V11;
	}
	gUSBC_IdxReg->TXCSR_L   = 0x40;                //��DATA0��ʼ����  

	
	#ifdef  USB_AUTOSET_TEST
	gUSBC_IdxReg->TXCSR_H  = 0x80;                 //�Զ����ͣ����ŵ�FIFO�е�ֵ�Ͱ���Сһ��ʱ���Զ�������TxPktRdy,������	                                        
	#endif
	
	gUSBC_IdxReg->TXPSZR_L   = tempL;              //���Ͱ��Ĵ�С����
	gUSBC_IdxReg->TXPSZR_H   = tempH;
	
	gUSBC_IdxReg->RXCSR_L   = 0x80;                //��DATA0��ʼ����
	gUSBC_IdxReg->RXCSR_H   = 0x0;
	  
	gUSBC_IdxReg->RXPSZR_L   = tempL;              //���հ��Ĵ�С����
	gUSBC_IdxReg->RXPSZR_H   = tempH;
	  
	gUSBC_IdxReg->TXCSR_L  = DEV_TXCSR_FLUSH_FIFO;    //��FIFO
	gUSBC_IdxReg->RXCSR_L  = DEV_RXCSR_FLUSH_FIFO;

	gpram_bulkPhase = 0;	//Clear SCSI Command

}
	
	
