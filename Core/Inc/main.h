/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define BUTTON0_Pin GPIO_PIN_0
#define BUTTON0_GPIO_Port GPIOC
#define BUTTON1_Pin GPIO_PIN_1
#define BUTTON1_GPIO_Port GPIOC
#define BUTTON2_Pin GPIO_PIN_2
#define BUTTON2_GPIO_Port GPIOC
#define PhotoSensor_EXTI0_Pin GPIO_PIN_0
#define PhotoSensor_EXTI0_GPIO_Port GPIOA
#define PhotoSensor_EXTI0_EXTI_IRQn EXTI0_IRQn
#define PhotoSensor_EXTI1_Pin GPIO_PIN_1
#define PhotoSensor_EXTI1_GPIO_Port GPIOA
#define PhotoSensor_EXTI1_EXTI_IRQn EXTI1_IRQn
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define PhotoSensor_EXTI4_Pin GPIO_PIN_4
#define PhotoSensor_EXTI4_GPIO_Port GPIOA
#define PhotoSensor_EXTI4_EXTI_IRQn EXTI4_IRQn
#define PhotoSensor_EXTI5_Pin GPIO_PIN_5
#define PhotoSensor_EXTI5_GPIO_Port GPIOA
#define PhotoSensor_EXTI5_EXTI_IRQn EXTI9_5_IRQn
#define BUTTON3_Pin GPIO_PIN_4
#define BUTTON3_GPIO_Port GPIOC
#define LED0_Pin GPIO_PIN_0
#define LED0_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_1
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_2
#define LED2_GPIO_Port GPIOB
#define LATCH_74HC595_Pin GPIO_PIN_15
#define LATCH_74HC595_GPIO_Port GPIOB
#define StepMotor_IN1_Pin GPIO_PIN_6
#define StepMotor_IN1_GPIO_Port GPIOC
#define StepMotor_IN2_Pin GPIO_PIN_7
#define StepMotor_IN2_GPIO_Port GPIOC
#define StepMotor_IN3_Pin GPIO_PIN_8
#define StepMotor_IN3_GPIO_Port GPIOC
#define StepMotor_IN4_Pin GPIO_PIN_9
#define StepMotor_IN4_GPIO_Port GPIOC
#define FND_DIGIT1_Pin GPIO_PIN_9
#define FND_DIGIT1_GPIO_Port GPIOA
#define FND_DATA_Pin GPIO_PIN_10
#define FND_DATA_GPIO_Port GPIOA
#define FND_LATCH_CLOCK_Pin GPIO_PIN_11
#define FND_LATCH_CLOCK_GPIO_Port GPIOA
#define FND_SHIFT_CLOCK_Pin GPIO_PIN_12
#define FND_SHIFT_CLOCK_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define FND_DIGIT2_Pin GPIO_PIN_10
#define FND_DIGIT2_GPIO_Port GPIOC
#define FND_DIGIT3_Pin GPIO_PIN_11
#define FND_DIGIT3_GPIO_Port GPIOC
#define FND_DIGIT4_Pin GPIO_PIN_12
#define FND_DIGIT4_GPIO_Port GPIOC
#define LED3_Pin GPIO_PIN_3
#define LED3_GPIO_Port GPIOB
#define LED4_Pin GPIO_PIN_4
#define LED4_GPIO_Port GPIOB
#define LED5_Pin GPIO_PIN_5
#define LED5_GPIO_Port GPIOB
#define LED6_Pin GPIO_PIN_6
#define LED6_GPIO_Port GPIOB
#define built_LED_Pin GPIO_PIN_7
#define built_LED_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
