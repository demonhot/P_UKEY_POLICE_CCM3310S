/*All rights reserved */
/*************************************************
  	  Copyright (C), 2010-2013, C*Core Technology Co., Ltd
  File name:main.c                             // �ļ���
  Author:       Version:1.0     Date:          // ���ߡ��汾���������
  Description:                                 // ������ϸ˵���˳����ļ���ɵ���Ҫ����
  Others:                                      // �������ݵ�˵��
  History:                                     // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
                                               // �߼��޸����ݼ���  
    1. Date:         2013-03-13
       Author:       �쿭
       Modification: ���ա�����о�Ƽ����޹�˾��ƷӦ�ò�C���Ա�̹淶��V1.0��׼��
/*************************************************/
#define  MAIN_SYMBOL
#include "includes.h"

/*
����ͨ�Ż�����: 
g_APDU_BUFָ��APDU_DATA_ADDR��ַ
apdu_responseָ��RESPONSE_DATA_ADDR��ַ
ĿǰAPDU��response��������С�������2K������޸Ļ�������С����Ҫ�޸�Linkfile�ļ���DATA����ʼ��ַ
*/
#define APDU_DATA_ADDR			(0x00800200)		//USB FIFO��ַ��Ϊ8�ֽ�������
#define RESPONSE_DATA_ADDR		(0x00800A00)		//USB FIFO��ַ��Ϊ8�ֽ�������
unsigned char g_reboot;

void InitGlobalVariable(void)
{
	g_APDU_BUF = (UINT8*)APDU_DATA_ADDR;
	apdu_response = (UINT8*)RESPONSE_DATA_ADDR;
	g_APDU_LEN = 0;
	g_reboot=0;
	

	//ʹ��EMDAC��DMAC��SHAģ��ʱ��
	*(volatile unsigned int *)(0x00c30004) &= ~0x300200;  //enable
	InitTrng();
	SM2_Enable();
	RsaModuleEnable();
	SM1_Enable();
	DES_Enable();
	AES_Enable();
	SMS4_Enable();
	SSF33_Enable();

	//��ȡSM1��Կ
	F_Read(EFLASH_SM1_EK_ADDR, (UINT8*)g_sm1EK, EFLASH_SM1_EK_SIZE);
	F_Read(EFLASH_SM1_AK_ADDR, (UINT8*)g_sm1AK, EFLASH_SM1_AK_SIZE);
	F_Read(EFLASH_SM1_SK_ADDR, (UINT8*)g_sm1SK, EFLASH_SM1_SK_SIZE);
	F_Read(EFLASH_SM1_IV_ADDR, (UINT8*)g_sm1IV, EFLASH_SM1_IV_SIZE);

	//��ȡSM2ǩ���û�ID
	g_useridLen = F_Read_u8(EFLASH_SM2_USERID_LENGTH_ADDR);
	F_Read(EFLASH_SM2_USERID_ADDR, (UINT8*)g_userid, EFLASH_SM2_USERID_SIZE);

	
	//��ȡDES��Կ
	F_Read(EFLASH_DES_KEY_ADDR, (UINT8*)g_desKey, EFLASH_DES_KEY_SIZE);
	F_Read(EFLASH_DES_IV_ADDR, (UINT8*)g_desIV, EFLASH_DES_IV_SIZE);

	//��ȡAES��Կ
	F_Read(EFLASH_AES_KEY_ADDR, (UINT8*)g_aesKey, EFLASH_AES_KEY_SIZE);
	F_Read(EFLASH_AES_IV_ADDR, (UINT8*)g_aesIV, EFLASH_AES_IV_SIZE);
	
	//��ȡSMS4��Կ
	F_Read(EFLASH_SMS4_KEY_ADDR, (UINT8*)g_sms4Key, EFLASH_SMS4_KEY_SIZE);
	F_Read(EFLASH_SMS4_IV_ADDR, (UINT8*)g_sms4IV, EFLASH_SMS4_IV_SIZE);

	//��ȡSSF33��Կ
	F_Read(EFLASH_SSF33_MK_ADDR, (UINT8*)g_ssf33MK, EFLASH_SSF33_MK_SIZE);
	F_Read(EFLASH_SSF33_SK_ADDR, (UINT8*)g_ssf33SK, EFLASH_SSF33_SK_SIZE);
	F_Read(EFLASH_SSF33_IV_ADDR, (UINT8*)g_ssf33IV, EFLASH_SSF33_IV_SIZE);
}


void interrupt_config(void)
{
	//�����ж�
	Disable_Interrupts;	
	NIER_reg      = 0x0;               //�������е��ж�
	ICR_reg       = 0;                 //���������жϣ���Ĭ�ϲ����Զ������жϣ�������Ҫ���ж�ģʽ����Ϊ�����ж�
	PLSR25_reg  = PRIORITY_LEVEL_4;    //USB�жϣ����ȼ���Ϊ4
	NIER_reg   |= 0x0010;              //ʹ�����ȼ�Ϊ4���ж�ʹ��	
	Enable_Interrupts;	
}


void Chip_Reset(unsigned char flag)
{
	unsigned int i;

	unsigned char buf[16];

	if(flag==1)
	{
		buf[0] = 0xff;
		F_Write( 0x00003803,buf, 1);		
		USB_DisConnect();
	}
		
	//for(i=0x50000;i>0;i--){};
	//
	asm
	{
		lrw r1, 0x200;
		jmp r1;	
	}	

	return;
}


void main()
{
	UINT8 ucISR;	
	UINT16 recvLen, sendLen;
	
	ModuleClk_On(MODULE_CLK_USBC|MODULE_CLK_CRYPTO|MODULE_CLK_TRNG|MODULE_CLK_SM1|MODULE_CLK_SHA1|MODULE_CLK_SPI1|MODULE_CLK_SPI2);	//��CRYPTO/TRNGģ���ʱ��
    
	SetClk();

	//EnBootMode();

	
	set_clkd(g_sys_clk);

	InitGlobalVariable();

	//hal_RSAinit
	hal_RSAinit();	

	//sm2 init
	g_sm2Bits = 256;
	SM2Init(g_sm2Bits);
	
	//usb init		
	USBDev_Init();

	//interrupt init
	interrupt_config();

	//uart init
	ModuleClk_On(MODULE_CLK_SCI);	//��SCIģ���ʱ��
//	UARTInit(g_sys_clk*1000,9600,TEN,ParityDIS,EVE);

	//flash interface init
	HAL_Flash_Interface_Init();

	//cos init
	cos_init(0);

//	HAL_UART_SendData("in main\r\n",sizeof("in main\r\n"));
	while(1)
	{
#ifndef USB_USEINT		//��ѯ��ʽ	
		gpram_msgflags1 |= gUSBC_ComReg->INTRTX_L;                 //ȡ��USB�ķ����жϼĴ����е�ֵ
		ucISR = gUSBC_ComReg->INTRRX_L;                        //ȡ��USB�Ľ����жϼĴ����е�ֵ


		if(g_reboot==2)
		{
			Chip_Reset(1);
		}
		if(g_reboot==4)
		{
			Chip_Reset(0);
		}



		//EP1~EP7�����ж�
		g_uchUSBRcvStatus = (ucISR&0xFE); 
		
		//EP0�ж�
		if(gpram_msgflags1 & 0x1)
		{
			if (gpram_USBAddrFlag == 1)                        //�����ַ��д��
			{
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
		
#endif		//#ifndef USB_USEINT	

		if (g_uchUSBRcvStatus & (1<<INDEX_EP1))     
		{
			g_uchUSBRcvStatus &= ~(1<<INDEX_EP1); 
			USBDev_EP1_RX_ISR();
		}

		if (g_uchUSBSendStatus & (1<<INDEX_EP1))
		{
			g_uchUSBSendStatus &= (~(1<<INDEX_EP1));
			USBDev_EP1_TX_ISR();	
		}
	}
}
						
				
						
		
