#include "flacplay.h"
#include "i2s.h"
#include "WM8523.h"
#include "malloc.h"
#include "usart.h"
#include "key.h"
#include "oled.h"
#include "delay.h"
#include "text.h"
#include "audioplay.h"
#include "includes.h"
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
 
__flacctrl * flacctrl;	//flac������ƽṹ��

 
//����FLAC�ļ�
//fx:flac�ļ�ָ��
//fc:flac��������
//����ֵ:0,�����ɹ�
//    ����,�������
u8 flac_init(FIL* fx,__flacctrl* fctrl,FLACContext* fc)
{
	FLAC_Tag * flactag;
	MD_Block_Head *flacblkh;
	u8 *buf; 
	u8 endofmetadata=0;			//���һ��metadata���
	int blocklength; 
	u32 br;
	u8 res;

	buf=mymalloc(SRAMIN,512);	//����512�ֽ��ڴ�
	if(!buf)return 1;			//�ڴ�����ʧ�� 
	f_lseek(fx,0);				//ƫ�Ƶ��ļ�ͷ
	f_read(fx,buf,4,&br);		//��ȡ4�ֽ� 
	flactag=(FLAC_Tag*)buf;		//ǿ��ת��Ϊflac tag��ǩ
	if(strncmp("fLaC",(char*)flactag->id,4)!=0) 
	{
		myfree(SRAMIN,buf);		//�ͷ��ڴ�
		return 2;				//��flac�ļ�
    } 
    while(!endofmetadata) 
	{
		f_read(fx,buf,4,&br);
        if(br<4)break;
		flacblkh=(MD_Block_Head*)buf;
		endofmetadata=flacblkh->head&0X80;	//�ж��ǲ������һ��block?
		blocklength=((u32)flacblkh->size[0]<<16)|((u16)flacblkh->size[1]<<8)|(flacblkh->size[2]);//�õ����С
        if((flacblkh->head&0x7f)==0) 		//head���7λΪ0,���ʾ��STREAMINFO��
        { 
			      res=f_read(fx,buf,blocklength,&br);
            if(res!=FR_OK)break;  
            fc->min_blocksize=((u16)buf[0]<<8) |buf[1];					//��С���С
            fc->max_blocksize=((u16)buf[2]<<8) |buf[3];					//�����С
            fc->min_framesize=((u32)buf[4]<<16)|((u16)buf[5]<<8)|buf[6];//��С֡��С
            fc->max_framesize=((u32)buf[7]<<16)|((u16)buf[8]<<8)|buf[9];//���֡��С
            fc->samplerate=((u32)buf[10]<<12)|((u16)buf[11]<<4)|((buf[12]&0xf0)>>4);//������
            fc->channels=((buf[12]&0x0e)>>1)+1;							//��Ƶͨ����
            fc->bps=((((u16)buf[12]&0x01)<<4)|((buf[13]&0xf0)>>4))+1;	//����λ��16?24?32? 
            fc->totalsamples=((u32)buf[14]<<24)|((u32)buf[15]<<16)|((u16)buf[16]<<8)|buf[17];//һ���������ܲ�����
			      fctrl->samplerate=fc->samplerate;
			      fctrl->totsec=(fc->totalsamples/fc->samplerate);//�õ���ʱ�� 
        }else 	//��������֡�Ĵ��� 
		{ 
            if(f_lseek(fx,fx->fptr+blocklength)!=FR_OK)
            { 
				myfree(SRAMIN,buf);
				return 3;
            }
		}
    } 
	myfree(SRAMIN,buf);//�ͷ��ڴ�.
	if(fctrl->totsec)
	{
		fctrl->outsamples=fc->max_blocksize*2;//PCM���������(*2,��ʾ2��������������)
		fctrl->bps=fc->bps;			//����λ��(16/24/32)
		fctrl->datastart=fx->fptr;	//FLAC����֡��ʼ�ĵ�ַ
		fctrl->bitrate=((fx->fsize-fctrl->datastart)*8)/fctrl->totsec;//�õ�FLAC��λ��
	}else return 4;	//��ʱ��Ϊ0?�������flac�ļ�
	return 0;
} 
vu8 flactransferend=0;	//i2s������ɱ�־
vu8 flacwitchbuf=0;		//i2sbufxָʾ��־ 
//FLAC DMA���ͻص�����

void flac_i2s_dma_tx_callback(void) 
{    
	u16 i;
	u16 size;
	if(DMA1_Stream5->CR&(1<<19))
	{
		flacwitchbuf=0;
		if((audiodev.status&0X01)==0)//��ͣ��,���0
		{ 
			if(flacctrl->bps==24)size=flacctrl->outsamples*4;
			else size=flacctrl->outsamples*2;
			for(i=0;i<size;i++)audiodev.i2sbuf1[i]=0;
		}
	}else 
	{
		flacwitchbuf=1;
		if((audiodev.status&0X01)==0)//��ͣ��,���0
		{
			if(flacctrl->bps==24)size=flacctrl->outsamples*4;
			else size=flacctrl->outsamples*2;
			for(i=0;i<size;i++)audiodev.i2sbuf2[i]=0;
		}
	} 
	flactransferend=1;
} 
//�õ���ǰ����ʱ��
//fx:�ļ�ָ��
//flacctrl:flac���ſ�����
void flac_get_curtime(FIL*fx,__flacctrl *flacctrl)
{
	long long fpos=0;  	 
	if(fx->fptr>flacctrl->datastart)fpos=fx->fptr-flacctrl->datastart;	//�õ���ǰ�ļ����ŵ��ĵط� 
	flacctrl->cursec=fpos*flacctrl->totsec/(fx->fsize-flacctrl->datastart);	//��ǰ���ŵ��ڶ�������?	
}
//flac�ļ�������˺���
//pos:��Ҫ��λ�����ļ�λ��
//����ֵ:��ǰ�ļ�λ��(����λ��Ľ��)
u32 flac_file_seek(u32 pos)
{
	if(pos>audiodev.file->fsize)
	{
		pos=audiodev.file->fsize;
	}
	f_lseek(audiodev.file,pos);
	return audiodev.file->fptr;
}
//����һ��FLAC����
//fname:FLAC�ļ�·��.
//����ֵ:0,�����������
//[b7]:0,����״̬;1,����״̬
//[b6:0]:b7=0ʱ,��ʾ������ 
//       b7=1ʱ,��ʾ�д���(���ﲻ�ж��������,0X80~0XFF,�����Ǵ���)
extern int vol;
u8 flac_play_song(u8* fname) 
{ 
	 OS_CPU_SR cpu_sr=0;
    FLACContext *fc=0; 
    int bytesleft;
    int consumed;
	u8 res=0; 
	u8 res1=0; 
	u32 i=0;
	u32 br=0; 
	u8* buffer=0;    
	u8* decbuf0=0;   
	u8* decbuf1=0;  
	u8* p8=0;   
	u32 flac_fptr=0; 
 	fc=mymalloc(SRAMIN,sizeof(FLACContext)); 
	flacctrl=mymalloc(SRAMIN,sizeof(__flacctrl)); 
	audiodev.file=(FIL*)mymalloc(SRAMIN,sizeof(FIL));
	audiodev.file_seek=flac_file_seek;
	if(!fc||!audiodev.file||!flacctrl)
		{
			
		OLED_ShowString(30,48,"malloc err",12);
		OLED_Refresh_Gram();//������ʾ��OLED
		delay_ms(2000);
		res=1;//�ڴ��������
	}
	else
	{ 
		memset(fc,0,sizeof(FLACContext));//fc������������ 
		res=f_open(audiodev.file,(char*)fname,FA_READ); //��ȡ�ļ����� 
		if(res==FR_OK)
		{
			res=flac_init(audiodev.file,flacctrl,fc);	//flac�����ʼ��   
			//OLED_ShowNum(31,48,fc->max_blocksize,6,12);//4608�̶�
			//OLED_ShowNum(31,48,fc->max_framesize,6,12);//16524z
			
			if(fc->min_blocksize==fc->max_blocksize&&fc->max_blocksize!=0)//����min_blocksize����max_blocksize
			{
				if(fc->bps==24)	//24λ��Ƶ����
				{	
					 if(flacctrl->samplerate/1000>=176)
					{
						OS_ENTER_CRITICAL();//�����ٽ���(�޷����жϴ��) 
	          //Stm32_Clock_Init(288,8,2,6);//����ʱ��,144Mhz 
	         // delay_init(144);			//��ʱ��ʼ�� 
						Stm32_Clock_Init(240,8,2,5,4);
						delay_init(120);
            OS_EXIT_CRITICAL();		//�˳��ٽ���(���Ա��жϴ��)		
					}
					//OLED_ShowNum(31,48,my_mem_perused(SRAMIN),2,12); //3
					audiodev.i2sbuf1=mymalloc(SRAMIN,fc->max_blocksize*8);//DMA����ڴ治��ʹ��CCM�ڴ棬ֻ��cpu������
					//OLED_ShowNum(31+17,48,my_mem_perused(SRAMIN),2,12); //18
					audiodev.i2sbuf2=mymalloc(SRAMIN,fc->max_blocksize*8);
         // OLED_ShowNum(31+17+17,48,my_mem_perused(SRAMIN),2,12); //34									
				}
				else//16λ��Ƶ���� 
				{
					
					//OLED_ShowNum(31,48,my_mem_perused(SRAMIN),2,12); //3
					audiodev.i2sbuf1=mymalloc(SRAMIN,fc->max_blocksize*4);
					//OLED_ShowNum(31+17,48,my_mem_perused(SRAMIN),2,12); //18
					audiodev.i2sbuf2=mymalloc(SRAMIN,fc->max_blocksize*4);
          //OLED_ShowNum(31+17+17,48,my_mem_perused(SRAMIN),2,12); //34					
				}
				buffer=mymalloc(SRAMIN,fc->max_framesize); 	//�������֡���� 24kb
				//OLED_ShowNum(31+17+17+17,48,my_mem_perused(SRAMIN),2,12); //40
				//OLED_ShowNum(36,48,fc->max_framesize,5,12);//
				decbuf0=mymalloc(SRAMIN,fc->max_blocksize*4);//18kb
				//OLED_ShowNum(30+36,48,fc->max_blocksize,5,12);//
				decbuf1=mymalloc(SRAMIN,fc->max_blocksize*4); //18kb
				 //OLED_ShowNum(31+17+17+17+17,24,my_mem_perused(SRAMIN),2,12);//60
				//OLED_ShowNum(31+17+17+17+17+17,24,my_mem_perused(SRAMIN),2,12);//60
				//OLED_Refresh_Gram();//������ʾ��OLED
				//delay_ms(2000);
			}
			else 
			{
				OLED_ShowString(30,48,"unkown err",12);
				OLED_Refresh_Gram();//������ʾ��OLED
			  delay_ms(2000);
				res+=1;//��֧�ֵ���Ƶ��ʽ 
			}	
		}
	}
	
	if(buffer&&audiodev.i2sbuf1&&audiodev.i2sbuf2&&decbuf0&&decbuf1&&res==0)
	{ 
		if(flacctrl->bps==24)	//24λ��Ƶ����
		{   
			WM8523_I2S_Cfg(1,2,2);//I2S 24λ
			I2S3_Init(0,2,0,2);	//�����ֱ�׼,��������,ʱ�ӵ͵�ƽ��Ч,24λ��չ֡����
			I2S3_TX_DMA_Init(audiodev.i2sbuf1,audiodev.i2sbuf2,flacctrl->outsamples*2);//����TX DMA
			memset(audiodev.i2sbuf1,0,fc->max_blocksize*8);
			memset(audiodev.i2sbuf2,0,fc->max_blocksize*8);
		}
		else if(flacctrl->bps==16)//16λ��Ƶ����
		{
			WM8523_I2S_Cfg(1,2,0);//�Ҷ���16λ
			I2S3_Init(0,2,0,1);		//�Ҷ����׼,16λ���ݳ���,��������,ʱ�ӵ͵�ƽ��Ч,��׼16λ
			I2S3_TX_DMA_Init(audiodev.i2sbuf1,audiodev.i2sbuf2,flacctrl->outsamples);//����TX DMA			
			memset(audiodev.i2sbuf1,0,fc->max_blocksize*4);
			memset(audiodev.i2sbuf2,0,fc->max_blocksize*4);
		}  
		res=I2S3_SampleRate_Set(fc->samplerate);		//���ò����� 
		if(res)//��֧�ֵĲ�����
			{
				myfree(SRAMIN,fc);
      	myfree(SRAMIN,flacctrl);
      	myfree(SRAMIN,flacctrl);
	      myfree(SRAMIN,audiodev.file);
	      myfree(SRAMIN,audiodev.i2sbuf1);
	      myfree(SRAMIN,audiodev.i2sbuf2); 
	      myfree(SRAMIN,buffer);
	      myfree(SRAMIN,decbuf0);
	      myfree(SRAMIN,decbuf1); 
				OS_ENTER_CRITICAL();//�����ٽ���(�޷����жϴ��) 
	      Stm32_Clock_Init(192,8,2,4,4);//����ʱ��,96Mhz 
	      delay_init(96);			//��ʱ��ʼ�� 
        OS_EXIT_CRITICAL();		//�˳��ٽ���(���Ա��жϴ��)
				OLED_ShowString(30,48,"unkown samplerate",12);
				OLED_Refresh_Gram();//������ʾ��OLED
				delay_ms(2000);
				return 1;
			}
		i2s_tx_callback=flac_i2s_dma_tx_callback;	//�ص�����ָ��flac_i2s_dma_tx_callback
		f_read(audiodev.file,buffer,fc->max_framesize,&br);//��ȡ���֡������		
		bytesleft=br;
		audio_start();					//��ʼ����  
		fc->decoded0=(int*)decbuf0;		//��������0
		fc->decoded1=(int*)decbuf1;  	//��������1 
		flac_fptr=audiodev.file->fptr;	//��¼��ǰ���ļ�λ��.
    while(bytesleft) 
		{   
			while(flactransferend==0)//�ȴ��������
			{ 
				delay_ms(1000/OS_TICKS_PER_SEC);
			}	
			if(flac_fptr!=audiodev.file->fptr)//˵���ⲿ�н����ļ����/���˲���
			{
				if(audiodev.file->fptr<flacctrl->datastart)//�����ݿ�ʼ֮ǰ??
				{
					f_lseek(audiodev.file,flacctrl->datastart);//ƫ�Ƶ����ݿ�ʼ�ĵط�
				} 
				f_read(audiodev.file,buffer,fc->max_framesize,&br); //��ȡһ�����֡��������
				bytesleft=flac_seek_frame(buffer,br,fc);		//����֡ 
				if(bytesleft>=0)								//�ҵ���ȷ��֡ͷ.
				{
					f_lseek(audiodev.file,audiodev.file->fptr-fc->max_framesize+bytesleft);
					f_read(audiodev.file,buffer,fc->max_framesize,&br); 
				}
				else
				{
					OLED_ShowString(30,12,"find frame err",12);
					OLED_Refresh_Gram();//������ʾ��OLED
					//Show_Str(0,24,127,12,"Ѱ��֡����:",12,0);
					delay_ms(2000);
				}// printf("flac seek error:%d\r\n",bytesleft); 
				bytesleft=br;
			}
			flactransferend=0;  
			if(flacwitchbuf==0)p8=audiodev.i2sbuf1;
			else p8=audiodev.i2sbuf2; 
			if(fc->bps==24)res1=flac_decode_frame24(fc,buffer,bytesleft,(s32*)p8);
			else res1=flac_decode_frame16(fc,buffer,bytesleft,(s16*)p8); 
			if(res1!=0)//��������� 
			{
				bytesleft++;
				//f_lseek(audiodev.file,audiodev.file->fptr+bytesleft);
				//audiodev.file_seek((float)audiodev.cursec++/(float)audiodev.totsec*(float)audiodev.file->fsize);
				res=AP_DEC_ERR;
				//break;
			}
			if(fc->bps==24)	//24λ��ʱ��,������Ҫ����������
			{ 
				for(i=0;i<fc->blocksize*8;)
				{
					p8[i+3]=p8[i+0];
					p8[i+0]=p8[i+1]; 
					p8[i+1]=p8[i+2]; 
					p8[i+2]=0;
					i+=4;
				} 		
			}
			consumed=fc->gb.index/8;
			memmove(buffer,&buffer[consumed],bytesleft-consumed);
			bytesleft-=consumed; 
			res=f_read(audiodev.file,&buffer[bytesleft],fc->max_framesize-bytesleft,&br); 
			if(res)//�����ݳ�����
			{
				res=AP_ERR;
				break;
			}
			if(br>0) 
			{
				bytesleft+=br;
			}
			flac_fptr=audiodev.file->fptr;	//��¼��ǰ���ļ�λ��.
			while(audiodev.status&(1<<1))	//����������
			{		  
				flac_get_curtime(audiodev.file,flacctrl);//�õ���ʱ��͵�ǰ���ŵ�ʱ�� 
				audiodev.totsec=flacctrl->totsec;		//��������
				audiodev.cursec=flacctrl->cursec;
				audiodev.bitrate=flacctrl->bitrate;
				audiodev.samplerate=flacctrl->samplerate;
				audiodev.bps=flacctrl->bps;	
 				if(audiodev.status&0X01)break;//û�а�����ͣ 
				else delay_ms(1000/OS_TICKS_PER_SEC);
			}
			if((audiodev.status&(1<<1))==0)		//�����������/�������
			{  
				break;
			}  
		} 
		audio_stop();
	}else res=AP_ERR;
	f_close(audiodev.file);
	myfree(SRAMIN,fc);
	myfree(SRAMIN,flacctrl);
	myfree(SRAMIN,audiodev.file);
	myfree(SRAMIN,audiodev.i2sbuf1);
	myfree(SRAMIN,audiodev.i2sbuf2); 
	myfree(SRAMIN,buffer);
	myfree(SRAMIN,decbuf0);
	myfree(SRAMIN,decbuf1);
  OS_ENTER_CRITICAL();//�����ٽ���(�޷����жϴ��) 
	Stm32_Clock_Init(192,8,2,4,4);//����ʱ��,96Mhz 
	delay_init(96);			//��ʱ��ʼ�� 
  OS_EXIT_CRITICAL();		//�˳��ٽ���(���Ա��жϴ��)			
    return res;
} 

	









































