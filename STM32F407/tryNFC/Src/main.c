
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "nfc.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint8_t aRxBuffer;
uint8_t UID_len;
uint8_t UID[7]; 

__IO  uint16_t uart_p = 0;
uint8_t data_buff[UART_BUFF_SIZE];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
uint8_t *get_rebuff(uint16_t *len); 
void clean_rebuff(void);


/*
bool nfc_mifareultralight_AuthenticateBlock(void);
bool isUnformated(void);
*/

/*
void nfc_read(void);
void nfc_write(uint8_t write_data);
void nfc_PsdVerifyKeyA(void);
*/
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
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
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_UART_Receive_IT(&huart1,&aRxBuffer,1); 
	printf("program strat!!!\n");
	nfc_WakeUp();
	nfc_SAMConfig();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
		HAL_NVIC_DisableIRQ(EXTI4_IRQn);
		if(nfc_InListPassiveTarget(PN532_MIFARE_ISO14443A, UID, &UID_len))
		{
			if(UID_len == 4)
				nfc_mifareclassic_Read();
				
			if(UID_len == 7)
				//nfc_mifareultralight_AuthenticateBlock();
				printf("ok");
			HAL_NVIC_EnableIRQ(EXTI4_IRQn);
			HAL_Delay(500);
		}
		
    //nfc_PsdVerifyKeyA();
  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Enables the Clock Security System 
    */
  HAL_RCC_EnableCSS();

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

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
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
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
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin : PE4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

}

/* USER CODE BEGIN 4 */

/*
bool nfc_mifareultralight_AuthenticateBlock(void)
{
	if(isUnformated())
	{
		printf("Tag Is Unformatted!!!\n");
		return false;
	}
	clean_rebuff();
	
	return false;
}

bool isUnformated(void)
{
	uint8_t data[12];
  uint8_t temp=0;
	uint16_t len;
	
	data[0]=0x00;
  data[1]=0x00;
  data[2]=0xFF;
 
  data[3]=0x05; //length
	data[4]=0x100-data[3];//LCS
	data[5]=0xD4;
	data[6]=0x40;
	data[7]=0x01;//Tg
	data[8]=0x30;//DataOut
	data[9]=0x04;//Page
	
	for(int i=5;i<10;i++)
		temp+=data[i];
	data[10]=0x100-temp;//DCS
	data[11]=0x00;	
	
	HAL_UART_Transmit(&huart1,&list_passive_device_cmd[0],12,0xffff);//?USART2??? length?????data
	while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TXE)==0); //????,??????
  
  HAL_Delay(150);
  get_rebuff(&len); 
	
	if(len > 0)
	{
		if(correct_AckFrame())
		{
			if(correct_CheckSum(len))
			{
				if(data_buff[12] == 0)
				{
					
					clean_rebuff();
					return false;
				}
				return (data_buff[13] != 0xFF && data_buff[14] != 0xFF && data_buff[15] != 0xFF && data_buff[16] != 0xFF);
			}
		}
	}
	clean_rebuff();
	return true;
}

void readCapabilityContainer(void)
{
	uint8_t data[12];
  uint8_t temp=0;
  uint8_t CheckCode=0; 
	uint16_t len;
	
	data[0]=0x00;
  data[1]=0x00;
  data[2]=0xFF;
 
  data[3]=0x05; //length
	data[4]=0x100-data[3];//LCS
	data[5]=0xD4;
	data[6]=0x40;
	data[7]=0x01;//Tg
	data[8]=0x30;//DataOut
	data[9]=0x03;//Page
	
	for(int i=5;i<10;i++)
		temp+=data[i];
	data[10]=0x100-temp;//DCS
	data[11]=0x00;	
	
	HAL_UART_Transmit(&huart1,&list_passive_device_cmd[0],12,0xffff);//?USART2??? length?????data
	while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TXE)==0); //????,??????
  
  HAL_Delay(150);
  get_rebuff(&len); 
	
	if(len > 0)
	{
		if(correct_AckFrame())
		{
			if(correct_CheckSum(len))
			{
				
			}
		}
	}
}
*/
/*
void  nfc_PsdVerifyKeyA(void)
{
  
	uint8_t i,data[22];
  uint8_t temp=0;
  uint8_t CheckCode=0; //?????
	uint16_t len; 
  data[0]=0x00;
  data[1]=0x00;
  data[2]=0xFF;
  
  data[3]=0x0F; //? ??
  data[4]=0x100-data[3]; //? ?? ??  0x100-data[3]
  
  data[5]=0xD4; //?????
  data[6]=0x40; //?????
  
  data[7]=0x01;
  data[8]=0x60;
  data[9]=0x03; 
  
  data[10]=0xFF; //KEY A ?? FF FF FF FF FF FF
  data[11]=0xFF;
  data[12]=0xFF;
  data[13]=0xFF;
  data[14]=0xFF;
  data[15]=0xFF;
  
  data[16]=UID[0];
  data[17]=UID[1];
  data[18]=UID[2];
  data[19]=UID[3]; 
  
  for(i=5;i<20;i++)
  {
      temp+=data[i];
  }

  data[20]=0x100-temp;   //?? ??   0x100-  
  data[21]=0x00;  
	
  HAL_UART_Transmit(&huart1,&data[0],25,0xffff);//?USART2??? length?????data
  while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TXE)==0); //????,??????
  
  HAL_Delay(180); 
  temp=0;
  
  get_rebuff(&len); 
  //00 00 FF 04 FC D4 4A 01 00 E1 00    
	printf("len: %d",len);
  for(i=11;i<14;i++)
  {
      temp+=data_buff[i];
  }
  CheckCode=0x100-temp;
  
  if(CheckCode==data_buff[14])
  {
     clean_rebuff();

     if(flag_nfc_status==0)
     {      
       nfc_read();
       flag_nfc_status=0;
     }
     if(flag_nfc_status==2)
     {           
        nfc_write(0x55);//?????0x55
        flag_nfc_status=0;
     }     
     if(flag_nfc_status==3)
     {          
        nfc_write(0xAA);//?????0xAA
        flag_nfc_status=0;
     } 
  }   
}







void nfc_read(void)
{
  uint8_t i,data[12];
  uint8_t temp=0;
  uint8_t CheckCode=0; //?????
  uint16_t len; 
  data[0]=0x00;
  data[1]=0x00;
  data[2]=0xFF;
  
  data[3]=0x05; //? ??
  data[4]=0xFB; //? ?? ??  0x100-data[3]
  
  data[5]=0xD4; //?????
  data[6]=0x40; //?????
  
  data[7]=0x01;
  data[8]=0x30;
  data[9]=0x02; //?????16???? 
  
  temp=0;
  for(i=5;i<10;i++)
  {
      temp+=data[i];
  }
  data[10]=0x100-temp; 
  data[11]=0x00;  
  HAL_UART_Transmit(&huart1,&data[0],12,0xffff);//?USART2??? length?????data
  while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TXE)==0); //????,??????
  
  HAL_Delay(180); 
  temp=0;
  get_rebuff(&len); 
  
  for(i=11;i<30;i++)
  {
    temp+=data_buff[i];
  }
  CheckCode=0x100-temp;  
  if(CheckCode==data_buff[30])
  {
    printf("?????:%x\n",data_buff[14]);
    if((UID_backup[0]!=UID[0])|(UID_backup[1]!=UID[1])|(UID_backup[2]!=UID[2])|(UID_backup[3]!=UID[3]))
    {
      HAL_Delay(100);
    }
    UID_backup[0]=UID[0];
    UID_backup[1]=UID[1];
    UID_backup[2]=UID[2];
    UID_backup[3]=UID[3];
    clean_rebuff();
  }
}

void nfc_write(uint8_t write_data)
{
  uint8_t i,data[28];
  uint8_t temp=0;
  uint8_t CheckCode=0; //?????
  uint16_t len; 
  data[0]=0x00;
  data[1]=0x00;
  data[2]=0xFF;
  
  data[3]=0x15; //? ??
  data[4]=0xEB; //? ?? ??  0x100-data[3]
  
  data[5]=0xD4; //?????
  data[6]=0x40; //?????
  
  data[7]=0x01; //????6?? ??1????
  data[8]=0xA0; //?
  data[9]=0x02; //?????16???? 
  
  data[10]=write_data; //? 1 ?? ??
  data[11]=0x00;
  data[12]=0x00; //? 3 ?? ??
  data[13]=0x00;
  data[14]=0x00; //? 5 ?? ??
  data[15]=0x00;
  data[16]=0x00; //? 7 ?? ??
  data[17]=0x00;
  data[18]=0x00; //? 9 ?? ??
  data[19]=0x00;
  data[20]=0x00; //? 11 ?? ??
  data[21]=0x00;
  data[22]=0x00; //? 13 ?? ??
  data[23]=0x00;
  data[24]=0x00; //? 15 ?? ??
  data[25]=0x00;
  
  temp=0;
  for(i=5;i<26;i++)
  {
      temp+=data[i];
  }
  data[26]=0x100-temp; 
  data[27]=0x00;  
  
  HAL_UART_Transmit(&huart1,&data[0],28,0xffff);//?USART2??? length?????data
  while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TXE)==0); //????,??????
  
  HAL_Delay(180); 
  temp=0;
  get_rebuff(&len); 
   
  for(i=11;i<14;i++)
  {
    temp+=data_buff[i];
  }
  CheckCode=0x100-temp;
  if(CheckCode==data_buff[14])
  {

    if((UID_backup[0]!=UID[0])|(UID_backup[1]!=UID[1])|(UID_backup[2]!=UID[2])|(UID_backup[3]!=UID[3]))
    {
      HAL_Delay(100);
    }
    UID_backup[0]=UID[0];
    UID_backup[1]=UID[1];
    UID_backup[2]=UID[2];
    UID_backup[3]=UID[3];  
    clean_rebuff();    
  }
}
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  if(uart_p<UART_BUFF_SIZE)
  {
    data_buff[uart_p++] =aRxBuffer; 
    HAL_UART_Receive_IT(&huart1,&aRxBuffer,1);
  }  
}

uint8_t *get_rebuff(uint16_t *len) 
{
    *len = uart_p;
    return (uint8_t *)&data_buff;
}

void clean_rebuff(void)
{
  uint16_t i=UART_BUFF_SIZE+1;
  
  uart_p = 0;
	while(i)
		data_buff[--i]=0;
}

int fputc(int ch,FILE *f)
{
    uint8_t temp[1]={ch};
    HAL_UART_Transmit(&huart2,temp,1,2);
		return ch;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
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
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
