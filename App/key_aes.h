/**
 ******************************************************************************
 * @file    __KEY_AES_H
 * @author  JC
 * @version V1.0.0
 * @date    03/09/2020
 * @brief  
 ******************************************************************************
 */

#ifndef __KEY_AES_H
#define __KEY_AES_H

/******************************** Includes ************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "n32g032.h"
#include "systick.h"

#define ENCRY_PROTO_A   (0x01)
#define ENCRY_PROTO_B   (0x02)


/******************************** Private typedef   ************************************/
/******************************** Private define    ************************************/
/******************************** Private variables ************************************/

/******************************** Private functions ************************************/
/*****************************************************************
** 函数名   : key_srand
** 输入     : none
** 返回     : none
** 功能描述 : 随机数
****************************************************************/
void key_srand(void);
/*****************************************************************
** 函数名   : key_jbox_data_encryption_create
** 输入     : src  -- 源数据
              dst  -- 生成加密数据（生成的数据长度为 len + 2 + KEY_JBOX_ENCRY_KEY_LEN +1）
              len  -- 要加密数据长度
** 返回     : 0 -- 成功 
              1 -- 失败
** 功能描述 : JBOX主机生成加密数据
****************************************************************/
uint8_t key_jbox_data_encryption_create(uint8_t * src  ,uint8_t * dst , uint8_t len, uint8_t proto);
/*****************************************************************
** 函数名   : key_slave_decrypt_jbox_data
** 输入     : src  -- 源数据
              dst  -- 解码后的数据
** 返回     : 0 -- 成功 
              1 -- 失败
** 功能描述 : 从机解码JBOX加密数据
****************************************************************/
uint8_t key_slave_decrypt_jbox_data(uint8_t * src  ,uint8_t * dst, uint8_t proto );
/*****************************************************************
** 函数名   : key_master_ori_data_encryption
** 输入     : src  -- 源数据
              dst  -- 生成加密数据（生成的数据长度为 128 + 2 + KEY_JBOX_ENCRY_KEY_LEN +1）
** 返回     : 0 -- 成功 
              1 -- 失败
** 功能描述 : 主机生成听筒原始排线I2C加密数据(src 数据长度固定为128)
****************************************************************/
uint8_t key_master_ori_data_encryption(uint8_t * src  ,uint8_t * dst, uint8_t proto );
/*****************************************************************
** 函数名   : key_slave_decrypt_ori_data
** 输入     : src  -- 源数据
              dst  -- 解码后的数据
** 返回     : 0 -- 成功 
              1 -- 失败
** 功能描述 : 从机解码排线原始加密数据
****************************************************************/
uint8_t key_slave_decrypt_ori_data(uint8_t * src  ,uint8_t * dst, uint8_t proto );
/*****************************************************************
** 函数名   : key_jbox_data_encryption_create_by_master_key
** 输入     : key  -- 主机密钥
              src  -- 源数据
              dst  -- 生成加密数据（生成的数据长度为 len + 2 + KEY_JBOX_ENCRY_KEY_LEN +1）
              len  -- 要加密数据长度
** 返回     : 0 -- 成功 
              1 -- 失败
** 功能描述 : 主机生成随机数密钥，从机根据该密钥加密
****************************************************************/
uint8_t key_jbox_data_encryption_create_by_master_key(uint8_t * key ,uint8_t * src  ,uint8_t * dst , uint8_t len, uint8_t proto);
#endif  /*    __KEY_AES_H        */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/


