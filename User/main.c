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

/**************************** ������ ********************************/
/* 
 * ��������һ��ָ�룬����ָ��һ�����񣬵����񴴽���֮�����;�����һ��������
 * �Ժ�����Ҫ��������������Ҫͨ�������������������������������Լ�����ô
 * ����������ΪNULL��
 */
 /* ���������� */
static TaskHandle_t AppTaskCreate_Handle;
/* LED������ */
static TaskHandle_t LED1_Task_Handle;

/******************************* ȫ�ֱ������� ************************************/
/*
 * ��������дӦ�ó����ʱ�򣬿�����Ҫ�õ�һЩȫ�ֱ�����
 */
/* AppTaskCreate���������ջ */
static StackType_t AppTaskCreate_Stack[128];
/* LED�����ջ */
static StackType_t LED1_Task_Stack[128];

/* AppTaskCreate ������ƿ� */
static StaticTask_t AppTaskCreate_TCB;
/* LED ������ƿ� */
static StaticTask_t LED1_Task_TCB;

/*���������ջ*/
static StackType_t Idle_Task_Stack[configMINIMAL_STACK_SIZE];
/*��ʱ�������ջ*/
static StackType_t Timer_Task_Stack[configTIMER_TASK_STACK_DEPTH];

/*����������ƿ�*/
static StaticTask_t Idle_Task_TCB;
/*��ʱ��������ƿ�*/
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
    *ppxIdleTaskTCBBuffer = &Idle_Task_TCB;//����������ƿ��ڴ�
    *ppxIdleTaskStackBuffer = Idle_Task_Stack; //���������ջ�ڴ�
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE; //���������ջ��С
}

void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize)
{
    *ppxTimerTaskTCBBuffer = &Timer_Task_TCB; //��ʱ��������ƿ��ڴ�
    *ppxTimerTaskStackBuffer = Timer_Task_Stack; //��ʱ�������ջ�ڴ�
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH; //��ʱ�������ջ��С
}

static void AppTaskCreate(void *parameter)
{
    taskENTER_CRITICAL(); //�����ٽ���

    /* ����LED_Task���� */
    LED1_Task_Handle = xTaskCreateStatic((TaskFunction_t) LED1_tesk,
                                        (const char*) "LED1_tesk",//��������
                                        (uint32_t) 128,//�����ջ��С
                                        (void*) NULL,//���ݸ��������Ĳ���
                                        (UBaseType_t) 2,//�������ȼ�
                                        (StackType_t*) LED1_Task_Stack,//�����ջ�ڴ�
                                        (StaticTask_t*) &LED1_Task_TCB);//������ƿ��ڴ�

    if(NULL != LED1_Task_Handle) //��������ʧ��
    {
        printf("LED1_tesk task create success!\r\n");
    }
    else
    {
        printf("LED1_tesk task create failed!\r\n");
    }

    vTaskDelete(AppTaskCreate_Handle); //ɾ����������(AppTaskCreate_Handle)

    taskEXIT_CRITICAL(); //�˳��ٽ���

    while(1) //��������ִ�е�����
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
                                            (const char*) "AppTaskCreate",//��������
                                            (uint32_t) 128,//�����ջ��С
                                            (void*) NULL,//���ݸ��������Ĳ���
                                            (UBaseType_t) 3,//�������ȼ�
                                            (StackType_t*) AppTaskCreate_Stack,//�����ջ�ڴ�
                                            (StaticTask_t*) &AppTaskCreate_TCB);//������ƿ��ڴ�

    if(NULL != AppTaskCreate_Handle) //��������ɹ�
    {
        vTaskStartScheduler(); //�������������
    }

    while(1) //��������ִ�е�����
    {
    }
}
