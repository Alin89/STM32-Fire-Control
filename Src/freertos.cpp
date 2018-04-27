/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V.
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "usart.h"
#include "ENV_pack.h"
#include "hdc1080.h"
#include "i2c.h"

volatile float temp;
volatile uint8_t humi;
volatile uint8_t huoyan;
volatile uint8_t yanwu;
volatile uint8_t kongqi;
volatile uint8_t fencheng;
volatile uint8_t yangqi;
volatile uint8_t qiya;
volatile uint8_t read_data[2];
volatile uint16_t reg = 0;

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;
osThreadId Read_DataHandle;
osThreadId Send_DataHandle;
osMessageQId Data_queHandle;

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const *argument);
void ReadDataTask(void const *argument);
void SendDataTask(void const *argument);

/* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Hook prototypes */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void)
{
    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */

    /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
    /* USER CODE END RTOS_SEMAPHORES */

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* Create the thread(s) */
    /* definition and creation of defaultTask */
    osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
    defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

    /* definition and creation of Read_Data */
    osThreadDef(Read_Data, ReadDataTask, osPriorityAboveNormal, 0, 128);
    Read_DataHandle = osThreadCreate(osThread(Read_Data), NULL);

    /* definition and creation of Send_Data */
    osThreadDef(Send_Data, SendDataTask, osPriorityNormal, 0, 128);
    Send_DataHandle = osThreadCreate(osThread(Send_Data), NULL);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */

    /* Create the queue(s) */
    /* definition and creation of Data_que */
    osMessageQDef(Data_que, 16, uint16_t);
    Data_queHandle = osMessageCreate(osMessageQ(Data_que), NULL);

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    /* USER CODE END RTOS_QUEUES */
}

/* StartDefaultTask function */
void StartDefaultTask(void const *argument)
{

    /* USER CODE BEGIN StartDefaultTask */
    /* Infinite loop */
    for (;;)
    {
        osDelay(1);
    }
    /* USER CODE END StartDefaultTask */
}

/* ReadDataTask function */
void ReadDataTask(void const *argument)
{
    MX_I2C1_Init();
    /* USER CODE BEGIN ReadDataTask */
    USART4_printf("Read Data Task Start!\r\n");
    /* 配置433模块为从机发送模式 M0 = 1， M1 = 0 */
    HAL_GPIO_WritePin(M0_GPIO_Port, M0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(M1_GPIO_Port, M1_Pin, GPIO_PIN_RESET);
    USART4_printf("433 configure ok!\r\n");

    hdc1080_init(&hi2c1, Temperature_Resolution_14_bit, Humidity_Resolution_14_bit);

    /* Infinite loop */
    for (;;)
    {
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
        osDelay(1000);
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
        osDelay(1000);
        hdc1080_start_measurement(&hi2c1, (float *)&temp, (uint8_t *)&humi);
        env_pack.Set_Fire(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1));
        env_pack.Set_Smooth(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0));
        env_pack.Set_Dust(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2));
        env_pack.Set_Tmperture((uint8_t)temp);
        env_pack.Set_Humidity(humi);
        env_pack.format();
        env_pack.Send_Pack();
    }
    /* USER CODE END ReadDataTask */
}

/* SendDataTask function */
void SendDataTask(void const *argument)
{
    /* USER CODE BEGIN SendDataTask */
    /* Infinite loop */
    for (;;)
    {
        osDelay(1);
    }
    /* USER CODE END SendDataTask */
}

/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
