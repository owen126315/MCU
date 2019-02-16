#include "stm32f4xx_hal.h"
#include "nfc.h"

bool writeCommand(uint8_t* cmd_data, uint8_t len)
{
	uint8_t cmd[100];
	uint8_t cmd_len = 0;
	uint8_t checksum = 0;
	uint16_t data_return_len = 0;
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
	HAL_Delay(150);
	
	get_rebuff(&data_return_len); 
	if(data_return_len == 0)
		return false;
	if(!correct_AckFrame() || !correct_CheckSum(data_return_len))
	{
		clean_rebuff();
		return false;
	}	
	return true;
}

bool correct_AckFrame(void)
{
	uint8_t ack_frame[] = {0x00,0x00,0xFF,0x00,0xFF,0x00};
	for(int i=0; i<5; i++)
	{
		if(ack_frame[i] != data_buff[i])
		{
			printf("Error Frame!!!\n");
			return false;
		}				
	}
	return true;
}

bool correct_CheckSum(uint16_t len)
{
	uint8_t checkcode = 0;
	for(int i=11; i<len-2; i++)
		checkcode+=data_buff[i];
	checkcode = 0x100-checkcode;
	if(checkcode != data_buff[len-2])
	{
		printf("Error CheckSum!!!\n");
		return false;
	}
	return true;
}

void nfc_WakeUp(void)
{
	uint8_t wakeup_cmd[]={0x55,0x55,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x03,0xFD,0xD4,0x14,0x01,0x17,0x00};  
	uint16_t len;  
	
  while(1)
  { 	
		HAL_UART_Transmit(&huart1,&wakeup_cmd[0],24,0xffff);
  	while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TXE)==0);
    
    HAL_Delay(180); 
    get_rebuff(&len); 
		
    if(len > 0)
    {
			if(correct_AckFrame())
			{
				if(correct_CheckSum(len))
				{
					clean_rebuff();
					break;
				}
			}
			
			else
			{
				printf("Wakeup Fail!!!\n");
				clean_rebuff();
			}
    }
  }
}

void nfc_SAMConfig(void)
{
	uint8_t cmd[4];
	
	cmd[0]=0x14;
	cmd[1]=0x01;//Mode
	cmd[2]=0x14;//timeout
	cmd[3]=0x01;//IRQ

	while(1)
	{
		if(writeCommand(cmd, sizeof(cmd)))
		{
			clean_rebuff();
			break;
		}
				
		else
			printf("SAM Config Fail!!!\n");
	}
	printf("SAM Config Done!!!\n");
}

bool nfc_InListPassiveTarget(uint8_t cardbaudrate, uint8_t *uid, uint8_t* uidLength)
{
	for(int i=0; i<*uidLength; i++)
		uid[i] = 0;
	*uidLength = 0;
	uint8_t list_passive_device_cmd[3];
	list_passive_device_cmd[0] = PN532_COMMAND_INLISTPASSIVETARGET;
	list_passive_device_cmd[1] = 0x01;
	list_passive_device_cmd[2] = cardbaudrate;
	
	
	if(writeCommand(list_passive_device_cmd,sizeof(list_passive_device_cmd)))
	{
		// UID length : 4
		if(data_buff[9] == 12)
		{
			*uidLength = 4;
			uid[0]=data_buff[19];
			uid[1]=data_buff[20];
			uid[2]=data_buff[21];
			uid[3]=data_buff[22];
			if((uid[0]!=0)||(uid[1]!=0)||(uid[2]!=0)||(uid[3]!=0))
				printf("UID:%x %x %x %x\n",uid[0],uid[1],uid[2],uid[3]);					         
		}
				
		// UID length : 7
		if(data_buff[9] == 15)
		{
			*uidLength = 7;
			uid[0]=data_buff[19];
			uid[1]=data_buff[20];
			uid[2]=data_buff[21];
			uid[3]=data_buff[22];
			uid[4]=data_buff[23];
			uid[5]=data_buff[24];
			uid[6]=data_buff[25];
			if((uid[0]!=0)||(uid[1]!=0)||(uid[2]!=0)||(uid[3]!=0)||(uid[4]!=0)||(uid[5]!=0)||(uid[6]!=0))
				printf("UID:%x %x %x %x %x %x %x\n",uid[0],uid[1],uid[2],uid[3],uid[4],uid[5],uid[6]);					         
		}
		clean_rebuff();
		return true;
	}			
	clean_rebuff();
	printf("Scan Error!!!\n");
	return false;
}

bool nfc_mifareclassic_AuthenticateBlock(uint8_t *uid, uint8_t uidLen, uint32_t blockNumber, uint8_t keyNumber, uint8_t *keyData)
{	
	uint8_t AuthenticateBlock_cmd[14];
	
	AuthenticateBlock_cmd[0] = 0x40;
	AuthenticateBlock_cmd[1] = 0x01;//Tg
	AuthenticateBlock_cmd[2] = (keyNumber) ? 0x61:0x60;//MIFARE_CMD_AUTH_B:MIFARE_CMD_AUTH_A
	AuthenticateBlock_cmd[3] = blockNumber;//Block_num
	memcpy(AuthenticateBlock_cmd + 4, keyData, 6);//key
	memcpy(AuthenticateBlock_cmd + 10, uid, uidLen);//UID
	
	if(writeCommand(AuthenticateBlock_cmd, sizeof(AuthenticateBlock_cmd)))
	{
		if(data_buff[13] == 0)//status 
		{
			printf("Authenticated!!!\n");
			clean_rebuff();
			return true;
		}
	}
	return false;
}

bool nfc_mifareclassic_ReadDataBlock(uint8_t blockNumber, uint8_t *data)
{
	//clean the pervious data
	for(int i=0; i<16; i++)
		data_buff[i] = 0;
	
	uint8_t ReadDataBlock_cmd[4];
	
	ReadDataBlock_cmd[0]=PN532_COMMAND_INDATAEXCHANGE;
	ReadDataBlock_cmd[1]=0x01;//Tg
	ReadDataBlock_cmd[2]=0x30;//DataOut
	ReadDataBlock_cmd[3]=blockNumber;//Page
	
	if(writeCommand(ReadDataBlock_cmd, sizeof(ReadDataBlock_cmd)))
	{
		if(data_buff[13] != 0)
		{
			clean_rebuff();
			return false;
		}
		memcpy(data, data_buff+14,16);
		clean_rebuff();
		return true;
	}
	return false;
}

bool nfc_mifareclassic_decodeTlv(uint8_t *data, int* messageLength, int* messageStartIndex)
{
	int i;
	for(i=0; i<16; i++)
	{
		if(data[i] == 0x0)
		{
			//do nothing
		}
		else if(data[i] == 0x3)
			break;
		else
		{
			printf("Unknown TLV!!!\n");
			return false;
		}
	}
	
	if(data[i] != 0x3)
	{
		printf("Error. Can't decode message length!!!\n");
		return false;
	}
	
	if(data[i+1] == 0xFF)
	{
		*messageLength = ((0xFF & data[i+2]) << 8)|(0XFF & data[i+3]);
		*messageStartIndex = i+4;
	}
	
	else
	{
		*messageLength = data[i+1];
		*messageStartIndex = i+2;
	}
	return true;
}

int getBufferSize(int messageLength)
{
	int bufferSize = messageLength;
	// TLV header is 2 or 4 bytes, TLV terminator is 1 byte.
  if (messageLength < 0xFF)
  {
    bufferSize += 2 + 1;
  }
  else
  {
    bufferSize += 4 + 1;
  }

  // bufferSize needs to be a multiple of BLOCK_SIZE
  if (bufferSize % 16 != 0)
  {
    bufferSize = ((bufferSize / 16) + 1) * 16;
  }

  return bufferSize;
}

bool nfc_mifareclassic_IsFirstBlock (uint32_t uiBlock)
{
	if (uiBlock < 128)
		return ((uiBlock) % 4 == 0);
  else
    return ((uiBlock) % 16 == 0);
}

bool nfc_mifareclassic_IsTrailerBlock (uint32_t uiBlock)
{
    // Test if we are in the small or big sectors
    if (uiBlock < 128)
        return ((uiBlock + 1) % 4 == 0);
    else
        return ((uiBlock + 1) % 16 == 0);
}

void getMessage(uint8_t *data, int len, uint8_t* text_len)
{ 
	int i = 0;
	while(i<len)
	{
		if(data[i++] == 0x03)
			break;
	}
	clean_rebuff();
	*text_len = data[i+3]-3;
	memcpy(data_buff,&data[i+8],*text_len);
}

void nfc_mifareclassic_Read(void)
{
	uint8_t key[] = {0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7};
	uint8_t current_Block = 4;
	uint8_t data[16];
	int message_len;
	int message_start_index;
				
	clean_rebuff();
	if(nfc_mifareclassic_AuthenticateBlock(UID, UID_len, current_Block, 0, key))
	{				
		if(nfc_mifareclassic_ReadDataBlock(current_Block, data))
		{
			if(nfc_mifareclassic_decodeTlv(data, &message_len, &message_start_index))
			{				
				int index = 0;
				int bufferSize = getBufferSize(message_len);
				uint8_t temp[bufferSize];
				uint8_t text_len;
				while(index < bufferSize)
				{
					if(nfc_mifareclassic_IsFirstBlock(current_Block))
					{
						if(!nfc_mifareclassic_AuthenticateBlock(UID, UID_len, current_Block, 0, key))
						{
							printf("Error. Block Authentication failed for %d!!!\n",current_Block);
							return;
						}		
					}
								
					if(!nfc_mifareclassic_ReadDataBlock(current_Block, &temp[index]))
						printf("Read failed!!!\n");
								
					index += 16;
					current_Block++;
								
					if(nfc_mifareclassic_IsTrailerBlock(current_Block))
					{
						printf("Skipping block %d!!!\n",current_Block);
						current_Block++;
					}
				}
				printf("message is :");
				getMessage(temp, bufferSize, &text_len);
				for(int i=0; i<text_len; i++)
					printf("%c", data_buff[i]);
			}
				
			else
				printf("Read Error!!!\n");
		}
	}	
}


uint8_t getPayloadLength(uint8_t text_len)
{
	return 3 + text_len;
}

bool nfc_mifareclassic_WriteDataBlock (uint8_t blockNumber, uint8_t *data)
{
	uint8_t WriteDataBlock_cmd[4+16];
	WriteDataBlock_cmd[0] = PN532_COMMAND_INDATAEXCHANGE;
	WriteDataBlock_cmd[1] = 0x01;
	WriteDataBlock_cmd[2] = 0xA0;//MIFARE_CMD_WRITE
	WriteDataBlock_cmd[3] = blockNumber;
	memcpy(WriteDataBlock_cmd + 4, data, 16);
	
	if(writeCommand(WriteDataBlock_cmd, sizeof(WriteDataBlock_cmd)))
	{
		if(data_buff[13] != 0)
		{
			clean_rebuff();
			return false;
		}
	}
	clean_rebuff();
	return true;
}

bool nfc_mifareclassic_write(uint8_t *text, uint8_t text_len)
{
	clean_rebuff();
	uint8_t payload_len = getPayloadLength(text_len);
	uint8_t length_field = payload_len + 4;
	int data_size = getBufferSize(length_field+7);
	uint8_t data[data_size];
	
	data[0] = 0x00;
	data[1] = 0x00;
	data[2] = NDEF_FIELD_TYPE;
	data[3] = length_field;
	data[4] = NDEF_RECORD_HEADER;
	data[5] = 0x01;//Type Length
	data[6] = payload_len;
	data[7] = 0x54;//Record Type
	data[8] = 0x02;//length of "en"
	data[9] = 0x65;// "en"
	data[10] = 0x6E;
	
	memcpy(data+11, text, text_len);
	
	data[11 + text_len] = 0xFE;
	
	int index = 0;
  int current_Block = 4;
  uint8_t key[6] = { 0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7 };
	
	while(index < data_size)
	{
		if(nfc_mifareclassic_IsFirstBlock(current_Block))
		{
			if(!nfc_mifareclassic_AuthenticateBlock(UID, UID_len, current_Block, 0, key))
			{
				printf("Error. Block Authentication failed for %d!!!\n",current_Block);
				clean_rebuff();
				return false;
			}						
		}
		
		if(!nfc_mifareclassic_WriteDataBlock (current_Block, &data[index]))
		{
			printf("Write failed for Block %d!!!\n",current_Block);
			clean_rebuff();
			return false;
		}
		
		index += 16;
    current_Block++;
		
		if(nfc_mifareclassic_IsTrailerBlock(current_Block))
		{
			printf("Skipping block %d!!!\n",current_Block);
			current_Block++;
		}
	}
	clean_rebuff();
	return true;
}