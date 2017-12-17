
#define	USBDEV_ISR_SYMBOL
#include "includes.h"


/**************************************************************************************/
void USBDev_ISR(void)
{
	unsigned char  ucISR;
	unsigned char  sectorOffset;
	
	gpram_msgflags |= gUSBC_ComReg->INTRUSB;                   //ȡ��USB�жϼĴ����е�ֵ
	
	gpram_msgflags1 |= gUSBC_ComReg->INTRTX_L;                 //ȡ��USB�ķ����жϼĴ����е�ֵ
	ucISR     = gUSBC_ComReg->INTRRX_L;                        //ȡ��USB�Ľ����жϼĴ����е�ֵ
		
		
	if(ucISR & (1<<USB_ENDPOINT_INDEX))                        //�����EP1�Ľ����ж���λ����ôռ��gpram_msgflags�е�BIT4λ���д���
	{
		gpram_msgflags  |= 0x10;
	}
	
	if(gpram_msgflags1 & 0x1)                                  //�����EP0�ж�
	{
		if (gpram_USBAddrFlag == 1)                        //�����ַ��д��
		{
			gUSBC_ComReg->FADDRR = gpram_USBNewAddr;   //���µ�ַ�Ž�ȥ���в���
			gpram_USBAddrFlag  = 0;
		}
		gpram_msgflags1 &= 0xFE;                           //��EP0�жϱ�־			
		
		USBDev_EP0_ISR();
	}
	if (gpram_msgflags & 0x10)                                 //�����EP1�Ľ����ж���λ
	{

		gpram_msgflags &= 0xEF;                            //��EP1�����жϱ�־
		g_uchUSBStatus |= 0x01;		
	}
	if (gpram_msgflags1 & (1<<USB_ENDPOINT_INDEX) )            //�����EP1�ķ����ж���λ
	{
		gpram_msgflags1 &= ( ~(1<<USB_ENDPOINT_INDEX) );   //��EP1�����жϱ�־
		g_uchUSBStatus |= 0x10;	
		
	}
			
	if (gpram_msgflags & 0x04)                                 //�����bus reset
	{
		gpram_msgflags &= 0xFB;    
		USBDev_BusReset(0);

	}
		
	if(gpram_msgflags & 0x1)                                   //�����suspend
	{
		gpram_msgflags &= 0xFE;		

	}		
}
/////////////////////////////////////////////////////////////////////////////////////////





/*************************************************************************************/
void USBDev_EP0_ISR(void)
{
	//unsigned char ucReqType=0,ucCount=0;
	unsigned char ucReg=0;

	gUSBC_ComReg->EINDEX  = CONTROL_EP;     
	ucReg= gUSBC_IdxReg->E0CSR_L;
	
	//clear SentStall bit
	if(ucReg &0x04)      //���SendStall������ɣ������־
	{
		ucReg &=0xFB;		
	}

	if(ucReg & 0x10)     //	���setup end��������ô��setup end��־λ
	{
		ucReg |=0x80;
	}
  
	gUSBC_IdxReg->E0CSR_L  = ucReg;


	if(ucReg & 0x01)    //������յ����ݰ�
	{
		USBDev_Ep0Handler();
	}    

}
/////////////////////////////////////////////////////////////////////////////////////////
unsigned char USBDev_EP1_RX_ISR(unsigned char* pData)
{
	unsigned char  ucRegLow;
	unsigned char  number;
	number = 0;
	gUSBC_ComReg->EINDEX = USB_ENDPOINT_INDEX;
	ucRegLow  = gUSBC_IdxReg->RXCSR_L;
	if(ucRegLow & DEV_RXCSR_SENTSTALL)                            //�����⵽sendstall���
		{
			ucRegLow  &= 0xBF;                                    //����ñ�־λ
			ucRegLow  |= DEV_RXCSR_CLR_DATA_TOG;                  //��λ�ö˵������
			gUSBC_IdxReg->RXCSR_L  = ucRegLow;                    
		}
	if(ucRegLow & DEV_RXCSR_RXPKTRDY  )
  		{
  			number = usb_receive(pData);  		
  		}
  		return (number);
}

void USBDev_EP1_TX_ISR(unsigned char* pData,unsigned short len)
{
	    gUSBC_ComReg->EINDEX  = USB_ENDPOINT_INDEX;
  		usb_send(pData,len); 
}


