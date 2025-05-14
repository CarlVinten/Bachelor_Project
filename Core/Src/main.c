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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "my_hal.h"
#include "my_spi.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define WRITE_READ_SEQ_SINGLE_1


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

XSPI_HandleTypeDef hospi1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_OCTOSPI1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

	uint8_t spi_read_buffer_1[257];
	uint8_t spi_write_buffer_1[257];

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
  MX_USART2_UART_Init();
  MX_OCTOSPI1_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_Delay(2000);
  uart_print((uint8_t *)"\r\nInitializing\r\n", &huart2);

  if(hospi1.State == HAL_XSPI_STATE_READY){
	  uart_print((uint8_t *)"HAL OK\r\n", &huart2);
  }else{
	  uart_print((uint8_t *)"HAL NOT OK\r\n", &huart2);
  }

  uart_print((uint8_t *) "Reset device\r\n", &huart2);
  check_spi_return(reset(&hospi1));

  /*uart_print((uint8_t *)"Setting QE bit \r\n", &huart2);
  check_spi_return(set_QE(&hospi1));*/


  uint32_t address = 0x6f000;

  while (1)
  {
   fill_page_buffer_1(0b01010101, spi_write_buffer_1);
   for(address = 0xff + 1; 1; address += 8){
      // test reading
	//chip_erase(spi_write_buffer_1, &hospi1, &huart2);

    get_HAL_error(erase_sector(&hospi1, address), &huart2);
    get_HAL_error(test_write_n_bytes_single(16, spi_write_buffer_1, &hospi1, address, &huart2), &huart2);
    uart_print((uint8_t *)"Flush_buffer\r\n", &huart2);
    get_HAL_error(flush_buffer(&hospi1), &huart2);


    get_HAL_error(test_read_n_bytes_dual_2(16, spi_read_buffer_1, &hospi1, address, &huart2), &huart2);
	uart_print((uint8_t *)"read from flash: \r\n", &huart2);
	uart_print(spi_read_buffer_1, &huart2);
	uart_print((uint8_t *)"\r\n", &huart2);
	memset(spi_read_buffer_1, 0, 256);
    get_HAL_error(test_read_n_bytes_single(16, spi_read_buffer_1, &hospi1, address, &huart2), &huart2);
	uart_print((uint8_t *)"read from flash: \r\n", &huart2);
	uart_print(spi_read_buffer_1, &huart2);
	uart_print((uint8_t *)"\r\n", &huart2);
	memset(spi_read_buffer_1, 0, 256);
	//check_spi_return(set_QE(&hospi1));
    get_HAL_error(test_read_n_bytes_dual_1(16, spi_read_buffer_1, &hospi1, address, &huart2), &huart2);
	uart_print((uint8_t *)"read from flash: \r\n", &huart2);
	uart_print(spi_read_buffer_1, &huart2);
	uart_print((uint8_t *)"\r\n", &huart2);
	memset(spi_read_buffer_1, 0, 256);
	//check_QE(&hospi1, &huart2);

   }

	get_HAL_error(test_read_n_bytes_quad_4(16, spi_read_buffer_1, &hospi1, address, &huart2), &huart2);
	uart_print((uint8_t *)"read from flash: \r\n", &huart2);
	uart_print(spi_read_buffer_1, &huart2);
	uart_print((uint8_t *)"\r\n", &huart2);
	memset(spi_read_buffer_1, 0, 256);

	get_HAL_error(test_read_n_bytes_quad_3(16, spi_read_buffer_1, &hospi1, address, &huart2), &huart2);
	uart_print((uint8_t *)"read from flash: \r\n", &huart2);
	uart_print(spi_read_buffer_1, &huart2);
	uart_print((uint8_t *)"\r\n", &huart2);
	memset(spi_read_buffer_1, 0, 256);

    get_HAL_error(test_read_n_bytes_quad_1(16, spi_read_buffer_1, &hospi1, address, &huart2), &huart2);
    uart_print((uint8_t *)"read from flash: \r\n", &huart2);
   	uart_print(spi_read_buffer_1, &huart2);
   	uart_print((uint8_t *)"\r\n", &huart2);
   	memset(spi_read_buffer_1, 0, 256);
    get_HAL_error(test_read_n_bytes_quad_2(16, spi_read_buffer_1, &hospi1, address, &huart2), &huart2);
    uart_print((uint8_t *)"read from flash: \r\n", &huart2);
   	uart_print(spi_read_buffer_1, &huart2);
   	uart_print((uint8_t *)"\r\n", &huart2);
   	memset(spi_read_buffer_1, 0, 256);

    while(1){}
      /* while(1){

	  octo_spi_return = HAL_XSPI_Command(&hospi1, &spi_command, 60000);

	  uart_print("Cmd Init\r\n", &huart2);
	  get_HAL_error(octo_spi_return, &huart2);
	  octo_spi_return = HAL_XSPI_Transmit(&hospi1, spi_buf, 5000);
	  get_HAL_error(octo_spi_return, &huart2);
	  HAL_Delay(1000);

	  }*/
	  //dummy_buf = itoa(HAL_XSPI_GetState(&hospi1), (uint8_t *)dummy_buf, 1);
	  //uart_print((uint8_t *)dummy_buf, &huart2);
	  //HAL_Delay(1000);
#ifdef WRITE_READ_SEQ_SINGLE_1
	  for(uint8_t write_uint8_tacter = 0x01; write_uint8_tacter < 256; write_uint8_tacter++){
	  fill_page_buffer_1(write_uint8_tacter, spi_write_buffer_1);
	  uint32_t address = 0x005000;
	  uart_print((uint8_t *)"\r\nWrite\r\n", &huart2);
	  get_HAL_error(write_page(spi_write_buffer_1, &hospi1, address), &huart2);
	  HAL_Delay(2500);
	  uart_print((uint8_t *)"\r\nRead\r\n", &huart2);
	  get_HAL_error(read_page(spi_read_buffer_1, &hospi1, address), &huart2);
	  HAL_Delay(2500);
	  uart_print((uint8_t *)"\r\nErase\r\n", &huart2);
	  get_HAL_error(erase_sector(&hospi1, address), &huart2);
	  HAL_Delay(2500);
	  uart_print((uint8_t *)"\r\nRead\r\n", &huart2);
	  get_HAL_error(read_page(spi_read_buffer_1, &hospi1, address), &huart2);
	  HAL_Delay(2500);
	  }

#endif
#ifdef WRITE_READ_SEQ_DUAL_1
	  for(uint8_t write_character = 0x01; write_character < 256; write_character++){
	  fill_page_buffer_1(write_uint8_tacter, spi_write_buffer_1);
	  uint32_t address = 0x001000;
	  uart_print("\r\nWrite\r\n", &huart2);
	  get_HAL_error(dual_write(spi_write_buffer_1, &hospi1, address), &huart2);
	  HAL_Delay(2500);
	  uart_print("\r\nRead\r\n", &huart2);
	  get_HAL_error(read_page(spi_read_buffer_1, &hospi1, address), &huart2);
	  HAL_Delay(2500);
	  uart_print("\r\nErase\r\n", &huart2);
	  get_HAL_error(erase_sector(&hospi1, address), &huart2);
	  HAL_Delay(2500);
	  uart_print("\r\nRead\r\n", &huart2);
	  get_HAL_error(read_page(spi_read_buffer_1, &hospi1, address), &huart2);
	  HAL_Delay(2500);
	  }
#endif
#ifdef WRITE_READ_SEQ_QUAD_1
	  for(uint8_t write_character = 0x01; write_character < 256; write_character++){
	  	  fill_page_buffer_1(write_character, spi_write_buffer_1);
	  	  uint32_t address = 0x001000;
	  	  uart_print("\r\nWrite\r\n", &huart2);
	  	  get_HAL_error(quad_write(spi_write_buffer_1, &hospi1, address), &huart2);
	  	  HAL_Delay(2500);
	  	  uart_print("\r\nRead\r\n", &huart2);
	  	  get_HAL_error(read_page(spi_read_buffer_1, &hospi1, address), &huart2);
	  	  HAL_Delay(2500);
	  	  uart_print("\r\nErase\r\n", &huart2);
	  	  get_HAL_error(erase_sector(&hospi1, address), &huart2);
	  	  HAL_Delay(2500);
	  	  uart_print("\r\nRead\r\n", &huart2);
	  	  get_HAL_error(read_page(spi_read_buffer_1, &hospi1, address), &huart2);
	  	  HAL_Delay(2500);
	  	  }
#endif
#ifdef READ_DEVICE_INFO // Code for reading device id
	  uart_print("\r\nSignature\r\n", &huart2);
	  octo_spi_return = read_electronic_signature((char *)spi_buf, &hospi1);
	  get_HAL_error(octo_spi_return, &huart2);
	  HAL_Delay(5000);
	  uart_print("\r\nDevice_id\r\n", &huart2);
	  octo_spi_return = read_device_id((char *)spi_buf, &hospi1);
	  get_HAL_error(octo_spi_return, &huart2);

#endif
	  //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
	  //hospi1.State = HAL_XSPI_STATE_CMD_CFG;
	  HAL_Delay(5000);
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV8;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the programming delay
  */
  __HAL_FLASH_SET_PROGRAM_DELAY(FLASH_PROGRAMMING_DELAY_1);
}

/**
  * @brief OCTOSPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_OCTOSPI1_Init(void)
{

  /* USER CODE BEGIN OCTOSPI1_Init 0 */

  /* USER CODE END OCTOSPI1_Init 0 */

  /* USER CODE BEGIN OCTOSPI1_Init 1 */

  /* USER CODE END OCTOSPI1_Init 1 */
  /* OCTOSPI1 parameter configuration*/
  hospi1.Instance = OCTOSPI1;
  hospi1.Init.FifoThresholdByte = 31;
  hospi1.Init.MemoryMode = HAL_XSPI_SINGLE_MEM;
  hospi1.Init.MemoryType = HAL_XSPI_MEMTYPE_MACRONIX;
  hospi1.Init.MemorySize = HAL_XSPI_SIZE_64MB;
  hospi1.Init.ChipSelectHighTimeCycle = 1;
  hospi1.Init.FreeRunningClock = HAL_XSPI_FREERUNCLK_DISABLE;
  hospi1.Init.ClockMode = HAL_XSPI_CLOCK_MODE_0;
  hospi1.Init.WrapSize = HAL_XSPI_WRAP_NOT_SUPPORTED;
  hospi1.Init.ClockPrescaler = 2;
  hospi1.Init.SampleShifting = HAL_XSPI_SAMPLE_SHIFT_NONE;
  hospi1.Init.DelayHoldQuarterCycle = HAL_XSPI_DHQC_DISABLE;
  hospi1.Init.ChipSelectBoundary = HAL_XSPI_BONDARYOF_128MB;
  hospi1.Init.DelayBlockBypass = HAL_XSPI_DELAY_BLOCK_BYPASS;
  hospi1.Init.Refresh = 0;
  if (HAL_XSPI_Init(&hospi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN OCTOSPI1_Init 2 */

  /* USER CODE END OCTOSPI1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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
