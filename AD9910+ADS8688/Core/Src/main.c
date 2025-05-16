/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "AD9910.h"
#include "ADS8688.h"
#include "math.h"
#include "tjc_usart_hmi.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ADCCHS 1
#define ADCPOINT 512
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint16_t value[10000];
//UART_HandleTypeDef huart2;
extern uchar cfr2[4]; // CFR2瀵�瀛��ㄦ�版��
extern uchar cfr1[4]; // CFR1瀵�瀛��ㄦ�版��
u16 ADS_CH_Value[ADCCHS] = { 0 };
float adc_data[ADCCHS][ADCPOINT];
float adc_fs = 0;
float u1, u2, u3, u4 = 0;
float vpp[ADCCHS];
char str[100];
int a = 10;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void adcin() {
	uint16_t i;
	for (i = 0; i < ADCPOINT; i++) {
		uint16_t j;
		Get_AUTO_RST_Mode_ADC_Data(ADCCHS, ADS_CH_Value);

		for (j = 0; j < ADCCHS; j++) {
//			ADS_CH_Value[j] = Get_MAN_Ch_n_Mode_Data();
			adc_data[j][i] = (ADS_CH_Value[j] - 32768) * 20.48 / 65535;
		}

	}
}

void findvpp() {

	float freq_pin = 1000;
	int point = adc_fs / freq_pin + 1;
	uint16_t i, j;
	float ans[ADCCHS];
	float e[ADCCHS];
	float power[ADCCHS];
	float vdc[ADCCHS];
	float sum[ADCCHS] = { 0 };

	for (j = 0; j < ADCCHS; j++) {
		for (i = 0; i < ADCPOINT; i++)
			sum[j] = sum[j] + adc_data[j][i] * 3.3 / 4096;
		vdc[j] = sum[j] / 1024;
		for (i = 0; i < ADCPOINT; i++) {
			e[j] = e[j]
					+ ((powf((adc_data[j][i] * 3.3 / 4096 - vdc[j]), 2)
							+ powf((adc_data[j][i + 1] * 3.3 / 4096 - vdc[j]),
									2)) / adc_fs / 2);
		}
		power[j] = e[j] * freq_pin / (1024 / point);
		ans[j] = 2 * sqrtf(2 * power[j]);
		vpp[j] = ans[j];
	}
}

float Rout() {
	findvpp();
	u3 = vpp[2];
	u4 = vpp[3];
	int RL = 2000;
	return (u3 - u4) * RL / u4;
}

float Rin() {
	findvpp();
	u1 = vpp[0];
	u2 = vpp[1];
	int RP = 3000;
	return u2 * RP / (u1 - u2);
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
  MX_USART1_UART_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */

	HAL_Delay(72);	//涓��靛欢�剁��寰�绋冲��
	Init_ad9910();	//��濮���AD9910����
	HAL_Delay(72);
//��缃��镐�棰���妫�娴�妯″�锛�PFD锛����板����瀹���璺�锛�DLL锛�
//	AD9910_DRG_FreInit_AutoSet(ENABLE);		// ENABLE:����DLL妯″�锛�DISABLE:����DLL妯″�
//娉ㄩ��锛�����DLL妯″�涓�锛�AD9910浼����ㄨ��寸�镐�棰���妫�娴���锛������ㄦā寮���瑕���杩�DRCTL瀵�瀛��ㄨ�琛��у��
//娉ㄩ��锛�DLL锛��板����瀹���璺�锛��ㄤ�绋冲��棰�������锛�纭�淇�杈��洪�����绋冲����
//娉ㄩ��锛�PFD锛��镐�棰���妫�娴���锛��ㄤ�妫�娴�杈��ュ����棰���涓����ㄩ�����宸�寮�
//	AD9910_DRG_FrePara_Set(100000, 100000000, 100, 100, 100,100);						// ��缃�DLL����
//AD9910_DRG_FrePara_Set(400000, 300000000, 1200000, 2200000, 100,300);		// ��缃�DLL���拌����
//娉ㄩ��锛�DLL���拌�剧疆浼�褰卞��棰�����瀹�����搴���绋冲����
//娉ㄩ��锛�������DLL���板��浠ュ��蹇�棰�����瀹�杩�绋�骞舵��楂�棰���绋冲����

//RAM娉㈠舰�����ㄦā寮���缃�
//AD9910_RAM_WAVE_Set(SQUARE_WAVE);	// ��缃�RAM娉㈠舰�����ㄦā寮�
//娉ㄩ��锛�SQUARE_WAVE琛ㄧず�规尝锛�TRIG_WAVE琛ㄧず瑙���娉㈠舰锛�SQUARE_WAVE琛ㄧず�规尝锛�SINC_WAVE琛ㄧずSINC娉㈠舰

//�烘��棰�����骞�搴�璁剧疆
	Freq_convert(1000);   // 璁剧疆棰���涓�1kHz锛���浣�Hz
	Write_Amplitude(200); // 璁剧疆骞�搴��硷�1-800mV���村��锛�锛�杩���璁剧疆涓�200瀵瑰���骞�搴�
	ADS8688_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
//		sprintf(str, "n0.val=%d", 700);
//		tjc_send_string(str);
		adcin();

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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
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
	while (1) {
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
