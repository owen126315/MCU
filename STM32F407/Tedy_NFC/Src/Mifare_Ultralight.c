#include "Mifare_Ultralight.h"

uint16_t tagCapacity;
uint16_t messageLength;
uint16_t bufferSize;
uint16_t ndefStartIndex;

int Mifare_Ultralight_Read(NFC_Tag *tag)
{
	readCapabilityContainer();
	findNdefMessage();
	calculateBufferSize();
	
	int success;
	uint8_t page;
	uint8_t index = 0;
	uint8_t buffer[bufferSize];
	
	for(page = ULTRALIGHT_DATA_START_PAGE; page < ULTRALIGHT_MAX_PAGE; page++)
	{
		// read the data
		if(!Mifare_Ultralight_ReadPage(page, &buffer[index]))
		{
			messageLength = 0;
			break;
		}

		if (index >= (messageLength + ndefStartIndex))
		{
			break;
		}

		index += ULTRALIGHT_PAGE_SIZE;
	}
	
	if(Get_NDEF_Message(&buffer[ndefStartIndex], messageLength, &tag->ndefMessage))
		return 1;
	
	else
		return 0;
}

uint8_t Mifare_Ultralight_ReadPage(uint8_t page, uint8_t *buffer)
{
	uint8_t readpage_cmd[4];
	readpage_cmd[0] = PN532_COMMAND_INDATAEXCHANGE;
	readpage_cmd[1] = 1;
	readpage_cmd[2] = MIFARE_CMD_READ;
	readpage_cmd[3] = page;
	
	if(PN532_WriteCommand(readpage_cmd, sizeof(readpage_cmd)))
	{
		if(uart1_rx_data[12] == 0x41 && uart1_rx_data[13] == 0)//status 
		{
			memcpy(buffer, &uart1_rx_data[14], 4);
			clean_uart1_rx_data();
			return 1;
		}
	}
	clean_uart1_rx_data();
	return 0;
}

void findNdefMessage(void)
{
	int page;
  uint8_t data[12]; // 3 pages
	uint8_t *data_ptr = data;
	
	// the nxp read command reads 4 pages, unfortunately adafruit give me one page at a time
	int success = 1;
	for (page = 4; page < 6; page++)
	{
			success = success && Mifare_Ultralight_ReadPage(page, data_ptr);
			data_ptr += ULTRALIGHT_PAGE_SIZE;
			
	}
	
	if (success)
	{
		if (data[0] == 0x03)
		{
				messageLength = data[1];
				ndefStartIndex = 2;
		}
		else if (data[5] == 0x3) // page 5 byte 1
		{
				// TODO should really read the lock control TLV to ensure byte[5] is correct
				messageLength = data[6];
				ndefStartIndex = 7;
		}
	}
}

void readCapabilityContainer(void)
{
	uint8_t data[4];
	
	if(Mifare_Ultralight_ReadPage(3, data) == 1)
	{
		tagCapacity = data[2] * 8;
	}
}

void calculateBufferSize(void)
{
	// TLV terminator 0xFE is 1 byte
	bufferSize = messageLength + ndefStartIndex + 1;

	if(bufferSize % ULTRALIGHT_READ_SIZE != 0)
	{
		// buffer must be an increment of page size
		bufferSize = ((bufferSize / ULTRALIGHT_READ_SIZE) + 1) * ULTRALIGHT_READ_SIZE;
	}
}

int Mifare_Ultralight_Write(NFC_Tag *tag)
{
	readCapabilityContainer();
	messageLength = NDEF_Message_GetEncodeSize(&tag->ndefMessage);
	ndefStartIndex = messageLength < 0xFF ? 2 : 4;
	calculateBufferSize();
	
	if(bufferSize>tagCapacity) 
		return 0;
	
	uint8_t encoded[bufferSize];
	uint8_t position = 0;
  uint8_t page = ULTRALIGHT_DATA_START_PAGE;
	
	// Set message size. With ultralight should always be less than 0xFF but who knows?
	
	encoded[0] = 0x3;
	if (messageLength < 0xFF)
	{
		encoded[1] = messageLength;
	}
	else
	{
		encoded[1] = 0xFF;
		encoded[2] = ((messageLength >> 8) & 0xFF);
		encoded[3] = (messageLength & 0xFF);
	}
	
	NDEF_Message_Encode(encoded+2, &tag->ndefMessage);
	memset(encoded+ndefStartIndex+messageLength, 0, bufferSize - ndefStartIndex - messageLength);
  encoded[ndefStartIndex+messageLength] = 0xFE; // terminator

	while(position < bufferSize)//bufferSize is always times pagesize so no "last chunk" check
	{ 
		// write page
		Mifare_Ultralight_WritePage(page++, &encoded[position]);
		position+=ULTRALIGHT_PAGE_SIZE;
	}
  return 1;
}

int Mifare_Ultralight_WritePage(uint8_t page, uint8_t *buffer)
{
	uint8_t write_page_cmd[8];
	write_page_cmd[0] = PN532_COMMAND_INDATAEXCHANGE;
	write_page_cmd[1] = 1;                           /* Card number */
	write_page_cmd[2] = MIFARE_CMD_WRITE_ULTRALIGHT; /* Mifare UL Write cmd = 0xA2 */
	write_page_cmd[3] = page;                        /* page Number (0..63) */
	memcpy (write_page_cmd + 4, buffer, 4);          /* Data Payload */

	if(PN532_WriteCommand(write_page_cmd, sizeof(write_page_cmd)))
		return 1;
	else
		return 0;
}
