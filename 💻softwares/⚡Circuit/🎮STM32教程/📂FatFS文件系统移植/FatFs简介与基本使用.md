FATFS(File Allocation Table File System)是轻量级的文件系统。
FatFS移植教程参考[[💻softwares/⚡Circuit/🎮STM32教程/五、扩展模块/SD卡存储和显示图片和音乐#(2) FATFS 文件系统移植|FATFS 文件系统移植]] , 部分资料参考 https://www.yuque.com/lishutong-docs/kb097t
另外, 提供了适用于微小型控制器的 Petit FATFs 模块

## 一、文件系统的概念
首先, 对于简单的数据可以直接储存在MCU内部的Flash区域，而大量数据往往通过外挂SD卡进行存储数据。
包括存储数据， 格式类型等等; 而大容量一般使用结构化的方式存储数据。

由于文件的修改等等可能导致文件大小的改变, 对存储设备的存储空间区域必须采用与顺序存储不同的方式进行存储。
对于空闲的数据块, 管理 SD 卡需要对相应的部分进行增删改查。

常见的文件系统包括: 
FAT12/FAT16/FAT32, 兼容小中容量存储截止
NTFS: 微软文件系统
YAFFS(Yet Another Flash File System)
ROMFS：用于嵌入固件, 不支持写入
JFFS2
EXT2/EXT3/EXT4: 主要是Linux操作系统使用的

## 二、FAT文件系统的实现策略
<b><mark style="background: transparent; color: blue">数据结构为BPB结构</mark></b>,并存储在 boot sector 中。具体可以参考FAT手册p9

#### 1. 存储分区结构
具体而言, 将整个存储区的分区
![[Excalidraw/FatFs简介与基本使用 2024-04-29 08.58.12|500]]
在保留区中,FAT文件系统将sector合并为cluster, 其中cluster可能是一个或者多个块, <mark style="background: transparent; color: red">如果没有文件系统, 则创建系统进行格式化时, 必须指定簇的大小</mark>，而FAT文件的访问是以cluster为单位的。

#### 2. 文件存储方式
FAT文件系统是以簇链的方式存储文件的, 即以链表存储文件系统。 <mark style="background: transparent; color: red">即对于一个文件可以占据多个簇, 并在簇的末尾给定下一个簇的地址信息</mark>。链接关系是放在FAT表中的。
![[attachments/Pasted image 20240429092832.png]]
#### 3. 目录存储办法
目录仅保存多个directory条目集合, 目录是从顶层目录中获取的, 具体可以参考
![[attachments/Pasted image 20240429093129.png]]

参考手册p23，文件系统的结构包括文件名, 文件地址, 文件属性, 创建时间和修改时间, 
![[attachments/Pasted image 20240429093500.png]]
包括FAT12, FAT16,FAT32, exFAT, 其中FAT32提供了最大2TB的支持。
同时支持 GPT(global Partition Table)
![[attachments/Pasted image 20240429094420.png]]
参考 [Application Notes](http://elm-chan.org/fsw/ff/doc/appnote.html )， 可以获取不同平台下的
![[attachments/Pasted image 20240429095758.png]]
针对FATFs, 裁剪代码空间的部分是利用conf中的定义来裁剪的,具体参考下表:
![[attachments/Pasted image 20240429095956.png|800]]
而FAT12, 16到32部分的存储分配如图所示:
![[attachments/fat_map.png|800]]
FATFs的基本接口过程如图所示:
![[attachments/Pasted image 20240429095640.png]]
## 三、函数的使用
#### 1. 创建文件
在Windows下, 搜索(create and format disk partition)disk Management可以使用自带的磁盘分盘工具。
![[attachments/Pasted image 20240429102929.png]]
使用Action > Attach 选中需要的vhd文件, 这个操作相当于挂载(f_mount)操作, 即可实现挂载操作

挂载文件系统: f_mount , 卸载使用 f_unmount

针对路径, 必须使用diskio.c 中 `#define` 对应的驱动器数字符号, 即可实现访问和挂载
检查文件系统是否存在是传入0, 1部分


对于打开文件, 可以采用宏的方式传递, 也可以采用如下方法:
![[attachments/Pasted image 20240429114345.png]]


在创建文件时, 必须调用f_open和f_close函数部分, 如果不调用f_close, 则创建失败

> [!caution] 文件格式指定
> 在传入fopen最后一个参数时, 必须是上面宏定义的合并形式。

```cpp
// 打开文件
FIL file;
res = f_open(&file, "1:/b.txt", FA_CREATE_NEW);
if (res != FR_OK) {
	printf("open file error.\n");
	return -1;
}
f_close(&file);
```

### 2. 文件的读写操作
#### 1. 简单读取和写入
对于读取文件 每一次读取字节时, 应当将读取的字节量放入一个数组中
```cpp 
FRESULT f_read(){}
FRESULT f_write(){};
```

在read时, 传入的地址索引指明了文件的字符串部分。而每一次通过文件的首地址加上sector * count 的offset 字节量, 在FIL中, fptr 为文件当前的读写指针部分; 
```cpp
typedef struct {
	FFOBJID	obj;			/* Object identifier (must be the 1st member to detect invalid object pointer) */
	BYTE	flag;			/* File status flags */
	BYTE	err;			/* Abort flag (error code) */
	FSIZE_t	fptr;			/* File read/write pointer (Zeroed on file open) */
	DWORD	clust;			/* Current cluster of fpter (invalid when fptr is 0) */
	LBA_t	sect;			/* Sector number appearing in buf[] (0:invalid) */
#if !FF_FS_READONLY
	LBA_t	dir_sect;		/* Sector number containing the directory entry (not used at exFAT) */
	BYTE*	dir_ptr;		/* Pointer to the directory entry in the win[] (not used at exFAT) */
#endif
#if FF_USE_FASTSEEK
	DWORD*	cltbl;			/* Pointer to the cluster link map table (nulled on open, set by application) */
#endif
#if !FF_FS_TINY
	BYTE	buf[FF_MAX_SS];	/* File private data read/write window */
#endif
} FIL;
```

为了将文件的指针最终定位到文件的末尾, 每一次需要读取一定字节数量的部分, 而显然是不能够通过res值获取是否读取到文件结尾的。因此为了<b><mark style="background: transparent; color: blue">判断是否结束, 需要在循环中传入并判断一个read_size的数值</mark></b> 

> [!caution] 读取字节的设置问题
> f_read函数第二个参数是buff地址, 第三个参数是读取的字节量, 第四个只需要传入一个新地址, 而传回总共获得的数据量;
> 在f_read过程中, 传入的 btr 参数必须是 `sizeof(read_buf)-1` , 否则会导致出现读取错误。

示例代码如下:
```cpp fold title:简单读写的代码
/* Write CARDINFO.txt for test */
SDCard_Err SDCard_WriteFile(void){
    // try create a file in SD card;
    FIL file;  FRESULT res;
    UINT write_size;
    res = f_open(&file, "1:/cardinfo.txt", FA_CREATE_ALWAYS|FA_WRITE);
    if ((res&FR_DENIED)== FR_DENIED){
        return SDCard_Err::SDCardErr_StorgeFullERR;
    }else if(res != FR_OK){
        return SDCard_Err::SDCardErr_FileCreateERR;
    }
    static uint8_t write_buf[512] = "hello,wreg!";
    res = f_write(&file, write_buf, sizeof(write_buf), &write_size);
    if (res != FR_OK){
        return SDCard_Err::SDCardErr_FileWriteERR;
    }
    res = f_close(&file);
    if (res != FR_OK) return SDCard_Err::SDCardErr_FileCloseERR;
    return SDCard_Err::SDCardErr_NOERR;
}

// Read the file 
SDCard_Err SDCard_ReadFile(void){
    FIL file; FRESULT res;
    UINT read_size;
    static uint8_t read_buf[512];

    res = f_open(&file, "1:/cardinfo.txt", FA_READ);
    if (res!= FR_OK) return SDCard_Err::SDCardErr_FileOpenErr;

    while (1){
        // **** important -> the btr parameter must be sizeof (read_buf) - 1, or first bit will error****
        res = f_read(&file, read_buf, sizeof(read_buf)-1, &read_size); //
        if (res!=FR_OK) return SDCard_Err::SDCardErr_FileReadERR;
        if (read_size < sizeof(read_buf)) {  // the file read ended
            break;
        }
    }
    read_buf[read_size] = '\0';  // write a stop sign to the end;
    f_close(&file);

    for (int i = 0; i<11; i++){
        OLED_ShowChar(4,i+2, read_buf[i]);
    }
    return SDCard_Err::SDCardErr_NOERR;
}
```

> [!caution] sizeof 的获取对象
> 对于字符串数组 `char write_buf[100]` 使用 sizeof 可以返回整个数组的大小, 但是对于指针类型 `char* write_buf`,sizeof () 返回的仅是指针的大小(4), 因此仅写入了4个字符时, 往往是由于指针取sizeof导致的错误; 
> 而strlen对于指针则取到的是一直到\0的长度, 有时也可用 `strlen(s) * sizeof(char)`

#### 2. 文件的写入刷新(sync部分)
对于文件的写入, 往往在写入过程中，会遇到掉电或者程序崩溃的问题。由于断电会导致内容丢失, 而文件不会关闭等。因此我们我们需要考虑文件的写入过程中的崩溃问题， 因此

对于程序崩溃等, 可以向一个非法地址写入数据等等。
此时, 如果检查获得的文件, 实际上获得的部分是0， 即没有写入任何的数据， 全部丢失。 

这个原因是由于 FATFS 文件系统有一个缓存, 而文件本身的信息也是在FATFS中写入的, 虽然写入了数据, 但会**导致没有文件大小等等信息**， 文件名等信息, 因此会识别不到对应的文件夹。 

对于需要边写入边保存的部分, 则使用如下的函数: 
```cpp
for (int i = 0; i < 100000; i++) {
static char write_buf[100];
int len = sprintf(write_buf, "%d: %s\n", i, "123456789");

UINT write_size;
res = f_write(&file, write_buf, len, &write_size);
f_sync(&file);

if (res != FR_OK) {
	printf("\n ------ write error ---------\n");
	break;
}
// 触发系统崩溃，如果不加f_sync()，文件内容会完全看不到，文件元数据也进行了缓存
if (i == 50000) {
	
	*(unsigned char*)0 = 0x1234;
}
```

调用 sync 的是实质是在fs中有一个窗口win作为缓存对象, sync即将fs对象中的win窗口部分写入文件。

最为常用的方法是直接在 f_write 之后直接加上f_sync
另外需要说明的是, 尽可能不要频繁调用 f_sync 函数。如可以写到一定范围然后再调用f_sync部分。(对应C语言中的fflush函数)

#### 3. 以字符串方式读取和写入文件
对于中文和英文字符串, 
方法1:使用f_gets函数, 其中类型是TCHAR, **这是一种针对不同字符串编码的不同类型**; 首先需要修改ffconf.h中的如下定义(这个定义是用来使能字符串函数): 包括`f_gets(), f_putc(), f_puts(), f_printf().` 

| #define FF_USE_STRFUNC | 1   | 0: Disable. FF_PRINT_LLI, FF_PRINT_FLOAT and FF_STRF_ENCODE have no effect.<br>1: Enable without LF-CRLF conversion.<br>2: Enable with LF-CRLF conversion. |
| ---------------------- | --- | ---------------------------------------------------------------------------------------------------------------------------------------------------------- |
| #define FF_PRINT_LLI   | 1   |                                                                                                                                                            |
| #define FF_PRINT_FLOAT | 1   |                                                                                                                                                            |
| #define FF_STRF_ENCODE | 3   |                                                                                                                                                            |

```cpp 
static TCHAR read_buffer[100];
f_gets(read_buf, )
```

我们以f_gets为例
```cpp 
TCHAR* f_gets (
	TCHAR* buff,	/* Pointer to the buffer to store read string */
	int len,		/* Size of string buffer (items) */
	FIL* fp			/* Pointer to the file object */
)
```

```cpp fold title:OLED读取文件并且刷新显示字符串的代码
/********** f_gets, f_printf *******************/
SDCard_Err SDCard_ReadChar(){
    FIL file; FRESULT res;
    res = f_open(&file, "1:/cardinfo.txt", FA_READ);
    if (res != FR_OK) return SDCard_Err::SDCardErr_FileOpenErr;
    static TCHAR char_buf[16];
    // cycle read the 
    while (1) {
        TCHAR *str = f_gets(char_buf,16, &file);
        if (str== NULL){
            break;
        }
        OLED_ShowString(4,1,str);
        int i =0;
        for ( ;i < 16; i++){
            if (str[i] == '\0') break;
        }
        while (i < 16){
            OLED_ShowChar(4,i+1,' ');
            i++;
        }
        Delay_s(1);
    }
    res = f_close(&file);
    if (res!= FR_OK) return SDCard_Err::SDCardErr_FileCloseERR;
    return SDCard_Err::SDCardErr_NOERR;
}
```

另外需要说明, f_read一次是读取一部分。
f_gets函数, 是一次读取一行然后输出的。如果这行没有读完, 则继续读取到这一行末尾(到末尾会停止读取);
在文件中, 每一行的部分的末尾是 `\r\n` 表示回车。(Linux是`\n`, Windows是`\r\n`部分)而**具体可以通过 FF_USE_STRFU配置**。其中使用2时, 则`\r\n`结尾, 即将`\r\n`转换成`\n`进行返回。一般情况下使用1即可;

对于 f_puts 和 f_putc 函数,可以对文件进行打开和以追加的方式进行写入。另外需要主动加入换行符。
```cpp 
f_puts("rtos is running\n", &file);
f_putc('o', &file);
f_putc('k', &file);
```

另外, 使能转换时, 会在fputs中将\n转换为\r\n 

#### 4. 读写定位函数
对应于 C 语言库中的fseek函数，f_lseek() 函数，只有`FIL*fp` 和 ofs (偏移)变量， **用于偏移文件的定位指针**。 此时， 再次读写file对应的位置即为偏移位置

f_rewind 是将指针读写位置重新移动到文件的开头的函数。
```cpp fold title:lseek和rewind基本用法
// 读取显示文本文件内容
static char read_buf[100];
UINT read_size;

// 从偏移32开始读取
f_lseek(&file, 32);
f_read(&file, read_buf, sizeof(read_buf), &read_size);

// 回退一部分距离，再进行读取
f_lseek(&file, 100);
f_read(&file, read_buf, sizeof(read_buf), &read_size);

// 回退到开头
f_rewind(&file);
f_read(&file, read_buf, sizeof(read_buf), &read_size);

// 在文件关闭时，有文件写入刷新的调用
f_close(&file);
```

- 利用f_lseek实现<mark style="background: transparent; color: red">文件的扩容</mark>。
需要说明的是, 当f_lseek读写超出已知的文件大小时, 但是指针位置移动仍然能够进行。 f_seek会自动扩容。 
例如只有100kb的文件, 可以直接f_lseek扩充到200bit,并进行写入操作。
```cpp
// 从偏移32开始写，前面32字节是什么不确定
f_lseek(&file, 32);
f_write(&file, buf, sizeof(buf), &result_size);

// 继续写，再进行读取
f_lseek(&file, 1000);
f_write(&file, buf, sizeof(buf), &result_size);
```

另外, 下列函数也是常用函数, 用于获取文件信息以及当前的文件大小等等, 判断是否读取到文件末尾等等;
```cpp
FSIZE f_tell(*FILE);    // 用于获取文件指针当前的读写位置(返回相对于文件开头的字节偏移量)
FSIZE f_size(*FILE);  // 返回当前的文件大小(单位Byte)
int f_eof (*FILE);        // 判断当前指针是否达到文件的尾部
f_truncate(*FILE);              // 大文件截断， 即在当前位置进行截断(用于检查是否); 例如截断到前1000kb,则联合调用f_lseek和f_truncate
```

> [!NOTE] 判断读写完毕
> 判断是否读取完毕的一种方法是使用一个while循环并使用f_read函数直到读取尺寸不匹配, 但也可以在while中判断 while(!feof(&file)); 这个对于f_gets() 无法传入&size 的部分

#### 6. 文件的连续空间分配
由于实际的FATFS文件系统的存储空间分配实际上是不连续的, 而实际上<mark style="background: transparent; color: red">使用FAT表存储簇之间的关系</mark>。
由于不连续的读写效率可能不是特别高, 因而使用FAT表存储簇之间的关系<b><mark style="background: transparent; color: blue">FATFS也提供了强制连续存储空间分配的方案</mark></b>。此时直接连续读取簇即可实现文件读写。 

> [!NOTE] 说明
> 这个功能主要是用于实现SD卡连续读多个扇区的命令的。从而可以批量读取文件的数据。

需要说明的是，**此种规范并不是FATFs文件系统本身支持的**。但是提供了对应接口
```cpp
f_expand(FILE, FSIZE_t, opt);   // 其中fsz为扩展后的文件大小, opt为分配模式。

#define FILE_SIZE (100 * 1024)
// 示例代码
res = f_expand(&file, FILE_SIZE, 1);   // The f_expand function prepares or allocates a contiguous data area to the file
// 省去判断代码
static  char buf[FILE_SIZE];
// 如果f_write, 仍然是在for循环中查找指针, 而我们只需直接调用 disk_write , 这样就不用查找指针了。 

// 以下是官网提供的访问连续存储空间的代码
/* Accessing the contiguous file via low-level disk functions */
/* Get physical location of the file data */
drv = fp->obj.fs->pdrv;
lba = fp->obj.fs->database + fp->obj.fs->csize * (fp->obj.sclust - 2);
/* Write 2048 sectors from top of the file at a time */
res = _disk_write_(drv, buffer, lba, 2048);
```


> [!caution] 注意事项
> 1. 首先,expand初始的文件大小必须为0(否则可能由于无法分配出问题);
> 2. lba为起始扇区号, 而这个扇区号必须大于数据区的最小标号。(使用file.obj.fs->database + file.obj.sclust - 2)* fp->obj.fs->csize其中 database 为起始的扇区号, 而file.obj.sclust为起始的簇号, 而数据区的起始簇号为2; 因此减去2之后乘每个簇的扇区数。 
> 3. 如果重新调用f_write，可能会导致不连续的情况, 因此谨慎使用f_write部分;

#### 7. 在读取同时转发数据的方法
往往我们需要将文件中的数据读取出并转发其他设备, 包括串口和网络等等。
其中最简单的方法是先读取到RAM中并发送, 但是由于RAM比较小, 可以选择直接将数据读取出并发送给缓存区。而<mark style="background: transparent; color: red">如果对性能有特别要求， 则可以考虑直接发送给其他设备</mark>。

首先修改 ffconf.h 中的如下定义来使能转发功能:

| #define FF_USE_FORWARD                                                   | 1   |
| ------------------------------------------------------------------------ | --- |
| /* This option switches f_forward() function. (0:Disable or 1:Enable) */ |     |

对于f_read 和 f_write 部分是使用buff作为缓存区的, f_foward 提供了直接转发数据的功能。因而可以不使用f_read函数, 直接调f_foward来进行转发给对应的部分; 
```cpp 
FRESULT f_forward (
	FIL* fp, 						/* Pointer to the file object */
	UINT (*func)(const BYTE*,UINT),	/* Pointer to the streaming function */
	UINT btf,						/* Number of bytes to forward */
	UINT* bf						/* Pointer to number of bytes forwarded */
)
```
对于f_foward函数, 首先<mark style="background: transparent; color: red">会先调用传入的 func 函数指针, 用于判断传入设备是否已经就绪</mark>, 当可以发送数据时, 即将数据存入缓存中然后再调用一次函数。这样就可以在 func 内部进行读取数据和转发。根据官网给的示例, 具体定义方法如下: 传入的 btf 代表要发送的数据量，而btf=0时检查是否能够发送。

如果不能够发送, 则f_foward函数将立刻终止,
```cpp 
/*------------------------------------------------------------------------*/
/* Sample code of data transfer function to be called back from f_forward */
/*------------------------------------------------------------------------*/
UINT out_stream (   /* Returns number of bytes sent or stream status */
    const BYTE *p,  /* Pointer to the data block to be sent */
    UINT btf        /* >0: Transfer call (Number of bytes to be sent). 0: Sense call */
)
{
    UINT cnt = 0;
    if (btf == 0) {     /* Sense call */
        /* Return stream status (0: Busy, 1: Ready) */
        /* When once it returned ready to sense call, it must accept a byte at least */
        /* at subsequent transfer call, or f_forward will fail with FR_INT_ERR. */
        if (FIFO_READY) cnt = 1;
    }
    else {              /* Transfer call */
        do {    /* Repeat while there is any data to be sent and the stream is ready */
            FIFO_PORT = *p++;
            cnt++;
        } while (cnt < btf && FIFO_READY);
    }

    return cnt;
}

// 实际函数的调用办法:
FRESULT rc; FIL fil; UINT dmy; 
/* Open the audio file in read only mode */
rc = f_open(&fil, fn, FA_READ);
if (rc) return rc;

/* Repeat until the file pointer reaches end of the file */
while (rc == FR_OK && !f_eof(&fil)) {

	/* some processes... */
	 
	/* Fill output stream periodicaly or on-demand */
	rc = f_forward(&fil, out_stream, 1000, &dmy);
}
```

首先读取文件, 如果仅仅调用一次f_foward, 需要保证传入的btf足够大来保证发完文件, f_foward对应的部分即可。在官网中的f_foward是用于播放音频文件部分的示例代码。 

需要说明, 每一次读取的字节量不确定。

## 四、目录与文件操作
### (1) 目录与文件系统的查询方法
在FATFS系统中, 可以使用 f_stat 来获取某一个文件的信息，包括文件的目录， 创建日期和文件名等等。

例如, 获取目录下的文件信息: 
```cpp 
f_stat("1:",*FILINFO); // 查询目录下的文件信息
// 其中,  fileinfo结构体如下
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

其中, 需要说明的是， 参考FILEINFO部分, fdate是一个16位数据, 并从1980年起始的。
文件名的长度是可以修改的: The size of fname[] and altname[] each can be configured in LFN configuration. 上述指的是如下的设置: 

| #define FF_USE_LFN   | 0   | 是否支持长文件名                                                               |
| -------------------- | --- | ---------------------------------------------------------------------- |
| #define FF_MAX_LFN   | 255 | 可以支持最长255字节的文件名                                                        |
| #define FF_CODE_PAGE | 437 | 这个是指定译码语言标准,当为936时, 则会占用至少180kb的缓存空间, 因此保证支持中文编码时, 要求至少有256kb的Flash内存。 |
| #define FF_LFN_BUF   |     | 255                                                                    |
| #define FF_SFN_BUF   |     | 12                                                                     |

根据讲解需要修改FF_LEN_UNICODE 和 FF_STRF_ENCODE 配置:
When FF_LFN_UNICODE >= 1 with LFN enabled, string functions convert the character
encoding in it. FF_STRF_ENCODE selects assumption of character encoding ON THE FILE
to be read/written via those functions.

```cpp title:示例代码如下
FRESULT fr;
FILINFO fno;
const char *fname = "file.txt";

printf("Test for \"%s\"...\n", fname);

fr = _f_stat_(fname, &fno);
switch (fr) {

case FR_OK:
	printf("Size: %lu\n", fno.fsize);
	printf("Timestamp: %u-%02u-%02u, %02u:%02u\n",
		   (fno.fdate >> 9) + 1980, fno.fdate >> 5 & 15, fno.fdate & 31,
		   fno.ftime >> 11, fno.ftime >> 5 & 63);
	printf("Attributes: %c%c%c%c%c\n",
		   (fno.fattrib & AM_DIR) ? 'D' : '-',      // 是否为目录
		   (fno.fattrib & AM_RDO) ? 'R' : '-',    // 只读文件
		   (fno.fattrib & AM_HID) ? 'H' : '-',     // 隐藏文件
		   (fno.fattrib & AM_SYS) ? 'S' : '-',     // 系统文件(不可被获取)
		   (fno.fattrib & AM_ARC) ? 'A' : '-');    // 档案文件
	break;

case FR_NO_FILE:
case FR_NO_PATH:
	printf("\"%s\" is not exist.\n", fname);
	break;

default:
	printf("An error occured. (%d)\n", fr);
}
```

在访问某一目录下的文件时， 可以同时得到目录和文件部分, 区别的方法是判断 fno.fattrib & AM_DIR ， 如果为1则是目录, 为0则是文件 (显然目录大小为0)。具体参考如下的表格: 
![[attachments/Pasted image 20240430102240.png]]
文件名的长度为:
```cpp
#if FF_USE_LFN
TCHAR   altname[FF_SFN_BUF + 1];/* Alternative file name */  -> 12 maximum 
TCHAR   fname[FF_LFN_BUF + 1];  /* Primary file name */  -> 256 maximum 
```
### (2) 遍历目录
- [f_opendir](http://elm-chan.org/fsw/ff/doc/opendir.html) - Open a directory
- [f_closedir](http://elm-chan.org/fsw/ff/doc/closedir.html) - Close an open directory
- [f_readdir](http://elm-chan.org/fsw/ff/doc/readdir.html) - Read a directory item

另外, 在正常扫描到dir结尾时, 仍然会返回 FR_OK, 但是 fno.fname[0] == 0 此时为目录项为空字符串。则代表已经读取完毕。
```cpp title:示例代码
for (;;) {
    res = _f_readdir_(&dir, &fno);                   /* Read a directory item */
	if (res != FR_OK || fno.fname[0] == 0) break;  /* Error or end of dir */
	if (fno.fattrib & AM_DIR) {            /* Directory */
		printf("   <DIR>   %s\n", fno.fname);
		ndir++;
	} else {                               /* File */
		printf("%10u %s\n", fno.fsize, fno.fname);
		nfile++;
	}
}
```
官网上也给出了递归读取并且列出所有文件的方法，实际上是递归读取即可。

FATFS不<mark style="background: transparent; color: red">支持排序的操作, 而是从目录的列表顺序是实际存储的列表顺序</mark>。 

另外还有如下常用函数(往往用于按照格式查找文件即可):
- [f_findfirst](http://elm-chan.org/fsw/ff/doc/findfirst.html) - Open a directory and read the first item matched -> 0 
- [f_findnext](http://elm-chan.org/fsw/ff/doc/findnext.html) - Read a next item matched
 
### (3) 相对路径访问
由于使用绝对路径构造路径的字符串会导致不方便, 造成空间的浪费。
- [f_chdir](http://elm-chan.org/fsw/ff/doc/chdir.html) - Change current directory  // 改变工作目录
- [f_chdrive](http://elm-chan.org/fsw/ff/doc/chdrive.html) - Change current drive    // 改变工作盘符

例如:f_chdir("0:/rtos"); 此时访问 "0:/rtos/os_sem.c" 只需要使用 f_state("0:os_sem.c");
需要传入驱动器路径;

- [f_unlink](http://elm-chan.org/fsw/ff/doc/unlink.html) - Remove a file or sub-directory         // 删除文件或者目录
- [f_rename](http://elm-chan.org/fsw/ff/doc/rename.html) - Rename/Move a file or sub-directory    // 用于重命名和移动

需要说明的是, 如果删除路径, 则会导致删除失败。这是由于<mark style="background: transparent; color: red">接口不支持删除非空的目录</mark>; 
此时需要遍历底层工作目录并将全部文件删除, 再删除路径即可。定义一个remove_dir函数删除即可。
f_unlink直接使用 file_info.fname 作为参数即可删除对应的文件。 

- [f_mkdir](http://elm-chan.org/fsw/ff/doc/mkdir.html) - Create a sub-directory   // 创建时必须只使用一层的路径设置
如果是多层路径，则需要解析路径并且自动按照层来创建。

- [f_chmod](http://elm-chan.org/fsw/ff/doc/chmod.html) - Change attribute of a file or sub-directory // 修改文件的属性(例如只读)
- [f_utime](http://elm-chan.org/fsw/ff/doc/utime.html) - Change timestamp of a file or sub-directory // 修改时间戳 

```cpp
res = f_rename("os_sem.c", "sys_sem.c");  // 如果不加上驱动器号 -> 使用设置的相对路径的盘(0:/rtos);
// 另外, 除了重命名, 也可以进行移动文件或者目录。
res = f_rename("0:sys_sem.c", "0:/sys_sem.c"); // 注意: 如果有  /  则为绝对路径
res = f_chmod ("0:sys_sem.c", AM_RDO,  AM_RDO | AM_HID);  // 其中第二个是设置的总共参数, 第三个是设置参数(只读且不隐藏, 只在第三个参数上有的而第二个参数没有则不mask)
```


另外 , 如果将 0 设置为相对盘符, 即**有一个变量(CurrVol)用于记录当前缺省的盘符**。
- f_chdrive("1:");
```cpp
f_open("\rtos\\space")；默认为0; 
f_makedir("Helo")
```

## 剩余空间和卷标的获取
```cpp 
FATFS fatfs;  DWORD cnt;
res = f_getfree("0", &cnt, &fatfs)   //  查询某个系统中的空闲的簇的数量;
// 剩余空间 =  cnt * 簇的扇区数(格式化时指定) * sector * 512(b) / 1024    (kb)
bytes = cnt * fatfs-> csize(簇的大小)  * 512;
```

卷标是获取和修改磁盘的名称, 但是一般不使用，会导致文件读取出现问题
```cpp 
setlabel("0:disk aa");  // 修改名称(必须f_mount之后)
```

对于文件, 一般都是保存在父目录下以结构体保存的。都是使用f_write等等的部分都是以大写字符存储的。 

