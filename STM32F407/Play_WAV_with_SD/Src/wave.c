#include "wave.h"

UINT num_of_bytes;
uint32_t sample_num;
uint8_t current_buffer, is_playing, ready_to_play;

WAVE_Header header;
Sample_buffer sample_buffer[2];

uint8_t count = 0;

int play_wave(char *file_name)
{
	if(f_mount(&SDFatFS, SDPath, 1) != FR_OK)
		while(1);
	if(f_open(&SDFile, file_name, 1) == FR_OK)
	{
		uint8_t buffer_4_bytes[4],buffer_2_bytes[2];
		
		//read "riff"
		if(f_read(&SDFile, header.riff, sizeof(header.riff), &num_of_bytes) != FR_OK)
			return 0;
		//read overall size
		if(f_read(&SDFile, buffer_4_bytes, sizeof(buffer_4_bytes), &num_of_bytes) != FR_OK)
			return 0;
		header.overall_size = buffer_4_bytes[0] |
												 (buffer_4_bytes[1]<<8) | 
												 (buffer_4_bytes[2]<<16) | 
												 (buffer_4_bytes[3]<<24);

		//read wave marker
		if(f_read(&SDFile, header.wave, sizeof(header.wave), &num_of_bytes) != FR_OK)
			return 0;
		//read Fmt marker
		if(f_read(&SDFile, header.fmt_chunk_marker, sizeof(header.fmt_chunk_marker), &num_of_bytes) != FR_OK)
			return 0;
		//read length of fmt header
		if(f_read(&SDFile, buffer_4_bytes, sizeof(buffer_4_bytes), &num_of_bytes) != FR_OK)
			return 0;
		header.length_of_fmt = buffer_4_bytes[0] |
													(buffer_4_bytes[1] << 8) |
													(buffer_4_bytes[2] << 16) |
													(buffer_4_bytes[3] << 24);
													
		//read format_type											
		if(f_read(&SDFile, buffer_2_bytes, sizeof(buffer_2_bytes), &num_of_bytes) != FR_OK)
			return 0;
		header.format_type = buffer_2_bytes[0] | (buffer_2_bytes[1] << 8);

		//read number of channels 
		if(f_read(&SDFile, buffer_2_bytes, sizeof(buffer_2_bytes), &num_of_bytes) != FR_OK)
			return 0;
		header.channels = buffer_2_bytes[0] | (buffer_2_bytes[1] << 8);

		//read sample rate
		if(f_read(&SDFile, buffer_4_bytes, sizeof(buffer_4_bytes), &num_of_bytes) != FR_OK)
			return 0;
		header.sample_rate = buffer_4_bytes[0] |
												(buffer_4_bytes[1] << 8) |
												(buffer_4_bytes[2] << 16) |
												(buffer_4_bytes[3] << 24);
												
		//read byte rate
		if(f_read(&SDFile, buffer_4_bytes, sizeof(buffer_4_bytes), &num_of_bytes) != FR_OK)
			return 0;
		header.byterate  = buffer_4_bytes[0] |
											(buffer_4_bytes[1] << 8) |
											(buffer_4_bytes[2] << 16) |
											(buffer_4_bytes[3] << 24);

		//read block alignment
		if(f_read(&SDFile, buffer_2_bytes, sizeof(buffer_2_bytes), &num_of_bytes) != FR_OK)
			return 0;
		header.block_align = buffer_2_bytes[0] | (buffer_2_bytes[1] << 8);

		//read bit per sample 
		if(f_read(&SDFile, buffer_2_bytes, sizeof(buffer_2_bytes), &num_of_bytes) != FR_OK)
			return 0;
		header.bits_per_sample = buffer_2_bytes[0] | (buffer_2_bytes[1] << 8);

		//read data marker
		if(f_read(&SDFile, header.data_chunk_header, sizeof(header.data_chunk_header), &num_of_bytes) != FR_OK)
			return 0;
		while (strcmp((char*)header.data_chunk_header, "data") != 0)
		{
			f_lseek(&SDFile, f_tell(&SDFile) - 3);
			if(f_read(&SDFile, header.data_chunk_header, sizeof(header.data_chunk_header), &num_of_bytes) != FR_OK)
				return 0;
		}

		//read size of data chunk
		if(f_read(&SDFile, buffer_4_bytes, sizeof(buffer_4_bytes), &num_of_bytes) != FR_OK)
			return 0;
		header.data_size  = buffer_4_bytes[0] |
											 (buffer_4_bytes[1] << 8) |
											 (buffer_4_bytes[2] << 16) |
											 (buffer_4_bytes[3] << 24);

		//calculate number of samples
		sample_num = (8 * header.data_size) / (header.channels * header.bits_per_sample);				


		//read first two rounds sample data
//		HAL_TIM_Base_Start_IT(&htim7);
//		HAL_TIM_Base_Stop(&htim7);
//		htim7.Instance->CNT = 0;
//		count = 0;
		HAL_TIM_Base_Start_IT(&htim7);
		if(!read_sample(&sample_buffer[0]))
			while(1);
		HAL_TIM_Base_Stop(&htim7);
//		htim7.Instance->CNT = 0;
//		count = 0;
		if(!read_sample(&sample_buffer[1]))
			while(1);

		//timer 6 setup
		if(header.sample_rate == 44100)
		{
			htim6.Init.Prescaler = 1-1;
			htim6.Init.Period = 1904-1;
		}
		else if(header.sample_rate == 22050)
		{
			htim6.Init.Prescaler = 1-1;
			htim6.Init.Period = 3809-1;
		}
		else if(header.sample_rate == 16000)
		{
			htim6.Init.Prescaler = 42-1;
			htim6.Init.Period = 125-1;
		}
		
		
		if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
		{
			_Error_Handler(__FILE__, __LINE__);
		}
		HAL_TIM_Base_Start(&htim6);
		
		//start DAC channel and DMA
		HAL_DAC_Start(&hdac,DAC_CHANNEL_1);
		
		current_buffer = 0;
		//HAL_TIM_Base_Start_IT(&htim7);
		if(header.bits_per_sample == 16)
			HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*)sample_buffer[current_buffer].buffer, sample_buffer[0].sample_num, DAC_ALIGN_12B_R);
		
		if(header.bits_per_sample == 8)
			HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*)sample_buffer[current_buffer].buffer, sample_buffer[0].sample_num, DAC_ALIGN_8B_R);
		
		is_playing = 1;
	
		return 1;	
	}
	return 0;
}

int read_sample(Sample_buffer *buffer)
{
	switch(header.bits_per_sample)
	{
		case 8:
		{
			if((int)(sample_num - sample_buffer_size/2) >= 0)
			{
				if(f_read(&SDFile, buffer->buffer, sample_buffer_size/2, &num_of_bytes) != FR_OK)
				{
					HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_1);
					return 0;
				}	
				
				buffer->sample_num = sample_buffer_size/2;
			}
			
			else
			{
				if(f_read(&SDFile, buffer->buffer, sample_num, &num_of_bytes) != FR_OK)
				{
					HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_1);
					return 0;
				}
				
				buffer->sample_num = sample_num;		
				f_close(&SDFile);
			}
			//convert PCM to DAC value
			decode_PCM_to_DAC(buffer);
			break;
		}
		
		case 16:
		{
			if((int)(sample_num - sample_buffer_size) >= 0)
			{
				if(f_read(&SDFile, buffer->buffer, sample_buffer_size, &num_of_bytes) != FR_OK)
				{
					HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_1);
					return 0;
				}	
				
				buffer->sample_num = sample_buffer_size/2;
			}
			
			else
			{
				if(f_read(&SDFile, buffer->buffer, sample_num<<2, &num_of_bytes) != FR_OK)
				{
					HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_1);
					return 0;
				}
				
				buffer->sample_num = sample_num;		
				f_close(&SDFile);
			}
			//convert PCM to DAC value
			decode_PCM_to_DAC(buffer);
		}		
	}
//			HAL_TIM_Base_Stop(&htim7);
//		htim7.Instance->CNT = 0;
//		count = 0;
	
	
	
	return 1;
}

void decode_PCM_to_DAC(Sample_buffer *buffer)
{
	switch (header.bits_per_sample)
	{
		case 16:
		{	
			for(uint16_t i=0; i<buffer->sample_num; i++)
			{
				int sign = 1;
				int16_t temp = (buffer->buffer[i] <<8) | (buffer->buffer[i] >>8);;
				buffer->buffer[i] = ( temp + 32768) * 4095 / 65535;
			}				
			break;
		}
		
		case 8: 
		{
			uint8_t temp[buffer->sample_num];
			memcpy(temp,buffer->buffer,buffer->sample_num);
			for(uint16_t i=0; i<buffer->sample_num; i++)
			{
				buffer->buffer[i] = 0x00FF & temp[i];
			}
		}
		
	}
}
