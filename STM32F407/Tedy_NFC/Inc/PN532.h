#ifndef __PN532_H
#define __PN532_H

#include "stm32f4xx_hal.h"
#include "usart.h"
#include "NFC_Tag.h"


#define PN532_MIFARE_ISO14443A 							(0x00)
#define PN532_PREAMBLE               				(0x00)
#define PN532_STARTCODE1            			  (0x00)
#define PN532_STARTCODE2             				(0xFF)
#define PN532_POSTAMBLE              				(0x00)

#define PN532_HOSTTOPN532             			(0xD4)
#define PN532_PN532TOHOST             			(0xD5)
#define PN532_COMMAND_INDATAEXCHANGE        (0x40)


#define PN532_COMMAND_INLISTPASSIVETARGET   (0x4A)
#define NDEF_RECORD_HEADER									(0xD1)
#define NDEF_FIELD_TYPE											(0x03)

#define MIFARE_CMD_READ                     (0x30)
#define MIFARE_CMD_WRITE                    (0xA0)
#define MIFARE_CMD_WRITE_ULTRALIGHT         (0xA2)


void PN532_WakeUp(void);
int PN532_WriteCommand(uint8_t* cmd_data, uint8_t len);
int PN532_Correct_AckFrame(void);
int PN532_Correct_CheckSum(uint16_t len);
void PN532_SAMConfig(void);
int PN532_InListPassiveTarget(uint8_t cardbaudrate, NFC_Tag *tag);
int PN532_Read_Tag(NFC_Tag *tag);
int PN532_Write_Tag(NFC_Tag *tag);

#endif //__PN532_H
