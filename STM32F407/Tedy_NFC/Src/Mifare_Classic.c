#include "Mifare_Classic.h"

#define BLOCK_SIZE 16
#define LONG_TLV_SIZE 4
#define SHORT_TLV_SIZE 2

int Mifare_Classic_Read(NFC_Tag *tag)
{
	PN532_WakeUp();
	uint8_t key[] = {0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7};	
	
	uint8_t messageStartIndex = 0;
  uint8_t messageLength = 0;
	uint8_t current_Block = 4;
	uint8_t data[16];
	
	if(!Mifare_Classic_Authenicate_Block(tag, current_Block, 0, key))
		return 0;
	if(!Mifare_Classic_ReadDataBlock(current_Block, data))
		return 0;
	if(!Mifare_Classic_DecodeTlv(data, &messageLength, &messageStartIndex))		
		return 0;
	
	//message length loop
	uint8_t index = 0;
	uint16_t buffer_size = Mifare_Classic_GetBufferSize(messageLength);
	uint8_t buffer[buffer_size];
	memset(buffer, 0, buffer_size);
	
	while (index < buffer_size)
	{
		if(Mifare_Classic_IsFirstBlock(current_Block))
		{
			if(!Mifare_Classic_Authenicate_Block(tag, current_Block, 0, key))
				return 0;		
		}
		
		if(!Mifare_Classic_ReadDataBlock(current_Block, &buffer[index]))
			return 0;
		
		index += BLOCK_SIZE;
		current_Block++;
		
		if(Mifare_Classic_IsTrailerBlock(current_Block))
			current_Block++;
	}	
	
	return Get_NDEF_Message(&buffer[messageStartIndex], messageLength, &tag->ndefMessage);
}

int Mifare_Classic_Authenicate_Block(NFC_Tag *tag, uint8_t blockNumber, uint8_t keyNumber, uint8_t *keyData)
{
	uint8_t AuthenticateBlock_cmd[14];
	
	AuthenticateBlock_cmd[0] = PN532_COMMAND_INDATAEXCHANGE;
	AuthenticateBlock_cmd[1] = 0x01;//Tg
	AuthenticateBlock_cmd[2] = (keyNumber) ? 0x61:0x60;//MIFARE_CMD_AUTH_B:MIFARE_CMD_AUTH_A
	AuthenticateBlock_cmd[3] = blockNumber;//Block_num
	memcpy(&AuthenticateBlock_cmd[0] + 4, &keyData[0], 6);//key
	memcpy(AuthenticateBlock_cmd + 10, tag->uid, tag->uid_len);//UID
	
	if(PN532_WriteCommand(AuthenticateBlock_cmd, sizeof(AuthenticateBlock_cmd)))
	{
		if(uart1_rx_data[12] == 0x41 && uart1_rx_data[13] == 0)//status 
		{
			clean_uart1_rx_data();
			return 1;
		}
	}
	return 0;
}

int Mifare_Classic_ReadDataBlock(uint8_t blockNumber, uint8_t *data)
{
	uint8_t ReadDataBlock_cmd[4];
	
	ReadDataBlock_cmd[0]=PN532_COMMAND_INDATAEXCHANGE;
	ReadDataBlock_cmd[1]=0x01;//Tg
	ReadDataBlock_cmd[2]=0x30;//DataOut
	ReadDataBlock_cmd[3]=blockNumber;//Page	
	
	if(PN532_WriteCommand(ReadDataBlock_cmd, sizeof(ReadDataBlock_cmd)))
	{
		if(uart1_rx_data[12] != 0x41 || uart1_rx_data[13] != 0)
		{
			clean_uart1_rx_data();
			return 0;
		}
	
		memcpy(&data[0], &uart1_rx_data[14],16);
		clean_uart1_rx_data();
		return 1;
	}
	return 0;
}

int Mifare_Classic_DecodeTlv(uint8_t *data, uint8_t *messageLength, uint8_t *messageStartIndex)
{
	int i = Mifare_Classic_GetNdefStartIndex(data);
	if (i < 0 || data[i] != 0x3)
  {
    return 0;
  }
	
	if(data[i+1] == 0xFF)
  {
		*messageLength = ((0xFF & data[i+2]) << 8) | (0xFF & data[i+3]);
    *messageStartIndex = i + LONG_TLV_SIZE;
  }
  else
  {
		*messageLength = data[i+1];
		*messageStartIndex = i + SHORT_TLV_SIZE;
  }
  return 1;
}

int Mifare_Classic_GetNdefStartIndex(uint8_t *data)
{
	for (int i = 0; i < BLOCK_SIZE; i++)
  {
		switch(data[i])
		{
			case 0x0:	break;
			case 0x3:	return i;
			default:	return -2;		
		}
	}
  return -1;
}

uint8_t Mifare_Classic_GetBufferSize(uint8_t message_len)
{	
	int bufferSize = message_len;

  // TLV header is 2 or 4 bytes, TLV terminator is 1 byte.
  if (message_len < 0xFF)
		bufferSize += SHORT_TLV_SIZE + 1;
  else
    bufferSize += LONG_TLV_SIZE + 1;    

  // bufferSize needs to be a multiple of BLOCK_SIZE
  if (bufferSize % BLOCK_SIZE != 0)
		bufferSize = ((bufferSize / BLOCK_SIZE) + 1) * BLOCK_SIZE;

  return bufferSize;
}

int Mifare_Classic_IsFirstBlock(uint32_t uiBlock)
{
	if (uiBlock < 128)
		return ((uiBlock) % 4 == 0);
	else
		return ((uiBlock) % 16 == 0);
}

int Mifare_Classic_IsTrailerBlock (uint32_t uiBlock)
{
	if (uiBlock < 128)
		return ((uiBlock + 1) % 4 == 0);
	else
		return ((uiBlock + 1) % 16 == 0);
}

int Mifare_Classic_Write(NFC_Tag *tag)
{
	PN532_WakeUp();
	uint8_t encoded_size = NDEF_Message_GetEncodeSize(&tag->ndefMessage);
	uint8_t encoded[encoded_size];
	memset(encoded, 0, encoded_size);
	NDEF_Message_Encode(encoded, &tag->ndefMessage);
	
	uint8_t buffer_size = Mifare_Classic_GetBufferSize(sizeof(encoded))+2;
	uint8_t buffer[buffer_size]; //2 bytes for empty record
	memset(buffer, 0, buffer_size);
	
	if(encoded_size < 0xFF)
	{
		buffer[2] = 0x3;
		buffer[3] = sizeof(encoded);
		memcpy(&buffer[4], encoded, sizeof(encoded));
		buffer[5+sizeof(encoded)] = 0xFE; // terminator
	}
	else
	{
		buffer[2] = 0x3;
		buffer[3] = 0xFF;
		buffer[4] = ((sizeof(encoded) >> 8) & 0xFF);
		buffer[5] = (sizeof(encoded) & 0xFF);
		memcpy(&buffer[6], encoded, sizeof(encoded));
		buffer[7+sizeof(encoded)] = 0xFE; // terminator
	}
	
	//Write to tag
	uint8_t index = 0;
  uint8_t current_Block = 4;
  uint8_t key[6] = { 0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7 }; 
	while (index < sizeof(buffer))
	{
		if(Mifare_Classic_IsFirstBlock(current_Block))
		{
			if(!Mifare_Classic_Authenicate_Block(tag, current_Block, 0, key))
				return 0;	
		}
		
		if(!Mifare_Classic_WriteDataBlock(current_Block, &buffer[index]))
			return 0;
		
		index += BLOCK_SIZE;
    current_Block++;
		
		if(Mifare_Classic_IsTrailerBlock(current_Block))
			current_Block++;
	}	
	return 1;
}

int Mifare_Classic_WriteDataBlock(uint8_t blockNumber, uint8_t *data)
{
	uint8_t WriteDataBlock_cmd[4+16];
	
	WriteDataBlock_cmd[0] = PN532_COMMAND_INDATAEXCHANGE;
	WriteDataBlock_cmd[1] = 1;
	WriteDataBlock_cmd[2] = MIFARE_CMD_WRITE;
	WriteDataBlock_cmd[3] = blockNumber;
	memcpy(WriteDataBlock_cmd + 4, data, 16);
	
	if(PN532_WriteCommand(WriteDataBlock_cmd, sizeof(WriteDataBlock_cmd)))
	{
		if(uart1_rx_data[13] != 0)
		{
			clean_uart1_rx_data();
			return 0;
		}			
	}
	clean_uart1_rx_data();
	return 1;
}
