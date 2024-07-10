/* 
* HardWare > SDCard_Driver > SDCard_Driver_spi.cpp
*/
#include "SDCard_driver_spi.h"
#include "Delay.h"


static FATFS fs;                /* global workspace for FATFS     */
static char fs_buffer[512];  /* global variable for fs buffer */
static uint16_t sdcard_file_counter = 0;

/********************  Static functions prototypes *************************/

static uint8_t _sd_combine_dir(const char* root_dir, const char* dir, char* result_dir);
static uint8_t _sd_get_parent_path(const char* filePath, char* dst_str);
static uint8_t _sd_get_filename(const char* filePath, char* dst_str);

/********************  Static functions Definition *************************/

/// @brief combine the root directory and directory using "/"
/// @param root_dir     if use NULL, will not combine it.
/// @param dir          root directory;
/// @param result_dir   use char* dir = NULL and use it address for it.
/// @warning  dst_str must have enough space (MAX_ABSOLUTE_PATH_LENGTH) to receive data;
/// @return 0: combine succeed;   1: failed;
static uint8_t _sd_combine_dir(const char* root_dir, const char* dir, char* result_dir){
    if (strlen(root_dir) + strlen(dir) + 2 > MAX_ABSOLUTE_PATH_LENGTH) return 1;
    memset(result_dir, 0, strlen(result_dir));
    sprintf(result_dir, "%s/%s",root_dir, dir);
    return 0;
}

/// @brief get the absolute path of the parent path 
/// @param filePath 
/// @param dst_str 
/// @return 0: succeed;   1: failed;
/// @warning  dst_str must have enough space
static uint8_t _sd_get_parent_path(const char* filePath, char* dst_str){
    if (strlen(filePath) > MAX_ABSOLUTE_PATH_LENGTH) return 1;
    strcpy(dst_str, filePath);
    char* lastSlash = strrchr(dst_str, '/');
    if (lastSlash != NULL) {
        (*lastSlash) = '\0';    // set it to end character 
        return 0;
    }
    return 1;  // can't find the parent dir 
}

/// @brief get the file or folder name from the path 
/// @param filePath 
/// @param dst_str 
/// @return 0: succeed;   1: failed;
/// @warning the dst_str must have enough space for filePath or HardFault will happen;
static uint8_t _sd_get_filename(const char* filePath, char* dst_str){
    if (strlen(filePath) > MAX_ABSOLUTE_PATH_LENGTH) return 1;
    const char* lastSlash = strrchr(filePath,'/');
    if (lastSlash !=NULL){
        strcpy(dst_str,lastSlash+1);
        return 0;
    }
    return 1;
}

/// @brief retrieve if file is 
/// @param fno 
/// @note The file/sub-directory must not have read-only attribute (AM_RDO), or the function will be rejected with FR_DENIED.
///       The sub-directory must be empty and must not be current directory, or the function will be rejected with FR_DENIED.
///       The file/sub-directory must not be opened, or the FAT volume can be collapsed. It will be rejected safely when file lock function is enabled.
/// @return 
static uint8_t _sd_file_available_cb(FILINFO fno){
    if ((fno.fattrib & AM_HID) || (fno.fattrib & AM_RDO)) return 1;
    return 0;
}

// 2. The sub-directory must be empty and must not be current director
static uint8_t _sd_count_filenumber_cb(FILINFO fno){
    if (!(fno.fattrib & AM_DIR)) {
        if (fno.fattrib & AM_HID) return 0; 
        sdcard_file_counter++;
    }
    return 0;
}


/// ========================== SDCard public functions  =========================

/// @brief Init The SD card 
/// @return 0: Initialize Succeed
///         1: SD Card Initialize Error
SDCard_Err SDCard_Init(void){
    /* At the first init, send until response is 0xFF to */
    SD_LowLevel_Init(); // Try Init SPI
    SD_CS_LOW();
    for (int t = 0, r = 0x00; t < 0x0FFF && r!= 0xFF; t++){
        r = SD_WriteByte(0xFF);
    }
    SD_CS_HIGH();
    Delay_ms(200);

    FRESULT res;
    for (uint8_t re = 0; re < 2; re++){  // try for 2 times 
        res = f_mount(&fs, SDCARD_ROOTDIRECTORY, 1);   // Mount File System Immediately
        if (res == FR_OK) break;
    }
    /*---------- Mount File System ---------- */
    if (res == FR_OK) return NOERR;
    // for no file system, it will format the card to mount the File System
    else if (res == FR_NO_FILESYSTEM){  // OLED_ShowString(1,1,"no system");
        /* format the SD card*/
        MKFS_PARM opt;
        opt.fmt = FM_FAT32;
        opt.align = 0;      // use disk_ioctl for  sector number 
        opt.au_size = 512;  // cluster size
        opt.n_fat = 1;      // number of fats
        opt.n_root= 0;      // default (512)
        res = f_mkfs(SDCARD_ROOTDIRECTORY,&opt, fs_buffer, 512);   // format the SD card
        if (res == FR_OK){
            Delay_s(5);  // ** wait for formatting(Change later) **
			res = f_mount(&fs,"1:",1);
            if (res != FR_OK){
                return INITERR;
            }
            return NOERR;   // mounted succeed
        }else{
            return INITERR; // mounted error 
        }
    }
    return INITERR;  // Initialize Succeed
}

/// @brief get file system space in kb
/// @param totalspace Total space of file system 
/// @param freespace  Free  space of file system
/// @return 
SDCard_Err SDCard_GetSpace(uint32_t *totalspace, uint32_t *freespace){
    DWORD freeclst;
    FATFS *fs;
    if (f_getfree("1:", &freeclst, &fs)) return NoInfo;

    // for clust is always 512 kb (csize : sector per cluster)
    *totalspace = (fs->n_fatent - 2) * fs->csize /2; // (fs.n_fatent - 2) * 512/1024 
    *freespace  = freeclst * fs->csize /2;

    return NOERR;
}

/// @brief Write a info file for SD card and log its type, serial number, etc. 
/// @param 
/// @return 
SDCard_Err SDCard_WriteInfoFile(void){
    FIL fp; FRESULT res; SDCard_Err result = NOERR;
    uint32_t tot_space, free_space;
    
    SD_CardInfo card_info;
    if (SD_GetCardInfo(&card_info)) return NoInfo;
    if (SDCard_GetSpace(&tot_space, &free_space)) return NoInfo;
    
    char result_dir[MAX_ABSOLUTE_PATH_LENGTH];
    _sd_combine_dir(SDCARD_ROOTDIRECTORY, "cardinfo.txt", result_dir);  // create cardinfo.txt at root directory
    res = f_open(&fp, result_dir, FA_CREATE_ALWAYS|FA_WRITE);
    
    if (res != FR_OK) { f_close(&fp);  return FileOpenERR; }
    // try write the SD Card Multipule times and just check the last res
    char* msg[10];
    msg[0] = "=============== SD card Infomation Logs ===============\r\n";
    
    switch (SD_CARD_TYPE)
    {
    case SD_TYPE_V2:
        msg[1] = "Type : SD Card 2.0\r\n"; break;
    case SD_TYPE_V2HC:
        msg[1] = "Type : SD High Capacity Card 2.0\r\n"; break;
    case SD_TYPE_MMC:
        msg[1] = "Type : MMC Card\r\n"; break;
    }
    msg[2] = (char*)malloc(35);
    sprintf(msg[2], "total sector count: %d\r\n",card_info.SD_csd.SectorCount);
    
    msg[3] = (char*)malloc(35);
    sprintf(msg[3], "card space total: %.2f MB\r\n", tot_space/1024.0);
    
    msg[4] = (char*)malloc(35);
    sprintf(msg[4], "card space free: %.2f MB\r\n", free_space/1024.0);
    
    msg[5] = (char*)malloc(200);
    uint16_t year  = ((card_info.SD_cid.ManufactDate&0xFF0)>> 4) + 2000;
    uint16_t month = ((card_info.SD_cid.ManufactDate)&0xF);
    sprintf(msg[5], "manufacture in :%d\\%d,product serial number: %d\r\n",year, month, (uint32_t)card_info.SD_cid.ProdSN);
    msg[6] = "Enjoy it!\r\n";
    for (int i = 0; i<7; i++){
       if (f_puts(msg[i], &fp)<0) result = FileWriteERR;
    }
    free(msg[2]); free(msg[3]); free(msg[4]); free(msg[5]);  // free the space after construct
    f_close(&fp);
    return result;
}

/// @brief Create File in the specified directory
/// @param path_to_create  create at root use "root_directory", or use "current_directory"
/// @param dir      directory to be created (abs shouldn't exceed MAX_ABSOLUTE_PATH_LENGTH characters)
/// @param override if set to false return error when file exists.
/// @return 
SDCard_Err SDCard_CreateFile(const char* path, bool override){
    FIL file; FRESULT res;
    SDCard_Err result = NOERR;
    if (!override){
        res = f_open(&file, path, FA_CREATE_NEW);
    }
    else{
        res = f_open(&file, path, FA_CREATE_ALWAYS);
    }
    if (res!=FR_OK) result = FileCreateERR;
    f_close(&file);
    return result;
}

/// @brief write to specified file (if file not exists, it would create).
/// @param path_to_create create at root use "root_directory", or use "current_directory"
/// @param dir       directory of the file 
/// @param write_buf the string to write to the file 
/// @param append    if set to false, open mode "w", true -> "a"
/// @note  this function can only write file < 512bytes, but can switch to append mode;
/// @return 
SDCard_Err SDCard_WriteFile(const char* path, const char* write_buf, bool append){
    // try create a file in SD card;
    FIL file;  FRESULT res; SDCard_Err result = NOERR;
    UINT write_size;                              // note: this just receives the number of bytes written 
    BYTE write_mode = FA_CREATE_ALWAYS|FA_WRITE;       // "w" for not append, "a" for append
    if (append) write_mode = FA_OPEN_APPEND|FA_WRITE; 
    
    res = f_open(&file, path, write_mode);  // must use result dir for it. 
    if ((res&FR_DENIED) == FR_DENIED) return StorgeFull;
    else if(res != FR_OK) return FileCreateERR;
    
    // write the string (note: use strlen() not use sizeof())
    res = f_write(&file, write_buf, strlen(write_buf), &write_size); if (res != FR_OK) result = FileWriteERR;
    f_close(&file);
    return result; 
}

/// @brief Create Directory on SD card Disk, if it exits, it not create it. 
/// @param path_to_create if create at root use "root_directory", or use "current directory"
/// @param dir folder directory (depend on "path_to_create" specified) 
/// @return 
SDCard_Err SDCard_CreateFolder(const char* path){
    FRESULT res;
    DIR dir;

    res = f_opendir(&dir, path);
    if (res!=FR_OK){
        res = f_mkdir(path);  // try to create 
    }
    f_closedir(&dir);  // try close dir;
    if (res!=FR_OK) return  FileCreateERR;
    return NOERR;
}

/// @brief Delete the file or folder (use retrieve option to delete all in the folder)
/// @param path 
/// @param retrieve  if set to false, it would failed if the folder is not empty.
/// @note  all files under the folder  must be deleteable (not Hidden and not read-only);
/// @return 
SDCard_Err SDCard_DeleteFolder(const char* path, bool retrieve){
    SDCard_Err result = NOERR;
    FRESULT res;
    FILINFO fno;
    
    if (f_stat(path, &fno) || (fno.fattrib & AM_HID) || (fno.fattrib & AM_RDO)) return FileInfoERR;
    if(!retrieve) {
        res = f_unlink(path);
        if (res!= FR_OK) result = DirDeleteERR;
    }else{
        if (fno.fattrib & AM_DIR){
            DIR dir;
            if (f_opendir(&dir,path)!=FR_OK) return DirOpenERR;
            while (1){
                char res_dir[MAX_ABSOLUTE_PATH_LENGTH];
                res = f_readdir(&dir, &fno);
                if (res!= FR_OK || fno.fname[0] == 0) break;   // read end, break normally.
                if (_sd_combine_dir(path, fno.fname, res_dir)){ result = DirCreateERR; break;}
                if (SDCard_DeleteFolder(res_dir,true)!= NOERR) { result = DirDeleteERR; break;}
                // every sub-folder and file should be deleteable 
            }
            f_closedir(&dir); 
            res = f_unlink(path);
            if (res!= FR_OK) result = DirDeleteERR;
        }else {
            return SDCard_DeleteFolder(path, false);
        }
    }
    return result;
}

#ifdef __cplusplus
extern "C"{
#endif 

/// @brief read the path, and call info_func_cb with the FILINFO type infomation
/// @param path 
/// @param info_func_cb return 0 is execute succeed;
/// @return 
SDCard_Err SDCard_ListPath(const char* path, uint8_t (*info_func_cb)(FILINFO fno)){
    DIR dir;
    if (f_opendir(&dir , path)!= FR_OK) return DirOpenERR;
    FILINFO fno;
    while (1){
        FRESULT res = f_readdir(&dir, &fno);
        if (res != FR_OK || fno.fname[0] == 0) break;
        if (info_func_cb != NULL && info_func_cb(fno)) { f_closedir(&dir); return FileInfoERR;}
    }
    f_closedir(&dir);
    return NOERR;
}

/// @brief retrive all the folder and file of the "path" specified 
/// @param path 
/// @param info_func_cb 
/// @return 
SDCard_Err SDCard_ListPath_retrieve(const char* path, uint8_t (*info_func_cb)(FILINFO fno)){
    DIR dir;
    if (f_opendir(&dir, path)!=FR_OK) return DirOpenERR;
    FILINFO fno;
    while (1){
        FRESULT res = f_readdir(&dir, &fno);
        if (res!= FR_OK || fno.fname[0] == 0) break;
        if(info_func_cb!=NULL && info_func_cb(fno)) { f_closedir(&dir); return FileInfoERR;}
        if (fno.fattrib & AM_DIR){
            char str[MAX_ABSOLUTE_PATH_LENGTH];
            if (_sd_combine_dir(path, fno.fname, str)) continue;  // when the length is too long 
            SDCard_ListPath_retrieve(str, info_func_cb);
        }
    }
    f_closedir(&dir);
    return NOERR;
}


/// @brief Read the File sector by sector and each read action would call read_file func 
/// @param path 
/// @param read_file_func_cb 
/// @return 
SDCard_Err SDCard_ReadFile(const char* path, uint8_t (*read_file_func_cb)(const char* buff)){
    FIL file; FRESULT res;
    UINT read_size;
    res = f_open(&file, path, FA_READ);  if (res!= FR_OK) return FileOpenERR;
    
    while (1){
        // **** important -> the btr parameter must be sizeof (read_buf) - 1, or first bit will error****
        memset(fs_buffer , 0, strlen(fs_buffer));
        res = f_read(&file, fs_buffer, sizeof(fs_buffer), &read_size);
        if (res!=FR_OK) { // if the SD card plug out, it returns.
            f_close(&file);  return FileReadERR;
        }
        if (read_file_func_cb!=NULL && read_file_func_cb(fs_buffer)){
            f_close(&file); return FileReadERR;
        }
        if (read_size == 0) break;  // the file read ended;
    }
    f_close(&file);
    return NOERR;
}

/// @brief Copy file from  the src_dir to dst_dir (will rename it.)
/// @param force_cp    if set to false, if dst_dir exists, the function will return err, true to replace the dst file.
/// @param process_func it receive the process proportion (from 0 to 100), n is number of task remained;
/// @return 
SDCard_Err SDCard_CopyFile(const char* src_dir, const char* dst_dir, bool force_cp, void (*process_func)(uint8_t prop)){
    FIL fp1, fp2;
    char BUF[MAX_ABSOLUTE_PATH_LENGTH];  // give 512byte space for buffer for faster copy and move; -> 
    FRESULT res;  FILINFO fno;
    UINT read_size, write_size;
    SDCard_Err result = NOERR;
    
    if (strcmp(src_dir, dst_dir) == 0) return FileCreateERR;
    
    res = f_open(&fp1, src_dir, FA_READ);  
    if (res != FR_OK) return FileOpenERR;
    res = f_stat(src_dir, &fno); if (res != FR_OK || (fno.fattrib & AM_HID)) {
        f_close(&fp1); return FileOpenERR;
    }  // can't copy hidden files
    
    if (force_cp){
        res = f_open(&fp2 , dst_dir, FA_WRITE|FA_CREATE_ALWAYS);
    }else{
        res = f_open(&fp2 , dst_dir, FA_CREATE_NEW|FA_WRITE);
    }
    if (res != FR_OK) { f_close(&fp1); return FileOpenERR;}

    uint32_t space_tot = f_size(&fp1)/1024 * 2;   // the total space in sector (1 sector = 512byte )
    uint32_t space_cp  = 0;                       // the sector that copied
    
    while (1){
        res = f_read(&fp1, BUF, sizeof(BUF), &read_size);
        if (res!=FR_OK) {
            result = FileReadERR; break;  // if the SD card plug out, it occurs 
        }
        if (read_size == 0) break;        // eof error or break interval;
        res = f_write(&fp2, BUF, read_size, &write_size);
        if (res!=FR_OK) {
            result = FileWriteERR; break; // the destination can't be written.
        }
        space_cp += 1;    // 512 byte copied;
        if (process_func!=NULL) process_func((int)((float)space_cp/space_tot * 100));
        if (write_size < read_size) break;    //  disk full error
    }
    
    f_close(&fp1); f_close(&fp2);
    return result;
}


/// @brief Copy Folder or file to the "dest_fold_dir" location. (if the dst_fold_dir not exist, will try create it)
/// @param src_dir      
/// @param dst_fold_dir directory for destination folder
/// @param force_cp     if force to copy files if file already exist.
/// @param process_func  receive the process proportion (from 0 to 100), n is number of task remained 
///            (use sdcard_file_counter for remain file to copy);
/// @note  call SDCard_ListPath_retrieve first to get files to cp in user_copy function 
/// @return 
SDCard_Err SDCard_CopyFolder(const char* src_dir, const char* dst_fold_dir, bool force_cp, void (*process_func)(uint8_t prop)){
    FRESULT res;
    FILINFO fno, fno_child;   // fno : info of parent folder;  fno_child: info of child foldder
    DIR dir;                  // firstly, will create directory under the dst_fold_dir 
    char path1[MAX_ABSOLUTE_PATH_LENGTH];
    _sd_get_parent_path(src_dir, path1);

    // avoid to copy to the same place or hidden file 
    if (strcmp(path1, dst_fold_dir) == 0) return DirCreateERR;  
    if (f_stat(src_dir, &fno)  ||  (fno.fattrib & AM_HID)) return FileInfoERR;
    
    // in this loop, each file directory is listed as result_dir
    if (fno.fattrib & AM_DIR){
        char path2[MAX_ABSOLUTE_PATH_LENGTH];
        /* if not exist, try create this directory.(include the dst_folder and under dst_dir)*/
        if (SDCard_CreateFolder(dst_fold_dir) || 
            _sd_combine_dir(dst_fold_dir, fno.fname, path2) || 
                (SDCard_CreateFolder(path2)))
            return DirCreateERR;

        if (f_opendir(&dir,src_dir)!= FR_OK) return DirOpenERR;
        // use path2 as the dst folder name; path 1 as fully name of files in src folder;
        while (1){    // List Path and copy every file to the destination.            
            res = f_readdir(&dir, &fno_child);      // read the directory and get the name 
            if (_sd_combine_dir(src_dir, fno_child.fname, path1)) {
                f_closedir(&dir); return DirCreateERR;
            }
            if (res!= FR_OK || fno_child.fname[0] == 0) break;     // read end
            if (SDCard_CopyFolder(path1, path2, force_cp, NULL)!=FR_OK){
                // copy folder retrieve. also tip when
                /* SDCard_File_Error Handeler ............ */  
            }
        }
        f_closedir(&dir);
    }
    else  // if src_dir is a file, copy the file to destination
    {
        SDCard_Err result = SDCard_CreateFolder(dst_fold_dir);
        if (result) return DirCreateERR;
        if (_sd_combine_dir(dst_fold_dir, fno.fname, path1)) return DirCreateERR;
        if (SDCard_CopyFile(src_dir, path1, force_cp, process_func)!=FR_OK){
            // jump the invalid file copy. 
            /* SDCard_File_Error Handeler ............ */
        }
        if (sdcard_file_counter) sdcard_file_counter--;
    }
    Delay_ms(100);  // have a short delay for stable.
    return NOERR;
}


/// @brief rename file or folder
/// @param path      total path of file 
/// @param path_new  total path of folder
/// @return 
SDCard_Err SDCard_RenameFolder(const char* path, const char* path_new){
    FILINFO fno;
    if (strcmp(path,path_new) == 0) return FileCreateERR;
    f_stat(path, &fno);  // firstly, judge whether path is folder or file
    if ((fno.fattrib & AM_HID) || (fno.fattrib & AM_RDO)) return FileInfoERR;
    if (f_rename(path, path_new)!=FR_OK) return DirCreateERR;
    return NOERR;
}

/// @brief Move file or folder to dst_fold_dir (if dst_folder_dir not exist, will try create it)
/// @param src_dir directory of file/folder to be moved 
/// @param dst_fold_dir directory to move to 
/// @note if same name folder already exists in dst_fold_dir, it will not move and return error
/// @return 
SDCard_Err SDCard_MoveFolder(const char* src_dir, const char* dst_fold_dir){
    FILINFO fno;
    if (f_stat(src_dir, &fno)!=FR_OK) return FileOpenERR;
    if ((fno.fattrib & AM_HID) || (fno.fattrib & AM_RDO)) return FileInfoERR;
    
    if ((fno.fattrib & AM_DIR) && SDCard_ListPath_retrieve(src_dir, _sd_file_available_cb)) return FileInfoERR;
    
    char res_dir[MAX_ABSOLUTE_PATH_LENGTH], res_dir2[MAX_ABSOLUTE_PATH_LENGTH];
    if (_sd_get_parent_path(src_dir, res_dir) || 
        (strcmp(res_dir, dst_fold_dir) == 0)  ||   // dst dir shouldn't same the dir of src_dir
        _sd_get_filename(src_dir, res_dir2)   ||
        _sd_combine_dir(dst_fold_dir, res_dir2, res_dir))
        return DirCreateERR;
    
    // first, try create dst_fold_dir
    if (SDCard_CreateFolder(dst_fold_dir)) return DirCreateERR;
    
    // res_dir is the path to create folder (if alerday exists, not do it)
    if (f_rename(src_dir, res_dir)!= FR_OK) return FileWriteERR;
    return NOERR;
}

#ifdef __cplusplus
}
#endif // __cplusplus
