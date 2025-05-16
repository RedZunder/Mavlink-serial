/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "string.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"

#include "mavlink_codec.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma location=0x2004c000
ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
#pragma location=0x2004c0a0
ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */

#elif defined ( __CC_ARM )  /* MDK ARM Compiler */

__attribute__((at(0x2004c000))) ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
__attribute__((at(0x2004c0a0))) ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */

#elif defined ( __GNUC__ ) /* GNU Compiler */

ETH_DMADescTypeDef DMARxDscrTab[ETH_RX_DESC_CNT] __attribute__((section(".RxDecripSection"))); /* Ethernet Rx DMA Descriptors */
ETH_DMADescTypeDef DMATxDscrTab[ETH_TX_DESC_CNT] __attribute__((section(".TxDecripSection")));   /* Ethernet Tx DMA Descriptors */
#endif

ETH_TxPacketConfig TxConfig;

ETH_HandleTypeDef heth;

TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

	unsigned char mssgBytes[] = {0xfd, 0x9, 0x0, 0x0, 0x8f, 0x5, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0x3, 0x51, 0x3, 0x3, 0xc9, 0x7c,/*END OF HEARTBEAT*/0xfd, 0x1c, 0x0, 0x0, 0xcd, 0x1, 0x1, 0x21, 0x0, 0x0, 0x30, 0xb2, 0x10, 0x0, 0x55, 0xc0, 0x3b, 0x1f, 0x57, 0xd9, 0x1a, 0xa, 0x66, 0x2, 0x1, 0x0, 0x54, 0xa, 0x0, 0x0, 0xff, 0xff, 0x1, 0x0, 0x0, 0x0, 0x69, 0x49, 0x98, 0x83, 0xfd, 0x1f, 0x0, 0x0, 0xce, 0x1, 0x1, 0x1, 0x0, 0x0, 0x2f, 0xfc, 0x71, 0x53, 0x2f, 0xfc, 0x61, 0x52, 0x2f, 0xfc, 0x71, 0x57, 0x0, 0x0, 0x38, 0x31, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x12, 0xf6, 0xc3, 0xfd, 0x2, 0x0, 0x0, 0xcf, 0x1, 0x1, 0x7d, 0x0, 0x0, 0x88, 0x13, 0xba, 0xca, 0xfd, 0x7, 0x0, 0x0, 0xd0, 0x1, 0x1, 0x98, 0x0, 0x0, 0x0, 0x0, 0xff, 0xff, 0x0, 0x0, 0x2, 0xe9, 0x66, 0xfd, 0x17, 0x0, 0x0, 0xd1, 0x1, 0x1, 0x3e, 0x0, 0x0, 0x14, 0xd7, 0x7f, 0xb9, 0x87, 0x58, 0x56, 0xb9, 0x3d, 0xa, 0x87, 0x38, 0x0, 0x0, 0x0, 0x0, 0x5b, 0x83, 0xb2, 0x3c, 0x55, 0xff, 0x91, 0xdc, 0xef, 0xfd, 0x5, 0x0, 0x0, 0xd2, 0x1, 0x1, 0x2a, 0x0, 0x0, 0x0, 0x0, 0xb, 0x0, 0x2, 0xd5, 0xe, 0xfd, 0xc, 0x0, 0x0, 0xd3, 0x1, 0x1, 0x24, 0x0, 0x0, 0x55, 0xf, 0x38, 0x41, 0xe8, 0x3, 0xe8, 0x3, 0xe8, 0x3, 0xe8, 0x3, 0xac, 0x1e, 0xfd, 0x2a, 0x0, 0x0, 0xd4, 0x1, 0x1, 0x41, 0x0, 0x0, 0x30, 0xb2, 0x10, 0x0, 0xdc, 0x5, 0xdc, 0x5, 0xe8, 0x3, 0xdc, 0x5, 0x8, 0x7, 0xe8, 0x3, 0xe8, 0x3, 0x8, 0x7, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x10, 0xff, 0x38, 0xa, 0xfd, 0x1d, 0x0, 0x0, 0xd5, 0x1, 0x1, 0x1b, 0x0, 0x0, 0x55, 0xf, 0x38, 0x41, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x17, 0xfc, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x44, 0xff, 0xa, 0x0, 0xd0, 0x1, 0x0, 0x5f, 0x11, 0xfe, 0x47, 0xfd, 0x18, 0x0, 0x0, 0xd6, 0x1, 0x1, 0x74, 0x0, 0x0, 0x30, 0xb2, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x17, 0xfc, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x44, 0xff, 0xa, 0x0, 0xd0, 0x1, 0x5f, 0x11, 0xe2, 0x55, 0xfd, 0x16, 0x0, 0x0, 0xd7, 0x1, 0x1, 0x81, 0x0, 0x0, 0x30, 0xb2, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x44, 0xff, 0xa, 0x0, 0xd0, 0x1, 0x7d, 0x17, 0xfd, 0xe, 0x0, 0x0, 0xd8, 0x1, 0x1, 0x1d, 0x0, 0x0, 0x30, 0xb2, 0x10, 0x0, 0x5, 0x56, 0x7b, 0x44, 0x0, 0x0, 0x0, 0x0, 0x81, 0xd, 0xd7, 0x41, 0xfd, 0xe, 0x0, 0x0, 0xd9, 0x1, 0x1, 0x89, 0x0, 0x0, 0x30, 0xb2, 0x10, 0x0, 0x58, 0x56, 0x7b, 0x44, 0x0, 0x0, 0x0, 0x0, 0x81, 0xd, 0x31, 0x49, 0xfd, 0x2b, 0x0, 0x0, 0xda, 0x1, 0x1, 0x18, 0x0, 0x0, 0x28, 0x2, 0x35, 0x41, 0x0, 0x0, 0x0, 0x0, 0x56, 0xc0, 0x3b, 0x1f, 0x56, 0xd9, 0x1a, 0xa, 0xe4, 0x1, 0x1, 0x0, 0x79, 0x0, 0xc8, 0x0, 0x0, 0x0, 0x49, 0x38, 0x6, 0xa, 0xe4, 0x1, 0x1, 0x0, 0x2c, 0x1, 0x0, 0x0, 0x2c, 0x1, 0x0, 0x0, 0x28, 0x38, 0xe6, 0xfd, 0xb, 0x0, 0x0, 0xdb, 0x1, 0x1, 0x2, 0x0, 0x0, 0xd6, 0x83, 0xac, 0xbe, 0xd0, 0x31, 0x6, 0x0, 0x30, 0xb2, 0x10, 0x80, 0x25, 0xfd, 0x1c, 0x0, 0x0, 0xdc, 0x1, 0x1, 0xa3, 0x0, 0x0, 0x1b, 0xaa, 0xf1, 0xba, 0x33, 0xcf, 0xec, 0xba, 0x7b, 0x5e, 0x1e, 0xbb, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xab, 0x81, 0x1a, 0x3b, 0x85, 0x8d, 0xc4, 0x3a, 0x79, 0xaf, 0xfd, 0x4, 0x0, 0x0, 0xdd, 0x1, 0x1, 0xa8, 0x0, 0x0, 0x0, 0x0, 0x34, 0xc3, 0xe9, 0x19, 0xfd, 0x16, 0x0, 0x0, 0xde, 0x1, 0x1, 0x88, 0x0, 0x0, 0x55, 0xc0, 0x3b, 0x1f, 0x57, 0xd9, 0x1a, 0xa, 0x6a, 0xa, 0x84, 0x42, 0x0, 0xc6, 0x4, 0x3e, 0x64, 0x0, 0x0, 0x0, 0xf8, 0x1, 0x3f, 0x12, 0xfd, 0x16, 0x0, 0x0, 0xdf, 0x1, 0x1, 0xc1, 0x0, 0x0, 0x3e, 0xcc, 0xd, 0x3c, 0xab, 0xa9, 0xa7, 0x3b, 0x23, 0xe5, 0x6e, 0x37, 0xa9, 0x6b, 0x6c, 0x3c, 0x0, 0x0, 0x0, 0x0, 0x3f, 0x3, 0x81, 0xb6, 0xfd, 0x1c, 0x0, 0x0, 0xe0, 0x1, 0x1, 0x20, 0x0, 0x0, 0x30, 0xb2, 0x10, 0x0, 0x48, 0x16, 0xdc, 0xc1, 0xe2, 0xa8, 0x29, 0x43, 0xab, 0x1d, 0x44, 0xc0, 0x75, 0x66, 0x98, 0xbc, 0xb8, 0xa2, 0x6d, 0x3c, 0xf7, 0x1d, 0x30, 0xb8, 0x76, 0xd9, 0xfd, 0x14, 0x0, 0x0, 0xe1, 0x1, 0x1, 0xf1, 0x0, 0x0, 0x55, 0xf, 0x38, 0x41, 0x0, 0x0, 0x0, 0x0, 0x70, 0x28, 0x48, 0x3b, 0x1e, 0x71, 0x33, 0x3b, 0x81, 0x4a, 0x32, 0x3b, 0xfc, 0x9a, 0xfd, 0x32, 0x0, 0x0, 0xe2, 0x1, 0x1, 0x57, 0x0, 0x0, 0x30, 0xb2, 0x10, 0x0, 0x54, 0xc0, 0x3b, 0x1f, 0x59, 0xd9, 0x1a, 0xa, 0xcd, 0x4c, 0x84, 0x42, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf8, 0xfd, 0xf7, 0x2a, 0xfd, 0x29, 0x0, 0x0, 0xe3, 0x1, 0x1, 0x93, 0x0, 0x0, 0x89, 0xa, 0x0, 0x0, 0xc7, 0x4, 0x0, 0x0, 0xff, 0x7f, 0x38, 0x31, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x12, 0x0, 0x0, 0x0, 0x0, 0x1, 0xfd, 0x5, 0xfd, 0x2c, 0x0, 0x0, 0xe4, 0x1, 0x1, 0x16, 0x2b, 0x0, 0x8f, 0x6, 0x8f, 0x6, 0x8f, 0x6, 0x8f, 0x6, 0x50, 0x0, 0x50, 0x0, 0x50, 0x0, 0x50, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x43, 0xa4, 0x43, 0xa4, 0x43, 0xa4, 0x43, 0xa4, 0x20, 0x20, 0x20, 0x20, 0xf1, 0xe5};
	unsigned char cmdBytes[] = {0xfd, 0x20, 0x0, 0x0, 0x5a, 0xff, 0xbe, 0x4c, 0x0, 0x0, 0x0, 0x0, 0x0, 0x43, 0x0, 0x0, 0x80, 0x3f, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xb0, 0x0, 0x1, 0x1, 0xff, 0x99};

	uint8_t byte=0x00;
	uint32_t i=0;										//iterator

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ETH_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */









/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */



////////
/*
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{

	HAL_UARTEx_ReceiveToIdle_IT(&huart1, (uint8_t *)rx, 6);

}*/

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  // Check which version of the timer triggered this callback and toggle LED
  if (htim == &htim4 )
  {
    broadcast_heartbeat();
  }
}




/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ETH_Init();
  MX_USART3_UART_Init();
  MX_USART1_UART_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  HAL_TIM_Base_Start_IT(&htim4);

	while (1)
  {

	  //HAL_UART_Transmit(&huart3, (uint8_t *)mssg,sizeof(mssg),100);

	 /* if(!lastState)
	  {
		  if (HAL_GPIO_ReadPin(USER_Btn_GPIO_Port,USER_Btn_Pin) == GPIO_PIN_SET)
		  {

				//send packet
				HAL_HalfDuplex_EnableTransmitter(&huart1);

				HAL_UART_Transmit(&huart1, (uint8_t *)mssg,sizeof(mssg),100);

				//HAL_UARTEx_ReceiveToIdle_IT(&huart2, (uint8_t*)rx, 6);

				HAL_HalfDuplex_EnableReceiver(&huart1);
				HAL_UART_Receive(&huart1, (uint8_t*)rx, 64, 100);

			  lastState=true;
		  }
	  }
	  else
	  {
		  lastState=false;
	  }*/


		/*HAL_HalfDuplex_EnableReceiver(&huart1);
		HAL_UART_Receive(&huart1, (uint8_t*)rx, 64, 100);

		 	 */
	  //read answer


	  //show answer in serial terminal
		//HAL_UART_Transmit(&huart3,(uint8_t *) 255,1,100);
		//HAL_UART_Transmit(&huart1, (uint8_t *)rx,sizeof(rx),100);

		decode_mavlink_mssg(mssgBytes[i]);

		if(i<sizeof(mssgBytes))
			i++;




		HAL_Delay(10);







    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }







  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ETH Initialization Function
  * @param None
  * @retval None
  */
static void MX_ETH_Init(void)
{

  /* USER CODE BEGIN ETH_Init 0 */

  /* USER CODE END ETH_Init 0 */

   static uint8_t MACAddr[6];

  /* USER CODE BEGIN ETH_Init 1 */

  /* USER CODE END ETH_Init 1 */
  heth.Instance = ETH;
  MACAddr[0] = 0x00;
  MACAddr[1] = 0x80;
  MACAddr[2] = 0xE1;
  MACAddr[3] = 0x00;
  MACAddr[4] = 0x00;
  MACAddr[5] = 0x00;
  heth.Init.MACAddr = &MACAddr[0];
  heth.Init.MediaInterface = HAL_ETH_RMII_MODE;
  heth.Init.TxDesc = DMATxDscrTab;
  heth.Init.RxDesc = DMARxDscrTab;
  heth.Init.RxBuffLen = 1524;

  /* USER CODE BEGIN MACADDRESS */

  /* USER CODE END MACADDRESS */

  if (HAL_ETH_Init(&heth) != HAL_OK)
  {
    Error_Handler();
  }

  memset(&TxConfig, 0 , sizeof(ETH_TxPacketConfig));
  TxConfig.Attributes = ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;
  TxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
  TxConfig.CRCPadCtrl = ETH_CRC_PAD_INSERT;
  /* USER CODE BEGIN ETH_Init 2 */

  /* USER CODE END ETH_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 36000-1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 2000-1;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 460800;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_HalfDuplex_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : USB_DM_Pin USB_DP_Pin */
  GPIO_InitStruct.Pin = USB_DM_Pin|USB_DP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
