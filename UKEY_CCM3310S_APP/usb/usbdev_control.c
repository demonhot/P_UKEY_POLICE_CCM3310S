							/*All rights reserved */
/*********************************************************************
Copyright (C), 2010-2013, C*Core Technology Co., Ltd
File name: usbdev_control.c
Author:        Version:        Date: 20121230
Description:  usb�豸ö�ٹ��̵�ʵ��
Others: 
History:                 
  1. Date:
      Author:
      Modification:
  2. ...
**********************************************************************/

#define  USBDEV_CONTROL_SYMBOL	
#include "includes.h"

/*************************************************
Function: read_ep_buf
Description: ��FIFO�ж�����
Calls:  
Called By:  
Input: 
    INT8U* out_buf:�������ݵ���ʼ��ַ
    INT32U ep:FIFO��ַ
    INT16U uiLens:���������ݳ���
Output: ��
Return: ��
Others: ��
*************************************************/
void read_ep_buf(UINT8 *out_buf,UINT32 ep,UINT16 uiLens)
{
	UINT16 i=0;
	for (i=0;i<uiLens;i++)
	{
		out_buf[i] = read_port_byte(ep);
	}
}

/*************************************************
Function: write_ep_buf
Description: ��FIFO��д����
Calls:  
Called By:  
Input: 
    INT32U ep:FIFO��ַ
    INT8U* out_buf:Ҫд�����ݵ���ʼ��ַ
    INT16U uiLens:д������ݳ���
Output: ��
Return: ��
Others: ��
*************************************************/
void write_ep_buf(UINT32 ep,UINT8 *out_buf,UINT16 uiLens)
{
	UINT16 i;
	for (i=0;i<uiLens;i++)
	{
		write_port_byte(ep,out_buf[i]);
	}
}

/*************************************************
Function: write_ep0_buf
Description: ͨ���˵�0�������ݣ����һ��
Calls:  
Called By:  
Input: 
    INT8U* in_buf:Ҫ���͵����ݵ���ʼ��ַ
    INT16U uiLens:Ҫ���͵����ݳ���
Output: ��
Return: ��
Others: ��
*************************************************/
void write_ep0_buf(UINT8 *in_buf, UINT16 uiLen)
{
	UINT8  ucReg  = gUSBC_IdxReg->E0CSR_L;
	write_ep_buf(USB_FIFO_ENDPOINT_0,in_buf,uiLen);	    
	ucReg  |=  DEV_CSR0_DATAEND | DEV_CSR0_TXPKTRDY;    // buffer�����󣬸���USB��ʼ����
	gUSBC_IdxReg->E0CSR_L  =  ucReg;	
}

/*************************************************
Function: write_ep0_buf_notEnd
Description: ͨ���˵�0�������ݣ��������һ��
Calls:  
Called By:  
Input: 
    INT8U* in_buf:Ҫ���͵����ݵ���ʼ��ַ
    INT16U uiLens:Ҫ���͵����ݳ���
Output: ��
Return: ��
Others: ��
*************************************************/
void write_ep0_buf_notEnd(UINT8 *in_buf, UINT16 uiLen)
{
	UINT8  ucReg  = gUSBC_IdxReg->E0CSR_L;
	write_ep_buf(USB_FIFO_ENDPOINT_0,in_buf,uiLen);	    
	ucReg  |=  DEV_CSR0_TXPKTRDY;    // buffer�����󣬸���USB��ʼ���ͣ��������һ��
	gUSBC_IdxReg->E0CSR_L  =  ucReg;	
}

void write_ep0_buf_long(UINT16 requstLen, UINT8 *databuf, UINT16 dataLen)
{
	UINT16 sentLen = 0;

	if (dataLen > USB_MAX_PACKET_SIZE_EP0)		//��������С����һ������С����ְ�����
	{
		if (requstLen > USB_MAX_PACKET_SIZE_EP0)
		{
			//TODO:ʹ���жϷ�ʽʱCDC�෵������������������
#ifdef USB_USEINT		
			gUSBC_ComReg->INTRTXE_L &=0xfe;		//�ر�EP0�ж�
#endif
			while ((requstLen-sentLen)>USB_MAX_PACKET_SIZE_EP0)
			{
				write_ep0_buf_notEnd(databuf+sentLen,USB_MAX_PACKET_SIZE_EP0);
				while((gUSBC_ComReg->INTRTX_L &0x01)==0x00);
				sentLen += USB_MAX_PACKET_SIZE_EP0;
			}
			write_ep0_buf(databuf+sentLen,requstLen-sentLen);
#ifdef USB_USEINT			
			gUSBC_ComReg->INTRTXE_L |=0x01;		//��EP0�ж�
#endif
		}
		else
		{
			write_ep0_buf(databuf,requstLen);
		}
	}
	else	//һ���Ϳɷ������
	{
		requstLen = (requstLen>dataLen)?dataLen:requstLen;		//host���ܻᷢ�ͳ����ֽ�ΪFF������
		write_ep0_buf(databuf,requstLen);
	}
}
			
void  USBDev_EP0_SendStall(void)
{
	UINT8  ucReg = gUSBC_IdxReg->E0CSR_L;
	ucReg |= DEV_CSR0_SENDSTALL;
	gUSBC_IdxReg->E0CSR_L   = ucReg;
}


/*************************************************
Function: USBDev_Ep0Handler
Description: �˵�0�����ݴ���
Calls:  
Called By:  
Input: ��
Output: ��
Return: ��
Others: ��
*************************************************/
void USBDev_Ep0Handler(void)
{
	UINT8  ucReq  = 0;
	UINT16   wValue = 0;

#ifdef EP0_SETGET_REPORT
	if (setreport_flag == 1)		//���յ�report����setup��������out��
	{
		read_ep_buf((unsigned char*)report_buffer,USB_FIFO_ENDPOINT_0,64);
		USBDev_ClearRx0();		//оƬ�Զ�����״̬���̰�
		setreport_flag = 0;		
		return;
	}
#endif	

#ifdef USB_CDC_CLASS
	if(gUSBC_IdxReg->E0COUNTR_L != 8)		//���ô������ԣ���setup��������out��
	{
		read_ep_buf(LineCoding,USB_FIFO_ENDPOINT_0,7);
		USBDev_ClearRx0();	//|DEV_CSR0_DATAEND;		//оƬ�Զ�����״̬���̰�
		return;
	}
#endif	
	
	read_ep_buf((UINT8*)&dev_std_req,USB_FIFO_ENDPOINT_0,8);		//TODO:EP0�Ľ���FIFO��СĬ��Ϊ8�ֽ�
	USBDev_ClearRx0();
	//printf_debug((INT8U*)"EP0:", 4, 1, 0);		//TODO:���ܼ�̫���ӡ������ͨ�������⣬Ҫ�󷵻���Ӧʱ�䲻��̫��?
	//printf_debug((INT8U*)&dev_std_req, 8, 0, 1);
	
	ucReq  = dev_std_req.bRequest;
	
	if(1 == ((dev_std_req.bmRequestType>>5) & 0x3))   //������
	{
		switch(ucReq)
		{
			case 0xFE://0xa1:GetMaxLUN
			{
				ucReq=gpram_maxLun & 0x7F;
				if(ucReq>4)
					ucReq=0;
				write_ep0_buf(&ucReq,1);	
				USBDev_BusReset(1);		
				break;	
			}
			case 0xFF://0x21:ClassReset
			{
				USBDev_BusReset(1);
				break;
			}
			default:
			{
				USBDev_EP0_SendStall();
				break;	
			}
		}
	}
	else		//��׼����
	{
		switch(ucReq)
		{
			case 0://GET_STATUS
			{
				USBDev_GetStatus();
				break;
			}
			case 1://CLEAR_FEATURE
			{				
				if(dev_std_req.bmRequestType == 0x2)//clear endpoint_halt, dev_std_req->bmRequestType			
					gpram_WakeupFeature = 0;
				break;
			}
			case 3://SET_FEATURE
			{
				if(dev_std_req.bmRequestType == 0x2)//halt endpoint,
					gpram_WakeupFeature = 1;
				if(dev_std_req.bmRequestType == 0x0)	//test mode
					USBDev_TestMode();
				break;
			}
			case 5:                               //USBЭ�飬set address
			{	
				ucReq = dev_std_req.wValue>>8;
				gpram_USBNewAddr  =  ucReq;
				gpram_USBAddrFlag = 1;
				write_ep0_buf(&ucReq,0);		//ֻ�����˼Ĵ�����־���˾䲻ҪҲ��
				break;	
			}
			case 6://GET_DESCRIPTOR
			{
				USBDev_GetDescriptor();
				break;
			}	
			case 8://GET_CONFIGURATION
			{				
				ucReq=gpram_configFlag;	
				write_ep0_buf(&ucReq,1);			
				break;
			}
			case 9://SET_CONFIGURATION
			{				
				gpram_configFlag = dev_std_req.wValue & 0x0F;	
				write_ep0_buf(&ucReq,0);		//ֻ�����˼Ĵ�����־���˾䲻ҪҲ��
				break;
			}
			case 10://GET_INTERFACE
			{				
				ucReq = 0;		//Return Alternate Setting value		
				write_ep0_buf(&ucReq,1);	//set 1 data packet		
				break;	
			}
			case 11://SET_INTERFACE
			{
				write_ep0_buf(&ucReq,0);		//ֻ�����˼Ĵ�����־���˾䲻ҪҲ��
				break;	
			}
			default:			
			{
				USBDev_EP0_SendStall();
				break;
			}
		}
	}
}


/*************************************************
Function: USBDev_GetDescriptor
Description: USB�������Ĵ���
Calls:  
Called By:  
Input: ��
Output: ��
Return: ��
Others: ��
*************************************************/
void USBDev_GetDescriptor(void)
{
	UINT16 ucRegLen;
	UINT8 ucReg=((UINT8*)&dev_std_req)[6];//dev_std_req->wCount>>8;
	UINT8 ucReg2=((UINT8*)&dev_std_req)[7];//dev_std_req->wCount
	UINT8 reqIndex=((UINT8*)&dev_std_req)[3]; //=dev_std_req->wValue & 0x0F;
	UINT8 ucIndex=((UINT8*)&dev_std_req)[2];//=dev_std_req->wValue>>8;
	UINT16 i;
	UINT16 Configuration_Descriptor_Len;

	unsigned char ucReg_h;
	ucReg_h = dev_std_req.wCount &0xff;
	
	ucRegLen = ((UINT16)ucReg2<<8)+ucReg;
	Configuration_Descriptor_Len = (USB_Configuration_Descriptor[3]<<8)+USB_Configuration_Descriptor[2];

	switch(reqIndex)
	{
		case DEVICE_TYPE:		 //�豸������
		{
			if(ucReg<=USB_Device_Descriptor[0])
			{
				write_ep0_buf((UINT8 *)USB_Device_Descriptor,ucReg);
			}
			else
			{
				write_ep0_buf((UINT8 *)USB_Device_Descriptor,8);
			}
			break;
		}
		
		case CONFIG_TYPE:		 //����������
		{
			write_ep0_buf_long(ucRegLen, (UINT8*)USB_Configuration_Descriptor, Configuration_Descriptor_Len);
			break;
		}

		case STRING_TYPE:		 //�ַ���������
		{
			if(ucIndex==0)
			{
				write_ep0_buf((UINT8*)UsbLanguageID,4);
			}
			else if(ucIndex==2)
			{
				ucReg = (ucReg>srialnumString[0])?srialnumString[0]:ucReg;
				write_ep0_buf((UINT8*)srialnumString,ucReg);
			}
			else if(ucIndex==3)
			{
				ucReg = (ucReg>USB_String_Descriptor[0])?USB_String_Descriptor[0]:ucReg;
				write_ep0_buf((UINT8*)USB_String_Descriptor,ucReg);
			}
			else if(ucIndex==4)
			{	//serial code
				ucReg = (ucReg>USB_Manufacturer_Descriptor[0])?USB_Manufacturer_Descriptor[0]:ucReg;
				write_ep0_buf((UINT8*)USB_Manufacturer_Descriptor,ucReg);
			}

			break;
		}
		case INTERFACE_TYPE:                    //�ӿ�������
		{
			USBDev_EP0_SendStall();
			break;
		}
		case ENDPOINT_TYPE:                    //�˵�������
		{
			USBDev_EP0_SendStall();
			break;
		}
		case  DEVICE_QUALIFIER:
		{
			if(ucReg<=USB_Device_Qualifier_Descriptor[0])
			{
				write_ep0_buf((UINT8*)USB_Device_Qualifier_Descriptor,ucReg);
			}
			else
			{
				write_ep0_buf((UINT8*)USB_Device_Qualifier_Descriptor,8);		//��������豸��������ʵ�ʳ��ȣ���ֻ����8�ֽ�
			}
			break;
		}
		
		case OTHER_SPEED:
		{
			if(ucReg<10)
			{
				write_ep0_buf((UINT8*)USB_Configuration_Descriptor,ucReg);
			}
			else
			{/*
				USBDEV_Configuration_Descriptor[22] = 0x40;
				USBDEV_Configuration_Descriptor[23] = 0x00;
				USBDEV_Configuration_Descriptor[29] = 0x40;
				USBDEV_Configuration_Descriptor[30] = 0x00;
				write_ep0_buf(USBDEV_Configuration_Descriptor,0x20);*/
			}
			break;
		}
		case INTERFACE_POWER:
		{
			USBDev_EP0_SendStall();
			break;
		}
#ifdef USB_HID_CLASS
		case REPORT_TYPE:                 //HIDЭ����涨�ı���������
		{
#ifdef HID_512			
			ucReg = 0x36;
#else
			ucReg = 0x34;
#endif
			write_ep0_buf((unsigned char*)USB_report_Descriptor,ucReg);
			break;
		}
#endif
		default:
		{
			USBDev_EP0_SendStall();
			break;	
		}
	}
}

void USBDev_ClearRx0()
{
	//set ServiceRxPktRdy to clear RxPktRdy
	write_port_byte(0x01000012,DEV_CSR0_SERVICE_RXPKTRDY);
}

/*
	Description:USB standard command GetStatus
*/
void USBDev_GetStatus(void)
{
	UINT8 txdat[2]={0,0};

	switch(dev_std_req.bmRequestType)//dev_std_req->bmRequestType
	{
		case REQUEST_ENDPOINT_STATUS:
			if(gpram_WakeupFeature == 1)
			txdat[0]=1;	
		case REQUEST_DEVICE_STATUS:
		case REQUEST_INTERFACE_STATUS:
			write_ep0_buf(txdat,2);
			break;
		default:
			USBDev_EP0_SendStall();
			break;
	}	
}
	
	
/*
	Description:support usb high speed test mode
*/
void USBDev_TestMode(void)
{
	UINT8 test_packet[53] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 
		0xAA, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 
		0xEE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xBF, 0xDF, 
		0xEF, 0xF7, 0xFB, 0xFD, 0xFC, 0x7E, 0xBF, 0xDF, 
		0xEF, 0xF7, 0xFB, 0xFD, 0x7E};
	
	switch(((UINT8 *)&dev_std_req)[5])
	{
		case 1:		//Test_J
			write_port_byte( 0x0100000f,0x02);
			//asm{wait}
			break;
		case 2:		//Test_K
			write_port_byte( 0x0100000f,0x04);
			//asm{wait}
			break;
		case 3:		//Test_SE0_NAK
			write_port_byte( 0x0100000f,0x01);
			//asm{wait}
			break;
		case 4:		//Test_Packet
			write_ep_buf(USB_FIFO_ENDPOINT_0,test_packet,53);
			write_port_byte(0x01000012,0x02);
			write_port_byte(0x0100000f,0x08);
			//asm{wait}
			break;
		default:
			break;			
	}
}
	
	

/*************************************************
Function: usb_receive
Description: USB�˵�1����һ������
Calls:  
Called By:  
Input: 
    INT8U *buf:�������ݵ���ʼ��ַ
Output: ��
Return: �������ݵĳ���
Others: ��
*************************************************/
UINT16 usb_receive(UINT8 *buf)
{
	UINT16   uiRxCount;
	UINT8    ucReg;
	uiRxCount   =  gUSBC_IdxReg->RXCount_H;
	uiRxCount   <<= 8;
	uiRxCount += gUSBC_IdxReg->RXCount_L;
	
	if(uiRxCount == 0)
		return;
	
	if(uiRxCount > 0)		//һ������ӦС��512�ֽ�
	{
		read_ep_buf((UINT8 *)buf,g_uchUSBFifoAddr,uiRxCount);
		ucReg  = gUSBC_IdxReg->RXCSR_L;
		ucReg  &= 0xFE;                 //���FIFO��������һ��������
		gUSBC_IdxReg->RXCSR_L  = ucReg;
	}
	return  uiRxCount;
}

/*************************************************
Function: usb_send
Description: USB�˵�1����һ������
Calls:  
Called By:  
Input: 
    INT8U *buf:�������ݵ���ʼ��ַ
    INT16U len:�������ݵĳ���
Output: ��
Return: ��
Others: ��
*************************************************/
void usb_send(UINT8 *buf, UINT16 len)
{
	UINT8  ucReg;

	write_ep_buf(g_uchUSBFifoAddr,buf,len);
	usb_SetTxSize(len);                                      //���͵�ʱ����Ĵ�СҪ��һ��

	//set TxPktRdy=1
	//if(uiLength!=0x40)	//when length = 0x40,TxpktRdy autoset,  TODO:û�д��Զ����ù��ܣ��Ҳ����ǹ̶���0x40
	{
		ucReg =	read_port_byte( 0x01000012);
		ucReg |= DEV_TXCSR_TXPKTRDY;
		write_port_byte( 0x01000012,ucReg);
#ifndef USB_USEINT		
		while((read_port_byte( 0x01000012)&DEV_TXCSR_TXPKTRDY) == DEV_TXCSR_TXPKTRDY);
#endif		
	}
}

void usb_SetTxSize(UINT16 Size)
{
	if(Size)
	{
		gUSBC_IdxReg->TXPSZR_L  = Size;
		gUSBC_IdxReg->TXPSZR_H  = (Size>>8);
	}
	else
	{
		if(gpram_usbVer  == 1)
		{
			gUSBC_IdxReg->TXPSZR_L  = USB_MAX_PACKET_SIZE_LOW;
			gUSBC_IdxReg->TXPSZR_H  = USB_MAX_PACKET_SIZE_HIGH;
		}
		else
		{
			gUSBC_IdxReg->TXPSZR_L  = USB_MAX_PACKET_SIZE_LOW_V11;
			gUSBC_IdxReg->TXPSZR_H  = USB_MAX_PACKET_SIZE_HIGH_V11;
		}
	}
}


void usb_ClearRxFlag(UINT8 EPindex)
{
    unsigned char ucReg = 0;

	gUSBC_ComReg->EINDEX = EPindex;
	//clear Rx
	ucReg = gUSBC_IdxReg->RXCSR_L;
	ucReg &=0xFE;	//Clear RxPktRdy
	gUSBC_IdxReg->RXCSR_L = ucReg;	
}

void usb_SetTxFlag(UINT8 EPindex)
{
	unsigned char ucReg = 0;
	
	gUSBC_ComReg->EINDEX = EPindex;
	
	ucReg = gUSBC_IdxReg->TXCSR_L;
	ucReg |= 0x01;	//Set TxPktRdy
	gUSBC_IdxReg->TXCSR_L = ucReg;	
}

//USBЭ���Ӧ��BUF��ͬ
void usb_SetFifoAddr(UINT8 EPindex, unsigned long fifoAddr, unsigned char flag)
{
	if(flag == 0)  //read
	{
		gUSBC_ComReg->EINDEX = EPindex;
		gUSBC_fifoReg->TX_fifoadd_L = ((fifoAddr>>3)&0xFF);
		gUSBC_fifoReg->TX_fifoadd_H = ((fifoAddr>>11)&0xFF);
	}
	else if(flag == 1)//write
	{
		gUSBC_ComReg->EINDEX = EPindex;
		gUSBC_fifoReg->RX_fifoadd_L = ((fifoAddr>>3)&0xFF);
		gUSBC_fifoReg->RX_fifoadd_H = ((fifoAddr>>11)&0xFF); 
	}
}


/*
Description:Send stall signal to host to ruined current transport
*/
void usb_SendRxStall(UINT8 EPindex)
{
	unsigned char ucReg;
	
	gUSBC_ComReg->EINDEX = EPindex;
	//read RXCSR register
	ucReg = gUSBC_IdxReg->RXCSR_L;
	//set SendStall(D5) =1 to genrerate a stall signal
	ucReg |= DEV_RXCSR_SENDSTALL;
	gUSBC_IdxReg->RXCSR_L = ucReg;
	//Set Endpoint1 is Halt in GetStatus
	gpram_WakeupFeature |= 0x01;
}

/*
Description:Send stall signal to host to ruined current transport
*/
void usb_SendTxStall(UINT8 EPindex)
{
	unsigned char ucReg;
	
	gUSBC_ComReg->EINDEX = EPindex;
	//read RXCSR register
	ucReg = gUSBC_IdxReg->TXCSR_L;
	//set SendStall(D5) =1 to genrerate a stall signal
	ucReg |= DEV_TXCSR_SEND_STALL;
	gUSBC_IdxReg->TXCSR_L = ucReg;
	//Set Endpoint1 is Halt in GetStatus
	gpram_WakeupFeature |= 0x01;
}

	
