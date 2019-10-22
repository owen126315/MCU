#include "NDEF_Message.h"

void NDEF_Message_Init(NDEF_Message *message)
{
	for(int i = 0; i<MAX_NDEF_RECORDS; i++)
		NDEF_Record_Init(&message->record[i]);
	message->record_Count = 0;
}

int Get_NDEF_Message(uint8_t *data, uint8_t numBytes, NDEF_Message *message)
{
	NDEF_Message_Init(message);
	uint8_t index = 0;

	while (index <= numBytes)
	{
		uint8_t tnf_byte = data[index];
		int mb = (tnf_byte & 0x80) != 0;
		int me = (tnf_byte & 0x40) != 0;
		int cf = (tnf_byte & 0x20) != 0;
		int sr = (tnf_byte & 0x10) != 0;
		int il = (tnf_byte & 0x8) != 0;
		uint8_t tnf = (tnf_byte & 0x7);
		
		index++;
    int typeLength = data[index];
		int payloadLength = 0;
		
		if(sr)
		{
			index++;
			payloadLength = data[index];
		}
		
		else
		{
			payloadLength =
				((0xFF & data[++index]) << 24)
			| ((0xFF & data[++index]) << 16)
			| ((0xFF & data[++index]) << 8)
			| (0xFF & data[++index]);
		}

		int idLength = 0;
		if(il)
		{
			index++;
			idLength = data[index];
		}

		index++;
		NDEF_Record_SetType(&data[index], typeLength, &message->record[message->record_Count]);
		index += typeLength;
		
		if(il)
		{
			NDEF_Record_SetId(&data[index], idLength, &message->record[message->record_Count]);
			index += idLength;
		}
		
		NDEF_Record_SetPayload(&data[index], payloadLength, &message->record[message->record_Count]);
		index += payloadLength;
		
		if(message->record_Count < MAX_NDEF_RECORDS)
		{
			message->record_Count++;
		}
		else
			return 0;
		
		if(me)
			break;
	}
	return 1;
}

int NDEF_Message_GetEncodeSize(NDEF_Message *message)
{
	int size = 0;
	for (int i = 0; i < message->record_Count; i++)	
		size += NDEF_Record_GetEncodedSize(&message->record[i]);
	
	return size;
}

void NDEF_Message_Encode(uint8_t *data, NDEF_Message *message) 
{
	uint8_t *data_ptr = &data[0];
	for(int i = 0; i < message->record_Count; i++)
	{
		NDEF_Record_Encode(data_ptr, i == 0, (i + 1) == message->record_Count, &message->record[i]);
		data_ptr += NDEF_Record_GetEncodedSize(&message->record[i]);
	}
}

//NDEF_Message NDEF_Message_Create_Message(NDEF_Record record)
//{
//	
//	for(int i=0; i<sizerecord)
//	message.record[i]
//	return message;
//}	
