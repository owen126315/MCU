#include "NFC_Tag.h"

//char* NFC_Tag_GetText(struct NFC_Tag *tag)
//{
//	if(tag->ndefMessage == NULL)
//		return NULL;
//	
//	for(int i=0; i< tag->ndefMessage->record_Count; i++)
//	{
//		for(int j=0; j < tag->ndefMessage->record[i].payloadLength; j++)
//		{
//			if(tag->ndefMessage->record[i].payload[j])
//		}
//	}
//}

void NFC_Clear_Tag(NFC_Tag *tag)
{
	tag->type = 0;
	tag->uid_len = 0;
	memset(tag->uid, 0, sizeof(tag->uid));
	
	tag->ndefMessage.record_Count = 0;
	
	for(int i=0; i<4; i++)
	{
		tag->ndefMessage.record[i].tnf = 0;
		tag->ndefMessage.record[i].typeLength = 0;
		tag->ndefMessage.record[i].payloadLength = 0;
		tag->ndefMessage.record[i].idLength = 0;
		memset(tag->ndefMessage.record[i].type, 0, sizeof(tag->ndefMessage.record[i].type));
		memset(tag->ndefMessage.record[i].id, 0, sizeof(tag->ndefMessage.record[i].id));
		memset(tag->ndefMessage.record[i].payload, 0, sizeof(tag->ndefMessage.record[i].payload));
	}
}
