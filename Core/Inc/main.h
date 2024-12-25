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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CY15B104Q_NWP_Pin GPIO_PIN_2
#define CY15B104Q_NWP_GPIO_Port GPIOA
#define CY15B104Q_NHOLD_Pin GPIO_PIN_3
#define CY15B104Q_NHOLD_GPIO_Port GPIOA
#define CY15B104Q_NCS_Pin GPIO_PIN_4
#define CY15B104Q_NCS_GPIO_Port GPIOA
#define CY15B104Q_SCK_Pin GPIO_PIN_5
#define CY15B104Q_SCK_GPIO_Port GPIOA
#define CY15B104Q_MISO_Pin GPIO_PIN_6
#define CY15B104Q_MISO_GPIO_Port GPIOA
#define CY15B104Q_MOSI_Pin GPIO_PIN_7
#define CY15B104Q_MOSI_GPIO_Port GPIOA
#define DEBUG_TX_Pin GPIO_PIN_9
#define DEBUG_TX_GPIO_Port GPIOA
#define DEBUG_RX_Pin GPIO_PIN_10
#define DEBUG_RX_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
