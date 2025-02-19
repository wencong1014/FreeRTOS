#ifndef __LED_H
#define __LED_H 
#include "n32g032.h"

#define LED1_PORT GPIOB
#define LED1_PIN  GPIO_PIN_1
#define LED1_OFF() LED1_PORT->PBC = LED1_PIN
#define LED1_ON()  LED1_PORT->PBSC = LED1_PIN

#define LED2_PORT GPIOB
#define LED2_PIN  GPIO_PIN_6
#define LED2_OFF() LED2_PORT->PBC = LED2_PIN
#define LED2_ON()  LED2_PORT->PBSC = LED2_PIN

#define LED3_PORT GPIOB
#define LED3_PIN  GPIO_PIN_7
#define LED3_OFF() LED3_PORT->PBC = LED3_PIN
#define LED3_ON()  LED3_PORT->PBSC = LED3_PIN


void LED_init(void);

#endif
