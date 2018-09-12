/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_hal.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint8_t  Rx_buffer[5],dataIn[5];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_I2C1_Init();

  /* USER CODE BEGIN 2 */
	HAL_I2C_Slave_Receive_IT(&hi2c1, Rx_buffer, 5);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
		
  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* I2C1 init function */
static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x2000090E;
  hi2c1.Init.OwnAddress1 = 192;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Analogue filter 
    */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Digital filter 
    */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7 
                          |GPIO_PIN_8|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_4 
                          |GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3 
                           PA4 PA5 PA6 PA7 
                           PA8 PA11 PA12 PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7 
                          |GPIO_PIN_8|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB3 PB4 
                           PB5 PB6 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_4 
                          |GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	memcpy(dataIn,Rx_buffer,5);

	if(dataIn[0]==0x01)	//mode 1 (set pin, 1=HIGH 0=LOW)
	{
		//Byte 1
		for(int i=0; i<8; i++)
		{
			switch(i)
			{
				case 0:
				{
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, dataIn[1]%2);
					break;
				}
				case 1:
				{
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, dataIn[1]%2);
					break;
				}
				case 2:
				{
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, dataIn[1]%2);
					break;
				}
				case 3:
				{
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, dataIn[1]%2);
					break;
				}
				case 4:
				{
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, dataIn[1]%2);
					break;
				}		
				case 5:
				{
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, dataIn[1]%2);
					break;
				}			
				case 6:
				{
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, dataIn[1]%2);
					break;
				}
				case 7:
				{
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, dataIn[1]%2);
					break;
				}
			}
			dataIn[1]=dataIn[1]>>1;
	}
		//Byte 2
		for(int i=0; i<8; i++)
		{
			switch(i)
			{
				case 0:
				{
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, dataIn[2]%2);
					break;
				}
				case 1:
				{
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, dataIn[2]%2);
					break;
				}
				case 2:
				{
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, dataIn[2]%2);
					break;
				}
				case 3:
				{
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, dataIn[2]%2);
					break;
				}
				case 4:
				{
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, dataIn[2]%2);
					break;
				}		
				case 5:
				{
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, dataIn[2]%2);
					break;
				}			
				case 6:
				{
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, dataIn[2]%2);
					break;
				}
				case 7:
				{
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, dataIn[2]%2);
					break;
				}
			}
			dataIn[2]=dataIn[2]>>1;
		}
		//Byte 3
		for(int i=0; i<3; i++)
		{
			switch(i)
			{
				case 0:
				{
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, dataIn[3]%2);
					break;
				}
				case 1:
				{
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, dataIn[3]%2);
					break;
				}
				case 2:
				{
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, dataIn[3]%2);
					break;
				}
			}
			dataIn[3]=dataIn[3]>>1;
		}
	}
	if(dataIn[0]==0x02)	//mode 2 (toggle all pin for specific time)
	{
		//Byte 1
		for(int i=0; i<8; i++)
		{
			switch(i)
			{
				case 0:
				{
					if(dataIn[1]%2)
						HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
					break;
				}
				case 1:
				{
					if(dataIn[1]%2)
						HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
					break;
				}
				case 2:
				{
					if(dataIn[1]%2)
						HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);
					break;
				}
				case 3:
				{
					if(dataIn[1]%2)
						HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_3);
					break;
				}
				case 4:
				{
					if(dataIn[1]%2)
						HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4);
					break;
				}		
				case 5:
				{
					if(dataIn[1]%2)
						HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
					break;
				}			
				case 6:
				{
					if(dataIn[1]%2)
						HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
					break;
				}
				case 7:
				{
					if(dataIn[1]%2)
						HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7);
					break;
				}
			}
			dataIn[1]=dataIn[1]>>1;
	}
		//Byte 2
		for(int i=0; i<8; i++)
		{
			switch(i)
			{
				case 0:
				{
					if(dataIn[2]%2)
						HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
					break;
				}
				case 1:
				{
					if(dataIn[2]%2)
						HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
					break;
				}
				case 2:
				{
					if(dataIn[2]%2)
						HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
					break;
				}
				case 3:
				{
					if(dataIn[2]%2)
						HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_11);
					break;
				}
				case 4:
				{
					if(dataIn[2]%2)
						HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_12);
					break;
				}		
				case 5:
				{
					if(dataIn[2]%2)
						HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_15);
					break;
				}			
				case 6:
				{
					if(dataIn[2]%2)
						HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
					break;
				}
				case 7:
				{
					if(dataIn[2]%2)
						HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
					break;
				}
			}
			dataIn[2]=dataIn[2]>>1;
		}
		//Byte 3
		for(int i=0; i<3; i++)
		{
			switch(i)
			{
				case 0:
				{
					if(dataIn[3]%2)
						HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
					break;
				}
				case 1:
				{
					if(dataIn[3]%2)
						HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);
					break;
				}
				case 2:
				{
					if(dataIn[3]%2)
						HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
					break;
				}
			}
			dataIn[3]=dataIn[3]>>1;
		}		
		//Delay(dataIn[4]);	//delay time = dataIn[1]*100 ms
		for(uint32_t i=0; i<dataIn[4];i++)
			for(uint32_t j=0;j<1000000;j++);
		memcpy(dataIn,Rx_buffer,5);
				//Byte 1
		for(int i=0; i<8; i++)
		{
			switch(i)
			{
				case 0:
				{
					if(dataIn[1]%2)
						HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
					break;
				}
				case 1:
				{
					if(dataIn[1]%2)
						HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
					break;
				}
				case 2:
				{
					if(dataIn[1]%2)
						HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);
					break;
				}
				case 3:
				{
					if(dataIn[1]%2)
						HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_3);
					break;
				}
				case 4:
				{
					if(dataIn[1]%2)
						HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4);
					break;
				}		
				case 5:
				{
					if(dataIn[1]%2)
						HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
					break;
				}			
				case 6:
				{
					if(dataIn[1]%2)
						HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
					break;
				}
				case 7:
				{
					if(dataIn[1]%2)
						HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7);
					break;
				}
			}
			dataIn[1]=dataIn[1]>>1;
	}
		//Byte 2
		for(int i=0; i<8; i++)
		{
			switch(i)
			{
				case 0:
				{
					if(dataIn[2]%2)
						HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
					break;
				}
				case 1:
				{
					if(dataIn[2]%2)
						HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
					break;
				}
				case 2:
				{
					if(dataIn[2]%2)
						HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
					break;
				}
				case 3:
				{
					if(dataIn[2]%2)
						HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_11);
					break;
				}
				case 4:
				{
					if(dataIn[2]%2)
						HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_12);
					break;
				}		
				case 5:
				{
					if(dataIn[2]%2)
						HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_15);
					break;
				}			
				case 6:
				{
					if(dataIn[2]%2)
						HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
					break;
				}
				case 7:
				{
					if(dataIn[2]%2)
						HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
					break;
				}
			}
			dataIn[2]=dataIn[2]>>1;
		}
		//Byte 3
		for(int i=0; i<3; i++)
		{
			switch(i)
			{
				case 0:
				{
					if(dataIn[3]%2)
						HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
					break;
				}
				case 1:
				{
					if(dataIn[3]%2)
						HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);
					break;
				}
				case 2:
				{
					if(dataIn[3]%2)
						HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
					break;
				}
			}
			dataIn[3]=dataIn[3]>>1;
		}
	}
	
	HAL_I2C_Slave_Receive_IT(&hi2c1, Rx_buffer, 5);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
