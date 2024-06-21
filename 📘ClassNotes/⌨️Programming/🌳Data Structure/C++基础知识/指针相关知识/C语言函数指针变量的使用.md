函数指针变量的一般的定义方法是  `返回类型  (*指针变量名)(参数列表)`, 例如
```c
int (*func)(int, int)
```

```c
ESP_Error_t esp8266_sendcmd(char* cmd,char* response,uint8_t  (*cmd_function)(char*)); // 这个是在头文件中的 声明

// 此时, esp8266_cmd_function_cb 即作为一个指针参数传入数组中。 同时在cmd_function 中指定了对应的传参名和变量名
ESP_Error_t esp8266_sendcmd(char* cmd,char* response, uint8_t (*cmd_function)(char*)){
    if (esp8266_cmd_function_cb!=NULL) cmd_function("Hello, world");
}
// 这个写出了如何在函数中调用一个函数指针(注意需要判断是否为NULL)
```
回调函数部分这样写:
```c
uint8_t testfunc(char* ch){
    OLED_ShowString(4,1,ch);
    return 0;
}
```

主函数中只需要这样即可:
```c
ESP_Error_t res1 = esp8266_sendcmd("AT\r\n","OK", testfunc); // 这样即可通过调用该函数testfunc, 让OLED屏幕上显示Hello,World 了
```

如果需要定义一个指针变量指向函数 testfunc, 我们可以按照下面进行。 
例如, 我们首先定义一个函数指针变量 esp8266_cmd_function_cb 如下:
```c
uint8_t (*esp8266_cmd_function_cb)(char*, char*);  // command call back function; 
```

然后将这个变量名指向 testfunc,如下所示:
```c
uint8_t (*esp8266_cmd_function_cb)(char*) =  &testfunc;
```

然后传入该变量名, 则效果是相同的:
```c
ESP_Error_t res1 = esp8266_sendcmd("AT\r\n","OK", esp8266_cmd_function_cb);
```

