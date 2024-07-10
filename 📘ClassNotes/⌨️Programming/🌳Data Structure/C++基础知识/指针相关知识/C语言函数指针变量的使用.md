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

### C++ 函数指针变量和C不兼容
对于C++函数指针变量, 虽然 extern "C"{} 会告诉编译器按照 C 的规则进行链接， 但是仍然无法解决函数指针类型的不兼容问题。

例如在有 --cpp11定义时, 对于上面有指针的函数, 则会出现编译错误: 
Error: L6218E: Undefined symbol esp8266_sendcmd (referred from esp8266_driver.o).

其解决方案是在对应的.c文件中, 再单独加入一个 extern "C"{}, 如下图:

```cpp title:在.c文件中的解决方案
#ifdef __cplusplus
    extern "C"{
#endif
    /// @brief send the command, wait for response string 
    /// @param cmd           string with command (must end with "\\r\\n")
    /// @param response      string need to check for response (such as "OK")
    /// @param cmd_function  if the response is get, it will call cmd_function and send current ESP_MSG_LIST
    ///                      for this function, return 0: execute successfully return 1: execute failed 
    /// @return if return ESP_RES_OK, command executed successfully.
    /// @note   data received during cmd execution will be ignored, and ESP_CMD_LIST will always be cleared after execution
    ESP_Error_t esp8266_sendcmd(const char* cmd,const char* response, uint8_t (*cmd_function)(ESP_MSG_LIST)){
        ESP_Error_t result = ESP_RES_CMD_NO_RESPONSE_ERR;
        _esp8266_sendstring(cmd);
        
        esp_timeout_counter = 0;  // reset the counter
        while (esp_timeout_counter < ESP8266_RESPONSE_TIMEOUT + extra_command_timeout){
            Delay_ms(1);
            uint8_t res;
            res = _esp8266_receive_msg_cb();        // 1: try receive msg to CMD List 
            if (res) return ESP_RES_RxBUFFER_FULL;  // detect if the buffer is full (if full, try execute command once);
            
            // compare the ESP_Cmd_List and response string, 
            for (int i=0; i < ESP_Cmd_List.cmd_buffer_length; i++){
                if (strcmp(ESP_Cmd_List.cmd_buffer[i], response) == 0) {   // if get the response expected 
                    uint8_t  r1 = 0;
                    if (cmd_function!=NULL) r1 = cmd_function(ESP_Cmd_List);  // r1 = 0 execute successfully, or failed
                    
                    if (!r1) result = ESP_RES_OK; 
                    else result = ESP_RES_CMD_EXEC_FAIL;

                    _esp8266_clearCmdList();  // after execution, clear the command list 
                    return result; 
                }
            }
            esp_timeout_counter++;
        }
        _esp8266_clearCmdList();  // always clear this buff
        
        // timeout error handeling
        if (esp_timeout_counter >= ESP8266_RESPONSE_TIMEOUT + extra_command_timeout){ 
            esp_timeout_counter = 0;
            _esp8266_clearRxBuffer();
            result = ESP_RES_CMD_NO_RESPONSE_ERR;
        }
        return result;
    }

#ifdef __cplusplus
}
#endif
```



链表备份
```c
// double-cycle linklist type
struct ListNode{
    FILINFO  *file_info;    // all type of
    ListNode *prev = NULL;
    ListNode *next = NULL;
};
typedef ListNode Filelist;
/// @brief create a linklist 
/// @param data -> will copy data from it.
/// @return 
ListNode* List_create(FILINFO *info){
    ListNode* list = (ListNode*)malloc(sizeof(ListNode));
    if (list == NULL) return NULL;
    
    if (info == NULL) {
        list->file_info = NULL;
    }else{
        list->file_info = (FILINFO*) malloc(sizeof(FILINFO));
        if (!(list->file_info)) return NULL;
        memcpy(list->file_info, info, sizeof(FILINFO));
    }
    list -> prev = list;
    list -> next = list;
    return list;
}

uint8_t List_append(struct ListNode* head, struct ListNode* node){
    if (!head || !node) return 1;
    ListNode* tmp = head->prev;
    head -> prev = node;
    tmp  -> next = node;
    node -> prev = tmp;
    node -> next = head;
    return 0;
}

/// @brief delete the list 
/// @param head 
void List_delete(struct ListNode* head) {
    if (head == NULL) return;
    struct ListNode* current = head->next;
    while (current!= head) {
        struct ListNode* tmp = current;
        current = current->next;
        if (tmp->file_info !=NULL) free(tmp->file_info);
        free(tmp);
        if (!current) break;
    }
    if (head->file_info!=NULL) free(head->file_info);
    free(head);
}
```