#ifndef __Mifare_Classic_H
#define __Mifare_Classic_H

#include "stm32f4xx_hal.h"
#include "usart.h"
#include "PN532.h"
#include "NFC_Tag.h"

int Mifare_Classic_Read(NFC_Tag *tag);
int Mifare_Classic_Authenicate_Block(NFC_Tag *tag, uint8_t blockNumber, uint8_t keyNumber, uint8_t *keyData);
int Mifare_Classic_ReadDataBlock(uint8_t blockNumber, uint8_t *data);
int Mifare_Classic_DecodeTlv(uint8_t *data, uint8_t *messageLength, uint8_t *messageStartIndex);
int Mifare_Classic_GetNdefStartIndex(uint8_t *data);
uint8_t Mifare_Classic_GetBufferSize(uint8_t message_len);
int Mifare_Classic_IsFirstBlock (uint32_t uiBlock);
int Mifare_Classic_IsTrailerBlock (uint32_t uiBlock);
int Mifare_Classic_Write(NFC_Tag *tag);
int Mifare_Classic_WriteDataBlock(uint8_t blockNumber, uint8_t *data);
#endif //__Mifare_Classic_H
