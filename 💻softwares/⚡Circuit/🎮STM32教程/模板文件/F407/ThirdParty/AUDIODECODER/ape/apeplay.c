#include <apeplay.h>
#include "sys.h"
#include "ff.h"
#include "usart.h"
#include "delay.h"
#include "string.h"
#include "malloc.h"
#include "key.h"
#include "i2s.h"
#include "oled.h"
#include "WM8523.h"
#include "audioplay.h"
#include "includes.h"
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
 
__apectrl * apectrl;	//APE���ſ��ƽṹ��

//apedecoder.c������Ҫ������ 
extern filter_int *filterbuf64;		//��Ҫ2816�ֽ� 
	
vu8 apetransferend=0;	//i2s������ɱ�־
vu8 apewitchbuf=0;		//i2sbufxָʾ��־

//APE DMA���ͻص�����
void ape_i2s_dma_tx_callback(void) 
{    
	u16 i;
	if(DMA1_Stream5->CR&(1<<19))
	{
		apewitchbuf=0;
		if((audiodev.status&0X01)==0)//��ͣ��,���0
		{
			for(i=0;i<APE_BLOCKS_PER_LOOP*4;i++)audiodev.i2sbuf1[i]=0;
		}
	}else 
	{
		apewitchbuf=1;
		if((audiodev.status&0X01)==0)//��ͣ��,���0
		{
			for(i=0;i<APE_BLOCKS_PER_LOOP*4;i++)audiodev.i2sbuf2[i]=0;
		}
	} 
	apetransferend=1;
} 
//���PCM���ݵ�DAC
//buf:PCM�����׵�ַ
//size:pcm������(16λΪ��λ) 
void ape_fill_buffer(u16* buf,u16 size)
{
	u16 i; 
	u16 *p;
	while(apetransferend==0)delay_ms(2000/OS_TICKS_PER_SEC);//�ȴ��������
	apetransferend=0;
	if(apewitchbuf==0)
	{
		p=(u16*)audiodev.i2sbuf1;
	}else 
	{
		p=(u16*)audiodev.i2sbuf2;
	}
	for(i=0;i<size;i++)p[i]=buf[i];
} 
//�õ���ǰ����ʱ��
//fx:�ļ�ָ��
//apectrl:apectrl���ſ�����
void ape_get_curtime(FIL*fx,__apectrl *apectrl)
{
	long long fpos=0;  	 
	if(fx->fptr>apectrl->datastart)fpos=fx->fptr-apectrl->datastart;	//�õ���ǰ�ļ����ŵ��ĵط� 
	apectrl->cursec=fpos*apectrl->totsec/(fx->fsize-apectrl->datastart);	//��ǰ���ŵ��ڶ�������?	
}  
//ape�ļ�������˺���
//pos:��Ҫ��λ�����ļ�λ��
//����ֵ:��ǰ�ļ�λ��(����λ��Ľ��)
u32 ape_file_seek(u32 pos)
{ 
	return audiodev.file->fptr;//ape�ļ���֧�ֿ������,ֱ�ӷ��ص�ǰ����λ��
}
//����һ��FLAC����
//fname:FLAC�ļ�·��.
//����ֵ:0,�����������
//[b7]:0,����״̬;1,����״̬
//[b6:0]:b7=0ʱ,��ʾ������ 
//       b7=1ʱ,��ʾ�д���(���ﲻ�ж��������,0X80~0XFF,�����Ǵ���)
extern int vol;
u8 ape_play_song(u8* fname) 
{
		OS_CPU_SR cpu_sr=0;
	  struct ape_ctx_t *apex; 
    int currentframe;
    int nblocks;
    int bytesconsumed;
    int bytesinbuffer;
    int blockstodecode;
    int firstbyte;
	int n; 
	
    u8 res=AP_ERR;
	u8 *readptr;
	u8 *buffer;
	int *decoded0;
	int *decoded1;
	u32 totalsamples; 
	filterbuf64=mymalloc(SRAMIN,2816);  
	apectrl=mymalloc(SRAMIN,sizeof(__apectrl));
	apex=mymalloc(SRAMIN,sizeof(struct ape_ctx_t));
	decoded0=mymalloc(SRAMIN,APE_BLOCKS_PER_LOOP*4);
	decoded1=mymalloc(SRAMIN,APE_BLOCKS_PER_LOOP*4); 
	audiodev.file=(FIL*)mymalloc(SRAMIN,sizeof(FIL));
	audiodev.i2sbuf1=mymalloc(SRAMIN,APE_BLOCKS_PER_LOOP*4);
	audiodev.i2sbuf2=mymalloc(SRAMIN,APE_BLOCKS_PER_LOOP*4); 
	audiodev.file_seek=ape_file_seek;
	buffer=mymalloc(SRAMIN,APE_FILE_BUF_SZ);
	if(filterbuf64&&apectrl&&apex&&decoded0&&decoded1&&audiodev.file&&audiodev.i2sbuf1&&audiodev.i2sbuf2&&buffer)//�����ڴ����붼OK.
	{ 
		memset(apex,0,sizeof(struct ape_ctx_t));
		memset(apectrl,0,sizeof(__apectrl));
		memset(audiodev.i2sbuf1,0,APE_BLOCKS_PER_LOOP*4);
		memset(audiodev.i2sbuf2,0,APE_BLOCKS_PER_LOOP*4);		
		f_open(audiodev.file,(char*)fname,FA_READ);		//���ļ�
		res=ape_parseheader(audiodev.file,apex);//����ape�ļ�ͷ 
		if(res==0)
		{  
			if((apex->compressiontype>3000)||(apex->fileversion<APE_MIN_VERSION)||(apex->fileversion>APE_MAX_VERSION||apex->bps!=16))
			{
				res=2;//ѹ���ʲ�֧��/�汾��֧��/����16λ��Ƶ��ʽ
			}else
			{
				if(apex->compressiontype<=2000)
				{
					OS_ENTER_CRITICAL();//�����ٽ���(�޷����жϴ��) 
		      Stm32_Clock_Init(240,8,2,5,4);//����ʱ��,144Mhz
	        delay_init(120);			//��ʱ��ʼ�� 
          OS_EXIT_CRITICAL();		//�˳��ٽ���(���Ա��жϴ��)	
				}
				else if(apex->compressiontype<=3000)
				{
					 OS_ENTER_CRITICAL();//�����ٽ���(�޷����жϴ��) 
	         Stm32_Clock_Init(276,8,2,5,4);//����ʱ��,144Mhz
	         delay_init(138);			//��ʱ��ʼ�� 
           OS_EXIT_CRITICAL();		//�˳��ٽ���(���Ա��жϴ��)
				}
				apectrl->bps=apex->bps;					//�õ��������(ape,���ǽ�֧��16λ)
				apectrl->samplerate=apex->samplerate;	//�õ�������,ape֧��48Khz���µĲ�����,�ڸ߾Ϳ��ܿ���...
				if(apex->totalframes>1)totalsamples=apex->finalframeblocks+apex->blocksperframe*(apex->totalframes-1);
				else totalsamples=apex->finalframeblocks;
				apectrl->totsec=totalsamples/apectrl->samplerate;//�õ��ļ���ʱ�� 
				apectrl->bitrate=(audiodev.file->fsize-apex->firstframe)*8/apectrl->totsec;//�õ�λ��
				apectrl->outsamples=APE_BLOCKS_PER_LOOP*2;//PCM���������(16λΪ��λ) 
				apectrl->datastart=apex->firstframe;	//�õ���һ֡�ĵ�ַ	 
			}
		}
	}
	if(res==0)
	{   
 		//ape_dumpinfo(apex);
 		WM8523_I2S_Cfg(1,2,0);//�Ҷ���16λ
		I2S3_Init(0,2,0,1);		//�Ҷ����׼,16λ���ݳ���,��������,ʱ�ӵ͵�ƽ��Ч,��׼16λ
		res=I2S3_SampleRate_Set(apex->samplerate);		//���ò����� 
		if(res||apectrl->samplerate>48000)//��֧�ֵĲ�����
			{
				f_close(audiodev.file);
				myfree(SRAMIN,filterbuf64); 
	      myfree(SRAMIN,apectrl);
	      myfree(SRAMIN,apex->seektable);
	      myfree(SRAMIN,apex);
	      myfree(SRAMIN,decoded0);
	      myfree(SRAMIN,decoded1);
	      myfree(SRAMIN,audiodev.file);
	      myfree(SRAMIN,audiodev.i2sbuf1);
	      myfree(SRAMIN,audiodev.i2sbuf2); 
	      myfree(SRAMIN,buffer); 
        OS_ENTER_CRITICAL();//�����ٽ���(�޷����жϴ��) 
	      Stm32_Clock_Init(192,8,2,4,4);//����ʱ��,72Mhz 
	      delay_init(96);			//��ʱ��ʼ��  
        OS_EXIT_CRITICAL();		//�˳��ٽ���(���Ա��жϴ��)				
				return AP_ERR;
			}
		I2S3_TX_DMA_Init(audiodev.i2sbuf1,audiodev.i2sbuf2,APE_BLOCKS_PER_LOOP*2);//����TX DMA
		i2s_tx_callback=ape_i2s_dma_tx_callback;	//�ص�����ָ��mp3_i2s_dma_tx_callback				
 		currentframe=0; 
		f_lseek(audiodev.file,apex->firstframe); 
		res=f_read(audiodev.file,buffer,APE_FILE_BUF_SZ,(u32*)&bytesinbuffer);	
		firstbyte=3;  		//Take account of the little-endian 32-bit byte ordering
		readptr=buffer;
		audio_start(); 
		while(currentframe<apex->totalframes&&res==0)//����δ�����֡?
		{ 
			//����һ֡�����ж��ٸ�blocks?
			if(currentframe==(apex->totalframes-1))nblocks=apex->finalframeblocks;
			else nblocks=apex->blocksperframe; 
			apex->currentframeblocks=nblocks; 
			//��ʼ��֡����
			init_frame_decoder(apex,readptr,&firstbyte,&bytesconsumed);
			readptr+=bytesconsumed;
			bytesinbuffer-=bytesconsumed; 						
      while(nblocks>0)//��ʼ֡����
			{
				blockstodecode=AUDIO_MIN(APE_BLOCKS_PER_LOOP,nblocks);//���һ��Ҫ�����blocks����  
				res=decode_chunk(apex,readptr,&firstbyte,&bytesconsumed,decoded0,decoded1,blockstodecode);
				if(res!=0)
				{
					//printf("frame decode err\r\n");
					res=AP_ERR;
					break;
				} 
				ape_fill_buffer((u16*)decoded1,APE_BLOCKS_PER_LOOP*2);   
				readptr+=bytesconsumed;			//����ָ��ƫ�Ƶ�������λ��
				bytesinbuffer-=bytesconsumed; 	//buffer���������������
				if(bytesconsumed>4*APE_BLOCKS_PER_LOOP)//���ִ�����
				{
					nblocks=0;
					res=AP_ERR;
					//printf("bytesconsumed:%d\r\n",bytesconsumed);
				}
				if(bytesinbuffer<4*APE_BLOCKS_PER_LOOP)//��Ҫ������������
				{ 
					memmove(buffer,readptr,bytesinbuffer);
					res=f_read(audiodev.file,buffer+bytesinbuffer,APE_FILE_BUF_SZ-bytesinbuffer,(u32*)&n);
					if(res)//������.
					{ 
						res=AP_ERR;
						break; 
					}
					bytesinbuffer+=n;  
					readptr=buffer;
				} 
				nblocks-=blockstodecode;//block�����ݼ�
				while(audiodev.status&(1<<1))//����������
				{		 
					ape_get_curtime(audiodev.file,apectrl);//�õ���ʱ��͵�ǰ���ŵ�ʱ�� 
					audiodev.totsec=apectrl->totsec;		//��������
					audiodev.cursec=apectrl->cursec;
					audiodev.bitrate=apectrl->bitrate;
					audiodev.samplerate=apectrl->samplerate;
					audiodev.bps=apectrl->bps;	 
					if(audiodev.status&0X01)break;//û�а�����ͣ 
				  else delay_ms(1000/OS_TICKS_PER_SEC);
				}
				if((audiodev.status&(1<<1))==0)		//�����������/�������
				{  
					nblocks=0;
					res=AP_PREV;
					break;
				}  
			} 
			currentframe++; 
		} 
		audio_stop();
	}
	f_close(audiodev.file);
	myfree(SRAMIN,filterbuf64); 
	myfree(SRAMIN,apectrl);
	myfree(SRAMIN,apex->seektable);
	myfree(SRAMIN,apex);
	myfree(SRAMIN,decoded0);
	myfree(SRAMIN,decoded1);
	myfree(SRAMIN,audiodev.file);
	myfree(SRAMIN,audiodev.i2sbuf1);
	myfree(SRAMIN,audiodev.i2sbuf2); 
	 myfree(SRAMIN,buffer);  
	OS_ENTER_CRITICAL();//�����ٽ���(�޷����жϴ��) 
	Stm32_Clock_Init(192,8,2,4,4);//����ʱ�ӳ�Ƶ��120Mhz
	delay_init(96);			//��ʱ��ʼ��  
      OS_EXIT_CRITICAL();		//�˳��ٽ���(���Ա��жϴ��)	
	return res;
}
























