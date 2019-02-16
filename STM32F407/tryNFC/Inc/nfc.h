#include "stm32f4xx_hal.h"

#define UART_BUFF_SIZE      					(1024)
#define PN532_MIFARE_ISO14443A 				(0x00)
#define PN532_PREAMBLE                (0x00)
#define PN532_STARTCODE1              (0x00)
#define PN532_STARTCODE2              (0xFF)
#define PN532_POSTAMBLE               (0x00)

#define PN532_HOSTTOPN532             (0xD4)
#define PN532_PN532TOHOST             (0xD5)
#define PN532_COMMAND_INDATAEXCHANGE        (0x40)


#define PN532_COMMAND_INLISTPASSIVETARGET   (0x4A)
#define NDEF_RECORD_HEADER									(0xD1)
#define NDEF_FIELD_TYPE											(0x03)


extern UART_HandleTypeDef huart1;
extern uint8_t data_buff[UART_BUFF_SIZE];
extern uint8_t UID_len;
extern uint8_t UID[7];
extern void clean_rebuff(void);
extern uint8_t *get_rebuff(uint16_t *len);

typedef enum { false, true } bool;

bool writeCommand(uint8_t* cmd_data, uint8_t len);
bool correct_AckFrame(void);
bool correct_CheckSum(uint16_t len);
void nfc_WakeUp(void);
void nfc_SAMConfig(void);

bool nfc_InListPassiveTarget(uint8_t cardbaudrate, uint8_t *uid, uint8_t *uidLength);
bool nfc_mifareclassic_AuthenticateBlock(uint8_t *uid, uint8_t uidLen, uint32_t blockNumber, uint8_t keyNumber, uint8_t *keyData);
bool nfc_mifareclassic_ReadDataBlock(uint8_t blockNumber, uint8_t *data);
bool nfc_mifareclassic_decodeTlv(uint8_t *data, int* messageLength, int* messageStartIndex);
bool nfc_mifareclassic_IsFirstBlock(uint32_t uiBlock);
bool nfc_mifareclassic_IsTrailerBlock(uint32_t uiBlock);
int getBufferSize(int messageLength);
void getMessage(uint8_t *data, int len, uint8_t *text_len);
void nfc_mifareclassic_Read(void);
uint8_t getPayloadLength(uint8_t text_len);

bool nfc_mifareclassic_WriteDataBlock(uint8_t blockNumber, uint8_t *data);
bool nfc_mifareclassic_write(uint8_t *str, uint8_t text_len);