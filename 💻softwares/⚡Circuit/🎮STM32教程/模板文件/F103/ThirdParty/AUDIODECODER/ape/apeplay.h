#ifndef __APEPLAY_H__ 
#define __APEPLAY_H__ 
#include "apedecoder.h"
#include "parser.h"
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//��������ֲ��RockBox��ape�����
//ALIENTEK STM32F407������
//APE �������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/6/29
//�汾��V1.0
//********************************************************************************
//V1.0 ˵��
//1,֧��16λ������/������ape�Ľ���
//2,���֧��96K��APE��ʽ(LV1����,LV2С��)
//3,LV1~LV3,��48K��������������,LV4,LV5��.
//4,��ĳЩape�ļ�,���ܲ�֧��,����Monkey's Audio�������ת��һ��,������������
////////////////////////////////////////////////////////////////////////////////// 	 
 
#define APE_FILE_BUF_SZ    			20*1024		//APE����ʱ,�ļ�buf��С
#define APE_BLOCKS_PER_LOOP			2*1024		//APE����ʱ,ÿ��ѭ������block�ĸ���

//APE���ƽṹ��
typedef __packed struct 
{ 
    u32 totsec ;				//���׸�ʱ��,��λ:��
    u32 cursec ;				//��ǰ����ʱ��
	
    u32 bitrate;	   			//������
	u32 samplerate;				//������
	u16 outsamples;				//PCM�����������С
	u16 bps;					//λ��,����16bit,24bit,32bit
	
	u32 datastart;				//����֡��ʼ��λ��(���ļ������ƫ��)
}__apectrl;

extern __apectrl * apectrl; 


void ape_fill_buffer(u16* buf,u16 size);
void ape_i2s_dma_tx_callback(void);
void ape_get_curtime(FIL*fx,__apectrl *apectrl);
u8 ape_play_song(u8* fname);
#endif
