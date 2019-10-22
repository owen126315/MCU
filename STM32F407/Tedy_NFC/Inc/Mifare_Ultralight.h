#ifndef __Mifare_Classic_H
#define __Mifare_Classic_H

#include "stm32f4xx_hal.h"
#include "usart.h"
#include "PN532.h"
#include "NFC_Tag.h"
#include "NDEF_Message.h"

#define ULTRALIGHT_PAGE_SIZE 4
#define ULTRALIGHT_READ_SIZE 4 // we should be able to read 16 bytes at a time

#define ULTRALIGHT_DATA_START_PAGE 4
#define ULTRALIGHT_MESSAGE_LENGTH_INDEX 1
#define ULTRALIGHT_DATA_START_INDEX 2
#define ULTRALIGHT_MAX_PAGE 63

int Mifare_Ultralight_Read(NFC_Tag *tag);
uint8_t Mifare_Ultralight_ReadPage(uint8_t page, uint8_t *buffer);
void readCapabilityContainer(void);
void findNdefMessage(void);
void calculateBufferSize(void);
int Mifare_Ultralight_Write(NFC_Tag *tag);
int Mifare_Ultralight_WritePage(uint8_t page, uint8_t *buffer);

#endif //__Mifare_Classic_H
