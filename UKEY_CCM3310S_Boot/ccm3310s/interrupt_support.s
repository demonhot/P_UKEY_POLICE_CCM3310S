/*All rights reserved */
/*************************************************
  	  Copyright (C), 2010-2013, C*Core Technology Co., Ltd
  File name:interrupt_support.s                // �ļ���
  Author:       Version:1.0     Date:          // ���ߡ��汾���������
  Description:                                 // ������ϸ˵���˳����ļ���ɵ���Ҫ����
  Others:                                      // �������ݵ�˵��
  History:                                     // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
                                               // �߼��޸����ݼ���  
    1. Date:         2013-03-13
       Author:       �쿭
       Modification: ���ա�����о�Ƽ����޹�˾��ƷӦ�ò�C���Ա�̹淶��V1.0��׼��
/*************************************************/
    .file   "interrupt_support.s"
    .text
    
######################################################################
# Control register read routines
######################################################################
    .export Read_PSR, Read_VBR

Read_PSR:
    mfcr    r2,cr0
    rts

Read_VBR:
    mfcr    r2,cr1
    rts

######################################################################
# Control register write routines
######################################################################
    .export Write_PSR, Write_VBR

Write_PSR:
    mtcr    r2,cr0
    rts

Write_VBR:
    mtcr    r2,cr1
    rts

##########################################################################
# Disable_Ints function (disables interrupts by clearing PSR.IE and PSR.FE
##########################################################################
    .export Disable_Ints
Disable_Ints:
    mfcr    r1, cr0		# get PSR
    bclri   r1, 0x4		# disable fast interrupts
    bclri   r1, 0x6		# disable normal interrupts
    mtcr    r1, cr0		# make it so by stashing it back into PSR
    jmp     r15

##########################################################################
# Enable_Ints function (enables interrupts by setting PSR.IE and PSR.FE
##########################################################################
    .export Enable_Ints
Enable_Ints:
    mfcr    r1, cr0		# get PSR	
    bseti   r1, 0x4		# enable fast interrupts
    bseti   r1, 0x6		# enable normal interrupts
    mtcr    r1, cr0		# put changes back into PSR
    jmp     r15

