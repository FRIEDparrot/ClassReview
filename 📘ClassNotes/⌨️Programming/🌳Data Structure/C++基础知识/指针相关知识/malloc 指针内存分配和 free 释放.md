### (1) malloc 一般使用方法
首先, 使用malloc 为指针分配动态内存空间, 例如给一个char* 分配动态内存:
```c
ESP_Cmd_List.cmd_buffer[idx] = malloc((strlen(pch) + 1) * sizeof(char));
```

> [!caution] 指针的free办法
> 使用 free 释放内存空间, 但是注意释放之后一定要置为空指针, 否则再访问时， 会出现野指针的崩溃错误:

```c title:指针free释放空间时最重要，最常用的语句
if (*str_output != NULL) {
    free(*str_output);
    *str_output = NULL;
}
```

注意:free 只能释放一次， 如果已经释放的指针, 再次使用free, 则会产生错误;

> [!caution] 在释放内存前的使用
> 一般我们对内存进行释放前必须有内存能够释放。
> 因此一般采用 `char* cmd = NULL` 的初始化方案。


### (2) 函数中为指针分配空间
如果是希望在函数中使用内存分配, 如果是为静态全局变量进行malloc时, 则能够成功分配。但是如果是以参数传入的, 则显然不能够成功分配。
```c
static uint8_t string_startmatch(const char* str, const char* str_start, char* str_output, uint16_t length){
str_output = malloc((length + 1) * sizeof(char));
}
```
这是由于,传入的char* str_output 实际上是原始变量的拷贝, 因此为其赋值和 malloc 

正确的方法是使用双指针, 传入字符串的地址值:
```c
static uint8_t string_startmatch(const char* str, const char* str_start, char** str_output, uint16_t length){
	free((*str_output)); 
	(*str_output) = NULL;
	uint16_t s = strlen(str_start);
	if (s + length > strlen(str) || strncmp(str, str_start, s) != 0) {
		// failed to compare the start string:
		return 1;
	}
	(*str_output) = malloc((length + 1) * sizeof(char));
	strncpy((*str_output), str+s, length);
	return 0;
}
```

### (3) 指针的整体赋值技术
如果我们将一个指针直接赋值给另外一个指针, 则另外一个指针会直接得到这个指针的部分, 进而访问此指针对应的字符串。这是一种节省空间的编程技术。

编程示例如下:其中将 esp8266_version_infomation 的每个指针直接赋值给了 info_struct 的指针
```c
ESP_Error_t esp8266_cmd_checkversioninfo(ESP_VERSION_INFO_t *info_struct){
    ESP_Error_t result = esp8266_sendcmd("AT+GMR\r\n", "OK", _esp_cmd_checkversion_cb);
    if (!info_struct) return ESP_RES_PARA_INVALID;
    // pointer redirection 
    info_struct->AT_version   = Esp8266_version_infomation.AT_version;
    info_struct->SDK_version  = Esp8266_version_infomation.SDK_version;
    info_struct->Compile_time = Esp8266_version_infomation.Compile_time;
    return  result;
}
```

此时, 主函数中定义一个info, 即可传入其地址,并获取应答信息。
```c
ESP_VERSION_INFO_t info;
if (esp8266_cmd_checkversioninfo(&info) == ESP_RES_OK) {
	OLED_ShowString(1,1, info.AT_version);
	OLED_ShowString(2,1, info.SDK_version);
	OLED_ShowString(3,1, info.Compile_time);
};
```

### (4) 不连续分配内存字符串访问
在C语言中，`*(str + i)` 和 `str[i]` 是等价的。它们都是用来访问字符串或字符数组中的元素。这两种方式都需要内存是连续分配的(即必须考虑使用static来连续分配)。

如果你的 `char*` 数组的内存是不连续分配的，那么 `*(str + i)` 可能无法正确地返回 `str[i]`。这是因为 `str + i` 会尝试访问 `str` 指针位置后的第 `i` 个内存位置，如果这些内存位置并不连续，那么结果可能是未定义的。

所以，如果你的 `char*` 数组是不连续的，你可能需要使用其他方式来访问它的元素，例如使用指针数组或者链表。这样，你可以直接通过索引或者遍历来访问元素，而不是通过内存位置。希望这个答案对你有所帮助！

```

```