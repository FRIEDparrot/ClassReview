对于定义函数可以接受可变参数的情况: 
基本方法是使用省略号(需要注意， 至少一个固定参数):
```cpp
int func(int arg1, int arg2,  ...){
}
```

对于可变参数的提取, 主要依赖于以下宏函数:
```cpp
va_list 
va_start
va_arg 
va_copy 
va_end
```

va_start: 在调用函数之后, ap 指向第一个可变参数的首地址(<mark style="background: transparent; color: red">last是最后一个传递给函数的固定参数</mark>)
va_arg:  提取某个固定类型的参数(例如int,double), 如果返回type不统一, 则会发生错误

```cpp
va_start(va_list ap, last);  // 其中ap 是va_list 对象 ,  在调用函数之后, ap 指向第一个可变参数的首地址
va_arg(va_list ap, type);
va_copy(va_list  dest, va_list src);  // 用于复制 
va_end(va_list ap);     // 本质是将指针置为 NULL 
```

### 使用示例
#### (1) 打印每个参数
```cpp
#include <stdio.h>
#include <stdarg.h>

void PrintArg(int num, ...)
{
	va_list ap;
	// 1.进行初始化
	va_start(ap, num);

	for (int i = 0; i < num; i++)
	{
		// 不断取出可变参数
		int a = va_arg(ap, int);
		printf("%d ", a);
	}
	// 销毁
	va_end(ap);
}

int main()
{
	PrintArg(4, 1, 3, 4, 5);
	return 0;
}
```

### (2) 串口发送函数
```cpp
#include <stdarg.h>
void Serial_Printf(const char* __format, ...){
	char ch[200];
	va_list arg;
	va_start(arg, __format); // receive the parameter from format 
	vsprintf(ch, __format, arg);   // printf into variable 
	va_end(arg); // release parameter list 
	Serial_SendString(ch);
}
```

对于ESP8266的传输, 为了保证传输准确，一种比较好的方法是重传几次直到获得正确的答复。如果没有正确答复, 则可以进行重传。(另外一种是固件编译和硬件应答, 其实不需要)

需要说明的是, 首先, va_arg 会从第一个后面的可变参数开始取

### (3) 利用va_list 循环构造命令 
将参数拼接起来, 获得对应的AT指令字符串的函数: 
```c
/// @brief concatnate each parameter of the command to the start command
/// @param str_dest 
/// @param start_str 
/// @param  
/// @return 
static uint8_t string_join_command(char** str_dest, const char* start_str,...){
    free((*str_dest));
    (*str_dest) = NULL;
    (*str_dest) = malloc(ESP8266_TX_BUFFER_SIZE * sizeof(char));
    strcpy((*str_dest), start_str);
    
    va_list arg;
    va_start (arg, start_str);  // begin param
    const char* str;            // use str for receive 
    
    uint16_t index = 0;
    while (1){
        str = va_arg(arg, const char*);   // receive the string
        if (str == NULL) break;
        if (strlen((*str_dest)) + strlen(str) + 1 > ESP8266_TX_BUFFER_SIZE){ 
            free((*str_dest)); (*str_dest) = NULL;
            va_end(arg);   // release the storge of parameter 
            return 1;
        }
        if (index!=0){
            (*str_dest) = strcat((*str_dest),",");
        }
        index ++;
        (*str_dest) = strcat((*str_dest), str);
    }
    va_end(arg);
    if (strlen((*str_dest)) + 2 > ESP8266_TX_BUFFER_SIZE) {
        free((*str_dest)); (*str_dest) = NULL;
        return 1;
    }
    (*str_dest) = strcat((*str_dest),"\r\n");
    return 0;
}
```

