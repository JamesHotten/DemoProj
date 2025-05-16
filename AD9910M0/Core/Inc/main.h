/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define REST_Pin GPIO_PIN_2
#define REST_GPIO_Port GPIOA
#define SCLK_Pin GPIO_PIN_3
#define SCLK_GPIO_Port GPIOA
#define DRC_Pin GPIO_PIN_4
#define DRC_GPIO_Port GPIOA
#define OSK_Pin GPIO_PIN_5
#define OSK_GPIO_Port GPIOA
#define PF0_Pin GPIO_PIN_6
#define PF0_GPIO_Port GPIOA
#define PF2_Pin GPIO_PIN_7
#define PF2_GPIO_Port GPIOA
#define DRO_Pin GPIO_PIN_1
#define DRO_GPIO_Port GPIOB
#define IOUP_Pin GPIO_PIN_2
#define IOUP_GPIO_Port GPIOB
#define PF1_Pin GPIO_PIN_12
#define PF1_GPIO_Port GPIOB
#define CS_Pin GPIO_PIN_8
#define CS_GPIO_Port GPIOA
#define PWR_Pin GPIO_PIN_5
#define PWR_GPIO_Port GPIOB
#define SDIO_Pin GPIO_PIN_6
#define SDIO_GPIO_Port GPIOB
#define DPH_Pin GPIO_PIN_7
#define DPH_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
