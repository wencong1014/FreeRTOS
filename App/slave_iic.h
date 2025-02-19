/*------------------------------------------------------------------------------
 * Copyright (c) 2020 JCXX. All rights reserved.
 *------------------------------------------------------------------------------
 * Autor:   lsp
 * Name:    slave_iic.h
 * Purpose: battery iic
 *----------------------------------------------------------------------------*/
#ifndef __SLAVE_IIC_H__
#define __SLAVE_IIC_H__

#include <stdint.h>

/* for extern */
void save_camera_param_to_flash(uint8_t *param, uint8_t *data);
void pack_camera_param_to_data(uint8_t *proto, uint8_t *data);
void read_camera_param_from_data(uint8_t *data, uint8_t *param);
void read_camera_data_from_proto(uint8_t *proto, uint8_t *data);
void pack_camera_data_to_proto(uint8_t *data, uint8_t *proto);
void calc_camera_cpuid_to_data(uint8_t *buf, uint8_t *data);
void calc_camera_flash_to_data(uint8_t *buf, uint8_t *data);
void calc_camera_read_rsa_to_data(uint8_t *buf, uint8_t *data);
void calc_camera_write_rsa_to_data(uint8_t *buf, uint8_t *data);
void calc_camera_md5_to_data(uint8_t *buf, uint8_t *data);
void calc_camera_ram_to_data(uint8_t *buf, uint8_t *data);
uint8_t proc_batt_reset_to_data(uint8_t *buf, uint8_t *data);
uint8_t duima(void);
#endif

