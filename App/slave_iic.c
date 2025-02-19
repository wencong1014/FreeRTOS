/*------------------------------------------------------------------------------
 * Copyright (c) 2020 JCXX. All rights reserved.
 *------------------------------------------------------------------------------
 * Autor:   lsp
 * Name:    slave_iic.c
 * Purpose: battery iic
 *----------------------------------------------------------------------------*/
#include <stdio.h>
#include "n32g032.h"
#include "systick.h"
#include "main.h"
#include "slave_iic.h"
#include "key_aes.h"
#include "base64.h"

extern uint8_t iic_rx[];
extern uint8_t iic_tx[];
extern uint8_t power_on_status;

/*!
    \brief      recv one byte
    \param[in]  none
    \param[out] none
    \retval     none
*/
uint8_t iic_monitor_recv(void)
{
    uint32_t i = 0;
    uint32_t pval = 0;
    uint8_t data = 0;

    for (i = 0; i < 8; i++)
    {
        WAIT_RCV_IIC_M_CLK;
        if (IS_IIC_SDA_MON_HIG)
        {
            data |= 0x80 >> i;
        }
    }

    RECV_IIC_MON_ACK;

    return data;
}

/*!
    \brief      recv one byte
    \param[in]  none
    \param[out] none
    \retval     none
*/
uint8_t iic_control_recv(void)
{
    uint32_t i = 0;
    uint32_t pval = 0;
    uint8_t data = 0;
    // uint32_t tous = 0;

    for (i = 0; i < 8; i++)
    {
        WAIT_RCV_IIC_M_CLK;
        if (IS_IIC_SDA_MON_HIG)
        {
            data |= 0x80 >> i;
        }
    }

    RECV_IIC_CTR_ACK;

    return data;
}

/*!
    \brief      send one byte
    \param[in]  none
    \param[out] none
    \retval     none
*/
void iic_control_send(uint8_t data)
{
    uint32_t i = 0;
    uint32_t pval = 0;
    // uint32_t tous = 0;

    for (i = 0; i < 8; i++)
    {
        if (data & (0x80 >> i))
        {
            SET_IIC_SDA_M_HIG;
        }
        else
        {
            SET_IIC_SDA_M_LOW;
        }

        WAIT_SND_IIC_M_CLK;
    }

    SEND_IIC_CTR_ACK;
}

/*!
    \brief      slave_iic_process
    \param[in]  batt_param_t
    \param[out] none
    \retval     none
*/
void slave_iic_process(camera_param_t *cam)
{
    uint32_t i = 0;
    uint32_t pval = 0;

    uint8_t data = 0;
    uint8_t dma_ready = false;
    static uint8_t dma_cnt = 0;

    uint8_t cmand = 0x00;
    uint8_t head1 = 0x00;
    uint8_t head2 = 0x00;
    uint8_t head3 = 0x00;

    uint8_t reset_falg = 0;

    while (1)
    {
        __disable_irq();
        WAIT_IIC_MON_START;
        if (IS_IIC_SCL_MON_HIG)
        {
            if (TRUE == cam->iic_v1s)
            {
                data = iic_control_recv();
                if (IIC_ADPAT_ADDR == data)
                {
                    head1 = iic_control_recv();
                    head2 = iic_control_recv();
                    head3 = iic_control_recv();

                    // 排线读写协议
                    if ((head1 == IIC_HEAD1_ADDR) && (head2 == IIC_HEAD2_ADDR) && (head3 == IIC_HEAD3_ADDR))
                    {
                        cmand = iic_control_recv();

                        if (V1S_CMD_VERSION == cmand)
                        {
                            for (i = 0; i < 12; i++)
                            {
                                iic_rx[i] = iic_control_recv();
                            }

                            key_jbox_data_encryption_create_by_master_key(&iic_rx[0], (uint8_t *)cam->soft_ver, iic_tx, 25, ENCRY_PROTO_B);

                            WAIT_IIC_MON_START;
                            if (IS_IIC_SCL_MON_HIG)
                            {
                                data = iic_control_recv();
                                for (i = 0; i < 40; i++)
                                {
                                    iic_control_send(iic_tx[i]);
                                }
                            }
                        }
                        else if (V1S_CMD_WR_ENCR == cmand)
                        {
                            for (i = 0; i < ENCR_PLEN; i++)
                            {
                                iic_rx[i] = iic_control_recv();
                            }

                            read_camera_param_from_data(iic_rx, cam->data_in);
                            save_camera_param_to_flash(cam->data_in, iic_tx);
                            memcpy(&cam->fpc_act_phone_data.camera_sn_size, (void *)ADDR_FLASH_SAVE, sizeof(fpc_act_phone_data_t));

                            WAIT_IIC_MON_START;
                            if (IS_IIC_SCL_MON_HIG)
                            {
                                data = iic_control_recv();
                                for (i = 0; i < 5; i++)
                                {
                                    iic_control_send(iic_tx[i]);
                                }
                            }
                        }
                        else if (V1S_CMD_RD_ENCR == cmand)
                        {
                            iic_rx[0] = iic_control_recv();

                            pack_camera_param_to_data(iic_rx, iic_tx);

                            WAIT_IIC_MON_START;
                            if (IS_IIC_SCL_MON_HIG)
                            {
                                data = iic_control_recv();
                                for (i = 0; i < ENCR_PLEN; i++)
                                {
                                    iic_control_send(iic_tx[i]);
                                }
                            }
                        }
                        else if (V1S_CMD_RD_RESL == cmand)
                        {
                            iic_tx[0] = 0xAA;
                            iic_tx[1] = 0xDD;
                            iic_tx[2] = V1S_CMD_RD_RESL;
                            iic_tx[3] = 0x01;

                            WAIT_IIC_MON_START;
                            if (IS_IIC_SCL_MON_HIG)
                            {
                                data = iic_control_recv();
                                for (i = 0; i < 4; i++)
                                {
                                    iic_control_send(iic_tx[i]);
                                }
                            }
                        }
                    }
                    // 鉴权协议、BOOT升级复位引导协议
                    else if ((head1 == IIC_HEAD1_ADDR_PRO) && (head2 == IIC_HEAD2_ADDR_PRO))
                    {
                        cmand = head3;

                        if (V1S_CMD_PROTO_C == cmand)
                        {
                            for (i = 0; i < ENCR_PLEN; i++)
                            {
                                iic_rx[i] = iic_control_recv();
                            }

                            read_camera_data_from_proto(iic_rx, cam->data_in);
                            calc_camera_cpuid_to_data(cam->data_in, cam->data_out);
                            pack_camera_data_to_proto(cam->data_out, iic_tx);

                            WAIT_IIC_MON_START;
                            if (IS_IIC_SCL_MON_HIG)
                            {
                                data = iic_control_recv();
                                for (i = 0; i < ENCR_PLEN; i++)
                                {
                                    iic_control_send(iic_tx[i]);
                                }
                            }
                        }
                        else if (V1S_CMD_PROTO_F == cmand)
                        {
                            for (i = 0; i < ENCR_PLEN; i++)
                            {
                                iic_rx[i] = iic_control_recv();
                            }

                            read_camera_data_from_proto(iic_rx, cam->data_in);
                            calc_camera_flash_to_data(cam->data_in, cam->data_out);
                            pack_camera_data_to_proto(cam->data_out, iic_tx);

                            WAIT_IIC_MON_START;
                            if (IS_IIC_SCL_MON_HIG)
                            {
                                data = iic_control_recv();
                                for (i = 0; i < ENCR_PLEN; i++)
                                {
                                    iic_control_send(iic_tx[i]);
                                }
                            }
                        }
                        else if (V1S_CMD_PROTO_G == cmand)
                        {
                            for (i = 0; i < ENCR_PLEN; i++)
                            {
                                iic_rx[i] = iic_control_recv();
                            }

                            read_camera_data_from_proto(iic_rx, cam->data_in);
                            calc_camera_read_rsa_to_data(cam->data_in, cam->data_out);
                            pack_camera_data_to_proto(cam->data_out, iic_tx);

                            WAIT_IIC_MON_START;
                            if (IS_IIC_SCL_MON_HIG)
                            {
                                data = iic_control_recv();
                                for (i = 0; i < ENCR_PLEN; i++)
                                {
                                    iic_control_send(iic_tx[i]);
                                }
                            }
                        }
                        else if (V1S_CMD_PROTO_W == cmand)
                        {
                            for (i = 0; i < ENCR_PLEN; i++)
                            {
                                iic_rx[i] = iic_control_recv();
                            }

                            read_camera_data_from_proto(iic_rx, cam->data_in);
                            calc_camera_write_rsa_to_data(cam->data_in, cam->data_out);
                            pack_camera_data_to_proto(cam->data_out, iic_tx);

                            WAIT_IIC_MON_START;
                            if (IS_IIC_SCL_MON_HIG)
                            {
                                data = iic_control_recv();
                                for (i = 0; i < ENCR_PLEN; i++)
                                {
                                    iic_control_send(iic_tx[i]);
                                }
                            }
                        }
                        else if (V1S_CMD_PROTO_K == cmand)
                        {
                            for (i = 0; i < ENCR_PLEN; i++)
                            {
                                iic_rx[i] = iic_control_recv();
                            }

                            read_camera_data_from_proto(iic_rx, cam->data_in);
                            calc_camera_md5_to_data(cam->data_in, cam->data_out);
                            pack_camera_data_to_proto(cam->data_out, iic_tx);

                            WAIT_IIC_MON_START;
                            if (IS_IIC_SCL_MON_HIG)
                            {
                                data = iic_control_recv();
                                for (i = 0; i < ENCR_PLEN; i++)
                                {
                                    iic_control_send(iic_tx[i]);
                                }
                            }
                        }
                        else if (V1S_CMD_PROTO_R == cmand)
                        {
                            for (i = 0; i < ENCR_PLEN; i++)
                            {
                                iic_rx[i] = iic_control_recv();
                            }

                            read_camera_data_from_proto(iic_rx, cam->data_in);
                            calc_camera_ram_to_data(cam->data_in, cam->data_out);
                            pack_camera_data_to_proto(cam->data_out, iic_tx);

                            WAIT_IIC_MON_START;
                            if (IS_IIC_SCL_MON_HIG)
                            {
                                data = iic_control_recv();
                                for (i = 0; i < ENCR_PLEN; i++)
                                {
                                    iic_control_send(iic_tx[i]);
                                }
                            }
                        }
                        else if (V1S_CMD_PROTO_S == cmand)
                        {
                            for (i = 0; i < ENCR_PLEN; i++)
                            {
                                iic_rx[i] = iic_control_recv();
                            }

                            read_camera_data_from_proto(iic_rx, cam->data_in);
                            reset_falg = proc_batt_reset_to_data(cam->data_in, cam->data_out);
                            pack_camera_data_to_proto(cam->data_out, iic_tx);

                            WAIT_IIC_MON_START;
                            if (IS_IIC_SCL_MON_HIG)
                            {
                                data = iic_control_recv();
                                for (i = 0; i < ENCR_PLEN; i++)
                                {
                                    iic_control_send(iic_tx[i]);
                                }
                                if (reset_falg == 0xB1)
                                {
                                    NVIC_SystemReset();
                                }
                            }
                        }
                    }
                }
                else if (IIC_CHECK_ADDR == data)
                {
                    cam->iic_v1s = TRUE;
                }           
                else
                {
                    cam->iic_v1s = FALSE;
                }
            }
            else
            {
                data = iic_monitor_recv();

                // 监听IIC
                if (IIC_CAMER_ADDR == data)
                {
                    data = iic_monitor_recv();
                    if (IIC_CAMER_REG0 == data)
                    {
                        data = iic_monitor_recv();
                        if (IIC_CAMER_REG1 == data)
                        {
                            data = iic_monitor_recv();
                            if (IIC_CAMER_REG2 == data)
                            {
                                dma_cnt++;
                                if(1 == dma_cnt)
                                    dma_ready = true;
                            }
                        }
//                        else if(IIC_CAMER_REG2 == data)
//                        {
//                            if(dma_cnt == 1)
//                            {
//                                dma_ready = true;
//                            }
//                        }
                    }
                }
                else if (IIC_CHECK_ADDR == data)
                {
                    cam->iic_v1s = TRUE;
                }
                else
                {
                    cam->iic_v1s = FALSE;
                }
            }
        }
        __enable_irq();

        // 准备对码
        if (true == dma_ready)
        {
            dma_ready = false;
            slave_data_process(cam);
        }
    }
}

uint8_t duima(void)
{
    uint32_t pval = 0;
    
    __disable_irq();
    WAIT_IIC_MON_START;
    if (IS_IIC_SCL_MON_HIG)
    {
        if(0x20 == iic_monitor_recv()) 
        {
            if(0x32 == iic_monitor_recv()) 
            {
                if(0x00 == iic_monitor_recv())
                {
                    if(0x01 == iic_monitor_recv())
                    {
                        __enable_irq();
                        return 1;
                    }
                }
            }   
        }
    }
    __enable_irq();
    
    return 0;
}
