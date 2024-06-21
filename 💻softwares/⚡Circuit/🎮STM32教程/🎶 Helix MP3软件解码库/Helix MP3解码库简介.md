除了使用软解码, 还可以使用WM98xx模块进行硬解码, 但是较为复杂繁琐。

## 一、MP3基本文件结构认识 
首先, 在源文件下, 添加 helix 以及 pub ,real 两个文件夹下的所有.c源文件; 以及 arm 下的所有.s 文件部分; 对于路径添加， 只需 helix 以及 pub 和 real 即可.
![[attachments/Pasted image 20240610215235.png]]

```cpp
/* 12-bit syncword if MPEG 1,2 only are supported 
 * #define	SYNCWORDH		0xff
 * #define	SYNCWORDL		0xf0
 */

typedef struct _MP3DecInfo {
	/* pointers to platform-specific data structures */
	void *FrameHeaderPS;
	void *SideInfoPS;
	void *ScaleFactorInfoPS;
	void *HuffmanInfoPS;
	void *DequantInfoPS;
	void *IMDCTInfoPS;
	void *SubbandInfoPS;

	/* buffer which must be large enough to hold largest possible main_data section */
	unsigned char mainBuf[MAINBUF_SIZE];

	/* special info for "free" bitrate files */
	int freeBitrateFlag;
	int freeBitrateSlots;

	/* user-accessible info */
	int bitrate;
	int nChans;
	int samprate;
	int nGrans;				/* granules per frame */
	int nGranSamps;			/* samples per granule */
	int nSlots;
	int layer;
	MPEGVersion version;

	int mainDataBegin;
	int mainDataBytes;

	int part23Length[MAX_NGRAN][MAX_NCHAN];

} MP3DecInfo;
```
