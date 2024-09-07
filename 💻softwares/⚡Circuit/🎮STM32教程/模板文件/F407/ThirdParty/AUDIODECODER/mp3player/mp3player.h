#ifndef __MP3PLAYER_H 
#define __MP3PLAYER_H 

#include "sys.h"
#include "musicplayer.h"
#include "ff.h"

/* file buffer size for mp3 player */
#define MP3_AUDIO_TX_BUFF_SZ     2 * 2304   /** this is typical memory size of mp3 frame (not change it!) */
#define MP3_FILE_READ_BUFF_SZ  5 * 1024   
/// @note: at this frame size, maximum support frame size 1152 (but maximum may be 1706)

#define MP3_TITSIZE_MAX       50
#define MP3_ARTSIZE_MAX       128

#define AUDIO_MIN(a,b)    ((a) < (b) ? (a) : (b))

extern volatile uint8_t*   music_player_buff0;
extern volatile uint8_t*   music_player_buff1;
extern volatile uint8_t*   music_player_buff_rx8;
extern volatile uint16_t*  music_player_buff_rx16;

extern volatile uint32_t music_player_buff_size;
extern volatile uint8_t music_name_playing[];

extern __player_ctrl playerctrl;   /** defined in musicplayer.c */


//ID3V1 TAG
typedef __packed struct 
{
    uint8_t id[3];		   	//ID, TAG three letters
    uint8_t title[30];		//The name of the song
    uint8_t artist[30];		//Artist's name
	uint8_t year[4];			//age
	uint8_t comment[30];		//remark
	uint8_t genre;			//genre
}ID3V1_Tag;

//ID3V2 tag header
typedef __packed struct 
{
    uint8_t id[3];		   	//ID
    uint8_t mversion;		//Major version number
    uint8_t sversion;		//The subversion number
    uint8_t flags;			//Label header logo
    uint8_t size[4];			//Tag size (excluding 10 bytes of tag header), so tag size = size+10.
}ID3V2_TagHead;

//ID3V2.3 HEADER
typedef __packed struct 
{
    uint8_t id[4];		   	//Frame ID
    uint8_t size[4];	    //Frame size
    uint16_t flags;			//Frame flags
}ID3V23_FrameHead;

//MP3 Xing frame information (not all listed, only useful parts listed)
typedef __packed struct 
{
    uint8_t id[4];		   	//Frame Ed, for the line / Infu
    uint8_t flags[4];		//Storage signs
    uint8_t frames[4];		//Total number of frames
	uint8_t fsize[4];		//Total file size (excluding ID3)
}MP3_FrameXing;
 
//MP3 VBRI frame information (not all listed, only useful parts listed)
typedef __packed struct 
{
    uint8_t id[4];		   	//Frame Ed, for the line / Infu
	uint8_t version[2];		//Version number
	uint8_t delay[2];		//Delay
	uint8_t quality[2];		//Audio quality, 0~100, the bigger the quality, the better
	uint8_t fsize[4];		//The total file size
	uint8_t frames[4];		//The total number of frames in the file
}MP3_FrameVBRI;

//MP3 control structure
typedef __packed struct 
{
    uint8_t title[MP3_TITSIZE_MAX];	//The name of the song
    uint8_t artist[MP3_ARTSIZE_MAX];	//Artist's name
    uint32_t totsec ;				//The duration of the whole song, unit: seconds
    uint32_t cursec ;				//The current playback duration
	
    uint32_t bitrate;	   			//bit rate
	uint32_t samplerate;				//Sample rate
	uint16_t outsamples;				//The amount of PCM output data (in 16-bit increments), mono MP3, is equal to the actual output*2 (convenient for DAC output)
	uint8_t  nChannels;
	uint32_t datastart;				//The position at which the data frame starts (offset in the file)
}__mp3ctrl;


uint8_t mp3_player_init(uint8_t* fpath);
uint8_t mp3_player_play(void);



#endif // !