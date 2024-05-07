```cpp
MKFS_PARM* opt = new MKFS_PARM();
opt->fmt = FM_FAT32;
opt->align = 0;      // use disk_ioctl for  sector number
opt->au_size = 512;  // cluster size
opt->n_fat = 1;
opt->n_root= 0;      // default (512)
res = f_mkfs("1:",opt, fs_buffer, 512);      // format the SD card 
```

注意， 格式化得到res后, 往往卡会进入忙状态, 必须等待SD卡格式化完成才能进行下一步。
