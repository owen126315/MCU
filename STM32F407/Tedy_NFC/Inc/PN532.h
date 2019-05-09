#include "stm32f4xx_hal.h"
#include "usart.h"

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

typedef enum { false, true } bool;

void PN532_WakeUp(void);
bool PN532_WriteCommand(uint8_t* cmd_data, uint8_t len);
bool PN532_Correct_AckFrame(void);
bool PN532_Correct_CheckSum(uint16_t len);
void PN532_SAMConfig(void);
bool PN532_InListPassiveTarget(uint8_t cardbaudrate, uint8_t *uid, uint8_t* uidLength);