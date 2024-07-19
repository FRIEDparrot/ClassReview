#ifndef __FLACPLAY_H__ 
#define __FLACPLAY_H__ 
#include <inttypes.h>
#include <string.h>
#include "flacdecoder.h"
#include "sys.h"  
#include "ff.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//��������ֲ��RockBox��flac�����
//ALIENTEK STM32F407������
//FLAC �������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/6/29
//�汾��V1.0
//********************************************************************************
//V1.0 ˵��
//1,֧��16/24λ������/������flac�Ľ���
//2,���֧��192K/16bit��96K/24bit��flac����  
////////////////////////////////////////////////////////////////////////////////// 	 

 
//flaC ��ǩ 
typedef __packed struct 
{
    u8 id[3];		   	//ID,���ļ���ʼλ��,������flaC 4����ĸ 
}FLAC_Tag;

//metadata ���ݿ�ͷ��Ϣ�ṹ�� 
typedef __packed struct 
{
    u8 head;		   	//metadata blockͷ
	u8 size[3];			//metadata block���ݳ���	
}MD_Block_Head;


//FLAC���ƽṹ��
typedef __packed struct 
{ 
    u32 totsec ;				//���׸�ʱ��,��λ:��
    u32 cursec ;				//��ǰ����ʱ��
	
    u32 bitrate;	   			//������
	u32 samplerate;				//������
	u16 outsamples;				//PCM�����������С
	u16 bps;					//λ��,����16bit,24bit,32bit
	
	u32 datastart;				//����֡��ʼ��λ��(���ļ������ƫ��)
}__flacctrl;

extern __flacctrl * flacctrl;


u8 flac_init(FIL* fx,__flacctrl* fctrl,FLACContext* fc);
void flac_i2s_dma_tx_callback(void);
void flac_get_curtime(FIL*fx,__flacctrl *flacx);
u8 flac_play_song(u8* fname);

#endif




























