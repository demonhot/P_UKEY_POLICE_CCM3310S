/*(c) Copyright 2003-2009��C*Core rights reserved */

//*****************************************************************************
//*\�ļ����ƣeecc.h
//*\ժҪ��    ecc��ص����ݽṹ��������������
//*\��ǰ�汾��Ver0.1

//******************************************************************************
#ifndef	__ECC_H__
#define	__ECC_H__

#ifndef bool
#define bool int
#endif

/*(c) Copyright 2003-2009��C*Core rights reserved */

//*****************************************************************************
//*\�ļ����ƣRbigInt.h
//*\ժҪ��    32λ������ص����ݽṹ��������������
//*\��ǰ�汾��Ver0.1
//******************************************************************************
#ifndef	__BIGINT_H__
#define	__BIGINT_H__


//����������ص����ݽṹ���������㺯��
#define ECC_BIGINT32_MAXLEN 	8

#define write_port_byte(port,adata) (*(volatile unsigned char *)(port))=((unsigned char)(adata))
#define read_port_byte(port)        (*(volatile unsigned char *)(port))

#define write_port_halfword(port,adata) (*(volatile unsigned short *)(port))=((unsigned short)(adata))
#define read_port_halfword(port)		(*(volatile unsigned short *)(port))

#define write_port_word(port,adata) (*(volatile unsigned long *)(port))=((unsigned long)(adata))
#define read_port_word(port)		(*(volatile unsigned long *)(port))

typedef unsigned char   UINT8;
typedef signed   char   SINT8;
typedef unsigned short  UINT16;
typedef signed   short  SINT16;
typedef unsigned int    UINT32;
typedef signed 	 int    SINT32;

typedef UINT32 BIG_INT;
typedef UINT16 BIG_HALF_DIGIT;

//���������32λ����ṹ
//auValue����ĵ�32λ��ַ��Ŵ����ĵ�32λ��Ч������32λ��ַ��Ŵ����ĸ�32λ��Ч��  
typedef struct 
{
 	UINT16   uLen;                         //32λ�����ʵ�ʳ���
	UINT32   auValue[ECC_BIGINT32_MAXLEN]; //32λ�����ֵ
}ECC_STU_BIGINT32;



/* Length of digit in bits */
#define BIG_LEN 32
#define BIG_HALF_DIGIT_BITS 16
/* Length of digit in bytes */
#define BIG_DIGIT_LEN (BIG_LEN / 8)

/* Maximum length in digits */
#define MAX_BIG_LEN MAX_RSA_MODULUS_LEN

/* Maximum digits */
#define MAX_BIG_DIGIT 0xffffffff
#define MAX_BIG_HALF_DIGIT 0xffff

#define BIG_LT   -1
#define BIG_EQ   0
#define BIG_GT 1

#define LOW_HALF(x) ((x) & MAX_BIG_HALF_DIGIT)
#define HIGH_HALF(x) (((x) >> BIG_HALF_DIGIT_BITS) & MAX_BIG_HALF_DIGIT)
#define TO_HIGH_HALF(x) (((BIG_INT)(x)) << BIG_HALF_DIGIT_BITS)
#define DIGIT_MSB(x) (unsigned int)(((x) >> (BIG_LEN - 1)) & 1)
#define DIGIT_2MSB(x) (unsigned int)(((x) >> (BIG_LEN - 2)) & 3)

//*----------------------------------------------------------------------------
//* \�������ƣ�void EccBig32Initial(ECC_STU_BIGINT *pstuA)
//* \���ܼ�飺�����ĳ�ʼ��������ֵȫ��ֵΪ0�����鳤�ȼ�Ϊ1
//* \�����������
//* \���������pstuA��ָ��ṹ��ECC_STU_BIGINT��ָ�룬��ʼ�����pstuA
//* \����ֵ  ����

//*----------------------------------------------------------------------------
void EccBig32Initial(ECC_STU_BIGINT32 *pstuA);

//*----------------------------------------------------------------------------
//* \�������ƣ�void EccByBufToBigInt32(UINT8 *pbyInBuf,UINT16 uInLen, ECC_STU_BIGINT32 *pstuOutBigInt)
//* \���ܼ�飺�����޷���8λ��������ת��Ϊ�ṹ��ECC_STU_BIGINT32�Ĵ�����ʽ          
//* \          ���� ����Ϊ8������ pbyOutBuf[0]=0x55;
//* \							  pbyOutBuf[1]=0x66;
//* \							  pbyOutBuf[2]=0x77;
//* \							  pbyOutBuf[3]=0x88;
//* \                             pbyOutBuf[4]=0x11;
//* \							  pbyOutBuf[5]=0x22;
//* \							  pbyOutBuf[6]=0x33;
//* \							  pbyOutBuf[7]=0x44;
//* \        ת��Ϊ ����Ϊ2�Ĵ��� pstuInBigInt->auValue[0]=0x11223344;
//* \   				          pstuInBigInt->auValue[1]=0x55667788;
//* \���������pbyInBuf��ָ���޷���8λ���������׵�ַ��ָ��
//* \          uInLen���޷���8λ��������ĳ���
//* \���������pstuOutBigInt��ָ��ṹ��ECC_STU_BIGINT32��ָ�룬���ת����Ķ�Ӧ����
//* \����ֵ  ����

//*----------------------------------------------------------------------------
void EccByBufToBigInt32(UINT8 *pbyInBuf,UINT16 uInLen, ECC_STU_BIGINT32 *pstuOutBigInt);



//*----------------------------------------------------------------------------
//* \�������ƣ�void EccBig32IntToByBuf(ECC_STU_BIGINT32 *pstuInBigInt, UINT8 *pbyOutBuf, UINT16 *puOutLen)
//* \���ܼ�飺����ṹ��ECC_STU_BIGINT32�Ĵ�����ʽת��Ϊ�޷���8λ��������  
//* \          ���� ����Ϊ2�Ĵ��� pstuInBigInt->auValue[0]=0x11223344;
//* \   				          pstuInBigInt->auValue[1]=0x55667788;
//* \          ת��Ϊ             pbyOutBuf[0]=0x55;
//* \							  pbyOutBuf[1]=0x66;
//* \							  pbyOutBuf[2]=0x77;
//* \							  pbyOutBuf[3]=0x88;
//* \                             pbyOutBuf[4]=0x11;
//* \							  pbyOutBuf[5]=0x22;
//* \							  pbyOutBuf[6]=0x33;
//* \							  pbyOutBuf[7]=0x44;
//* \���������pstuInBigInt��ָ��ṹ��ECC_STU_BIGINT32��ָ��
//* \���������pbyOutBuf��ָ���޷���8λ���������׵�ַ��ָ�룬���ת����Ķ�Ӧ����
//* \          puOutLen���޷���8λ��������ĳ���
//* \����ֵ  ����

//*----------------------------------------------------------------------------
void EccBig32IntToByBuf(ECC_STU_BIGINT32 *pstuInBigInt, UINT8 *pbyOutBuf, UINT16 *puOutLen);

#endif  //__BIGINT_H__


//------------------------------------------------------------

//����ECC�����ĸ�ʽ
typedef struct 
{
    UINT16 uBits;                               // ģ��P��λ��
	ECC_STU_BIGINT32  stuPrimeP;				// ģ��P
	ECC_STU_BIGINT32  stuCoefficientA;			// ��Բϵ��A	
	ECC_STU_BIGINT32  stuCoefficientB;			// ��Բϵ��B		
	ECC_STU_BIGINT32  stuGx;				    // ��Բ��������Gx
	ECC_STU_BIGINT32  stuGy;				    // ��Բ��������Gy
	ECC_STU_BIGINT32  stuPrimeN;				// ��Բ��������G�Ľ�	
} SM2_STU_PRAMS;


//����ECC��Կ�ĸ�ʽ
typedef struct 
{
	ECC_STU_BIGINT32  stuQx;			        // ��Կ����Qx	
	ECC_STU_BIGINT32  stuQy;				    // ��Կ����Qy
} SM2_STU_PUBKEY;

#endif

