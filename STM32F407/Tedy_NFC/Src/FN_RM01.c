#include "FN_RM01.h"

void FN_RM01_Get_File_Num(int *file_num)
{	
	char check_file_num_cmd[5];
	check_file_num_cmd[0] = 0x7E;
	check_file_num_cmd[1] = 0x03;
	check_file_num_cmd[2] = 0xC5;
	check_file_num_cmd[3] = 0xC8;
	check_file_num_cmd[4] = 0x7E;
	
	HAL_UART_Transmit(&huart2, (uint8_t*)&check_file_num_cmd, sizeof(check_file_num_cmd), 1000);
	HAL_Delay(100);
	
	*file_num += uart2_rx_data[1]<<8;
	*file_num += uart2_rx_data[2] - 3;
}

int FN_RM01_Play_Sound(char *file_name)
{
	char play_file_command[9];
	play_file_command[0] = 0x7E;
	play_file_command[1] = 0x07;
	play_file_command[2] = 0xA3;
	play_file_command[8] = 0x7E;
	play_file_command[7] += play_file_command[1];
	play_file_command[7] += play_file_command[2];
	
	for(int i=0;i<4;i++)
	{
		play_file_command[3+i] = file_name[i];
		play_file_command[sizeof(play_file_command)-2] += file_name[i];	
	}
	
	clean_uart2_rx_data();
	HAL_UART_Transmit(&huart2, (uint8_t*)&play_file_command, sizeof(play_file_command), 1000);
	HAL_Delay(100);
	
	if(uart2_rx_data[0] == 0x00)
		return 1;
	else
		return 0;
}

int FN_RM01_Record_Sound(char *file_name)
{
	char record_command[9];
	record_command[0] = 0x7E;
	record_command[1] = 0x07;
	record_command[2] = 0xD6;
	record_command[8] = 0x7E;
	record_command[7] += record_command[1];
	record_command[7] += record_command[2];
	
	for(int i=0;i<4;i++)
	{
		record_command[3+i] = file_name[i];
		record_command[sizeof(record_command)-2] += file_name[i];	
	}
	
	clean_uart2_rx_data();
	HAL_UART_Transmit(&huart2, (uint8_t*)&record_command, sizeof(record_command), 1000);
	HAL_Delay(100);
	
	if(uart2_rx_data[0] == 0x00)
		return 1;
	else
		return 0;
}

int FN_RM01_Stop_Record(void)
{
	char stop_record_command[5];
	stop_record_command[0] = 0x7E;
	stop_record_command[1] = 0x03;
	stop_record_command[2] = 0xD9;
	stop_record_command[3] = 0xDc;
	stop_record_command[4] = 0x7E;
	
	clean_uart2_rx_data();
	HAL_UART_Transmit(&huart2, (uint8_t*)&stop_record_command, sizeof(stop_record_command), 1000);
	HAL_Delay(100);
	
	if(uart2_rx_data[0] == 0x00)
		return 1;
	else
		return 0;
}

int FN_RM01_Remove_Sound(char *file_name)
{
	char remove_command[9];
	remove_command[0] = 0x7E;
	remove_command[1] = 0x07;
	remove_command[2] = 0xDB;
	remove_command[8] = 0x7E;
	remove_command[7] += remove_command[1];
	remove_command[7] += remove_command[2];
	
	for(int i=0;i<4;i++)
	{
		remove_command[3+i] = file_name[i];
		remove_command[sizeof(remove_command)-2] += file_name[i];	
	}
	
	clean_uart2_rx_data();
	HAL_UART_Transmit(&huart2, (uint8_t*)&remove_command, sizeof(remove_command), 1000);
	HAL_Delay(100);
	
	if(uart2_rx_data[0] == 0x00)
		return 1;
	else
		return 0;
}

void FN_RM01_Get_FileName(int file_num, char *file_name)
{
	file_name[0] = 0x30 | (file_num/1000);
	file_name[1] = 0x30 | (file_num/100)%10;
	file_name[2] = 0x30 | (file_num/10)%10;
	file_name[3] = 0x30 | (file_num%10);
}
