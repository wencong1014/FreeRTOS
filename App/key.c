/**
 * @file key.c
 * @brief    【描述】
 * @author WenC (test@test.com)
 * @version 1.0
 * @date2025-02-19
 * 
 * @copyright Copyright (c) 2025  TEST
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date       <th>Version <th>Author <th>Description
 * <tr><td>2025-02-19 <td>2.0     <td>test     <td>内容
 * </table>
 */
#include "key.h"

void key_init(void)
{
    GPIO_InitType GPIO_InitStructure;

    /* Enable the GPIO Clock */
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);

    /* Configure the GPIO pin */
    GPIO_InitStruct(&GPIO_InitStructure);
    GPIO_InitStructure.Pin = KEY1_PIN | KEY2_PIN | KEY3_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.GPIO_Pull = GPIO_PULL_UP;
    GPIO_InitPeripheral(KEY1_PORT, &GPIO_InitStructure);
}
