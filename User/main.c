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
#include "usart.h"
#include "LED.h"
#include "key.h"

#include "FreeRTOS.h"
#include "task.h"

/**************************** 任务句柄 ********************************/
/* 
 * 任务句柄是一个指针，用于指向一个任务，当任务创建好之后，它就具有了一个任务句柄
 * 以后我们要想操作这个任务都需要通过这个任务句柄，如果是自身的任务操作自己，那么
 * 这个句柄可以为NULL。
 */
 /* 创建任务句柄 */
static TaskHandle_t AppTaskCreate_Handle;
/* LED任务句柄 */
static TaskHandle_t LED1_Task_Handle;

/******************************* 全局变量声明 ************************************/
/*
 * 当我们在写应用程序的时候，可能需要用到一些全局变量。
 */
/* AppTaskCreate任务任务堆栈 */
static StackType_t AppTaskCreate_Stack[128];
/* LED任务堆栈 */
static StackType_t LED1_Task_Stack[128];

/* AppTaskCreate 任务控制块 */
static StaticTask_t AppTaskCreate_TCB;
/* LED 任务控制块 */
static StaticTask_t LED1_Task_TCB;

/*空闲任务堆栈*/
static StackType_t Idle_Task_Stack[configMINIMAL_STACK_SIZE];
/*定时器任务堆栈*/
static StackType_t Timer_Task_Stack[configTIMER_TASK_STACK_DEPTH];

/*空闲任务控制块*/
static StaticTask_t Idle_Task_TCB;
/*定时器任务控制块*/
static StaticTask_t Timer_Task_TCB;

static void LED1_tesk(void *parameter);
static void BSP_init(void);
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize);
static void AppTaskCreate(void *parameter);

static void BSP_init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    LED_init();

    key_init();

    usart_init();
}

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
    *ppxIdleTaskTCBBuffer = &Idle_Task_TCB;//空闲任务控制块内存
    *ppxIdleTaskStackBuffer = Idle_Task_Stack; //空闲任务堆栈内存
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE; //空闲任务堆栈大小
}

void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize)
{
    *ppxTimerTaskTCBBuffer = &Timer_Task_TCB; //定时器任务控制块内存
    *ppxTimerTaskStackBuffer = Timer_Task_Stack; //定时器任务堆栈内存
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH; //定时器任务堆栈大小
}

static void AppTaskCreate(void *parameter)
{
    taskENTER_CRITICAL(); //进入临界区

    /* 创建LED_Task任务 */
    LED1_Task_Handle = xTaskCreateStatic((TaskFunction_t) LED1_tesk,
                                        (const char*) "LED1_tesk",//任务名称
                                        (uint32_t) 128,//任务堆栈大小
                                        (void*) NULL,//传递给任务函数的参数
                                        (UBaseType_t) 2,//任务优先级
                                        (StackType_t*) LED1_Task_Stack,//任务堆栈内存
                                        (StaticTask_t*) &LED1_Task_TCB);//任务控制块内存

    if(NULL != LED1_Task_Handle) //创建任务失败
    {
        printf("LED1_tesk task create success!\r\n");
    }
    else
    {
        printf("LED1_tesk task create failed!\r\n");
    }

    vTaskDelete(AppTaskCreate_Handle); //删除自身任务(AppTaskCreate_Handle)

    taskEXIT_CRITICAL(); //退出临界区

    while(1) //正常不会执行到这里
    {
    }
}

void LED1_tesk(void *parameter)
{
    while(1)
    {
        LED1_ON();
        vTaskDelay(500);
        printf("LED1_tesk task running, LED ON\r\n");
        LED1_OFF();
        vTaskDelay(500);
        printf("LED1_tesk task running, LED OFF\r\n");
    }
}

/*!
    \brief      main
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    BSP_init();

    AppTaskCreate_Handle = xTaskCreateStatic((TaskFunction_t) AppTaskCreate,
                                            (const char*) "AppTaskCreate",//任务名称
                                            (uint32_t) 128,//任务堆栈大小
                                            (void*) NULL,//传递给任务函数的参数
                                            (UBaseType_t) 3,//任务优先级
                                            (StackType_t*) AppTaskCreate_Stack,//任务堆栈内存
                                            (StaticTask_t*) &AppTaskCreate_TCB);//任务控制块内存

    if(NULL != AppTaskCreate_Handle) //创建任务成功
    {
        vTaskStartScheduler(); //启动任务调度器
    }

    while(1) //正常不会执行到这里
    {
    }
}
