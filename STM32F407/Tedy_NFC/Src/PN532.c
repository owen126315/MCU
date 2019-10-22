#include "PN532.h"
#include "Mifare_Classic.h"
#include "Mifare_Ultralight.h"

void PN532_WakeUp(void)
{
	uint8_t wakeup_cmd[]={0x55,0x55,0x00,0x00,0x00,0x00}; 
	 
	HAL_UART_Transmit(&huart1,wakeup_cmd,sizeof(wakeup_cmd),0xffff);
  while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TXE)==0);
}

int PN532_WriteCommand(uint8_t* cmd_data, uint8_t len)
{
	uint8_t cmd[100];
	uint8_t cmd_len = 0;
	uint8_t checksum = 0;
	uint8_t data_return_len = 0;
	cmd[0] = PN532_PREAMBLE;
	cmd[1] = PN532_STARTCODE1;
	cmd[2] = PN532_STARTCODE2;
	cmd[3] = len + 1;
	cmd[4] = 0x100 - cmd[3];
	cmd[5] = PN532_HOSTTOPN532;
	cmd_len += 6;
	
	memcpy(cmd+6,cmd_data,len);
	cmd_len += len;
	
	for(int i=0; i<len+1; i++)
		checksum += cmd[5 + i];
	
	cmd[cmd_len++] = 0x100 - checksum;
	cmd[cmd_len++] = 0x00;
	
	HAL_UART_Transmit(&huart1,&cmd[0],cmd_len,0xffff);
	HAL_Delay(100);
	
	get_rx_data_len(&data_return_len); 
	if(data_return_len == 0)
		return 0;
	if(!PN532_Correct_AckFrame() || !PN532_Correct_CheckSum(data_return_len))
	{
		clean_uart1_rx_data();
		return 0;
	}	
	return 1;
}

int PN532_Correct_AckFrame(void)
{
	uint8_t ack_frame[] = {0x00,0x00,0xFF,0x00,0xFF,0x00};
	
	if(memcmp(ack_frame,uart1_rx_data,6) != 0)
		return 0;
	
	return 1;
}

int PN532_Correct_CheckSum(uint16_t len)
{
	uint8_t checkcode = 0;
	for(int i=11; i<len-2; i++)
		checkcode+=uart1_rx_data[i];
	checkcode = 0x100-checkcode;
	if(checkcode != uart1_rx_data[len-2])
		return 0;
	
	return 1;
}

void PN532_SAMConfig(void)
{
	uint8_t cmd[4];
	
	cmd[0]=0x14;
	cmd[1]=0x01;//Mode
	cmd[2]=0x14;//timeout
	cmd[3]=0x01;//IRQ

	while(1)
	{

		if(PN532_WriteCommand(cmd, sizeof(cmd)))
		{
			clean_uart1_rx_data();
			break;
		}			
	}
}


int PN532_InListPassiveTarget(uint8_t cardbaudrate, NFC_Tag *tag)
{	
	uint8_t list_passive_device_cmd[3];
	list_passive_device_cmd[0] = PN532_COMMAND_INLISTPASSIVETARGET;
	list_passive_device_cmd[1] = 0x01;
	list_passive_device_cmd[2] = cardbaudrate;
	
	if(PN532_WriteCommand(list_passive_device_cmd,sizeof(list_passive_device_cmd)))
	{
		// UID length : 4
		if(uart1_rx_data[9] == 12)
		{
			tag->uid_len = 4;
			tag->type = TAG_TYPE_MIFARE_CLASSIC;
		}				
			
		// UID length : 7
		if(uart1_rx_data[9] == 15)
		{
			tag->uid_len = 7;			
			tag->type = TAG_TYPE_MIFARE_ULTRALIGHT;
		}	
				
		memcpy(tag->uid,&uart1_rx_data[19],tag->uid_len);
		clean_uart1_rx_data();
		return 1;
	}	
	clean_uart1_rx_data();
	return 0;	
}

int PN532_Read_Tag(NFC_Tag *tag)
{
	int sucess = 0;
	switch(tag->type)
	{
		case TAG_TYPE_MIFARE_CLASSIC: 
		{
			sucess = Mifare_Classic_Read(tag);
			break;
		}
		case TAG_TYPE_MIFARE_ULTRALIGHT:
		{
			sucess = Mifare_Ultralight_Read(tag);
			break;
		}
	}
	
	if(sucess == 1)
		return 1;
	else 
		return 0;
}

int PN532_Write_Tag(NFC_Tag *tag)
{
	int sucess = 0;
	switch(tag->type)
	{
		case TAG_TYPE_MIFARE_CLASSIC: 
		{
			sucess = Mifare_Classic_Write(tag);
			break;
		}
		
		case TAG_TYPE_MIFARE_ULTRALIGHT:
		{
			sucess = Mifare_Ultralight_Write(tag);
			break;
		}
	}
	
	if(sucess == 1)
		return 1;
	else 
		return 0;
}
