#ifndef	_SHA_H_
#define	_SHA_H_

//*******Hash�㷨���ͱ��********//
#define		HASH_SM3_1		0
#define		HASH_SHA0_1		1
#define		HASH_SHA1_1		2
#define		HASH_SHA224_1		3
#define 	HASH_SHA256_1		4


//*******Hash�㷨���ݽṹ********//
typedef	struct Hash_tx
{
	unsigned int  hash_len;	  //���δ������ݳ��ȣ�	
	unsigned int  DataBuf[16]; //��������ʣ��δ����Register������
	unsigned char HASHMode;	  //Hash�㷨���ͱ��
	
}Hash_tx;

#endif