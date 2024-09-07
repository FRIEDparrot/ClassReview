#include "mp3player.h"
#include "mp3dec.h"
#include "es8388.h"    // hardware driver 
#include "string.h"
#include "stdio.h"

/** private variables */
static __mp3ctrl mp3ctrl;

static FIL mp3_file;    /** global variable for mp3 file to open */
static HMP3Decoder decoder;

/* Private function prototypes -----------------------------------------------*/ 
static uint8_t mp3_player_allocate_mem(void);
static uint8_t mp3_id3v1_decode(uint8_t* buf,__mp3ctrl *pctrl);
static uint8_t mp3_id3v2_decode(uint8_t* buf,uint32_t size,__mp3ctrl *pctrl);
static uint8_t mp3_get_info(uint8_t *pname, __mp3ctrl* pctrl);
static void    mp3_get_curtime(FIL*fx,__mp3ctrl *mp3x);

static volatile uint8_t mp3_dma_tar_mem = 0;
static volatile uint8_t mp3_dma_tc_flag = 0;

/* Private functions ---------------------------------------------------------*/ 

void mp3player_i2s_callback(void){
    if (es8388_dma_handler.Instance->CR & DMA_SxCR_CT){
        mp3_dma_tar_mem = 1;
    }else{
        mp3_dma_tar_mem = 0;
    }
    mp3_dma_tc_flag = 1;
}

/// @brief allocate memory for mp3 player 
/// @param  
/// @return 0 if successful, otherwise an error code. 
static uint8_t mp3_player_allocate_mem(void){
    musicplayer_FreeStroge();
    HAL_Delay(10);
    music_player_buff0 = (uint8_t*)__Malloc_Buff__(MP3_AUDIO_TX_BUFF_SZ);      /* 2304 * 2 */
    music_player_buff1 = (uint8_t*)__Malloc_Buff__(MP3_AUDIO_TX_BUFF_SZ);      /* 2304 * 2 */
    music_player_buff_rx8 = (uint8_t*)__Malloc_Buff__(MP3_FILE_READ_BUFF_SZ);  /* 5 * 1024 bytes */
    music_player_buff_rx16 = (uint16_t*)__Malloc_Buff__(MP3_AUDIO_TX_BUFF_SZ); /* size = 2304    */
    
    if (music_player_buff0 == NULL || music_player_buff1 == NULL ||  \
            music_player_buff_rx8 == NULL || music_player_buff_rx16 == NULL) {
        musicplayer_FreeStroge();    /* call the shared free function */
        return 1;
    }
    music_player_buff_size = MP3_AUDIO_TX_BUFF_SZ;
    return 0;
}

/// @brief Parse ID3V1 tags.
/// @param buf Input data buffer.
/// @param size Data size.
/// @param pctrl MP3 controller.
/// @return  0 if successful, otherwise an error code.
static uint8_t mp3_id3v1_decode(uint8_t* buf,__mp3ctrl *pctrl)
{
	ID3V1_Tag *id3v1tag;
	id3v1tag=(ID3V1_Tag*)buf;
	if (strncmp("TAG",(char*)id3v1tag->id,3)==0)//It is MP3 ID3V1 TAG
	{
		if(id3v1tag->title[0])strncpy((char*)pctrl->title,(char*)id3v1tag->title,sizeof(pctrl->title));
		if(id3v1tag->artist[0])strncpy((char*)pctrl->artist,(char*)id3v1tag->artist,sizeof(pctrl->artist));
	}else return 1;
	return 0;
}

/// @brief Parse ID3V2 tags.
/// @param buf: Input data buffer.
/// @param size: Data size.
/// @param pctrl: MP3 controller.
/// @return 0 if successful, otherwise an error code.
static uint8_t mp3_id3v2_decode(uint8_t* buf,uint32_t size,__mp3ctrl *pctrl)
{
	ID3V2_TagHead *taghead;
	ID3V23_FrameHead *framehead; 
	uint32_t t;
	uint32_t tagsize;	//tag size
	uint32_t frame_size;	//Frame size
	taghead=(ID3V2_TagHead*)buf;
	if(strncmp("ID3",(const char*)taghead->id,3)==0)//Presence of ID3?
	{
		tagsize=((uint32_t)taghead->size[0]<<21)|((uint32_t)taghead->size[1]<<14)|((uint16_t)taghead->size[2]<<7)|taghead->size[3];//Get the tag size
		pctrl->datastart=tagsize;		//Get the offset at the beginning of the mp3 data
		if(tagsize>size)tagsize=size;	//If the tagsize is greater than the input bufsize, only the data of the entered size is processed
		if(taghead->mversion<3)
		{ 
			// not supported mp3 version  
			return 1;
		}
		t=10;
		while(t<tagsize)
		{
			framehead=(ID3V23_FrameHead*)(buf+t);
			frame_size=((uint32_t)framehead->size[0]<<24)|((uint32_t)framehead->size[1]<<16)|((uint32_t)framehead->size[2]<<8)|framehead->size[3];//Get the frame size
 			if (strncmp("TT2",(char*)framehead->id,3)==0||strncmp("TIT2",(char*)framehead->id,4)==0)//Find the title frame of the song, Unicode format is not supported!!
			{
				strncpy((char*)pctrl->title,(char*)(buf+t+sizeof(ID3V23_FrameHead)+1),AUDIO_MIN(frame_size-1,MP3_TITSIZE_MAX-1));
			}
 			if (strncmp("TP1",(char*)framehead->id,3)==0||strncmp("TPE1",(char*)framehead->id,4)==0)//Locate the song artist frame
			{
				strncpy((char*)pctrl->artist,(char*)(buf+t+sizeof(ID3V23_FrameHead)+1),AUDIO_MIN(frame_size-1,MP3_ARTSIZE_MAX-1));
			}
			t+=frame_size+sizeof(ID3V23_FrameHead);
		} 
	}else pctrl->datastart=0;//ID3 does not exist, mp3 data starts from 0
	return 0;
}

// Get basic information about an MP3 file.
// pname: Path to the MP3 file.
// pctrl: MP3 control information structure.
// Returns: 0 if successful, otherwise an error code. 
static uint8_t mp3_get_info(uint8_t *pname, __mp3ctrl* pctrl)
{
    /** define the structure for decode */
    MP3FrameInfo  frame_info;
	MP3_FrameXing* fxing;
	MP3_FrameVBRI* fvbri;
    
	int offset=0;
	uint32_t p;
	short samples_per_frame;	// The number of samples per frame
	uint32_t totframes;			// Total number of frames
    uint8_t res;
    UINT br;

    /* ----- Open the file to get infomation ------ */ 
    if (f_open(&mp3_file,(const TCHAR*)pname,FA_READ)!= FR_OK) return 1;  
    res = f_read(&mp3_file, (char*)music_player_buff_rx8,MP3_FILE_READ_BUFF_SZ,&br);  // read first data in byte 0
    
    if(res!=FR_OK) //The file is read successfully, and ID3V2/ID3V1 is parsed and MP3 information is obtained
    {
        f_close(&mp3_file); return 1;
    }
    if (mp3_id3v2_decode((uint8_t*)music_player_buff_rx8, br, pctrl)){   // Parse ID3V2 data
        f_lseek(&mp3_file, f_size(&mp3_file)-128);	                    // Offset to the position of 128 from the bottom
        f_read(&mp3_file,(uint8_t*)music_player_buff_rx8, 128, &br);            // Read 128 bytes 
        // Parse ID3V1 data 
        if (mp3_id3v1_decode((uint8_t*)music_player_buff_rx8, pctrl)){
            f_close(&mp3_file);
            musicplayer_FreeStroge();
            return 1;
        }
    }
    
    /* ----- Get the MP3 first frame information ------ */ 
    decoder = MP3InitDecoder(); 		        // MP3 decoding application memory
    f_lseek(&mp3_file, pctrl->datastart);	    // Offset to where the data began
    f_read(&mp3_file, (char*)music_player_buff_rx8, MP3_FILE_READ_BUFF_SZ, &br);  // Read 5K bytes of mp3 data
    offset = MP3FindSyncWord((uint8_t*)music_player_buff_rx8, br);	      // Find frame synchronization information

    /* The frame synchronization information has been found, and the next wave of information is normal */
    if(decoder!=NULL && offset>=0 && MP3GetNextFrameInfo(decoder,&frame_info, &((unsigned char*)music_player_buff_rx8)[offset]) ==0)
    {
        p=offset+4+32;
        fvbri = (MP3_FrameVBRI*)(music_player_buff_rx8+p);  /** find VBRI frame header */
        if(strncmp("VBRI",(char*)fvbri->id,4)==0) // VBRI frame present (VBR format)
        {
            if (frame_info.version==MPEG1) samples_per_frame=1152;  // for MPEG1, layer 3 is equal to 1152 samples per frame
            else samples_per_frame=576;                            // for MPEG2/MPEG2.5, layer 3 samples per frame is equal to 576 
            totframes=((uint32_t)fvbri->frames[0]<<24)|((uint32_t)fvbri->frames[1]<<16)|((uint16_t)fvbri->frames[2]<<8)|fvbri->frames[3];//Gets the total number of frames
            
            pctrl->totsec=totframes*samples_per_frame/frame_info.samprate;//Get the total length of the file 
        }
        else	//If it is not a VBRI frame, try whether it is a Xing frame (VBR format)
        {
            if (frame_info.version==MPEG1)	// MPEG1
            {
                p=frame_info.nChans==2?32:17;
                samples_per_frame = 1152;	// MPEG1, layer 3 is equal to 1152 samples per frame
            }else
            {
                p=frame_info.nChans==2?17:9;
                samples_per_frame=576;		// MPEG2/MPEG2.5, layer 3 samples per frame is equal to 576
            }
            p+=offset+4;
            fxing=(MP3_FrameXing*)(music_player_buff_rx8 + p);
            if(strncmp("Xing",(char*)fxing->id,4)==0||strncmp("Info",(char*)fxing->id,4)==0)//It's an Xng frame
            {
                if(fxing->flags[3]&0X01)//The total frame field exists
                {
                    totframes=((uint32_t)fxing->frames[0]<<24)|((uint32_t)fxing->frames[1]<<16)|((uint16_t)fxing->frames[2]<<8)|fxing->frames[3];//Gets the total number of frames
                    pctrl->totsec=totframes*samples_per_frame/frame_info.samprate;//Get the total length of the file
                }
                else	// The total frames field does not exist 
                {
                    pctrl->totsec=f_size(&mp3_file)/(frame_info.bitrate/8);
                }
            }
            else 	   // CBR format, directly calculate the total playback time
            {
                pctrl->totsec=f_size(&mp3_file)/(frame_info.bitrate/8);
            }
        }
        pctrl->bitrate = frame_info.bitrate;			//Get the bitrate of the current frame
        pctrl->samplerate = frame_info.samprate; 	//Get the sample rate.
        if (frame_info.nChans==2) pctrl->outsamples=frame_info.outputSamps; // The amount of PCM data that is output
        else pctrl->outsamples=frame_info.outputSamps*2; //The amount of PCM data output, for mono MP3, directly*2, is completed as a two-channel output
    }
    else 
    {    
        res=1;//No sync frame found
    }
    MP3FreeDecoder(decoder);//Free up memory
	return res;
}

//Get the current playback time
//fx: file pointer
//MP3X: MP3 playback controller
static void mp3_get_curtime(FIL*fx,__mp3ctrl *mp3x)
{
	uint32_t fpos=0;
	if(fx->fptr>mp3x->datastart)fpos=fx->fptr-mp3x->datastart;	// Get the current file to play to the place
	mp3x->cursec=fpos*mp3x->totsec/(f_size(fx)-mp3x->datastart);	// get how many seconds is it playing  
}


uint8_t mp3_player_init(uint8_t* fpath){
    /* free memory and allocate 5kb to buffer 0 */
    musicplayer_FreeStroge();
    music_player_buff_rx8 = __Malloc_Buff__(MP3_FILE_READ_BUFF_SZ);
    if (music_player_buff_rx8 == NULL) return 1;
    
    /* read file and decode the message */
    uint8_t res = mp3_get_info(fpath, &mp3ctrl);
    musicplayer_FreeStroge();   /** clear memory after decoding message */
    if (res != 0) return 1;
    
    __Mem_Set___((void*) music_name_playing, 0, 255);
    strcpy((char*)music_name_playing, (char*)fpath);  // save the name of the current playing music
    
    if (mp3_player_allocate_mem()) return 1;
    
    printf("artist: %s\r\n", mp3ctrl.artist);
    printf("title: %s\r\n", mp3ctrl.title);
    printf("bitrate: %d\r\n", mp3ctrl.bitrate);
    printf("samplerate: %d\r\n", mp3ctrl.samplerate);
    printf("totsec: %d\r\n", mp3ctrl.totsec);
    printf("cursec: %d\r\n", mp3ctrl.cursec);
    printf("datastart: %d\r\n", mp3ctrl.datastart);
    printf("outsamples: %d\r\n", mp3ctrl.outsamples);

    return 0;
}

static void mp3_fill_buffer(uint32_t outsamples, uint32_t  nchans){
    /** transfer the decode data to es8388 for output */
    while (mp3_dma_tc_flag == 0);                 // wait for  the last DMA transfer complete
    mp3_dma_tc_flag = 0;                          // clear the DMA transfer complete flag
    // note : the 16-bit data is signed integar, so we cast it into 8 bit as follows:  
    uint8_t *p;
    if (mp3_dma_tar_mem == 0){
        p = (uint8_t*)music_player_buff1;
    }else{
        p = (uint8_t*)music_player_buff0;
    }
    
    if (nchans == 1 && outsamples <= MP3_AUDIO_TX_BUFF_SZ/4){ // chans == 1 -> copy mono to stereo 
        for (int i = 0; i < outsamples; i++){
            uint16_t temp = music_player_buff_rx16[i];
            p[4 * i] = (temp & 0xFF);
            p[4 * i + 1] = (temp >> 8);
            p[4 * i + 2] = p[4 * i];
            p[4 * i + 3] = p[4 * i + 1];
        }
    }else{ //  (nchans == 2) 
        for (int i = 0; i < outsamples; i++){
            uint16_t temp = music_player_buff_rx16[i];
            p[2 * i] = (temp & 0xFF);
            p[2 * i + 1] = (temp >> 8);
        }
    }
}

// start playing the current mp3 file 
/// @note : in play function, storge must be freed after calling it (use stop function)
uint8_t mp3_player_play(void){
    MP3FrameInfo mp3frameinfo;
    uint8_t res;
    unsigned char* readptr;	// MP3 decodes the read pointer
    int err = 0;            // error for mp3 decode  
    int offset=0;	        // Offset
    int bytesleft=0;        // buffer also has valid data remaining
    UINT br = 0;
    
    /**  Init hardware layer output controller */
    ES8388_i2s_tc_callback = mp3player_i2s_callback;   /* set up callback function */
    
    if (ES8388_OutPut_Init((uint8_t*)music_player_buff0, (uint8_t*)music_player_buff1, MP3_AUDIO_TX_BUFF_SZ) ||
        decoder == NULL || ES8388_SetSampleRate_Auto(mp3ctrl.samplerate))   // note : if use accurate -> can't set sample rate correctly;
    {
        return 1;   /** es8388 error */
    }
    ES8388_I2SModeSet(I2S_STANDARD_PHILIPS, I2S_DATAFORMAT_16B, 1, DMA_CIRCULAR, 1);
    musicplayer_setoutput(playerctrl.OutputDevice);
    musicplayer_setvolume( PLAYER_OUTPUT_DEVICE_HEADPHONE, playerctrl.ch1Volume);   /** Initialize  */
    musicplayer_setvolume( PLAYER_OUTPUT_DEVICE_SPEAKER, playerctrl.ch2Volume);
    
    decoder = MP3InitDecoder();     /* Init mp3 decoder */
    if (f_open(&mp3_file, (char*) music_name_playing, FA_READ)!=FR_OK) return 1;
    f_lseek(&mp3_file, mp3ctrl.datastart);	   //Skip the tag in the file header
    
    /** before start playing, try read the buffer and decode the 1st frame */
    res = f_read(&mp3_file, (void*)music_player_buff_rx8, MP3_FILE_READ_BUFF_SZ, &br);    
    
    /* try decode the first frame */
    if (res == FR_OK) {
        readptr = (uint8_t*) music_player_buff_rx8;
        bytesleft = br;
        offset = MP3FindSyncWord((unsigned char*)readptr, bytesleft);
        readptr += offset;
        bytesleft -= offset;
        err = MP3Decode(decoder, &readptr, &bytesleft, (short*)music_player_buff_rx16, 0);
        if (offset < 0 || err!=ERR_MP3_NONE) 
        {
            f_close(&mp3_file); return 1;
        }
        MP3GetLastFrameInfo(decoder, &mp3frameinfo);
        mp3ctrl.nChannels = mp3frameinfo.nChans;
        /* set buffer to mute first */
        __Mem_Set___((void*)music_player_buff0, 0, MP3_AUDIO_TX_BUFF_SZ);
        __Mem_Set___((void*)music_player_buff1, 0, MP3_AUDIO_TX_BUFF_SZ); 
    }
    else
    {
        f_close(&mp3_file); return 1;
    }


    /** start the music player and decode all the frames */
    ES8388_DMAStart();
    while(!f_eof(&mp3_file)){
        offset = 0;
        err = 0;
        
        /** fill the last frame */
        mp3_get_curtime(&mp3_file, &mp3ctrl);
        mp3_fill_buffer(mp3ctrl.outsamples, mp3ctrl.nChannels);
        
        /** begin decode this frame */
        offset = MP3FindSyncWord((unsigned char*)readptr, bytesleft);
        /* find the sync word from buff each time */
        if (offset < 0)
        {   /* if sync word can't be found, jump this frame */
            if (f_read(&mp3_file, (void*)music_player_buff_rx8, MP3_FILE_READ_BUFF_SZ, &br)!= FR_OK) break;
            /** reset read pointer and bytes left */
            readptr   = (unsigned char*)music_player_buff_rx8;
            bytesleft = br;
            continue;
        }
        /* fix the reading pointer and bytes left */
        else
        {
            /** decode from offset */
            readptr   += offset;
            bytesleft -= offset;
            /** retry 4 times for decode */
            for (int i = 0; i < 4; i ++){
                err = MP3Decode(decoder, &readptr, &bytesleft, (short*)music_player_buff_rx16, 0);
                if (err == ERR_MP3_NONE) break;
                readptr++;
                bytesleft--;
                offset = MP3FindSyncWord((unsigned char*)readptr, bytesleft);
                readptr += offset;
                bytesleft -= offset;
                if (bytesleft < 0|| offset < 0) break;
            }
            if (err != ERR_MP3_NONE){ /** if still decode failed, fill buff with mute */
                // printf("frame decode failed, replaced with mute\r\n");
                __Mem_Set___((void*)music_player_buff_rx16, 0, MP3_AUDIO_TX_BUFF_SZ);
            }
            // MP3GetLastFrameInfo(decoder, &mp3frameinfo);        /** get the last frame from decoder */
            if (bytesleft < MAINBUF_SIZE){  /* read  new data in the buffer */
                uint8_t byte_align = 4 - bytesleft&3;  // make sure the bytesleft is 4 bytes alignment 
                bytesleft += byte_align;
                memcpy((void*)music_player_buff_rx8, (void*)(readptr - byte_align), bytesleft);   /** copy the bytesleft length data to tx buffer  */
                res = f_read(&mp3_file,  (void*)(music_player_buff_rx8 + bytesleft), MP3_FILE_READ_BUFF_SZ - bytesleft, &br);
                if (res != FR_OK) break;
                if (br <  MP3_FILE_READ_BUFF_SZ - bytesleft){
                    memset((void*)(music_player_buff_rx8 + bytesleft + br), 0,  MP3_FILE_READ_BUFF_SZ - br);
                }
                readptr = (unsigned char*)music_player_buff_rx8;
                bytesleft += br;
            }
        }
    }
    ES8388_DMAStop();
    MP3FreeDecoder(decoder);
    return res;
}
