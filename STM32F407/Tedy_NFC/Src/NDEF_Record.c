#include "NDEF_Record.h"

void NDEF_Record_Init(NDEF_Record *record)
{
	record->tnf = 0;
	record->typeLength = 0;
	record->idLength = 0;
	record->payloadLength = 0;
	memset(record->type, 0, sizeof(record->type));
	memset(record->id, 0, sizeof(record->id));
	memset(record->payload, 0, sizeof(record->payload));
}

void NDEF_Record_SetType(uint8_t *type, uint8_t numBytes, NDEF_Record *record)
{
	memcpy(record->type, type, numBytes);
	record->typeLength = numBytes;
}

void NDEF_Record_SetId(uint8_t *id, uint8_t numBytes, NDEF_Record *record)
{
	memcpy(record->id, id, numBytes);
	record->idLength = numBytes;
}

void NDEF_Record_SetPayload(uint8_t* payload, uint8_t numBytes, NDEF_Record *record)
{
	memcpy(record->payload, payload, numBytes);
	record->payloadLength = numBytes;
}

uint8_t NDEF_Record_GetEncodedSize(NDEF_Record *record)
{
	uint8_t size = 2;

	if(record->payloadLength > 0xFF)
		size += 4;		
	else
		size += 1;
		
	if(record->idLength)
		size += 1;
	
	size += (record->typeLength + record->payloadLength + record->idLength); 
	return size;
}

void NDEF_Record_Encode(uint8_t *data, int firstRecord, int lastRecord, NDEF_Record *record)
{
	uint8_t *data_ptr = &data[0];
	
	*data_ptr = NDEF_Record_GetTnfByte(firstRecord, lastRecord, record);
  data_ptr += 1;
	
	*data_ptr = record->typeLength;
	data_ptr += 1;
	
	if (record->payloadLength <= 0xFF) 
	{  // short record
		*data_ptr = record->payloadLength;
		data_ptr += 1;
	} 
	else
	{ // long format
			// 4 bytes but we store length as an int
		data_ptr[0] = 0x0; // (_payloadLength >> 24) & 0xFF;
		data_ptr[1] = 0x0; // (_payloadLength >> 16) & 0xFF;
		data_ptr[2] = (record->payloadLength >> 8) & 0xFF;
		data_ptr[3] = record->payloadLength & 0xFF;
		data_ptr += 4;
	}
	
	if (record->idLength)
	{
		*data_ptr = record->idLength;
		data_ptr += 1;
	}
    
	memcpy(data_ptr, record->type, record->typeLength);
	data_ptr += record->typeLength;

	memcpy(data_ptr, record->payload, record->payloadLength);
	data_ptr += record->payloadLength;

	if (record->idLength)
	{
		memcpy(data_ptr, record->id, record->idLength);
		data_ptr += record->idLength;
	}
}

uint8_t NDEF_Record_GetTnfByte(int firstRecord, int lastRecord, NDEF_Record *record)
{
	uint8_t value = record->tnf;
	
	if(firstRecord) 
		value = value | 0x80;

  if(lastRecord) 
		value = value | 0x40;

	if(record->payloadLength <= 0xFF) 
    value = value | 0x10;

  if(record->idLength)
    value = value | 0x8;

  return value;
}

void NDEF_Record_Create_TextRecord(char *fileName, uint8_t len, NDEF_Record *record)
{
	
	NDEF_Record_Init(record);
	record->tnf = TNF_WELL_KNOWN;
	uint8_t RTD_TEXT[1] = { 0x54 };
	NDEF_Record_SetType(RTD_TEXT, sizeof(RTD_TEXT), record);
	
	uint8_t payload[1+2+len];
	
	memset(payload,0x02,1);
	memcpy(payload+1,"en",2);
	memcpy(payload+3,fileName,len);
	
	NDEF_Record_SetPayload(payload,sizeof(payload),record);	
}
