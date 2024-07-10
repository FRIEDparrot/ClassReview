### 1. 文件属性类型判定
对于 Fatfs 文件系统，可以通过如下的方法来进行遍历获取文件的属性信息(同理在**进行具有多属性具有真假两值的编程时, 也可以采用类似独热编码的思想**):
其中首先通过 f_opendir 获取 dir 信息, 然后循环 f_readdir 进行遍历目录。 

f_readdir **获取到的是 FILINFO 对象**, 而 **FILINFO 对象中, 具有如下多个属性:**   
```cpp
typedef struct {
	FSIZE_t	fsize;			/* File size */
	WORD	fdate;			/* Modified date */
	WORD	ftime;			/* Modified time */
	BYTE	fattrib;		/* File attribute */
#if FF_USE_LFN
	TCHAR	altname[FF_SFN_BUF + 1];/* Alternative file name */
	TCHAR	fname[FF_LFN_BUF + 1];	/* Primary file name */
#else
	TCHAR	fname[12 + 1];	/* File name */
#endif
} FILINFO;
```

```cpp
// read the data in the list 
SDCard_Err SDCard_ListPath(const char* path, Filelist *list, bool list_hidden){
    if (list!= NULL) {
        List_delete(list);  list = NULL;
    }
    DIR dir;
    if (f_opendir(&dir , path)!= FR_OK) return DirOpenERR;
    // if (fileinfo.fname[0] == '.') continue;     // neglect the upper dirctory.
    
    while (1){
        FILINFO fno;
        FRESULT res = f_readdir(&dir, &fno);
        if (res != FR_OK || fno.fname[0] == 0) break;
        if (fno.fattrib & AM_DIR){
            OLED_ShowString(1,1, fno.fname);
            OLED_ShowString(2,1, "Directory");
        }else{
            OLED_ShowString(1,1, fno.fname);
            OLED_ShowString(2,1, "File");
        }
        if (fno.fattrib & AM_HID) OLED_ShowString(3,1,"Hidden");
        Delay_ms(1000);
        OLED_ShowString(1,1,"                ");
        OLED_ShowString(2,1,"                ");
        OLED_ShowString(3,1,"                ");
    }
    f_closedir(&dir);
    return SDCard_Err::NOERR;
};
```

在 “System Volume Information” 
必须注意!!!  对于其中，所读取到的 fno.attrib 实际上是一个 BYTE(8位)类型, 因此, 使用 & 运算符能够查看这个 fno.attrib 的属性. 

例如是否为隐藏文件, 可以使用 `(fno.fattrib & AM_HID)` 判断。相应的, 在 ff.h 文件中
```cpp
/*--------------------------------------------------------------*/
/* Flags and Offset Address                                     */
/*--------------------------------------------------------------*/

/* File access mode and open method flags (3rd argument of f_open) */
#define	FA_READ				0x01
#define	FA_WRITE			0x02
#define	FA_OPEN_EXISTING	0x00
#define	FA_CREATE_NEW		0x04
#define	FA_CREATE_ALWAYS	0x08
#define	FA_OPEN_ALWAYS		0x10
#define	FA_OPEN_APPEND		0x30

/* Fast seek controls (2nd argument of f_lseek) */
#define CREATE_LINKMAP	((FSIZE_t)0 - 1)

/* Format options (2nd argument of f_mkfs) */
#define FM_FAT		0x01
#define FM_FAT32	0x02
#define FM_EXFAT	0x04
#define FM_ANY		0x07
#define FM_SFD		0x08

/* Filesystem type (FATFS.fs_type) */
#define FS_FAT12	1
#define FS_FAT16	2
#define FS_FAT32	3
#define FS_EXFAT	4

/* File attribute bits for directory entry (FILINFO.fattrib) */
#define	AM_RDO	0x01	/* Read only */
#define	AM_HID	0x02	/* Hidden */
#define	AM_SYS	0x04	/* System */
#define AM_DIR	0x10	/* Directory */
#define AM_ARC	0x20	/* Archive */
```

此外, 还有一个很好的编程思想, 就是直接通过宏定义 `#define` 复用底层函数, 也可以使用:
```cpp
/* Some API fucntions are implemented as macro */
#define f_eof(fp) ((int)((fp)->fptr == (fp)->obj.objsize))
#define f_error(fp) ((fp)->err)
#define f_tell(fp) ((fp)->fptr)
#define f_size(fp) ((fp)->obj.objsize)
#define f_rewind(fp) f_lseek((fp), 0)
#define f_rewinddir(dp) f_readdir((dp), 0)
#define f_rmdir(path) f_unlink(path)
#define f_unmount(path) f_mount(0, path, 0)
```

## FATFS 基本结构
首先, FATFS 以 FATFS 对象作为 workspace (工作区), 每次加载和卸载一个盘符均需要一个工作区对象:
```cpp
FATFS  fs0, fs1;
f_mount(&fs0,"0:", 0);
f_mount(&fs1,"0:", 0);
```


在官方给的示例程序中, 对于复制粘贴，在writefiles 时, 必须指定相应的文件大小, 即当copy时, 必须传入对应的 read_size, 例如 `f_write(&fp2, BUF, read_size, &write_size);`  第二是判断对应的错误条件，写入错误通过判断 if (write_size < read_size) 来进行判断。

注意: 如果一次性写入太多字节， 会导致 FR_INVALID错误, 因此, 必须采用512字节的(官方给了一个4096字节的程序, 但是那个太大了，会导致在 f_write时产生 INVALID_OBJECT 错误),因此使用512字节的缓冲区: 

实际上, SD卡复制一首歌是比较慢的, 为了能够实际有一定时间, 我们可以使用进度条标记复制进度。
另外, 我们也可以使用 f_eof 来获取是否达到文件结尾;
由于一般移动和剪切只需要使用复制和删除原文件进行，但是进度条不同。对于一般的文件复制，可以使用下面的代码: 
```cpp
/// @brief Copy file from  the src_dir to dst_dir 
/// @param force_cp    if set to false, if dst_dir exists, the function will return err, true to replace the dst file.
/// @param process_func it receive the process proportion (from 0 to 100) 
/// @return 
SDCard_Err SDCard_CopyFile(const char* src_dir, const char* dst_dir, bool force_cp, void (*process_func)(uint8_t prop)){
    FIL fp1, fp2;
    char BUF[512];  // give 512byte space for buffer for faster copy and move; -> 
    FRESULT res;  FILINFO fno;
    UINT read_size, write_size;
    
    res = f_open(&fp1, src_dir, FA_READ); if (res != FR_OK) return DirOpenERR;
    res = f_stat(src_dir, &fno); if (res != FR_OK || (fno & AM_HID)) return FileOpenERR;  // can't copy hidden files 
    
    if (force_cp){
        res = f_open(&fp2 , dst_dir, FA_WRITE|FA_CREATE_ALWAYS);
    }else{
        res = f_open(&fp2 , dst_dir, FA_CREATE_NEW|FA_WRITE);
    }
    if (res != FR_OK) return DirOpenERR;
    
    uint32_t space_tot = f_size(&fp1)/1024 * 2;   // the total space in sector (1 sector = 512byte )
    uint32_t space_cp  = 0;                   // the sector that copied
    
    while (1){
        res = f_read(&fp1, BUF, sizeof(BUF), &read_size);
        if (res!=FR_OK) return FileReadERR;                // if the SD card plug out, it returns. 
        if (read_size == 0) break;                         // eof error or break interval;
        res = f_write(&fp2, BUF, read_size, &write_size);
        if (res!=FR_OK) return FileWriteERR;  // the destination can't be written.
        
        space_cp += 1;    // 512 byte copied;
        if (process_func!=NULL) process_func((int)((float)space_cp/space_tot * 100));
        if (write_size < read_size) break;    //  disk full error
    }
    f_close(&fp1); f_close(&fp2);
    return NOERR;
}

void Show_prop(uint8_t prop){
    OLED_ShowNum(3,1, prop, 3);
    OLED_ShowString(3,4, "%");
}
```

实际上, 我们更希望复制时将整个文件夹复制过去, 
获取文件或者路径属性方法: `f_stat();` 






