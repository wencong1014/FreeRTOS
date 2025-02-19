/**
 ******************************************************************************
 * @file    key_aes.C
 * @version V1.0.0
 * @date    03/09/2020
 * @brief
 ******************************************************************************
 * @attention
 *  V1.00
 *  1.主机模式 ： 主机先向从机获取4个字节的加密数据存进buff中,如；buff[4] ；
				  调用 key_random_buff_load(buff)函数 ;
									发送加密数据时，调用key_data_encryption()函数生成加密数据
 *
 *  2.从机模式 ： 主机获取加密数据时，调用 key_random_buff_copy()  ;
				  解密时调用key_data_decrypt_2();

		V1.01
	1.增加听筒排线特定加密解密方式
 *
 */

/******************************** Includes ************************************/
#include "key_aes.h"
#include "util.h"
#include "main.h"

/******************************** Private typedef   ************************************/

/******************************** Private define    ************************************/
/*****************************************************************
** 函数名   : key_get_random
** 输入     : none
** 返回     : none
** 功能描述 : 获取随机数
****************************************************************/
static uint8_t key_get_random(uint8_t seed)
{
	//    srand(seed);

	seed = rand();

	return seed;
}
/*****************************************************************
** 函数名   : key_srand
** 输入     : none
** 返回     : none
** 功能描述 : 随机数
****************************************************************/
void key_srand(void)
{
	static uint32_t seed;

	seed++;

	srand(seed);
}

/*****************************************************************
** 函数名   : key_magic_init_get_1
** 输入     : magic --
			  len   -- 数据长度
** 返回     : none
** 功能描述 : 获取magic初始化数据
****************************************************************/
static void key_magic_init_get_1(uint8_t *magic, uint8_t len)
{
	uint8_t i;
	magic[0] = 0x34;
	magic[1] = 0x42;
	magic[2] = 0xFC;
	magic[3] = 0x63;
	magic[4] = 0x8A;
	magic[5] = 0x9B;
	magic[6] = 0x9E;
	magic[7] = 0xB8;
	magic[8] = 0x4E;
	magic[9] = 0xB6;
	magic[10] = 0x8A;
	magic[11] = 0x0C;
	magic[12] = 0xA5;
	magic[13] = 0x16;
	magic[14] = 0x38;
	magic[15] = 0x2C;
	magic[16] = 0xE4;
	magic[17] = 0x9A;
	magic[18] = 0xE5;
	magic[19] = 0x1E;
	magic[20] = 0xE5;
	magic[21] = 0x72;
	magic[22] = 0x92;
	magic[23] = 0xB5;
	magic[24] = 0xB8;
	magic[25] = 0xA2;
	magic[26] = 0xF4;
	magic[27] = 0x69;
	magic[28] = 0x96;
	magic[29] = 0x2E;
	magic[30] = 0xE5;
	magic[31] = 0x6C;

	magic[32] = 0x57;
	magic[33] = 0xEA;
	magic[34] = 0xEC;
	magic[35] = 0x61;
	magic[36] = 0x4A;
	magic[37] = 0xE5;
	magic[38] = 0x5A;
	magic[39] = 0x6C;
	magic[40] = 0x8E;
	magic[41] = 0x4E;
	magic[42] = 0xCC;
	magic[43] = 0xCA;
	magic[44] = 0x6B;
	magic[45] = 0xB9;
	magic[46] = 0xE3;
	magic[47] = 0x69;
	magic[48] = 0xE8;
	magic[49] = 0xEA;
	magic[50] = 0x3A;
	magic[51] = 0x9E;
	magic[52] = 0xC5;
	magic[53] = 0x54;
	magic[54] = 0x32;
	magic[55] = 0x78;
	magic[56] = 0xEB;
	magic[57] = 0xC2;
	magic[58] = 0xF1;
	magic[59] = 0x63;
	magic[60] = 0xE6;
	magic[61] = 0xCE;
	magic[62] = 0xEF;
	magic[63] = 0x8C;

	if (len > 64)
	{
		for (i = 0; i < (len - 64); i++)
		{
			magic[64 + i] = magic[63 - i] ^ magic[i];
		}
	}
}

/*****************************************************************
** 函数名   : key_magic_init_get_2
** 输入     : magic --
			  len   -- 数据长度
** 返回     : none
** 功能描述 : 获取magic初始化数据
****************************************************************/
static void key_magic_init_get_2(uint8_t *magic, uint8_t len)
{
	uint8_t i;
	magic[0] = 0x5B;
	magic[1] = 0x76;
	magic[2] = 0x9F;
	magic[3] = 0x9A;
	magic[4] = 0x65;
	magic[5] = 0xC1;
	magic[6] = 0x68;
	magic[7] = 0x16;
	magic[8] = 0x72;
	magic[9] = 0xBC;
	magic[10] = 0xB4;
	magic[11] = 0xC1;
	magic[12] = 0x6C;
	magic[13] = 0x0F;
	magic[14] = 0xB1;
	magic[15] = 0x25;
	magic[16] = 0x90;
	magic[17] = 0xE1;
	magic[18] = 0x74;
	magic[19] = 0x2C;
	magic[20] = 0xE2;
	magic[21] = 0x05;
	magic[22] = 0xE0;
	magic[23] = 0x90;
	magic[24] = 0x50;
	magic[25] = 0xDC;
	magic[26] = 0xF0;
	magic[27] = 0x08;
	magic[28] = 0x42;
	magic[29] = 0x1E;
	magic[30] = 0x60;
	magic[31] = 0x49;

	magic[32] = 0x2A;
	magic[33] = 0x6D;
	magic[34] = 0x3F;
	magic[35] = 0xDD;
	magic[36] = 0x92;
	magic[37] = 0x54;
	magic[38] = 0x25;
	magic[39] = 0x9A;
	magic[40] = 0xB3;
	magic[41] = 0x56;
	magic[42] = 0x4B;
	magic[43] = 0x59;
	magic[44] = 0xDD;
	magic[45] = 0x2F;
	magic[46] = 0xE5;
	magic[47] = 0xFA;
	magic[48] = 0x5D;
	magic[49] = 0x8D;
	magic[50] = 0xDD;
	magic[51] = 0x52;
	magic[52] = 0xCC;
	magic[53] = 0xC8;
	magic[54] = 0x29;
	magic[55] = 0xCE;
	magic[56] = 0x94;
	magic[57] = 0xE3;
	magic[58] = 0x79;
	magic[59] = 0x54;
	magic[60] = 0x75;
	magic[61] = 0xE2;
	magic[62] = 0x8D;
	magic[63] = 0xED;

	if (len > 64)
	{
		for (i = 0; i < (len - 64); i++)
		{
			magic[64 + i] = magic[63 - i] ^ magic[i];
		}
	}
}

/*****************************************************************
** 函数名   : key_jbox_data_encryption_create
** 输入     : src  -- 源数据
			  dst  -- 生成加密数据（生成的数据长度为 len + 2 + KEY_JBOX_ENCRY_KEY_LEN +1）
			  len  -- 要加密数据长度
** 返回     : 0 -- 成功
			  1 -- 失败
** 功能描述 : JBOX主机生成加密数据
****************************************************************/
#define KEY_JBOX_ENCRY_KEY_LEN 12
uint8_t key_jbox_data_encryption_create(uint8_t *src, uint8_t *dst, uint8_t len, uint8_t proto)
{
	uint8_t i, j;
	uint8_t temp_8;
	uint8_t num;
	uint8_t num_2;
	uint8_t key_checkout;
	uint8_t my_magic[100];
	uint8_t encry_data[101];
	uint8_t temp_key_buf[KEY_JBOX_ENCRY_KEY_LEN];

	// 获取随机数密钥
	for (i = 0; i < 12; i++)
	{
		temp_key_buf[i] = key_get_random(i);
	}

	// 校验位
	key_checkout = (temp_key_buf[9] ^ temp_key_buf[6]) + 9;

	if (ENCRY_PROTO_A == proto)
	{
		key_magic_init_get_1(my_magic, 100);
	}
	else if (ENCRY_PROTO_B == proto)
	{
		key_magic_init_get_2(my_magic, 100);
	}
	num = len / (KEY_JBOX_ENCRY_KEY_LEN - 3);
	num_2 = len - num * (KEY_JBOX_ENCRY_KEY_LEN - 3);

	// 数据加密
	for (i = 0; i < (KEY_JBOX_ENCRY_KEY_LEN - 3); i++)
	{
		for (j = 0; j < num; j++)
		{
			encry_data[i * num + j] = ((src[i * num + j] ^ my_magic[i * num + j]) ^ temp_key_buf[i + 2]) + j;
		}
	}

	if (num_2 > 0)
	{
		for (i = 0; i < num_2; i++)
		{
			encry_data[(KEY_JBOX_ENCRY_KEY_LEN - 3) * num + i] = ((src[(KEY_JBOX_ENCRY_KEY_LEN - 3) * num + i] ^ my_magic[(KEY_JBOX_ENCRY_KEY_LEN - 3) * num + i]) ^ temp_key_buf[9]) + i;
		}
	}

	/* 单独加密 */
	encry_data[len / 2 + 1] = encry_data[len / 2 + 1] + temp_key_buf[4] + (uint8_t)GetCrc_16((char *)&temp_key_buf[6], 1);
	encry_data[len / 4 + 1] = encry_data[len / 4 + 1] + temp_key_buf[2] + (uint8_t)GetCrc_16((char *)&temp_key_buf[5], 1);
	encry_data[len / 6 + 1] = encry_data[len / 6 + 1] + temp_key_buf[7] + (uint8_t)GetCrc_16((char *)&temp_key_buf[9], 1);
	encry_data[len / 8 + 1] = encry_data[len / 8 + 1] + temp_key_buf[3] + (uint8_t)GetCrc_16((char *)&temp_key_buf[8], 1);

	encry_data[len] = (key_checkout + temp_key_buf[7] + (uint8_t)GetCrc_16((char *)&temp_key_buf[3], 1)) ^ temp_key_buf[2];

	// 生成发送数据
	for (i = 0; i < (KEY_JBOX_ENCRY_KEY_LEN); i++)
	{
		temp_8 = i + KEY_JBOX_ENCRY_KEY_LEN;
		dst[i + 2] = (temp_key_buf[i] ^ (uint8_t)GetCrc_16((char *)&(temp_8), 1)) + i;
	}

	memcpy(&dst[KEY_JBOX_ENCRY_KEY_LEN + 2], encry_data, (len + 1));

	dst[0] = (dst[5] ^ dst[8]) + KEY_JBOX_ENCRY_KEY_LEN;
	dst[1] = (dst[3] ^ dst[10]) + len;

	return 0;
}

/*****************************************************************
** 函数名   : key_slave_decrypt_jbox_data
** 输入     : src  -- 源数据
			  dst  -- 解码后的数据
** 返回     : 0 -- 成功
			  1 -- 失败
** 功能描述 : 从机解码JBOX加密数据
****************************************************************/
uint8_t key_slave_decrypt_jbox_data(uint8_t *src, uint8_t *dst, uint8_t proto)
{
	uint8_t i, j;
	uint8_t temp_8;
	uint8_t num;
	uint8_t num_2;
	uint8_t key_len;
	uint8_t encry_len;
	uint8_t key_checkout;
	uint8_t my_magic[100];
	uint8_t encry_data[101];
	uint8_t temp_key_buf[KEY_JBOX_ENCRY_KEY_LEN];

	// 解码数据长度
	key_len = src[0] - (src[5] ^ src[8]);
	encry_len = src[1] - (src[3] ^ src[10]);

	if (key_len > KEY_JBOX_ENCRY_KEY_LEN)
	{
		return 1;
	}

	if (encry_len > 100)
	{
		return 1;
	}

	// 解随机数数列 -- 随机密钥
	for (i = 0; i < key_len; i++)
	{
		temp_8 = i + KEY_JBOX_ENCRY_KEY_LEN;
		temp_key_buf[i] = (src[i + 2] - i) ^ (((uint8_t)GetCrc_16((char *)&temp_8, 1)));
	}

	// 解码校验位
	key_checkout = (temp_key_buf[9] ^ temp_key_buf[6]) + 9;
	temp_8 = (src[encry_len + KEY_JBOX_ENCRY_KEY_LEN + 2] ^ temp_key_buf[2]) - temp_key_buf[7] - (uint8_t)GetCrc_16((char *)&temp_key_buf[3], 1);
	if (temp_8 != key_checkout)
	{
		/* 加密数据不合法  */
		return 1;
	}

	if (ENCRY_PROTO_A == proto)
	{
		key_magic_init_get_1(my_magic, 100);
	}
	else if (ENCRY_PROTO_B == proto)
	{
		key_magic_init_get_2(my_magic, 100);
	}
	num = encry_len / (key_len - 3);
	num_2 = encry_len - num * (KEY_JBOX_ENCRY_KEY_LEN - 3);

	// 解码数据
	src[encry_len / 2 + 1 + KEY_JBOX_ENCRY_KEY_LEN + 2] = src[encry_len / 2 + 1 + KEY_JBOX_ENCRY_KEY_LEN + 2] - temp_key_buf[4] - (uint8_t)GetCrc_16((char *)&temp_key_buf[6], 1);
	src[encry_len / 4 + 1 + KEY_JBOX_ENCRY_KEY_LEN + 2] = src[encry_len / 4 + 1 + KEY_JBOX_ENCRY_KEY_LEN + 2] - temp_key_buf[2] - (uint8_t)GetCrc_16((char *)&temp_key_buf[5], 1);
	src[encry_len / 6 + 1 + KEY_JBOX_ENCRY_KEY_LEN + 2] = src[encry_len / 6 + 1 + KEY_JBOX_ENCRY_KEY_LEN + 2] - temp_key_buf[7] - (uint8_t)GetCrc_16((char *)&temp_key_buf[9], 1);
	src[encry_len / 8 + 1 + KEY_JBOX_ENCRY_KEY_LEN + 2] = src[encry_len / 8 + 1 + KEY_JBOX_ENCRY_KEY_LEN + 2] - temp_key_buf[3] - (uint8_t)GetCrc_16((char *)&temp_key_buf[8], 1);

	for (i = 0; i < (key_len - 3); i++)
	{
		for (j = 0; j < num; j++)
		{
			temp_8 = src[i * num + j + KEY_JBOX_ENCRY_KEY_LEN + 2];
			dst[i * num + j] = ((temp_8 - j) ^ (temp_key_buf[i + 2])) ^ (my_magic[i * num + j]);
		}
	}

	if (num_2 > 0)
	{
		for (i = 0; i < num_2; i++)
		{
			temp_8 = src[(KEY_JBOX_ENCRY_KEY_LEN - 3) * num + i + KEY_JBOX_ENCRY_KEY_LEN + 2];
			dst[(KEY_JBOX_ENCRY_KEY_LEN - 3) * num + i] = ((temp_8 - i) ^ (temp_key_buf[9])) ^ (my_magic[(KEY_JBOX_ENCRY_KEY_LEN - 3) * num + i]);
		}
	}

	return 0;
}

/*****************************************************************
** 函数名   : key_master_ori_data_encryption
** 输入     : src  -- 源数据
			  dst  -- 生成加密数据（生成的数据长度为 128 + 2 + KEY_JBOX_ENCRY_KEY_LEN +1）
** 返回     : 0 -- 成功
			  1 -- 失败
** 功能描述 : 主机生成听筒原始排线I2C加密数据(src 数据长度固定为128)
****************************************************************/
uint8_t key_master_ori_data_encryption(uint8_t *src, uint8_t *dst, uint8_t proto)
{
	uint8_t i, j;
	uint8_t temp_8;
	uint8_t num;
	uint8_t num_2;
	uint8_t key_checkout;
	uint8_t my_magic[100];
	uint8_t encry_data[129];
	uint8_t temp_key_buf[KEY_JBOX_ENCRY_KEY_LEN];

	// 获取随机数密钥
	for (i = 0; i < 12; i++)
	{
		temp_key_buf[i] = key_get_random(i);
	}

	// 校验位
	key_checkout = (temp_key_buf[9] ^ temp_key_buf[6]) + 9;

	if (ENCRY_PROTO_A == proto)
	{
		key_magic_init_get_1(my_magic, 100);
	}
	else if (ENCRY_PROTO_B == proto)
	{
		key_magic_init_get_2(my_magic, 100);
	}

	// 数据加密
	for (i = 0; i < (KEY_JBOX_ENCRY_KEY_LEN - 4); i++)
	{
		for (j = 0; j < 8; j++)
		{
			encry_data[i * 8 + j] = ((src[i * 8 + j] ^ my_magic[i * 8 + j]) ^ temp_key_buf[i + 2]) + j;
		}
	}

	for (i = 0; i < (KEY_JBOX_ENCRY_KEY_LEN - 4); i++)
	{
		for (j = 0; j < 8; j++)
		{
			encry_data[64 + i * 8 + j] = ((src[64 + i * 8 + j] ^ my_magic[99 - i * 8 - j]) ^ temp_key_buf[i + 2]) + j;
		}
	}

	/* 单独加密 */
	encry_data[27] = encry_data[27] + temp_key_buf[4] + (uint8_t)GetCrc_16((char *)&temp_key_buf[6], 1);
	encry_data[43] = encry_data[43] + temp_key_buf[2] + (uint8_t)GetCrc_16((char *)&temp_key_buf[5], 1);
	encry_data[62] = encry_data[62] + temp_key_buf[7] + (uint8_t)GetCrc_16((char *)&temp_key_buf[9], 1);
	encry_data[72] = encry_data[72] + temp_key_buf[3] + (uint8_t)GetCrc_16((char *)&temp_key_buf[8], 1);

	encry_data[128] = (key_checkout + temp_key_buf[7] + (uint8_t)GetCrc_16((char *)&temp_key_buf[3], 1)) ^ temp_key_buf[2];

	// 生成发送数据
	for (i = 0; i < (KEY_JBOX_ENCRY_KEY_LEN); i++)
	{
		temp_8 = i + KEY_JBOX_ENCRY_KEY_LEN;
		dst[i + 2] = (temp_key_buf[i] ^ (uint8_t)GetCrc_16((char *)&(temp_8), 1)) + i;
	}

	memcpy(&dst[KEY_JBOX_ENCRY_KEY_LEN + 2], encry_data, 129);

	dst[0] = (dst[5] ^ dst[8]) + KEY_JBOX_ENCRY_KEY_LEN;
	dst[1] = (dst[3] ^ dst[10]) + 128;

	return 0;
}

/*****************************************************************
** 函数名   : key_slave_decrypt_ori_data
** 输入     : src  -- 源数据
			  dst  -- 解码后的数据
** 返回     : 0 -- 成功
			  1 -- 失败
** 功能描述 : 从机解码排线原始加密数据
****************************************************************/
uint8_t key_slave_decrypt_ori_data(uint8_t *src, uint8_t *dst, uint8_t proto)
{
	uint8_t i, j;
	uint8_t temp_8;
	uint8_t num;
	uint8_t num_2;
	uint8_t key_len;
	uint8_t encry_len;
	uint8_t key_checkout;
	uint8_t my_magic[100];
	uint8_t encry_data[128];
	uint8_t temp_key_buf[KEY_JBOX_ENCRY_KEY_LEN];

	// 解码数据长度
	key_len = src[0] - (src[5] ^ src[8]);
	encry_len = src[1] - (src[3] ^ src[10]);

	if (key_len > KEY_JBOX_ENCRY_KEY_LEN)
	{
		return 1;
	}

	if (encry_len > 128)
	{
		return 1;
	}

	// 解随机数数列 -- 随机密钥
	for (i = 0; i < key_len; i++)
	{
		temp_8 = i + KEY_JBOX_ENCRY_KEY_LEN;
		temp_key_buf[i] = (src[i + 2] - i) ^ (((uint8_t)GetCrc_16((char *)&temp_8, 1)));
	}

	// 解码校验位
	key_checkout = (temp_key_buf[9] ^ temp_key_buf[6]) + 9;
	temp_8 = (src[encry_len + KEY_JBOX_ENCRY_KEY_LEN + 2] ^ temp_key_buf[2]) - temp_key_buf[7] - (uint8_t)GetCrc_16((char *)&temp_key_buf[3], 1);
	if (temp_8 != key_checkout)
	{
		/* 加密数据不合法  */
		return 1;
	}

	if (ENCRY_PROTO_A == proto)
	{
		key_magic_init_get_1(my_magic, 100);
	}
	else if (ENCRY_PROTO_B == proto)
	{
		key_magic_init_get_2(my_magic, 100);
	}

	// 解码数据
	src[27 + KEY_JBOX_ENCRY_KEY_LEN + 2] = src[27 + KEY_JBOX_ENCRY_KEY_LEN + 2] - temp_key_buf[4] - (uint8_t)GetCrc_16((char *)&temp_key_buf[6], 1);
	src[43 + KEY_JBOX_ENCRY_KEY_LEN + 2] = src[43 + KEY_JBOX_ENCRY_KEY_LEN + 2] - temp_key_buf[2] - (uint8_t)GetCrc_16((char *)&temp_key_buf[5], 1);
	src[62 + KEY_JBOX_ENCRY_KEY_LEN + 2] = src[62 + KEY_JBOX_ENCRY_KEY_LEN + 2] - temp_key_buf[7] - (uint8_t)GetCrc_16((char *)&temp_key_buf[9], 1);
	src[72 + KEY_JBOX_ENCRY_KEY_LEN + 2] = src[72 + KEY_JBOX_ENCRY_KEY_LEN + 2] - temp_key_buf[3] - (uint8_t)GetCrc_16((char *)&temp_key_buf[8], 1);

	for (i = 0; i < (KEY_JBOX_ENCRY_KEY_LEN - 4); i++)
	{
		for (j = 0; j < 8; j++)
		{
			temp_8 = src[i * 8 + j + KEY_JBOX_ENCRY_KEY_LEN + 2];
			dst[i * 8 + j] = ((temp_8 - j) ^ (temp_key_buf[i + 2])) ^ (my_magic[i * 8 + j]);
		}
	}

	for (i = 0; i < (KEY_JBOX_ENCRY_KEY_LEN - 4); i++)
	{
		for (j = 0; j < 8; j++)
		{
			temp_8 = src[64 + i * 8 + j + KEY_JBOX_ENCRY_KEY_LEN + 2];
			dst[64 + i * 8 + j] = ((temp_8 - j) ^ (temp_key_buf[i + 2])) ^ (my_magic[99 - i * 8 - j]);
		}
	}

	return 0;
}

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
uint8_t key_jbox_data_encryption_create_by_master_key(uint8_t *key, uint8_t *src, uint8_t *dst, uint8_t len, uint8_t proto)
{
	uint8_t i, j;
	uint8_t temp_8;
	uint8_t num;
	uint8_t num_2;
	uint8_t key_checkout;
	uint8_t my_magic[100];
	uint8_t encry_data[101];
	uint8_t temp_key_buf[KEY_JBOX_ENCRY_KEY_LEN];

	// 获取随机数密钥
	memcpy(temp_key_buf, key, 12);

	// 校验位
	key_checkout = (temp_key_buf[9] ^ temp_key_buf[6]) + 9;

	if (ENCRY_PROTO_A == proto)
	{
		key_magic_init_get_1(my_magic, 100);
	}
	else if (ENCRY_PROTO_B == proto)
	{
		key_magic_init_get_2(my_magic, 100);
	}
	num = len / (KEY_JBOX_ENCRY_KEY_LEN - 3);
	num_2 = len - num * (KEY_JBOX_ENCRY_KEY_LEN - 3);

	// 数据加密
	for (i = 0; i < (KEY_JBOX_ENCRY_KEY_LEN - 3); i++)
	{
		for (j = 0; j < num; j++)
		{
			encry_data[i * num + j] = ((src[i * num + j] ^ my_magic[i * num + j]) ^ temp_key_buf[i + 2]) + j;
		}
	}

	if (num_2 > 0)
	{
		for (i = 0; i < num_2; i++)
		{
			encry_data[(KEY_JBOX_ENCRY_KEY_LEN - 3) * num + i] = ((src[(KEY_JBOX_ENCRY_KEY_LEN - 3) * num + i] ^ my_magic[(KEY_JBOX_ENCRY_KEY_LEN - 3) * num + i]) ^ temp_key_buf[9]) + i;
		}
	}

	/* 单独加密 */
	encry_data[len / 2 + 1] = encry_data[len / 2 + 1] + temp_key_buf[4] + (uint8_t)GetCrc_16((char *)&temp_key_buf[6], 1);
	encry_data[len / 4 + 1] = encry_data[len / 4 + 1] + temp_key_buf[2] + (uint8_t)GetCrc_16((char *)&temp_key_buf[5], 1);
	encry_data[len / 6 + 1] = encry_data[len / 6 + 1] + temp_key_buf[7] + (uint8_t)GetCrc_16((char *)&temp_key_buf[9], 1);
	encry_data[len / 8 + 1] = encry_data[len / 8 + 1] + temp_key_buf[3] + (uint8_t)GetCrc_16((char *)&temp_key_buf[8], 1);

	encry_data[len] = (key_checkout + temp_key_buf[7] + (uint8_t)GetCrc_16((char *)&temp_key_buf[3], 1)) ^ temp_key_buf[2];

	// 生成发送数据
	for (i = 0; i < (KEY_JBOX_ENCRY_KEY_LEN); i++)
	{
		temp_8 = i + KEY_JBOX_ENCRY_KEY_LEN;
		dst[i + 2] = (temp_key_buf[i] ^ (uint8_t)GetCrc_16((char *)&(temp_8), 1)) + i;
	}

	memcpy(&dst[KEY_JBOX_ENCRY_KEY_LEN + 2], encry_data, (len + 1));

	dst[0] = (dst[5] ^ dst[8]) + KEY_JBOX_ENCRY_KEY_LEN;
	dst[1] = (dst[3] ^ dst[10]) + len;

	return 0;
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
