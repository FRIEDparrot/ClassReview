#ifndef _FLAC_DECODER_H
#define _FLAC_DECODER_H
 
#include "bitstreamf.h"
#include "sys.h"

typedef int32_t	s32;
typedef int16_t s16;
typedef int8_t  s8;

enum decorrelation_type {
    INDEPENDENT,
    LEFT_SIDE,
    RIGHT_SIDE,
    MID_SIDE,
};


//#define INDEPENDENT  0
//#define LEFT_SIDE    1
//#define RIGHT_SIDE   2
//#define MID_SIDE     3


typedef struct FLACContext 
{
	GetBitContext gb;

	int min_blocksize, max_blocksize;	//block����С/��������
	int min_framesize, max_framesize;	//��С/���֡��С
	int samplerate, channels;			//�����ʺ�ͨ����
	int blocksize;  					// last_blocksize
	int bps, curr_bps;
	unsigned long samplenumber;
	unsigned long totalsamples;
	enum decorrelation_type decorrelation;  

	int seektable;
	int seekpoints;

	int bitstream_size;
	int bitstream_index;

	int sample_skip;
	int framesize;

	int *decoded0;  // channel 0
	int *decoded1;  // channel 1
}FLACContext;

int flac_decode_frame24(FLACContext *s, uint8_t *buf, int buf_size, s32 *wavbuf);
int flac_decode_frame16(FLACContext *s, uint8_t *buf, int buf_size, s16 *wavbuf);
int flac_seek_frame(u8 *buf,u32 size,FLACContext * fc);
#endif
