#ifndef __FN_MR01_H
#define __FN_MR01_H

#include "usart.h"
#include "Math.h"
#include "stm32f4xx_hal.h"

void FN_RM01_Get_File_Num(int *file_num);
int FN_RM01_Play_Sound(char *file_name);
int FN_RM01_Record_Sound(char *file_name);
int FN_RM01_Stop_Record(void);
int FN_RM01_Remove_Sound(char *file_name);
void FN_RM01_Get_FileName(int file_num, char *file_name);
#endif //__FN_MR01_H
