#ifndef __UTIL_H__
#define __UTIL_H__

#include "key_aes.h"

/********************************************************************
* Function Name  : GetCRC16
* Description    : 计算CRC16值.
* Input          : *pData: 数据头指针  nLength:数据长度
* Output         : None
* Return         : CRC16码
*******************************************************************************/
extern unsigned short GetCrc_16(char * pData, int nLength);

/* 
* 函数名：FCSCreate
* 输入  ：FCSDataBuffer---要计算的数据
*         DataLength---数据长度
* 输出  : 无
* 描述  : 生成FCS校验和
*/
extern uint8_t FCSCreate(uint8_t * FCSDataBuffer, uint16_t DataLength);

/* 
* 函数名：HEXChangeBCD
* 输入  ：dat---要转换的数
* 输出  : 转换后的数值
* 描述  : 用十六进制显示十进制数
*		  如12->0x12
*/
extern uint8_t HEXChangeBCD(uint8_t dat);

/* 
* 函数名：BCDChangeHEX
* 输入  ：dat---要转换的数
* 输出  : 转换后的数值
* 描述  : 转换16进制为10进制
*		  如0x12->12
*/
extern uint8_t BCDChangeHEX(uint8_t dat);

#endif
