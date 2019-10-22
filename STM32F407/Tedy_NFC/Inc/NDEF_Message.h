#ifndef __NDEF_Message_H
#define __NDEF_Message_H

#include "stm32f4xx_hal.h"
#include "NDEF_Record.h"
#include "string.h"
#include "stdlib.h"
#define MAX_NDEF_RECORDS 	4

typedef struct{
	uint8_t record_Count;
	NDEF_Record record[MAX_NDEF_RECORDS];
} NDEF_Message;

void NDEF_Message_Init(NDEF_Message *message);
int Get_NDEF_Message(uint8_t *data, uint8_t numBytes, NDEF_Message *message);
int NDEF_Message_GetEncodeSize(NDEF_Message *message);
void NDEF_Message_Encode(uint8_t *data, NDEF_Message *message); 
//NDEF_Message NDEF_Message_Create_Message(NDEF_Record *record);
#endif //__NDEF_Message_H
