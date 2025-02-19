/**
 * @file LED.c
 * @brief    LED驱动控制
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
#include "LED.h"

/**
 * @brief 初始化LED
 *
 * 该函数用于初始化连接到GPIOB端口上的LED灯。
 *
 * @return 无
 */
void LED_init(void)
{
    GPIO_InitType GPIO_InitStructure;

    /* Enable the GPIO Clock */
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE);

    /* Configure the GPIO pin */
    GPIO_InitStruct(&GPIO_InitStructure);
    GPIO_InitStructure.Pin = LED1_PIN | LED2_PIN | LED2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.GPIO_Pull = GPIO_PULL_DOWN;
    GPIO_InitPeripheral(LED1_PORT, &GPIO_InitStructure);

    LED1_OFF();
    LED2_OFF();
    LED3_OFF();
}
