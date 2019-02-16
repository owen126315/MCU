#include "stm32f4xx_hal.h"
#include "myUART.h"

void print(char *str, uint16_t len)
{
	HAL_UART_Transmit(&huart2,(uint8_t*)str , len, 1000);
}