#ifndef __NDEF_Record_H
#define __NDEF_Record_H

#include "stm32f4xx_hal.h"
#include "string.h"
#include "stdlib.h"

#define TNF_EMPTY 0x0
#define TNF_WELL_KNOWN 0x01
#define TNF_MIME_MEDIA 0x02
#define TNF_ABSOLUTE_URI 0x03
#define TNF_EXTERNAL_TYPE 0x04
#define TNF_UNKNOWN 0x05
#define TNF_UNCHANGED 0x06
#define TNF_RESERVED 0x07

typedef struct {
	uint8_t tnf; // 3 bit
  uint8_t typeLength;
  uint8_t payloadLength;
  uint8_t idLength;
	uint8_t type[8];
  uint8_t id[8];
	uint8_t payload[256];
}NDEF_Record;

void NDEF_Record_Init(NDEF_Record *record);
void NDEF_Record_SetType(uint8_t *type, uint8_t numBytes, NDEF_Record *record);
void NDEF_Record_SetId(uint8_t *id, uint8_t numBytes, NDEF_Record *record);
void NDEF_Record_SetPayload(uint8_t* payload, uint8_t numBytes, NDEF_Record *record);
void NDEF_Record_Encode(uint8_t *data, int firstRecord, int lastRecord, NDEF_Record *record);
uint8_t NDEF_Record_GetEncodedSize(NDEF_Record *record);
uint8_t NDEF_Record_GetTnfByte(int firstRecord, int lastRecord, NDEF_Record *record);
void NDEF_Record_Create_TextRecord(char *fileName, uint8_t len, NDEF_Record *record);
#endif //__NDEF_Record_H
