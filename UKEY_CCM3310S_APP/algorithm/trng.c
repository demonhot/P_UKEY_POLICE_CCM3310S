/*All rights reserved */
/*************************************************
  	  Copyright (C), 2010-2013, C*Core Technology Co., Ltd
  File name:trng.c                             // �ļ���
  Author:       Version:1.0     Date:          // ���ߡ��汾���������
  Description:                                 // ������ϸ˵���˳����ļ���ɵ���Ҫ����
  Others:                                      // �������ݵ�˵��
  History:                                     // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
                                               // �߼��޸����ݼ���  
    1. Date:         2013-03-13
       Author:       �쿭
       Modification: ���ա�����о�Ƽ����޹�˾��ƷӦ�ò�C���Ա�̹淶��V1.0��׼��
/*************************************************/
#include "includes.h" 

#define   inp_w(addr)               *(volatile UINT32*)(addr)
#define   inp_h(addr)               *(volatile UINT16*)(addr)
#define   inp_b(addr)               *(volatile UINT8*)(addr)

#define   outp_w(addr, dat)         *(volatile UINT32*)(addr)=(dat)
#define   outp_h(addr, dat)         *(volatile UINT16*)(addr)=(dat)
#define   outp_b(addr, dat)         *(volatile UINT8*)(addr)=(dat)

//*-------------------------------------------------------------------------------------
//* �������ƣ�void RSA_InitTrng()
//* ���ܼ�飺�����ģ���ʼ��
//* �����������
//* �����������
//* ����ֵ��  ��

//*-------------------------------------------------------------------------------------
void InitTrng(void)
{
	//*(unsigned long *)(0xc30000+8) = 0;
	
    outp_w(TRNG_CSR, 0x00000504);             		//���������ʹ��
}

//*-------------------------------------------------------------------------------------
//* �������ƣ�UINT32 GetRandomWord(void)
//* ���ܼ�飺��������Ĵ�����ȡ32 bit�Ѿ������õ������
//* �����������
//* �����������
//* ����ֵ��  ȡ�������ֵ
//*-------------------------------------------------------------------------------------

UINT32 GetRandomWord(void)             
{    
    
     UINT32 ulRandomValue=0,ulRandomTmpOld,ulRandomTmpNew,i,j;

	ulRandomTmpNew = inp_w(TRNG_CSR);
	while((ulRandomTmpNew&0x00000800) == 0){
		ulRandomTmpNew = inp_w(TRNG_CSR);
	}
	ulRandomValue = inp_w(TRNG_DR);
	*(unsigned long *)TRNG_CSR |= 0x00000400;
	
    return ulRandomValue;  //����ȡ���������
}

