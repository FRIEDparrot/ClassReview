/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */

/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD  to physical drive 2 */
#define DEV_SD      3   


/*-----------------------------------------------------------------------*/
DSTATUS SD_disk_status(void); // declaration
/// @brief Get Drive Status
/// @param pdrv Physical drive nmuber to identify the drive
/// @return 
DSTATUS disk_status (BYTE pdrv)
{
	DSTATUS stat;
	int result;
	switch (pdrv) {
		case DEV_SD :
			result = SD_disk_status();
			return stat;
	}
	return STA_NOINIT;
}

// define the function here 
DSTATUS SD_disk_status(void){

	return RES_OK;
}

/*-----------------------------------------------------------------------*/
DSTATUS SD_disk_initialize(void);
/// @brief Inidialize a Drive 
/// @param pdrv Physical drive nmuber to identify the drive
/// @return 
DSTATUS disk_initialize (BYTE pdrv)
{
	DSTATUS stat;
	int result;
	switch (pdrv) {
		case DEV_SD:
			result = SD_disk_initialize();
			return stat;
	}
	return STA_NOINIT;
}

DSTATUS SD_disk_initialize(void){
	return RES_OK;
}

/*-----------------------------------------------------------------------*/
DRESULT SD_disk_read(BYTE* buff, LBA_t sectorm, UINT count);
/// @brief Read Sector(s)
/// @param prdv Physical drive nmuber to identify the drive */
/// @param buff Data buffer to store read data */
/// @param sector Start sector in LBA
/* Number of sectors to read */
DRESULT disk_read (BYTE pdrv,BYTE *buff,LBA_t sector,UINT count)
{
	DRESULT res;
	int result;

	switch (pdrv) {
		case DEV_SD :
			// translate the arguments here
			result = SD_disk_read(buff, sector, count);
			return res; 
	}
	return RES_PARERR;
}

DRESULT SD_disk_read(BYTE* buff, LBA_t sectorm, UINT count){
	return RES_OK;
}

/*-----------------------------------------------------------------------*/
DRESULT SD_disk_write(const BYTE* buff, LBA_t sector, UINT count);
#if FF_FS_READONLY == 0
/// @brief Write Sector(s)
/// @param pdrv drive nmuber to identify the drive 
/// @param buff Data to be written 
/// @param sector Start sector in LBA 
/// @param count Number of sectors to write 
DRESULT disk_write (BYTE pdrv,const BYTE *buff,LBA_t sector,UINT count){
	DRESULT res;
	int result;
	switch (pdrv) {
		case DEV_SD: 
			result = SD_disk_write(buff, sector, count);
			return res;
	}
	return RES_PARERR;
}
DRESULT SD_disk_write(const BYTE* buff, LBA_t sector, UINT count){ 
	return RES_OK;
}
#endif
/*-----------------------------------------------------------------------*/
DRESULT SD_disk_ioctl(void);

/// @brief Miscellaneous Functions                                               */
/// @param pdrv Physical drive nmuber (0..) */
/// @param cmd  Control code
/// @param buff Buffer to send/receive control data
DRESULT disk_ioctl (BYTE pdrv,BYTE cmd,void *buff)
{
	DRESULT res;
	int result;

	switch (pdrv) {
		case DEV_SD :
			// Process of the command the USB drive
			result = SD_disk_ioctl();
			return res;
	}
	return RES_PARERR;
}

DRESULT SD_disk_ioctl(void){
	return RES_OK;
}