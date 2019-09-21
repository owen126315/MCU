#ifndef __WAVE_H
#define __WAVE_H

#include "stm32f4xx_hal.h"

#define sample_buffer_size 2046

extern uint8_t current_buffer, is_playing, ready_to_play;

typedef struct  
{
	char riff[4];						// RIFF string
	uint32_t overall_size	;				// overall size of file in bytes
	char wave[4];						// WAVE string
	char fmt_chunk_marker[4];			// fmt string with trailing null char
	uint32_t length_of_fmt;					// length of the format data
	uint32_t format_type;					// format type. 1-PCM, 3- IEEE float, 6 - 8bit A law, 7 - 8bit mu law
	uint32_t channels;						// no.of channels
	uint32_t sample_rate;					// sampling rate (blocks per second)
	uint32_t byterate;						// SampleRate * NumChannels * BitsPerSample/8
	uint32_t block_align;					// NumChannels * BitsPerSample/8
	uint32_t bits_per_sample;				// bits per sample, 8- 8bits, 16- 16 bits etc
	char data_chunk_header [4];		// DATA string or FLLR string
	uint32_t data_size;						// NumSamples * NumChannels * BitsPerSample/8 - size of the next chunk that will be read
} WAVE_Header;

typedef struct 
{
	uint16_t buffer[sample_buffer_size];
	uint16_t sample_num;
}  Sample_buffer;

extern WAVE_Header header;
extern Sample_buffer sample_buffer[2];
extern uint32_t sample_num;


int play_wave(char *file_name);
int read_sample(Sample_buffer *buffer);
void decode_PCM_to_DAC(Sample_buffer *buffer);

#endif /*__wave_H */
