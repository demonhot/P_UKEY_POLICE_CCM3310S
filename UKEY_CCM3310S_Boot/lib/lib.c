
#define  LIB_SYMBOL	
#include "includes.h"

/*
void Delay(UINT32 t)
{
     while(t--  > 0);
}
*/

int strlen(const char *StrDest)  
{  
    int i;  
    i=0;  
    while((*StrDest++)!='\0')  
    {   
        i++;  
    }//���ѭ������˼�Ǵ��ַ�����һ���ַ��������ֻ�����ַ���������־'\0����ֹͣ����  
    return i;  
}  

void memcpy(unsigned char *pDst,const unsigned char *pSrc, UINT16 cnt)
{
	if(!pDst)
	{
		return ;
	}
	if(!pSrc)
	{
		return ;
	}
	while(cnt--)
	{
		*pDst=*pSrc;
		pDst++;
		pSrc++;
	}
}

void *memset( void *dst, const char val, register unsigned len)
{
	while(len--)
	{
		((char*)dst)[len] = val;
	}
	return dst;
}

UINT8 memcmp(const void *s1, const void *s2, register unsigned len)
{
	while(len--)
	{
		if ( ((char*)s1)[len] != ((char*)s2)[len] )
		{
			return 1;
		}
	}
	return 0;
}


void * memmove(void * dest,const void *src,register unsigned count)
{
	char *tmp, *s;

	if (dest <= src) {
		tmp = (char *) dest;
		s = (char *) src;
		while (count--)
			*tmp++ = *s++;
		}
	else {
		tmp = (char *) dest + count;
		s = (char *) src + count;
		while (count--)
			*--tmp = *--s;
		}

	return dest;
}


//ʹ��ͬһ�����������ѻ�����ǰ������ݸ��Ƶ�����
void memcpy_special(UINT8 * pDst, const UINT8 * pSrc, UINT16 cnt)
{
	UINT8 *p_u8Dest = (UINT8*)(pDst+cnt-1);
	UINT8 *p_u8Src = (UINT8*)(pSrc+cnt-1);

	if(!pDst)
	{
		return ;
	}
	if(!pSrc)
	{
		return ;
	}
	while(cnt--)
	{
		*p_u8Dest=*p_u8Src;
		p_u8Dest--;
		p_u8Src--;
	}
}


UINT8 Calculate_LRC(UINT8 *src, UINT16 len)
{
	UINT16 i;
	UINT8 lrc = 0;

	for (i=0; i<len; i++)
	{
		lrc ^= src[i];
	}

	//���ֵȡ��
	return (~lrc);
}

void XorArrays(UINT8 *dest, UINT8* pbySrc1, UINT8* pbySrc2, UINT8 byLen)
{
	while(byLen>0)
	{
		byLen--;
		dest[byLen] = pbySrc1[byLen]^pbySrc2[byLen];
	}
	return;
}

//lenΪ�ֳ��ȣ�����Ϊ��λ���巭ת�����ڲ���
void wordswap(UINT32 *dest, UINT32 *src, UINT32 wordLen)
{
	UINT32 i;
	UINT32 temp;

	//�������ʹ��ͬһ��BUF
	if (src == dest)
	{
		for (i=0; i<wordLen/2; i++)
		{
			temp = dest[i];
			dest[i] = src[wordLen-1-i];
			dest[wordLen-1-i] = temp;
		}
	}
	else
	{
		for (i=0; i<wordLen; i++)
		{
			dest[i] = src[wordLen-1-i];			
		}
	}
}

//lenΪ�ֽڳ��ȣ�����Ϊ��λ���巭ת�����ڲ���
BOOLEAN wordswap_u8(UINT8 *dest, UINT8 *src, UINT32 byteLen)
{
	UINT32 i;
	UINT32 length;
	UINT8 temp[4];
	
	if (byteLen%4 != 0)
		return FALSE;

	length = byteLen/4;

	//�������ʹ��ͬһ��BUF
	if (src == dest)
	{
		for (i=0; i<length/2; i++)
		{
			memcpy(temp, dest+4*i, 4);
			memcpy(dest+4*i, src+4*(length-1-i), 4);
			memcpy(src+4*(length-1-i), temp, 4);
		}
	}
	else
	{
		for (i=0; i<length; i++)
		{
			memcpy(dest+4*i, src+4*(length-1-i), 4);
		}
	}

	return TRUE;
}

//lenΪ�ֽڳ���
void byteswap(UINT8 *dest, UINT8 *src, UINT32 byteLen)
{
	UINT32 i;
	UINT8 temp;
	
	//�������ʹ��ͬһ��BUF
	if (src == dest)
	{
		for (i=0; i<byteLen/2; i++)
		{
			temp = dest[i];
			dest[i] = src[byteLen-1-i];
			dest[byteLen-1-i] = temp;
		}
	}
	else
	{
		for (i=0; i<byteLen; i++)
		{
			dest[i] = src[byteLen-1-i];
		}	
	}
}


//lenΪ�ֽڳ��ȣ����ڷ�ת�����岻��
BOOLEAN byteswap_inword_u8(UINT8 *dest, UINT8 *src, UINT32 byteLen)
{
	UINT32 i;
	UINT32 length;

	if (byteLen%4 != 0)
		return FALSE;

	length = byteLen/4;
	for(i=0; i<length; i++)
	{
		byteswap(dest+4*i, src+4*i, 4);
	}

	return TRUE;
}



