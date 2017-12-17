/*All rights reserved */
/*************************************************
  	  Copyright (C), 2010-2013, C*Core Technology Co., Ltd
  File name:startup.c                          // �ļ���
  Author:       Version:1.0     Date:          // ���ߡ��汾���������
  Description:                                 // ������ϸ˵���˳����ļ���ɵ���Ҫ����
  Others:                                      // �������ݵ�˵��
  History:                                     // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
                                               // �߼��޸����ݼ���  
    1. Date:         2013-03-13
       Author:       �쿭
       Modification: ���ա�����о�Ƽ����޹�˾��ƷӦ�ò�C���Ա�̹淶��V1.0��׼��
/*************************************************/
extern void (*const __vector_table[64])(void);
extern void __init_user(void);
extern void (*const __vector_table[64])(void);
extern void main();


extern int *__bss_begin;                //  start of .bss section
extern int *__bss_end;                  //  end of .bss section
extern int *__stack_begin;              //  start of stack section
extern int *__stack_end;                //  end of stack section




asm void _start(void)
{

// initialize the Vector Base Register (VBR)
	lrw     r3, __vector_table                     //��__vector_table����r3
	mtcr    r3, cr1                                //��__vector_table����vbr�Ĵ���
 
//Close watchdog
	lrw	    r3, 0x0e
	lrw     r2, 0x00c60000
	st.w    r3, (r2,0)

//Close TC
	lrw   r3,0x00
	lrw   r2,0x00d30000
	st.w  r3,(r2,0)
  
// initialize stack pointer
	lrw     r10, __stack_begin
	mov     r0, r10                       
	
	
	
// zero out .bss section
	lrw     r5, __bss_begin
	lrw     r6, __bss_end
	
	cmphs   r5, r6                         //���r5>=r6,Cλ��1
	bt      _end_zero_bss                  //���CΪ1������ת��_end_zero_bss
	xor     r4, r4                         //r4��0

_zero_bss:

	subi    r6, 1                          //r6��ֵ��1
	st.b    r4, (r6,0)                     //��0����r6�и����ĵ�ַ  �������ĵ�ַ��ǰ����ַ��
	cmplt   r5, r6                         //���r5<r6,Cλ��1
	bt      _zero_bss                      //���CΪ1������ת��_zero_bss
	
_end_zero_bss:

// copy ROM to RAM
	bsr     __init_user                    //��ת��__init_user
	
//  program entry point
	bsr   main                             //��ת��main

}



