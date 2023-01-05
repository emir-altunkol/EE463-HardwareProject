/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f1xx_hal.h"

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
#define I_SENSE_Pin GPIO_PIN_0
#define I_SENSE_GPIO_Port GPIOA
#define I_SET_Pin GPIO_PIN_1
#define I_SET_GPIO_Port GPIOA
#define SOFT_START_Pin GPIO_PIN_2
#define SOFT_START_GPIO_Port GPIOA
#define GEN_MODE_Pin GPIO_PIN_3
#define GEN_MODE_GPIO_Port GPIOA
#define GPIO_IN_D_Pin GPIO_PIN_4
#define GPIO_IN_D_GPIO_Port GPIOA
#define GPIO_OUT_D_Pin GPIO_PIN_5
#define GPIO_OUT_D_GPIO_Port GPIOA
#define PWM2_Pin GPIO_PIN_13
#define PWM2_GPIO_Port GPIOB
#define PWM4_Pin GPIO_PIN_14
#define PWM4_GPIO_Port GPIOB
#define PWM6_Pin GPIO_PIN_15
#define PWM6_GPIO_Port GPIOB
#define PWM1_Pin GPIO_PIN_8
#define PWM1_GPIO_Port GPIOA
#define PWM3_Pin GPIO_PIN_9
#define PWM3_GPIO_Port GPIOA
#define PWM5_Pin GPIO_PIN_10
#define PWM5_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
