/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
char mas[10];
bool flag =0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
static void EMS_GPIO_Init(void);
static void LEDs_GPIO_Init(void);
void bitHandler(uint8_t bit, uint16_t GPIO_Pin);
void stepHandler(uint8_t run, uint16_t step_cnt, uint16_t delay);
uint16_t read_from_EMS22();
uint8_t crc8(uint8_t *pcBlock, uint8_t len);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// PINA for Motor control
#define N_ENABLE GPIO_PIN_1
#define MS1     GPIO_PIN_2
#define MS2     GPIO_PIN_3
#define MS3     GPIO_PIN_4
#define STEP    GPIO_PIN_5
#define DIR     GPIO_PIN_6
#define MOTOR_PORT GPIOA

// PINB for EMS22A
//#define PIN_DI GPIO_PIN_11
#define PIN_CLK	GPIO_PIN_13
#define PIN_DO 	GPIO_PIN_14
#define PIN_CS 	GPIO_PIN_12
#define EMS22A_PORT GPIOB

// LED's PIN's
// PINB
#define LED1_YELLOW_PIN GPIO_PIN_1
#define LED2_BLUE_PIN GPIO_PIN_0
#define LED1_YELLOW_PORT GPIOB
#define LED2_BLUE_PORT GPIOB
// PINA
#define LED3_GREEN_PIN GPIO_PIN_7
#define LED3_GREEN_PORT GPIOA
// PIC
#define LED_BOARD_PIN GPIO_PIN_13
#define LED_BOARD_PORT GPIOC


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==huart1.Instance)
	{
		flag = 1;
		HAL_UART_Receive_IT(&huart1, (uint8_t*)mas, 4);
	}
}
//extern uint8_t VCP_write(uint8_t* Buf, uint16_t Len);
//extern int8_t VCP_read(uint8_t* Buf, uint32_t Len);

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
  //uint8_t rx_buf[10];
  uint8_t iter = 0;
  uint16_t pos_temp = 0;
  uint8_t calc_crc = 0;
  uint8_t encoderNumber = 0;
  //char len[2];
  char mas_len;
  char mas_with_len[10];//*mas_with_len = (char*)malloc(sizeof(char) * 1);
  //bool tempBit[6];
  char str[10];
  memset(str, 0, 10);
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  LEDs_GPIO_Init();
  EMS_GPIO_Init();
  HAL_UART_Receive_IT(&huart1, (uint8_t*)mas, 4);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  HAL_GPIO_WritePin(LED_BOARD_PORT, LED_BOARD_PIN, GPIO_PIN_RESET);
  HAL_Delay(1000);
  /*
  snprintf(str, 15, "Hello From U\n");
  HAL_UART_Transmit_IT(&huart1, (uint8_t*)str, strlen(str));
  HAL_GPIO_WritePin(LED1_YELLOW_PORT, LED1_YELLOW_PIN, GPIO_PIN_SET);

  while (rx_buf[0] != '0')
  {
	  HAL_UART_Receive(&huart1, (uint8_t*)rx_buf, 1, 100);
  }
  snprintf(str, 15, "You did it\n");
  HAL_UART_Transmit_IT(&huart1, (uint8_t*)str, strlen(str));
  HAL_GPIO_WritePin(LED2_BLUE_PORT, LED2_BLUE_PIN, GPIO_PIN_SET);
  */
  //HAL_UART_Transmit(&huart1, "yep\n", 7,1000);
  //printf("iter: %d\n", iter);
  //printf("\n\rWrite 0x80 < && > 0x01 for stepper motor driver\n");
  //printf("\n\rWrite 0x80 for encoder position\n");
  HAL_Delay(1000);
  memset(mas, 0, 10);
  memset(mas_with_len, 0, 10);
  while (1)
  {
	 //printf("\n\rlisten ");
	 //HAL_UART_Receive(&huart1, (uint8_t*)mas, 4, 1);
	 if(flag)
	 {

	 if ((mas[0] != 0) && (mas[0] < 10))
	 {
		 mas_len = mas[0] + 1;
		 for (iter = 0; iter < mas_len; iter++)
		 {
			 mas_with_len[iter] = mas[iter];
		 }

		 calc_crc = crc8(mas_with_len, mas_len);

		 //HAL_UART_Transmit(&huart1, (uint8_t*)calc_crc, 1, 1);
		 if (calc_crc == mas[mas_len])
		 {
			 //printf("\n\rcrc is good %d", calc_crc);
			 if (mas[2] == 64)
			 {
				 //printf("\n\rencoder com");
				 pos_temp = 0;
				 pos_temp = read_from_EMS22();
				 mas[0] = 3;
				 mas[1] = encoderNumber;
				 mas[2] = (pos_temp >> 8) & 0xff;
				 mas[3] = pos_temp & 0xff;
				 mas[4] = crc8(mas, 4);
				 HAL_UART_Transmit(&huart1, (uint8_t*)mas, 5, 1);
				 encoderNumber++;
				 //printf("\n\rEMS22:%d", pos_temp);
				 //if (pos_temp > 0)
				//	 printf("\n\rEMS22 more 0:%d", pos_temp);
				 //else
				//	 printf("\n\rEMS22 = 0");
			 }
			 else if (mas[2] < 64)
			 {
				 //printf("\n\rdriver com");

				 str[0] = (mas[2] & 0b00100000) ? 1 : 0;
				 str[1] = (mas[2] & 0b00010000) ? 1 : 0;
				 str[2] = (mas[2] & 0b00001000) ? 1 : 0;
				 str[3] = (mas[2] & 0b00000100) ? 1 : 0;
				 str[4] = (mas[2] & 0b00000010) ? 1 : 0;
				 str[5] = (mas[2] & 0b00000001) ? 1 : 0;
				// printf("\n\ri receive this: %d%d%d%d%d%d", str[0],
				 //			str[1],str[2],str[3],str[4],str[5]);
				 bitHandler(str[0], N_ENABLE);
				 bitHandler(str[1], MS1);
				 bitHandler(str[2], MS2);
				 bitHandler(str[3], MS3);
				 bitHandler(str[5], DIR);
				 stepHandler(str[4], 200, 500);
			 }
			 else if ((mas[2] > 64) && (mas[2] < 128))
			 {
				// printf("\n\rdriver and encoder com");

				 str[0] = (mas[2] & 0b00100000) ? 1 : 0;
				 str[1] = (mas[2] & 0b00010000) ? 1 : 0;
				 str[2] = (mas[2] & 0b00001000) ? 1 : 0;
				 str[3] = (mas[2] & 0b00000100) ? 1 : 0;
				 str[4] = (mas[2] & 0b00000010) ? 1 : 0;
				 str[5] = (mas[2] & 0b00000001) ? 1 : 0;
				// printf("\n\ri receive this: %d%d%d%d%d%d", str[0],
				// 				str[1],str[2],str[3],str[4],str[5]);
				 bitHandler(str[0], N_ENABLE);
				 bitHandler(str[1], MS1);
				 bitHandler(str[2], MS2);
				 bitHandler(str[3], MS3);
				 bitHandler(str[5], DIR);
				 stepHandler(str[4], 200, 500);

				 pos_temp = 0;
				 pos_temp = read_from_EMS22();
				 mas[0] = 3;
				 mas[1] = encoderNumber;
				 mas[2] = (pos_temp >> 8) & 0xff;
				 mas[3] = pos_temp & 0xff;
				 mas[4] = crc8(mas, 4);
				 HAL_UART_Transmit(&huart1, (uint8_t*)mas, 5, 1);
				 encoderNumber++;
				 //printf("\n\rEMS22:%d", pos_temp);
				// if (pos_temp > 0)
				//	 printf("\n\rEMS22 more 0:%d", pos_temp);
				// else
				//	 printf("\n\rEMS22 = 0");
			 }
			 else
			 {
				 //printf("\n\rleds com");

				 str[0] = (mas[2] & 0b00001000) ? 1 : 0;
				 str[1] = (mas[2] & 0b00000001) ? 1 : 0;
				 str[2] = (mas[2] & 0b00000010) ? 1 : 0;

				 if (str[0])
				 {
					 if ((str[1]) && (str[2]))
						 HAL_GPIO_WritePin(LED3_GREEN_PORT, LED3_GREEN_PIN, GPIO_PIN_SET);
					 else if (str[2])
						 HAL_GPIO_WritePin(LED2_BLUE_PORT, LED2_BLUE_PIN, GPIO_PIN_SET);
					 else
						 HAL_GPIO_WritePin(LED1_YELLOW_PORT, LED1_YELLOW_PIN, GPIO_PIN_SET);
				 } else {
					 if ((str[1]) && (str[2]))
						 HAL_GPIO_WritePin(LED3_GREEN_PORT, LED3_GREEN_PIN, GPIO_PIN_RESET);
					 else if (str[2])
						 HAL_GPIO_WritePin(LED2_BLUE_PORT, LED2_BLUE_PIN, GPIO_PIN_RESET);
					 else
					 	HAL_GPIO_WritePin(LED1_YELLOW_PORT, LED1_YELLOW_PIN, GPIO_PIN_RESET);
				 }

			 }

			 //printf("\n\r");
		 }
//			 else {
//			// printf("\n\rcrc is bad %d", calc_crc);
//		 }

		 memset(mas_with_len, 0, 10);
	 }

	 memset(mas, 0, 10);

	 flag =0;

	 //printf("\n\ri receive this: %d and this %d", len[0], len[1]);

		 //printf("\n\rlen more than zero ");

		 //mas_len = len[0] + 1;
		 //printf("\n\rmessage len %d ", mas_len);
		 //mas = (char*)realloc(mas, sizeof(char) * mas_len);
		 /*
		 HAL_UART_Receive(&huart1, (uint8_t*)mas, mas_len, 1000);
		 //mas_with_len = (char*)realloc(mas_with_len, sizeof(char) * mas_len);

		 mas_with_len[0] = len;
		 for (iter = 0; iter < len; iter++)
		 {
			 mas_with_len[iter + 1] = mas[iter];
		 }
		 calc_crc = crc8(mas_with_len, len + 1);
		 if (calc_crc == mas[len[0]])
		 {
			 printf("\n\rcrc is good ");
		 }
		 */


	 //memset(str, 0, 10);
	 }

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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4
                          |GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA1 PA2 PA3 PA4
                           PA5 PA6 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4
                          |GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
static void EMS_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitStruct.Pin = PIN_CS|PIN_CLK;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(EMS22A_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = PIN_DO;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(EMS22A_PORT, &GPIO_InitStruct);

	HAL_GPIO_WritePin(EMS22A_PORT, PIN_CLK, GPIO_PIN_SET);
	HAL_GPIO_WritePin(EMS22A_PORT, PIN_CS, GPIO_PIN_SET);
}

static void LEDs_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_BOARD_PORT, LED_BOARD_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED1_YELLOW_PORT, LED1_YELLOW_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED2_BLUE_PORT, LED2_BLUE_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED3_GREEN_PORT, LED3_GREEN_PIN, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_BOARD */
  GPIO_InitStruct.Pin = LED_BOARD_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_BOARD_PORT, &GPIO_InitStruct);

  /*Configure GPIO pin : LED2_BLUE & LED1_YELLOW */
  GPIO_InitStruct.Pin = LED2_BLUE_PIN|LED1_YELLOW_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED1_YELLOW_PORT, &GPIO_InitStruct);

  /*Configure GPIO pin : LED3_GREEN */
  GPIO_InitStruct.Pin = LED3_GREEN_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED3_GREEN_PORT, &GPIO_InitStruct);

}

uint16_t read_from_EMS22()
{
	HAL_GPIO_WritePin(GPIOB, PIN_CS, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, PIN_CS, GPIO_PIN_RESET);
	bool read_bit[16];
	uint16_t pos = 0;
	uint8_t iter;
	//uint8_t parity = 0;

	for (iter = 0; iter < 16; iter++)
	{
		HAL_GPIO_WritePin(GPIOB, PIN_CLK, GPIO_PIN_RESET);
		HAL_Delay(1);
		HAL_GPIO_WritePin(GPIOB, PIN_CLK, GPIO_PIN_SET);
		HAL_Delay(1);
		read_bit[iter] = HAL_GPIO_ReadPin(GPIOB, PIN_DO);

		//if (iter < 15)
		//	parity += read_bit[iter];
	}

	//parity %= 2;

	//if (read_bit[15] == parity)
	//{
	for (iter = 0; iter < 10; iter++)
	{
		//pos += read_bit[iter] * pow(2, 10 - (iter));
		pos += read_bit[iter] * (1 << (9 - iter));
	}
	HAL_GPIO_WritePin(GPIOB, PIN_CS, GPIO_PIN_RESET);
	return pos;
	//}

	//HAL_GPIO_WritePin(GPIOB, PIN_CS, GPIO_PIN_RESET);
	//VCP_write(" BAD ", 6);
	//return -1;
}
/*
void writeIntro()
{
	VCP_write("\n\rWrite 1 for stepper motor driver ", 33);
	VCP_write("\n\rWrite 2 for encoder position ", 33);
	//VCP_write("\n\rN_ENABLE MS1 MS2 MS3 STEP DIR: ", 33);
}
*/
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}

void bitHandler(uint8_t bit, uint16_t GPIO_Pin)
{
	if(bit & 0x1) {
		HAL_GPIO_WritePin(GPIOA, GPIO_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(GPIOA, GPIO_Pin, GPIO_PIN_RESET);
	}
}

void stepHandler(uint8_t run, uint16_t step_cnt, uint16_t delay)
{
	int i = 0;

	if (run & 0x1) {
		for(i = 0; i < step_cnt; i++)
		{
			HAL_Delay(1);
			HAL_GPIO_WritePin(GPIOA, STEP, GPIO_PIN_SET);

			HAL_Delay(1);
			HAL_GPIO_WritePin(GPIOA, STEP, GPIO_PIN_RESET);

			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

			//HAL_Delay(delay);
		}
	}
}

uint8_t crc8(uint8_t *pcBlock, uint8_t len)
{
	uint8_t crc = 0xFF;
	uint8_t i;

	while (len--)
	{
		crc ^= *pcBlock++;

		for (i = 0; i < 8; i++)
			crc = crc & 0x80 ?(crc << 1) ^ 0x31 : crc << 1;
	}

	return crc;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
