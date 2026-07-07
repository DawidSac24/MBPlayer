/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
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
#include <App/FileSystem.h>
#include "main.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "App/Log.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define AUDIO_BUFFER_SIZE 16384

uint16_t audio_buffer[AUDIO_BUFFER_SIZE];
volatile uint8_t buffer_state = 0; // 0 = idle, 1 = fill first half, 2 = fill second half
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

COM_InitTypeDef BspCOMInit;

I2S_HandleTypeDef hi2s1;
DMA_HandleTypeDef hdma_spi1_tx;

SD_HandleTypeDef hsd1;

/* USER CODE BEGIN PV */

extern UART_HandleTypeDef hcom_uart[];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2S1_Init(void);
static void MX_SDMMC1_SD_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//int _write(int fd, char *ptr, int len) {
//	HAL_StatusTypeDef hstatus;
//
//	if (fd == 1 || fd == 2) {
//		hstatus = HAL_UART_Transmit(&hcom_uart[COM1], (uint8_t*) ptr, len,
//		HAL_MAX_DELAY);
//		if (hstatus == HAL_OK)
//			return len;
//		else
//			return -1;
//	}
//	return -1;
//}
uint8_t BSP_SD_IsDetected(void) {
//	__IO uint8_t status = SD_PRESENT;
//
//	if (HAL_GPIO_ReadPin(SD_DET_GPIO_Port, SD_DET_Pin) != GPIO_PIN_RESET) {
//		status = SD_NOT_PRESENT;
//	}
//	return status;
	return SD_PRESENT;
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

	/* USER CODE BEGIN 1 */
	BSP_LED_Init(LED_GREEN);
	BSP_LED_Init(LED_YELLOW);
	BSP_LED_Init(LED_RED);
	setvbuf(stdout, NULL, _IONBF, 0);
//	HAL_I2S_Transmit_DMA(&hi2s2, (uint16_t*) audio_buffer, 4096);
	/* USER CODE END 1 */

	/* MPU Configuration--------------------------------------------------------*/
	MPU_Config();

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
	MX_DMA_Init();
	MX_I2S1_Init();
	MX_SDMMC1_SD_Init();
	MX_FATFS_Init();
	/* USER CODE BEGIN 2 */

	/* USER CODE END 2 */

	/* Initialize leds */

	/* Initialize USER push-button, will be used to trigger an interrupt each time it's pressed.*/
	BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

	/* Initialize COM1 port (115200, 8 bits (7-bit data + 1 stop bit), no parity */
	BspCOMInit.BaudRate = 115200;
	BspCOMInit.WordLength = COM_WORDLENGTH_8B;
	BspCOMInit.StopBits = COM_STOPBITS_1;
	BspCOMInit.Parity = COM_PARITY_NONE;
	BspCOMInit.HwFlowCtl = COM_HWCONTROL_NONE;
	if (BSP_COM_Init(COM1, &BspCOMInit) != BSP_ERROR_NONE) {
		Error_Handler();
	}

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	log_init(1, LOG_DEBUG);
	printf("\r\n----------\r\nStarting\r\n");

	// FatFS card mounting
	printf("Mounting the SD Card:\r\n");
	printf("\r\n-------------\r\n");
	FRESULT res = f_mount(&SDFatFS, (TCHAR const*) SDPath, 1);
	if (res != FR_OK) {
		printf("Unable to mount disk: %d\n", res);
		Error_Handler();
	}

	printf("SD Card Information: \r\n");
	printf("Block size : %ld\r\n", hsd1.SdCard.BlockSize);
	printf("Block nmbr : %ld\r\n", hsd1.SdCard.BlockNbr);
	printf("Card size : %ld\r\n",
			(hsd1.SdCard.BlockSize * hsd1.SdCard.BlockNbr) / 1000);
	printf("Card Version : %ld\r\n", hsd1.SdCard.CardVersion);

	list_files();

//	printf("Opening test.wav...\r\n");
//	FIL wavFile;
//	FRESULT res_wav = f_open(&wavFile, "test.wav", FA_READ);
//
//	if (res_wav == FR_OK) {
//		printf("File opened! Playing audio smoothly...\r\n");
//
//		f_lseek(&wavFile, 44); // Skip WAV header
//
//		UINT bytesRead;
//
//		// 1. Pre-fill the ENTIRE buffer before we start playing
//		f_read(&wavFile, audio_buffer, sizeof(audio_buffer), &bytesRead);
//
//		// 2. Start the DMA in Circular Mode!
//		// It will now play music in the background continuously without CPU help.
//		HAL_I2S_Transmit_DMA(&hi2s1, audio_buffer, AUDIO_BUFFER_SIZE);

	// 3. The Ping-Pong loop
	while (1) {
		log_message(CORE, LOG_INFO, "Hello World!\r\n");
		BSP_LED_Toggle(LED_GREEN);
		HAL_Delay(1000);

//			if (buffer_state == 1) {
//				// DMA is playing the second half. CPU fills the FIRST half.
//				// (sizeof(audio_buffer) / 2 is 4096 bytes)
//				f_read(&wavFile, &audio_buffer[0], sizeof(audio_buffer) / 2,
//						&bytesRead);
//				buffer_state = 0; // Go back to sleep
//			}
//
//			if (buffer_state == 2) {
//				// DMA is playing the first half. CPU fills the SECOND half.
//				f_read(&wavFile, &audio_buffer[AUDIO_BUFFER_SIZE / 2],
//						sizeof(audio_buffer) / 2, &bytesRead);
//				buffer_state = 0; // Go back to sleep
//			}
//
//			// If we hit the end of the file, rewind seamlessly
//			if (bytesRead == 0) {
//				f_lseek(&wavFile, 44);
//				// Read instantly to prevent the next chunk from playing silence
//				f_read(&wavFile,
//						&audio_buffer[
//								buffer_state == 1 ? 0 : AUDIO_BUFFER_SIZE / 2],
//						sizeof(audio_buffer) / 2, &bytesRead);
//				buffer_state = 0;
//			}
	}
//	}
	/* USER CODE END WHILE */

	/* USER CODE BEGIN 3 */

	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Supply configuration update enable
	 */
	HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

	/** Configure the main internal regulator output voltage
	 */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

	while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {
	}

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
	RCC_OscInitStruct.HSICalibrationValue = 64;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 4;
	RCC_OscInitStruct.PLL.PLLN = 12;
	RCC_OscInitStruct.PLL.PLLP = 1;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	RCC_OscInitStruct.PLL.PLLR = 2;
	RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
	RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
	RCC_OscInitStruct.PLL.PLLFRACN = 0;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1
			| RCC_CLOCKTYPE_D1PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
	RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief I2S1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2S1_Init(void) {

	/* USER CODE BEGIN I2S1_Init 0 */
	/* USER CODE END I2S1_Init 0 */

	/* USER CODE BEGIN I2S1_Init 1 */

	/* USER CODE END I2S1_Init 1 */
	hi2s1.Instance = SPI1;
	hi2s1.Init.Mode = I2S_MODE_MASTER_TX;
	hi2s1.Init.Standard = I2S_STANDARD_PHILIPS;
	hi2s1.Init.DataFormat = I2S_DATAFORMAT_16B;
	hi2s1.Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;
	hi2s1.Init.AudioFreq = I2S_AUDIOFREQ_44K;
	hi2s1.Init.CPOL = I2S_CPOL_LOW;
	hi2s1.Init.FirstBit = I2S_FIRSTBIT_MSB;
	hi2s1.Init.WSInversion = I2S_WS_INVERSION_DISABLE;
	hi2s1.Init.Data24BitAlignment = I2S_DATA_24BIT_ALIGNMENT_RIGHT;
	hi2s1.Init.MasterKeepIOState = I2S_MASTER_KEEP_IO_STATE_DISABLE;
	if (HAL_I2S_Init(&hi2s1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2S1_Init 2 */
	/* USER CODE END I2S1_Init 2 */

}

/**
 * @brief SDMMC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SDMMC1_SD_Init(void) {

	/* USER CODE BEGIN SDMMC1_Init 0 */
	/* USER CODE END SDMMC1_Init 0 */

	/* USER CODE BEGIN SDMMC1_Init 1 */

	/* USER CODE END SDMMC1_Init 1 */
	hsd1.Instance = SDMMC1;
	hsd1.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
	hsd1.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
	hsd1.Init.BusWide = SDMMC_BUS_WIDE_1B;
	hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
	hsd1.Init.ClockDiv = 20;
//	if (HAL_SD_Init(&hsd1) != HAL_OK) {
//		Error_Handler();
//	}
	/* USER CODE BEGIN SDMMC1_Init 2 */
//	if (HAL_SD_ConfigWideBusOperation(&hsd1, SDMMC_BUS_WIDE_4B) != HAL_OK) {
//		Error_Handler();
//	}
	/* USER CODE END SDMMC1_Init 2 */

}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void) {

	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Stream0_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/* USER CODE BEGIN MX_GPIO_Init_1 */

	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin : SD_DET_Pin */
	GPIO_InitStruct.Pin = SD_DET_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(SD_DET_GPIO_Port, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */

	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s) {
	buffer_state = 1;
}

// DMA tells us the SECOND half of the buffer just finished playing
void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s) {
	buffer_state = 2;
}
/* USER CODE END 4 */

/* MPU Configuration */

void MPU_Config(void) {
	MPU_Region_InitTypeDef MPU_InitStruct = { 0 };

	/* Disables the MPU */
	HAL_MPU_Disable();

	/** Initializes and configures the Region and the memory to be protected
	 */
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER0;
	MPU_InitStruct.BaseAddress = 0x0;
	MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
	MPU_InitStruct.SubRegionDisable = 0x87;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);
	/* Enables the MPU */
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */

	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
		BSP_LED_Toggle(LED_RED);
		BSP_LED_Toggle(LED_YELLOW);
		BSP_LED_Toggle(LED_GREEN);
		for (uint32_t i = 0; i < 5000000; i++)
			;
	}
	/* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
