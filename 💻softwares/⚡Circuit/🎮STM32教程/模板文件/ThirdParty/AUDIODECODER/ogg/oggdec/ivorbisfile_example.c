/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE OggVorbis 'TREMOR' CODEC SOURCE CODE.   *
 *                                                                  *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE OggVorbis 'TREMOR' SOURCE CODE IS (C) COPYRIGHT 1994-2002    *
 * BY THE Xiph.Org FOUNDATION http://www.xiph.org/                  *
 *                                                                  *
 ********************************************************************

 function: simple example decoder using vorbisidec

 ********************************************************************/

/* Takes a vorbis bitstream from stdin and writes raw stereo PCM to
   stdout using vorbisfile. Using vorbisfile is much simpler than
   dealing with libvorbis. */

#include "ivorbiscodec.h"
#include "ivorbisfile.h"
#include "sys.h"
#include "malloc.h"
#include "string.h"
#include "ff.h"
#include "malloc.h" 
#include <inttypes.h>
#include "audioplayer.h"
#include "key.h"

#include "i2s.h"
#include <wm8978.h>

__align(4) char pcmout1[4096]; /* take 4k out of the data segment, not the stack */
__align(4) char pcmout2[4096]; /* take 4k out of the data segment, not the stack */

u32 mbuff[500];
u16 bufsize=0;

//ogg malloc����ʵ��
void* ogg_malloc(u32 size)
{
	void *p;
	p=mymalloc(SRAMIN,size);
	printf("ogg_malloc:%x,%d\r\n",(u32)p,size);
	mbuff[bufsize]=(u32)p;
	bufsize++;
	return p;
}
//ogg calloc����ʵ��
void* ogg_calloc(u32 count,u32 size)
{
	void *p; 
	p=mymalloc(SRAMIN,count*size); 
	if(p)memset(p,0,count*size); 
	printf("ogg_calloc:%x,%d\r\n",(u32)p,count*size);
	mbuff[bufsize]=(u32)p;
	bufsize++;
	return p;
}
//ogg realloc����ʵ��
void *ogg_realloc(void *ptr,u32 size)  
{
	void *p;
	p=myrealloc(SRAMIN,ptr,size);
	printf("ogg_realloc:%x,%d\r\n",(u32)p,size);
	mbuff[bufsize]=(u32)p;
	bufsize++;
	return p;
}
//ogg free����ʵ��
void ogg_free(void *ptr)  
{
	u16 i;
	printf("ogg_free:%x\r\n",(u32)ptr);
	for(i=0;i<bufsize;i++)
	{
		if(mbuff[i]==(u32)ptr)mbuff[i]=0;//�ͷŵ��˵�,����.
	}
	myfree(SRAMIN,ptr);
}

vu8 transferend=0;		//i2s������ɱ�־
vu8 witchbuf=0;			//i2sbufxָʾ��־

//OGG DMA���ͻص�����
void ogg_i2s_dma_tx_callback(void) 
{    
	u16 i;
	if(DMA1_Stream4->CR&(1<<19))
	{
		witchbuf=0;
		if((audiodev.status&0X01)==0)//��ͣ��,���0
		{
			for(i=0;i<4096;i++)audiodev.i2sbuf1[i]=0;
		}
	}else 
	{
		witchbuf=1;
		if((audiodev.status&0X01)==0)//��ͣ��,���0
		{
			for(i=0;i<4096;i++)audiodev.i2sbuf2[i]=0;
		}
	} 
	transferend=1;
} 
//���PCM���ݵ�DAC
//buf:PCM�����׵�ַ
//size:pcm������(16λΪ��λ)
//nch:������(1,������,2������)
void ogg_fill_buffer(u16* buf,u16 size,u8 nch)
{ 
	while((DMA1->HISR&(1<<5))==0);//�ȴ��������
	DMA1->HIFCR|=1<<5;
	I2S_Play_Stop();//�ر�DMA����
	DMA1_Stream4->M0AR=(u32)buf;
	DMA1_Stream4->NDTR=size; 
	I2S_Play_Start();//����DMA����
}   
//I2S2 TX DMA����
//����Ϊ˫����ģʽ,������DMA��������ж�
//buf0:M0AR��ַ.
//buf1:M1AR��ַ.
//num:ÿ�δ���������
void OGG_TX_DMA_Init(u8* buf0,u16 num)
{  
	RCC->AHB1ENR|=1<<21;		//DMA1ʱ��ʹ��  
	while(DMA1_Stream4->CR&0X01);//�ȴ�DMA1_Stream4������ 
	DMA1->HIFCR|=0X3D<<6*0;		//���ͨ��4�������жϱ�־
	DMA1_Stream4->FCR=0X0000021;//����ΪĬ��ֵ	
	
	DMA1_Stream4->PAR=(u32)&SPI2->DR;		//�����ַΪ:SPI2->DR
	DMA1_Stream4->M0AR=(u32)buf0;//�ݲ�����
	//DMA1_Stream4->M1AR=(u32)buf1;//�ݲ�����
	DMA1_Stream4->NDTR=num;		//��ʱ���ó���Ϊ1
	DMA1_Stream4->CR=0;			//��ȫ����λCR�Ĵ���ֵ  
	DMA1_Stream4->CR|=1<<6;		//�洢��������ģʽ 
	DMA1_Stream4->CR|=0<<8;		//����ģʽ
	DMA1_Stream4->CR|=0<<9;		//���������ģʽ
	DMA1_Stream4->CR|=1<<10;	//�洢������ģʽ
	DMA1_Stream4->CR|=1<<11;	//�������ݳ���:16λ
	DMA1_Stream4->CR|=1<<13;	//�洢�����ݳ���:16λ
	DMA1_Stream4->CR|=2<<16;	//�����ȼ�
	DMA1_Stream4->CR|=0<<18;	//������ģʽ
	DMA1_Stream4->CR|=0<<21;	//����ͻ�����δ���
	DMA1_Stream4->CR|=0<<23;	//�洢��ͻ�����δ���
	DMA1_Stream4->CR|=0<<25;	//ѡ��ͨ��0 SPI2_TXͨ�� 

	DMA1_Stream4->FCR&=~(1<<2);	//��ʹ��FIFOģʽ
	DMA1_Stream4->FCR&=~(3<<0);	//��FIFO ����
	
	//DMA1_Stream4->CR|=1<<4;		//������������ж�
	//MY_NVIC_Init(0,0,DMA1_Stream4_IRQn,2);	//��ռ0�������ȼ�2����2  
} 

int ogg(char * path)
{
	FIL*fogg;
	OggVorbis_File vf;
	int eof=0;
	int current_section;
	int i=0;
	u8 res; 
	vorbis_comment *pov_comment;
	char **ptr;
	vorbis_info *vi; 
	long ret;
	u8 key;
	
	bufsize=0;
	memset(mbuff,0,1000);//ȫ������
	
	audiodev.i2sbuf1=mymalloc(SRAMIN,4096); 
	audiodev.i2sbuf2=mymalloc(SRAMIN,4096); 
	
	
	fogg=(FIL*)mymalloc(SRAMIN,sizeof(FIL));//��fogg�ļ�
	fogg=(FIL*)mymalloc(SRAMIN,sizeof(FIL));//��fogg�ļ�
	res=f_open(fogg,path,FA_READ);
	if(res)
	{
		printf("open file:%s error\r\n",path);
	}
	if(ov_open(fogg, &vf, NULL, 0) < 0) 
	{
		printf("Input does not appear to be an Ogg bitstream.\n");
		return -1;
	}  
	pov_comment 	=(vorbis_comment *)ov_comment(&vf,-1);
	ptr= pov_comment->user_comments;
	
	vi=(vorbis_info *)ov_info(&vf,-1);
	while(*ptr)
	{
		printf("\r\n%s\r\n",*ptr);
		++ptr;
	}
	printf("\r\nBitstream is %d channel, %ldHz\r\n",vi->channels,vi->rate);
	printf("Decoded length: %ld samples\r\n",(long)ov_pcm_total(&vf,-1));
	printf("Encoded by: %s\r\n\r\n",pov_comment->vendor);

	WM8978_ADDA_Cfg(1,0);	//����DAC
	WM8978_Input_Cfg(0,0,0);//�ر�����ͨ��
	WM8978_Output_Cfg(1,0);	//����DAC��� 

	WM8978_I2S_Cfg(2,0);	//�����ֱ�׼,16λ���ݳ���
	I2S2_Init(0,2,0,0);		//�����ֱ�׼,��������,ʱ�ӵ͵�ƽ��Ч,16λ��׼֡����
 	I2S2_SampleRate_Set(vi->rate);		//���ò����� 
	OGG_TX_DMA_Init(pcmout1,4096/2);	//����TX DMA
	
	//I2S2_TX_DMA_Init(audiodev.i2sbuf1,audiodev.i2sbuf2,4096/2);//����TX DMA
	i2s_tx_callback=ogg_i2s_dma_tx_callback;		//�ص�����ָmp3_i2s_dma_tx_callback
 	audio_start(); 
	while(!eof)
	{
		// rt_sem_take(&ogg_sem, RT_WAITING_FOREVER);
		if(i++ & 1)
		{ 
			ret=ov_read(&vf,pcmout1,sizeof(pcmout1),&current_section);
			if (ret == 0) 
			{
				/* EOF */
				eof=1;
			} else if (ret < 0) 
			{
				/* error in the stream.  Not a problem, just reporting it in
				case we (the app) cares.  In this case, we don't. */
			} else 
			{
				/* we don't bother dealing with sample rate changes, etc, but
				you'll have to*/
				//  fwrite(pcmout,1,ret,stdout);
				//	rt_device_write(snd_device, 0, (int16_t *)pcmout1,  ret);
				//printf("pcm1:%d\r\n",ret);
				ogg_fill_buffer((u16*)pcmout1,ret/2,2); 
			}
		}
		else
		{
			ret=ov_read(&vf,pcmout2,sizeof(pcmout2),&current_section);
			if (ret == 0) 
			{
				/* EOF */
				eof=1;
			} else if (ret < 0) 
			{
				/* error in the stream.  Not a problem, just reporting it in
				case we (the app) cares.  In this case, we don't. */
			} else 
			{
				/* we don't bother dealing with sample rate changes, etc, but
				you'll have to*/
				//  fwrite(pcmout,1,ret,stdout);
				//printf("pcm2:%d\r\n",ret);
				ogg_fill_buffer((u16*)pcmout2,ret/2,2); 
			} 
		} 
		key=KEY_Scan(0);
		if(key==KEY0_PRES)break;
	} 
	/* cleanup */
	ov_clear(&vf);  
	myfree(SRAMIN,fogg); 
	myfree(SRAMIN,audiodev.i2sbuf1); 
	myfree(SRAMIN,audiodev.i2sbuf2); 
	for(i=0;i<bufsize;i++)
	{
		printf("mbuff[%d]:%x\r\n",i,mbuff[i]); 
		if(mbuff[i]!=0)
		{
			myfree(SRAMIN,(void*)mbuff[i]);
		}
	}
	return 0;
} 



































