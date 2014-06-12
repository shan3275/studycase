#include <stdlib.h>
#include <stdio.h> 

/* 方法 1*/
/* 函数名: CRC8 * 
 * 多项式：x8+x2+x1+1
 * 功  能:CRC校验
 * 用  法:unsigned char CRC8(unsigned char *ptr, unsigned char len,unsigned char key)
 * 参数：ptr是待校验数据流的起始地址，len是待校验数据流的字节长度，key是生成多项式的简记式
 * 问题：最简单，速度最慢，占用内存最少
 * 程序例:*/
unsigned char CRC8(unsigned char *ptr, unsigned char len,unsigned char key)
{
    unsigned char i;
    unsigned char crc=0;
    while(len--!=0)
    {
	for(i=0x80; i!=0; i/=2)
	{
            if((crc&0x80)!=0) {crc*=2; crc^=key;} /* 余式CRC 乘以2 再求CRC */
	    else crc*=2;
	    if((*ptr&i)!=0) crc^=key; /* 再加上本位的CRC */
	}
	ptr++;
    }
	return(crc);
}

/* 方法 2 半字节查表法*/
/* 函数名：mK_cRctable
 * 功能  ：生成0-15对应的CRC校验码,其实就是计算机算法
 *
 * */
unsigned char CRC8HalfTable[16];
void mK_cRctable(unsigned char * cRctable_8, unsigned char key)
{
    unsigned char cRc_8=0;
    unsigned char i,j;

    for(i=0;i<16;i++)
    {
	cRc_8 = i;
	for(j=8;j>0;j--)
	{
	if(cRc_8&0x80) 
	cRc_8=(cRc_8<<1)^key; 
	else
	cRc_8<<=1; 
	}
	cRctable_8[i] = cRc_8;
    }
}
/* 函数名：GetCrc_8
 * 功能  ：计算数据流* pData的16位CRC校验码，数据流长度为nLength
 * */
unsigned char GetCrc_8(unsigned char * ptr, int len, unsigned char * PMBUS_CRC8_Table)
{
    unsigned char crc;
    unsigned char da;

    crc=0;
    while(len--!=0)
    {
        da=crc/16; /* 暂存CRC 的高四位 */
	crc<<=4; /* CRC 右移4 位，相当于取CRC 的低12 位）*/
	crc^=PMBUS_CRC8_Table[da^(*ptr/16)]; /* CRC 的高4 位和本字节的前半字节相加后查表计算CRC，然后加上上一次CRC的余数 */
	da=crc/16; /* 暂存CRC 的高4 位 */
	crc<<=4; /* CRC 右移4 位， 相当于CRC 的低12 位） */
	crc^=PMBUS_CRC8_Table[da^(*ptr&0x0f)]; /* CRC 的高4位和本字节的后半字节相加后查表计算CRC，然后再加上上一次CRC的余数 */
	ptr++;
    }
    return(crc);
}

/* 方法3 字节查表法 */

/* 函数名：mK_cRctable_256
 * 功能  ：生成0-255对应的CRC校验码,其实就是计算机算法
 *
 * */
unsigned char CRC8Table[256];
void mK_cRctable_256(unsigned char * cRctable_8, unsigned char key)
{
    unsigned char cRc_8=0;
    unsigned char j;
    unsigned int i;

    for(i=0;i<256;i++)
    {
	cRc_8 = i;
	for(j=8;j>0;j--)
	{
	if(cRc_8&0x80) 
	cRc_8=(cRc_8<<1)^key; 
	else
	cRc_8<<=1; 
	}
	cRctable_8[i] = cRc_8;
    }
}

unsigned char cal_crc(unsigned char *ptr, unsigned char len)
{
    unsigned char crc=0;
    while(len--!=0)
    {
        crc =CRC8Table[crc ^ *ptr++];
    }
    return(crc);
} 

int main(void)
{
   int n;
   int i;
   unsigned char str[9] = {0x23,0x23,0x23,0x34,0x45,0xab,0x45,0x98,0xcb};

   n = CRC8(str, 9, 7); 
   printf("Method 1 CRC-8 = 0x%.2x\n", n); 
   n = 0;
   mK_cRctable(CRC8HalfTable,7);
   for(i=0; i< 16; i++)
   {
	   printf("0x%.2x  ", CRC8HalfTable[i]);
   }
   printf("\n");
   n = GetCrc_8(str, 9, CRC8HalfTable);
   printf("Method 2 CRC-8 = 0x%.2x\n", n); 

   n = 0;
   mK_cRctable_256(CRC8Table, 7);
   for(i=0; i<=255; i++)
   {
	   if(i%16 == 0)
		   printf("\n");
	   printf("0x%.2x,", CRC8Table[i]);
   }
   n = cal_crc(str, 9);
   printf("\n");
   printf("Method 3 CRC-8 = 0x%.2x\n", n); 
   return 0; 
}
