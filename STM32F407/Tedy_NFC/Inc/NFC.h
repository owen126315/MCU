#include "stm32f4xx_hal.h"

struct NFC
{
	uint8_t type;
	uint8_t *uid;
	uint8_t rid_len;
};
