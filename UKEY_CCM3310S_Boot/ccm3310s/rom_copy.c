/*All rights reserved */
/*************************************************
  	  Copyright (C), 2010-2013, C*Core Technology Co., Ltd
  File name:rom_copy.c                         // �ļ���
  Author:       Version:1.0     Date:          // ���ߡ��汾���������
  Description:                                 // ������ϸ˵���˳����ļ���ɵ���Ҫ����
  Others:                                      // �������ݵ�˵��
  History:                                     // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
                                               // �߼��޸����ݼ���  
    1. Date:         2013-03-13
       Author:       �쿭
       Modification: ���ա�����о�Ƽ����޹�˾��ƷӦ�ò�C���Ա�̹淶��V1.0��׼��
/*************************************************/
#include <inttypes.h>    

/*  elf sections containing non-constant data */
extern unsigned  char __data_ROM_begin[];   // beginning of .data in ROM
extern unsigned  char __data_begin[];       // beginning of .data in RAM
extern unsigned  char __data_end[];      

/*
 *	__memcpy	-	linkable version of __memcpy() intrinsic function
 *
 *	If the compiler sees a call to the __memcpy() intrinsic whose 3rd argument
 *	is not a constant-expression, it will generate a call to this routine. We must
 *	move the data byte-by-byte as we do not know if the source and destination
 *	are aligned to any particular boundary.
 *
 */
asm void *__memcpy(register void *dst, register const void* src, register size_t len)
{
	tst		len,len;
	bf		L2;
L1:	ld.b	r1,(src,0);
	subi	len,1;
	st.b	r1,(dst,0);
	addi	src,1;
	addi	dst,1;
	tst		len,len;
	bt		L1;	
L2:
	jmp		r15
}

/*
 *  __copy_rom_section  - copy the exception/data vectors to their RAM destinations.
 *
 */
void __copy_rom_section(void* dst, const void* src, unsigned long int size)
{
    if (size && (dst != src))
    {
        __memcpy(dst, src, size);   
    }
}

/*
 *  __init_user - copy all the initialized non-constant data sections from ROM to RAM.
 *
 */
void __init_user(void)
{
     __copy_rom_section(__data_begin, __data_ROM_begin, __data_end - __data_begin);
}



