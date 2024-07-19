#include "wavplay.h" 
#include "audioplay.h"
#include "usart.h" 
#include "delay.h" 
#include "malloc.h"
#include "ff.h"
#include "i2s.h"
#include "wm8523.h"
#include "key.h"
#include "oled.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//WAV �������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/6/29
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved				
//********************************************************************************
//V1.0 ˵��
//1,֧��16λ/24λWAV�ļ�����
//2,��߿���֧�ֵ�192K/24bit��WAV��ʽ. 
////////////////////////////////////////////////////////////////////////////////// 	
 
__wavctrl wavctrl;		//WAV���ƽṹ��
vu8 wavtransferend=0;	//i2s������ɱ�־
vu8 wavwitchbuf=0;		//i2sbufxָʾ��־
 
//WAV������ʼ��
//fname:�ļ�·��+�ļ���
//wavx:wav ��Ϣ��Žṹ��ָ��
//����ֵ:0,�ɹ�;1,���ļ�ʧ��;2,��WAV�ļ�;3,DATA����δ�ҵ�.
u8 wav_decode_init(u8* fname,__wavctrl* wavx)
{
	FIL*ftemp;
	u8 *buf; 
	u32 br=0;
	u8 res=0;
	
	ChunkRIFF *riff;
	ChunkFMT *fmt;
	ChunkFACT *fact;
	ChunkDATA *data;
	ftemp=(FIL*)mymalloc(SRAMIN,sizeof(FIL));
	buf=mymalloc(SRAMIN,512);
	if(ftemp&&buf)	//�ڴ�����ɹ�
	{
		res=f_open(ftemp,(TCHAR*)fname,FA_READ);//���ļ�
		if(res==FR_OK)
		{
			f_read(ftemp,buf,512,&br);	//��ȡ512�ֽ�������
			riff=(ChunkRIFF *)buf;		//��ȡRIFF��
			if(riff->Format==0X45564157)//��WAV�ļ�
			{
				fmt=(ChunkFMT *)(buf+12);	//��ȡFMT�� 
				fact=(ChunkFACT *)(buf+12+8+fmt->ChunkSize);//��ȡFACT��
				if(fact->ChunkID==0X74636166||fact->ChunkID==0X5453494C)wavx->datastart=12+8+fmt->ChunkSize+8+fact->ChunkSize;//����fact/LIST���ʱ��(δ����)
				else wavx->datastart=12+8+fmt->ChunkSize;  
				data=(ChunkDATA *)(buf+wavx->datastart);	//��ȡDATA��
				if(data->ChunkID==0X61746164)//�����ɹ�!
				{
					wavx->audioformat=fmt->AudioFormat;		//��Ƶ��ʽ
					wavx->nchannels=fmt->NumOfChannels;		//ͨ����
					wavx->samplerate=fmt->SampleRate;		//������
					wavx->bitrate=fmt->ByteRate*8;			//�õ�λ��
					wavx->blockalign=fmt->BlockAlign;		//�����
					wavx->bps=fmt->BitsPerSample;			//λ��,16/24/32λ
					
					wavx->datasize=data->ChunkSize;			//���ݿ��С
					wavx->datastart=wavx->datastart+8;		//��������ʼ�ĵط�. 
					 
					
				}else res=3;//data����δ�ҵ�.
			}else res=2;//��wav�ļ�
			
		}else res=1;//���ļ�����
	}
	f_close(ftemp);
	myfree(SRAMIN,ftemp);//�ͷ��ڴ�
	myfree(SRAMIN,buf); 
	return 0;
}

//���buf
//buf:������
//size:���������
//bits:λ��(16/24)
//����ֵ:���������ݸ���
u32 wav_buffill(u8 *buf,u16 size,u8 bits)
{
	u16 readlen=0;
	u32 bread;
	u16 i;
	u8 *p;
	if(bits==24)//24bit��Ƶ,��Ҫ����һ��
	{
		if(wavctrl.nchannels==2)//˫����
		{
		  readlen=(size/4)*3;							//�˴�Ҫ��ȡ���ֽ���
		  f_read(audiodev.file,audiodev.tbuf,readlen,(UINT*)&bread);	//��ȡ����
		  p=audiodev.tbuf;
		  for(i=0;i<size;)
		  {
			  buf[i++]=p[1];
			  buf[i]=p[2]; 
			  i+=2;
			  buf[i++]=p[0];
			  p+=3;
		  } 
		}
		else if(wavctrl.nchannels==1)//������
		{
			readlen=(size/4)*3/2;//�˴�Ҫ��ȡ���ֽ���
		  f_read(audiodev.file,audiodev.tbuf,readlen,(UINT*)&bread);	//��ȡ����
		  p=audiodev.tbuf;
		  for(i=0;i<readlen/3;i++)
		  {
			  buf[8*i+7]=buf[8*i+3]=p[3*i+0];
				buf[8*i+6]=buf[8*i+2]=0;
		    buf[8*i+5]=buf[8*i+1]=p[3*i+2];
			  buf[8*i+4]=buf[8*i+0]=p[3*i+1]; 
		  } 
		}
		bread=(bread*4)/3;		//����Ĵ�С.
	}
	else if(bits==16||bits==32)
	{
		if(wavctrl.nchannels==1)//������
		{
			 f_read(audiodev.file,audiodev.tbuf,size/2,(UINT*)&bread);//16bit��Ƶ,��ȡһ������  
			 //����һ�����ݽ������Ҹ���Ϊ˫��������
			 p=audiodev.tbuf;
			if(wavctrl.bps==16)
			{
		   for(i=0;i<size/4;i++)
		   {
				  buf[4*i+2]=buf[4*i]=p[2*i];
				  buf[4*i+3]=buf[4*i+1]=p[2*i+1];
				 
		   } 
		  }
			else if(wavctrl.bps==32)
			{
				for(i=0;i<size/8;i++)
		   {
			    /*buf[2]=buf[0]=p[0];
				  buf[3]=buf[1]=p[1];
				  
          buf[6]=buf[4]=p[2];
				  buf[7]=buf[5]=p[3];
				  */
				  buf[8*i+4]=buf[8*i]=p[4*i];
				  buf[8*i+5]=buf[8*i+1]=p[4*i+1];
				  buf[8*i+6]=buf[8*i+2]=p[4*i+2];
				  buf[8*i+7]=buf[8*i+3]=p[4*i+3];
				 
		   } 
			}
			if(bread<size/2)//����������,����0
		  {
			  for(i=bread;i<size/2-bread;i++)buf[i]=0; 
		  }
			bread=2*bread;
		}
		else//˫����
		{
		  f_read(audiodev.file,buf,size,(UINT*)&bread);//16bit/32bit��Ƶ,ֱ�Ӷ�ȡ����  
		  if(bread<size)//����������,����0
		  {
			  for(i=bread;i<size-bread;i++)buf[i]=0; 
		  }
	  }
	}
	else if(bits==8)
	{
		if(wavctrl.nchannels==1)//������
		{
			f_read(audiodev.file,audiodev.tbuf,size/4,(UINT*)&bread);//
			p=audiodev.tbuf;
			for(i=0;i<size/4;i++)
		   {
				 buf[4*i+2]=buf[4*i]=0;
				 buf[4*i+3]=buf[4*i+1]=p[i]-128;
		   } 
			if(bread<size/4)//����������,����0
		  {
			  for(i=bread;i<size/4-bread;i++)buf[i]=0; 
		  }
			bread=4*bread;
		}
		else if(wavctrl.nchannels==2)//˫����
		{
			f_read(audiodev.file,audiodev.tbuf,size/2,(UINT*)&bread);//
			p=audiodev.tbuf;
			for(i=0;i<size/2;i++)
		   {
				 buf[2*i]=0;
				 buf[2*i+1]=p[i]-0x80; 
		   } 
			if(bread<size/2)//����������,����0
		  {
			  for(i=bread;i<size/2-bread;i++)buf[i]=0; 
		  }
			bread=2*bread;
		}
	}
	return bread;
}  
//WAV����ʱ,I2S DMA����ص�����
void wav_i2s_dma_tx_callback(void) 
{   
	u16 i;
	if(DMA1_Stream5->CR&(1<<19))
	{
		wavwitchbuf=0;
		if((audiodev.status&0X01)==0)
		{
			for(i=0;i<WAV_I2S_TX_DMA_BUFSIZE;i++)//��ͣ
			{
				audiodev.i2sbuf1[i]=0;//���0
			}
		}
	}else 
	{
		wavwitchbuf=1;
		if((audiodev.status&0X01)==0)
		{
			for(i=0;i<WAV_I2S_TX_DMA_BUFSIZE;i++)//��ͣ
			{
				audiodev.i2sbuf2[i]=0;//���0
			}
		}
	}
	wavtransferend=1;
} 
//�õ���ǰ����ʱ��
//fx:�ļ�ָ��
//wavx:wav���ſ�����
void wav_get_curtime(FIL*fx,__wavctrl *wavx)
{
	long long fpos;  	
 	wavx->totsec=wavx->datasize/(wavx->bitrate/8);	//�����ܳ���(��λ:��) 
	fpos=fx->fptr-wavx->datastart; 					//�õ���ǰ�ļ����ŵ��ĵط� 
	wavx->cursec=fpos*wavx->totsec/wavx->datasize;	//��ǰ���ŵ��ڶ�������?	
}
//����ĳ��WAV�ļ�
//fname:wav�ļ�·��.
//����ֵ:
//KEY2_PRES:��һ��
//KEY1_PRES:��һ��
//����:����
extern int vol;
u8 wav_play_song(u8* fname)
{
	u8 key;
	u8 key1;
	u8 t=0; 
	u8 res; 
  u8 mute=0;	
	u32 fillnum; 
	audiodev.file=(FIL*)mymalloc(SRAMIN,sizeof(FIL));
	audiodev.i2sbuf1=mymalloc(SRAMIN,WAV_I2S_TX_DMA_BUFSIZE);
	audiodev.i2sbuf2=mymalloc(SRAMIN,WAV_I2S_TX_DMA_BUFSIZE);
	audiodev.tbuf=mymalloc(SRAMIN,WAV_I2S_TX_DMA_BUFSIZE);
	if(audiodev.file&&audiodev.i2sbuf1&&audiodev.i2sbuf2&&audiodev.tbuf)
	{ 
		res=wav_decode_init(fname,&wavctrl);//�õ��ļ�����Ϣ
		
		if(res==0)//�����ļ��ɹ�       
		{
			if(wavctrl.bps==16||wavctrl.bps==8)
			{
			  WM8523_I2S_Cfg(1,2,0);	//�����ֱ�׼,16λ���ݳ���
				I2S3_Init(0,2,0,1);		//�����ֱ�׼,��������,ʱ�ӵ͵�ƽ��Ч,16λ��չ֡����
				I2S3_TX_DMA_Init(audiodev.i2sbuf1,audiodev.i2sbuf2,WAV_I2S_TX_DMA_BUFSIZE/2); //����TX DMA
			}else if(wavctrl.bps==24)
			{
				WM8523_I2S_Cfg(1,2,2);	//�����ֱ�׼,24λ���ݳ���
				I2S3_Init(0,2,0,2);		//�����ֱ�׼,��������,ʱ�ӵ͵�ƽ��Ч,24λ��չ֡����
				I2S3_TX_DMA_Init(audiodev.i2sbuf1,audiodev.i2sbuf2,WAV_I2S_TX_DMA_BUFSIZE/2); //����TX DMA
			}else if(wavctrl.bps==32)
			{
				WM8523_I2S_Cfg(1,2,3);	//�����ֱ�׼,32λ���ݳ���
				I2S3_Init(0,2,0,3);		//�����ֱ�׼,��������,ʱ�ӵ͵�ƽ��Ч,32λ֡����
				I2S3_TX_DMA_Init(audiodev.i2sbuf1,audiodev.i2sbuf2,WAV_I2S_TX_DMA_BUFSIZE/2); //����TX DMA
			}
			I2S3_SampleRate_Set(wavctrl.samplerate);//���ò�����
			I2S3_TX_DMA_Init(audiodev.i2sbuf1,audiodev.i2sbuf2,WAV_I2S_TX_DMA_BUFSIZE/2); //����TX DMA
			i2s_tx_callback=wav_i2s_dma_tx_callback;			//�ص�����ָwav_i2s_dma_callback
			audio_stop();
			res=f_open(audiodev.file,(TCHAR*)fname,FA_READ);	//���ļ�
			if(res==0)
			{
				f_lseek(audiodev.file, wavctrl.datastart);		//�����ļ�ͷ
				fillnum=wav_buffill(audiodev.i2sbuf1,WAV_I2S_TX_DMA_BUFSIZE,wavctrl.bps);
				fillnum=wav_buffill(audiodev.i2sbuf2,WAV_I2S_TX_DMA_BUFSIZE,wavctrl.bps);
				audio_start();  
				while(res==0)
				{ 
					while(wavtransferend==0);//�ȴ�wav�������; 
					wavtransferend=0;
					if(fillnum!=WAV_I2S_TX_DMA_BUFSIZE)//���Ž���?
					{
						res=KEY2_PRES;
						break;
					} 
 					if(wavwitchbuf)fillnum=wav_buffill(audiodev.i2sbuf2,WAV_I2S_TX_DMA_BUFSIZE,wavctrl.bps);//���buf2
					else fillnum=wav_buffill(audiodev.i2sbuf1,WAV_I2S_TX_DMA_BUFSIZE,wavctrl.bps);//���buf1
					while(1)
					{
						key=OP_KEY_Scan(0); 
						key1=VOL_KEY_Scan(0);
						if(key==KEY5_PRES)//��ͣ
						{
							if(audiodev.status&0X01)audiodev.status&=~(1<<0);
							else audiodev.status|=0X01;  
						}
						if(key==KEY3_PRES||key==KEY4_PRES)//��һ��/��һ��
						{
							res=key;
							break; 
						}
						if(key1==KEY7_PRES)//��ͣ
						{
							vol--;
							if(vol<0)
								vol=0;
							WM8523_HPvol_Set(vol);//��������0
						}
						else if(key1==KEY6_PRES)
						{
							vol++;
							if(vol>100)
								vol=100;
							WM8523_HPvol_Set(vol);//��������0
						}	
						if(key==KEY2_PRES)//����
						{
							mute=!mute;
							if(mute==0)
							{
								audiodev.status|=0X01;
								I2S_Play_Start();
							  //audio_start();
							  
							  //CS4398_Channel_Cfg(1,0x09);
							//CS4398_Mute_Set(0,0,0,0,0);
							}
							else
							{
								//CS4398_Channel_Cfg(1,0);
							  //audio_stop();
								audiodev.status&=~(1<<0);
							  I2S_Play_Stop();
							//CS4398_Mute_Set(1,1,1,1,1);
							}
						}
						wav_get_curtime(audiodev.file,&wavctrl);//�õ���ʱ��͵�ǰ���ŵ�ʱ�� 
            audio_msg_show(wavctrl.totsec,wavctrl.cursec,wavctrl.bitrate,vol);
            OLED_Refresh_Gram();						
						t++;
						if(t==20)
						{
							t=0;
 							//LED1=!LED1;
						}
						if((audiodev.status&0X01)==0)delay_ms(1);
						else break;
					}
				}
				audio_stop(); 
			}else res=0XFF; 
		}else res=0XFF;
	}else res=0XFF; 
	myfree(SRAMIN,audiodev.tbuf);	//�ͷ��ڴ�
	myfree(SRAMIN,audiodev.i2sbuf1);//�ͷ��ڴ�
	myfree(SRAMIN,audiodev.i2sbuf2);//�ͷ��ڴ� 
	myfree(SRAMIN,audiodev.file);	//�ͷ��ڴ� 
	return res;
} 
	









