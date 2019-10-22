#ifndef __NFC_Tag_H
#define __NFC_Tag_H

#include "NDEF_Message.h"

#define TAG_TYPE_MIFARE_CLASSIC				(0)
#define TAG_TYPE_MIFARE_ULTRALIGHT		(2)

typedef struct {
	uint8_t type;
	uint8_t uid[7];
	uint8_t uid_len;
	NDEF_Message ndefMessage;
} NFC_Tag;

void NFC_Clear_Tag(NFC_Tag *tag);
//char* NFC_Tag_GetText(struct NFC_Tag *tag);
#endif //NFC_Tag_H
