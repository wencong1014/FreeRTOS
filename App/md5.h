/*------------------------------------------------------------------------------
 * Copyright (c) 2019 JCXX. All rights reserved.
 *------------------------------------------------------------------------------
 * Autor:   lsp
 * Name:    md5.c
 * Purpose: md5 calculate
 *----------------------------------------------------------------------------*/
 
#include <stdint.h>

void md5_init(void);
void md5_random(uint8_t *dst, uint8_t line);
void md5(const uint8_t *initial_msg, int initial_len, uint8_t *digest);

