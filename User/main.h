/*------------------------------------------------------------------------------
 * Copyright (c) 2020 JCXX. All rights reserved.
 *------------------------------------------------------------------------------
 * Autor:   lsp
 * Name:    main.h
 * Purpose: home key
 *----------------------------------------------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H 
#include "n32g032.h"

//#define USING_N32_FUNC

typedef enum
{
    TYPE_NUM = 0,
    TYPE_MINI_PLUS,
    TYPE_PRO,
    TYPE_PROMAX,
    TYPE_MAX
}fpc_series_type_e;

//激活手机信息
typedef struct{
    uint8_t      camera_sn_size;
    uint8_t      camera_sn[14]; //像头码
    uint8_t      ecid_size;
    uint8_t      ecid[16];
    uint8_t      phone_name_size;
    uint8_t      phone_name[32];
    uint8_t      times;
}fpc_act_phone_data_t;

typedef struct
{
    uint8_t     enable;
    uint8_t     iic_v1s;
    
    char        soft_ver[32];

    fpc_act_phone_data_t fpc_act_phone_data;
    // uint8_t     camera_sn_size;
    // uint8_t     camera_sn[16];
    fpc_series_type_e fpc_series_type;
    
    uint8_t     data_in[128];
    uint8_t     data_out[128];
    
}camera_param_t;

#define CAMERA_SERIES   12

#if(CAMERA_SERIES == 8)
#define CAMERA_VERSION      "JC_CAM_WIDE_NT_V5.00_X"
#elif(CAMERA_SERIES == 10)
#define CAMERA_VERSION      "JC_CAM_WIDE_NT_V5.00_X"
#elif(CAMERA_SERIES == 11)
#define CAMERA_VERSION      "JC_CAM_WIDE_NT_V5.00_X1"
#elif(CAMERA_SERIES == 12)
#define CAMERA_VERSION      "JC_CAM_WIDE_NT_V5.00_X2"
#elif(CAMERA_SERIES == 13)
#define CAMERA_VERSION      "JC_CAM_WIDE_NT_V5.00_X3"
#elif(CAMERA_SERIES == 14)
#define CAMERA_VERSION      "JC_CAM_WIDE_NT_V5.00_X4"
#elif(CAMERA_SERIES == 15)
#define CAMERA_VERSION      "JC_CAM_WIDE_NT_V5.00_X5"
#endif

#define SN_SIZE_SERISE   11

#define SAVE_CAM_DATA_SIZE 62

#define TRUE    (1)
#define FALSE   (0)

#define ADDR_FLASH_BASE         (0x08000000)
#define ADDR_RAM_BASE           (0x20000000)
#define BYTE_FLASH_MAX          (0x08010000)
#define BYTE_RAM_MAX            (0x20004000)

#define  CHIP_FLASH_PAGE_SIZE             512

#define  SECURITY_READ_LEN                 (16)

//0x0800EC00
#define  ADDR_FLASH_INIT                   (BYTE_FLASH_MAX  - CHIP_FLASH_PAGE_SIZE * 10)  
#define  ADDR_FLASH_MAGIC                  (ADDR_FLASH_INIT + 256)
#define  ADDR_FLASH_MAGIC_END              (BYTE_FLASH_MAX  - CHIP_FLASH_PAGE_SIZE * 9)
//0x0800F000
#define  ADDR_FLASH_RSA                    (BYTE_FLASH_MAX  - CHIP_FLASH_PAGE_SIZE * 8)  
#define  ADDR_FLASH_RSA_END                (BYTE_FLASH_MAX  - CHIP_FLASH_PAGE_SIZE * 7)
//0x0800F400
#define  ADDR_FLASH_INFO                   (BYTE_FLASH_MAX  - CHIP_FLASH_PAGE_SIZE * 6)  
#define  ADDR_FLASH_INFO_END               (BYTE_FLASH_MAX  - CHIP_FLASH_PAGE_SIZE * 5)
//0x0800F800
#define  ADDR_FLASH_SAVE                   (BYTE_FLASH_MAX  - CHIP_FLASH_PAGE_SIZE * 4)//2k
#define  ADDR_FLASH_SAVE_END               (BYTE_FLASH_MAX  - CHIP_FLASH_PAGE_SIZE * 0)

#define IIC_CAMER_ADDR          (0x20)
#define IIC_CHECK_ADDR          (0x26)
#define IIC_ADPAT_ADDR          (0x28)
#define IIC_CAMER_REG0          (0x33)
#define IIC_CAMER_REG1          (0x00)
#define IIC_CAMER_REG2          (0x29)

#define IIC_HEAD1_ADDR          (0xAC)
#define IIC_HEAD2_ADDR          (0xCA)
#define IIC_HEAD3_ADDR          (0xAC)

#define IIC_HEAD1_ADDR_PRO      (0xAA)
#define IIC_HEAD2_ADDR_PRO      (0xDD)


#define V1S_CMD_VERSION     (0x10)  //读版本
#define V1S_CMD_WR_ENCR     (0x99)  //写数据
#define V1S_CMD_RD_ENCR     (0x9B)  //读数据
#define V1S_CMD_RD_RESL     (0x9A)  //清数据
#define V1S_CMD_PROTO_C     (0x9C)
#define V1S_CMD_PROTO_F     (0x9D)
#define V1S_CMD_PROTO_G     (0x9E)
#define V1S_CMD_PROTO_W     (0x9F)
#define V1S_CMD_PROTO_K     (0xE1)
#define V1S_CMD_PROTO_R     (0xE2)
#define V1S_CMD_PROTO_S     (0xE3)

#define CAM_DATA_PACK_MAX     (2)
#define CAM_DATA_PACK_LEN     (54)
#define RSA_MAX     (4)
#define RSA_LEN     (43)
#define MAGIC_LEN   (32)
#define DECR_LEN    (84)    // 132/4*3-15
#define ENCR_LEN    (132)   // 99/3*4
#define DECR_PLEN   (60)    // 100/4*3-15
#define ENCR_PLEN   (100)   // 75/3*4
#define PACK_LEN    (64)
#define PACK_NUM    (0x02)

#define PORT_SCL_M  GPIOB
#define PIN_SCL_M   GPIO_PIN_6

#define PORT_SDA_M  GPIOB
#define PIN_SDA_M   GPIO_PIN_7

#define PORT_SHDN   GPIOA
#define PIN_SHDN    GPIO_PIN_14

#define SET_IIC_SDA_M_HIG       (PORT_SDA_M->PBSC = PIN_SDA_M)
#define SET_IIC_SDA_M_LOW       (PORT_SDA_M->PBC  = PIN_SDA_M)

#define SET_IIC_SCL_M_HIG       (PORT_SCL_M->PBSC = PIN_SCL_M)
#define SET_IIC_SCL_M_LOW       (PORT_SCL_M->PBC  = PIN_SCL_M)

#define IS_IIC_SCL_MON_HIG      (pval & PIN_SCL_M)
#define IS_IIC_SDA_MON_HIG      (pval & PIN_SDA_M)

#define WAIT_IIC_SCL_M_HIG      do{pval=PORT_SCL_M->PID;}while(!(pval&PIN_SCL_M))
#define WAIT_IIC_SCL_M_LOW      do{pval=PORT_SCL_M->PID;}while( (pval&PIN_SCL_M))

#define WAIT_IIC_SDA_M_HIG      do{pval=PORT_SDA_M->PID;}while(!(pval&PIN_SDA_M))
#define WAIT_IIC_SDA_M_LOW      do{pval=PORT_SDA_M->PID;}while( (pval&PIN_SDA_M))

#define WAIT_IIC_MON_START      WAIT_IIC_SDA_M_HIG;WAIT_IIC_SDA_M_LOW
#define WAIT_IIC_MON_STOP       WAIT_IIC_SDA_M_LOW;WAIT_IIC_SDA_M_HIG

#define WAIT_RCV_IIC_M_CLK      WAIT_IIC_SCL_M_LOW;WAIT_IIC_SCL_M_HIG
#define WAIT_SND_IIC_M_CLK      WAIT_IIC_SCL_M_HIG;WAIT_IIC_SCL_M_LOW

#define RECV_IIC_MON_ACK        WAIT_IIC_SCL_M_LOW;WAIT_IIC_SCL_M_HIG//;IS_IIC_SDA_MON_HIG
#define RECV_IIC_CTR_ACK        WAIT_IIC_SCL_M_LOW;SET_IIC_SDA_M_LOW;WAIT_IIC_SCL_M_HIG;WAIT_IIC_SCL_M_LOW;SET_IIC_SDA_M_HIG
#define SEND_IIC_CTR_ACK        SET_IIC_SDA_M_HIG;WAIT_IIC_SCL_M_HIG;WAIT_IIC_SCL_M_LOW

#define SHDW_OPEN    GPIO_WriteBit(PORT_SHDN, PIN_SHDN, Bit_RESET)
#define SHDW_CLOSE   GPIO_WriteBit(PORT_SHDN, PIN_SHDN, Bit_SET)

#define KEY_IN  GPIO_ReadInputDataBit(GPIOA, GPIO_PIN_4)

#endif


