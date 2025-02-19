#ifndef __KEY_H
#define __KEY_H 
#include "n32g032.h"

#define KEY1_PORT GPIOA
#define KEY1_PIN  GPIO_PIN_4
#define KEY1_IN  (KEY1_PORT->PID) >> KEY1_PIN

#define KEY2_PORT GPIOA
#define KEY2_PIN  GPIO_PIN_5
#define KEY2_IN  (KEY2_PORT->PID) >> KEY2_PIN

#define KEY3_PORT GPIOA
#define KEY3_PIN  GPIO_PIN_6
#define KEY3_IN  (KEY3_PORT->PID) >> KEY3_PIN

void key_init(void);

#endif
