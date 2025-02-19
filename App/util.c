/*
* 文件名：util.c
* 描述：  本文件实现各种校验算法
*/
#include "util.h"

extern const uint16_t crc16_tab[256];

/********************************************************************
* Function Name  : GetCRC16
* Description    : 计算CRC16值.
* Input          : *pData: 数据头指针  nLength:数据长度
* Output         : None
* Return         : CRC16码
*******************************************************************************/
unsigned short GetCrc_16(char * pData, int nLength)
{
	unsigned short cRc_16 = 0xFFFF;
	unsigned char temp;
	
	while(nLength-- > 0)
	{
		temp = cRc_16 & 0xFF; 
		cRc_16 = (cRc_16 >> 8) ^ crc16_tab[(temp ^ *pData++) & 0xFF];
	}
	
	return cRc_16;
}

/* 
* 函数名：FCSCreate
* 输入  ：FCSDataBuffer---要计算的数据
*         DataLength---数据长度
* 输出  : 无
* 描述  : 生成FCS校验和
*/
uint8_t FCSCreate(uint8_t * FCSDataBuffer, uint16_t DataLength)
{
    uint8_t FCSData = 0;
    uint16_t i = 0;
    for (i = 0; i < DataLength ; i++) {
        FCSData += FCSDataBuffer[i];
    }
    return FCSData;
}

/* 
* 函数名：HEXChangeBCD
* 输入  ：dat---要转换的数
* 输出  : 转换后的数值
* 描述  : 用十六进制显示十进制数
*		  如12->0x12
*/
uint8_t HEXChangeBCD(uint8_t dat)
{
    return ((dat/10)<<4) | (dat%10);
}

/* 
* 函数名：BCDChangeHEX
* 输入  ：dat---要转换的数
* 输出  : 转换后的数值
* 描述  : 转换16进制为10进制
*		  如0x12->12
*/
uint8_t BCDChangeHEX(uint8_t dat)
{
    return (uint8_t)((dat/0x10)*10) + (uint8_t)(dat%0x10);
}
