/*------------------------------------------------------------------------------
 * Copyright (c) 2020 JCXX. All rights reserved.
 *------------------------------------------------------------------------------
 * Autor:   lsp
 * Name:    main.c
 * Purpose: home key
 *----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "n32g032.h"
#include "systick.h"
#include "main.h"
#include "slave_iic.h"
#include "key_aes.h"
#include "base64.h"
#include "md5.h"
#include "camera_data.h"
#include "n32g032_rcc.h"
#include "misc.h"
#include "usart.h"

/*----------------------------------------------------------------------------*/
static unsigned char cpu_id[12];
static unsigned char magic_key[MAGIC_LEN];
static unsigned char magic_calc[MAGIC_LEN];
static unsigned char magic_read[MAGIC_LEN];

uint8_t iic_rx[256];
uint8_t iic_tx[256];
static uint8_t encr_buf[256];
static uint8_t decr_buf[256];
static uint8_t rsa_buf[256];
static uint32_t iic_data_tick = 0;
static uint32_t iic_pack_tick = 0;
static uint8_t *i2c_ack_ptr = NULL; /*  i2c应答指针所指向的数据 */

static camera_param_t camera = {0};

/*----------------------------------------------------------------------------*/
const uint16_t crc16_tab[256] = {
    0x0000, 0xc0c1, 0xc181, 0x0140, 0xc301, 0x03c0, 0x0280, 0xc241,
    0xc601, 0x06c0, 0x0780, 0xc741, 0x0500, 0xc5c1, 0xc481, 0x0440,
    0xcc01, 0x0cc0, 0x0d80, 0xcd41, 0x0f00, 0xcfc1, 0xce81, 0x0e40,
    0x0a00, 0xcac1, 0xcb81, 0x0b40, 0xc901, 0x09c0, 0x0880, 0xc841,
    0xd801, 0x18c0, 0x1980, 0xd941, 0x1b00, 0xdbc1, 0xda81, 0x1a40,
    0x1e00, 0xdec1, 0xdf81, 0x1f40, 0xdd01, 0x1dc0, 0x1c80, 0xdc41,
    0x1400, 0xd4c1, 0xd581, 0x1540, 0xd701, 0x17c0, 0x1680, 0xd641,
    0xd201, 0x12c0, 0x1380, 0xd341, 0x1100, 0xd1c1, 0xd081, 0x1040,
    0xf001, 0x30c0, 0x3180, 0xf141, 0x3300, 0xf3c1, 0xf281, 0x3240,
    0x3600, 0xf6c1, 0xf781, 0x3740, 0xf501, 0x35c0, 0x3480, 0xf441,
    0x3c00, 0xfcc1, 0xfd81, 0x3d40, 0xff01, 0x3fc0, 0x3e80, 0xfe41,
    0xfa01, 0x3ac0, 0x3b80, 0xfb41, 0x3900, 0xf9c1, 0xf881, 0x3840,
    0x2800, 0xe8c1, 0xe981, 0x2940, 0xeb01, 0x2bc0, 0x2a80, 0xea41,
    0xee01, 0x2ec0, 0x2f80, 0xef41, 0x2d00, 0xedc1, 0xec81, 0x2c40,
    0xe401, 0x24c0, 0x2580, 0xe541, 0x2700, 0xe7c1, 0xe681, 0x2640,
    0x2200, 0xe2c1, 0xe381, 0x2340, 0xe101, 0x21c0, 0x2080, 0xe041,
    0xa001, 0x60c0, 0x6180, 0xa141, 0x6300, 0xa3c1, 0xa281, 0x6240,
    0x6600, 0xa6c1, 0xa781, 0x6740, 0xa501, 0x65c0, 0x6480, 0xa441,
    0x6c00, 0xacc1, 0xad81, 0x6d40, 0xaf01, 0x6fc0, 0x6e80, 0xae41,
    0xaa01, 0x6ac0, 0x6b80, 0xab41, 0x6900, 0xa9c1, 0xa881, 0x6840,
    0x7800, 0xb8c1, 0xb981, 0x7940, 0xbb01, 0x7bc0, 0x7a80, 0xba41,
    0xbe01, 0x7ec0, 0x7f80, 0xbf41, 0x7d00, 0xbdc1, 0xbc81, 0x7c40,
    0xb401, 0x74c0, 0x7580, 0xb541, 0x7700, 0xb7c1, 0xb681, 0x7640,
    0x7200, 0xb2c1, 0xb381, 0x7340, 0xb101, 0x71c0, 0x7080, 0xb041,
    0x5000, 0x90c1, 0x9181, 0x5140, 0x9301, 0x53c0, 0x5280, 0x9241,
    0x9601, 0x56c0, 0x5780, 0x9741, 0x5500, 0x95c1, 0x9481, 0x5440,
    0x9c01, 0x5cc0, 0x5d80, 0x9d41, 0x5f00, 0x9fc1, 0x9e81, 0x5e40,
    0x5a00, 0x9ac1, 0x9b81, 0x5b40, 0x9901, 0x59c0, 0x5880, 0x9841,
    0x8801, 0x48c0, 0x4980, 0x8941, 0x4b00, 0x8bc1, 0x8a81, 0x4a40,
    0x4e00, 0x8ec1, 0x8f81, 0x4f40, 0x8d01, 0x4dc0, 0x4c80, 0x8c41,
    0x4400, 0x84c1, 0x8581, 0x4540, 0x8701, 0x47c0, 0x4680, 0x8641,
    0x8201, 0x42c0, 0x4380, 0x8341, 0x4100, 0x81c1, 0x8081, 0x4040};

/* about flash */
/*----------------------------------------------------------------------------*/

/*!
    \brief      erase fmc pages from FMC_WRITE_START_ADDR to FMC_WRITE_END_ADDR
    \param[in]  none
    \param[out] none
    \retval     none
*/
void fmc_write_data(uint32_t addr, uint8_t *data, uint32_t len)
{
    uint32_t i = 0;
    uint32_t v = 0;

    /* Unlocks the FLASH Program Erase Controller */
    FLASH_Unlock();

    /* Erase */
    FLASH_EraseOnePage(addr);

    /* Program */
    for (i = 0; i < len; i += 4)
    {
        v = data[i + 0] + (data[i + 1] << 8) + (data[i + 2] << 16) + (data[i + 3] << 24);
        FLASH_ProgramWord(addr + i, v);
    }

    /* Locks the FLASH Program Erase Controller */
    FLASH_Lock();
}

/**
 * @brief erase fmc pages
 * @param addr
 */
void fmc_erase_data(uint32_t addr)
{
    /* Unlocks the FLASH Program Erase Controller */
    FLASH_Unlock();

    /* Erase */
    FLASH_EraseOnePage(addr);

    /* Locks the FLASH Program Erase Controller */
    FLASH_Lock();
}

/*----------------------------------------------------------------------------*/
void inv_uint8_t(uint8_t *dstbuf, uint8_t *srcbuf)
{
    int i;
    uint8_t temp = 0;

    for (i = 0; i < 8; i++)
    {
        if (srcbuf[0] & (1 << i))
        {
            temp |= 1 << (7 - i);
        }
    }

    dstbuf[0] = temp;
}

/*----------------------------------------------------------------------------*/
void inv_uint16_t(uint16_t *dstbuf, uint16_t *srcbuf)
{
    int i;
    uint16_t temp = 0;

    for (i = 0; i < 16; i++)
    {
        if (srcbuf[0] & (1 << i))
        {
            temp |= 1 << (15 - i);
        }
    }

    dstbuf[0] = temp;
}

/*----------------------------------------------------------------------------*/
uint16_t get_crc_code_by_tabl(const uint8_t *buf, uint32_t len, uint16_t init)
{
    uint32_t i = 0;
    uint16_t crc = init;

    for (i = 0; i < len; i++)
    {
        crc = crc16_tab[(crc ^ buf[i]) & 0xff] ^ ((crc >> 8) & 0xff);
    }

    return crc;
}

/*----------------------------------------------------------------------------*/
void magic_crc_calc(const uint8_t *key, uint32_t key_len, uint8_t *id_ptr, uint32_t id_len, uint8_t *calc)
{
    uint32_t i = 0;
    uint32_t k = 0;
    uint16_t crc = 0xFFFF;
    uint16_t poly = 0x8005;
    uint8_t data = 0;
    uint32_t len = 0;
    uint8_t *id = NULL;

    for (k = 0; k < key_len; k += 2)
    {
        len = id_len;
        id = id_ptr;
        crc = key[k + 0] * 256 + key[k + 1];
        while (len--)
        {
            data = *(id++);
            inv_uint8_t(&data, &data);
            crc ^= (data << 8);

            for (i = 0; i < 8; i++)
            {
                if (crc & 0x8000)
                {
                    crc = (crc << 1) ^ poly;
                }
                else
                {
                    crc = crc << 1;
                }
            }
        }
        inv_uint16_t(&crc, &crc);
        calc[k + 0] = (crc >> 8) & 0x00FF;
        calc[k + 1] = (crc >> 0) & 0x00FF;
    }
}

/*----------------------------------------------------------------------------*/
void board_calc_magic(uint8_t *magic)
{
    magic_crc_calc(magic_key, MAGIC_LEN, cpu_id, 12, magic);
}

/*----------------------------------------------------------------------------*/
void board_read_magic(uint32_t addr, uint8_t *magic)
{
    memcpy(magic, (uint8_t *)addr, MAGIC_LEN);
}

/*----------------------------------------------------------------------------*/
void board_write_magic(uint32_t addr, uint8_t *magic)
{
    if (0 == memcmp((uint8_t *)ADDR_FLASH_INIT, magic_key, MAGIC_LEN))
    {
        fmc_write_data(addr, magic, MAGIC_LEN);
    }
}

/*----------------------------------------------------------------------------*/
void board_chipid_pointer(uint8_t *addr)
{
    addr[0] = 0xFD;
    addr[0]--;
    addr[2] = 0xFE;
    addr[2]++;
    addr[1] = 0xF3;
    addr[1]++;
    addr[3] = 0x20;
    addr[3]--;
}

/*----------------------------------------------------------------------------*/

/*65 1A AB 87 D4 7B 33 35 20 18 66 A6 33 8A 9C C7 C3 4B D9 F2 DC D9 13 F5 3C FF EA F6 69 A0 6B 3E*/
void board_get_magic(uint8_t *magic)
{
    magic[0] = 0x65;
    magic[1] = 0x1A;
    magic[2] = 0xAB;
    magic[3] = 0x87;
    magic[4] = 0xD4;
    magic[5] = 0x7B;
    magic[6] = 0x33;
    magic[7] = 0x35;
    magic[8] = 0x20;
    magic[9] = 0x18;
    magic[10] = 0x66;
    magic[11] = 0xA6;
    magic[12] = 0x33;
    magic[13] = 0x8A;
    magic[14] = 0x9C;
    magic[15] = 0xC7;
    magic[16] = 0xC3;
    magic[17] = 0x4B;
    magic[18] = 0xD9;
    magic[19] = 0xF2;
    magic[20] = 0xDC;
    magic[21] = 0xD9;
    magic[22] = 0x13;
    magic[23] = 0xF5;
    magic[24] = 0x3C;
    magic[25] = 0xFF;
    magic[26] = 0xEA;
    magic[27] = 0xF6;
    magic[28] = 0x69;
    magic[29] = 0xA0;
    magic[30] = 0x6B;
    magic[31] = 0x3E;
}

/*----------------------------------------------------------------------------*/
void board_magic_init(void)
{
    uint8_t i = 0;
    uint32_t point = 0;

    // get cpu id from address
    // memcpy(cpu_id, (uint8_t *)(0x1FFFF4FC), 12);
    board_chipid_pointer((uint8_t *)&point);
    memcpy(cpu_id, (uint8_t *)point, 12);

    board_get_magic(magic_key);
    board_calc_magic(magic_calc);
    board_write_magic(ADDR_FLASH_MAGIC, magic_calc);
    board_read_magic(ADDR_FLASH_MAGIC, magic_read);

    if (0 != memcmp(magic_read, magic_calc, 32))
    {
        // for(i=0; i<1000; i++);
    }

    board_write_magic(ADDR_FLASH_INIT, magic_calc);
}

/*!
    \brief      camera power init
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_init(void)
{
    GPIO_InitType GPIO_InitStructure;

    /* Enable the GPIO Clock */
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE);

    /* Configure the GPIO pin */
    GPIO_InitStruct(&GPIO_InitStructure);
    GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.GPIO_Pull = GPIO_PULL_DOWN;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);

    GPIO_WriteBit(GPIOB, GPIO_PIN_1, Bit_SET);
    GPIO_WriteBit(GPIOB, GPIO_PIN_6, Bit_SET);
    GPIO_WriteBit(GPIOB, GPIO_PIN_7, Bit_SET);
}

void key_init(void)
{
    GPIO_InitType GPIO_InitStructure;

    /* Enable the GPIO Clock */
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);

    /* Configure the GPIO pin */
    GPIO_InitStruct(&GPIO_InitStructure);
    GPIO_InitStructure.Pin = GPIO_PIN_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.GPIO_Pull = GPIO_PULL_UP;
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
}

/*!
    \brief      main
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    
}
