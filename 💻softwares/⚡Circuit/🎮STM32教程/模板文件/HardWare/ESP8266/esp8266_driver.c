#include "stm32f10x.h"
#include "esp8266_driver.h"
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>   // dynamic storge for CMD_LIST
#include "Delay.h"
// =========== struct and typedef  =================


/* =========== private  definition ============= */ 
#define ESP_Set_EN(x)  GPIO_WriteBit(ESP_GPIO, ESP_Pin_EN, (BitAction)(x))
#define ESP_Set_RST(x) GPIO_WriteBit(ESP_GPIO, ESP_Pin_RST,(BitAction)(x))
#define ESP_Set_GPIO0(x) GPIO_WriteBit(ESP_GPIO, ESP_Pin_GPIO0, (BitAction)(x))
#define ESP_Set_GPIO2(x) GPIO_WriteBit(ESP_GPIO, ESP_Pin_GPIO2, (BitAction)(x))

/* ========== static variables definition =================  */
static char ESP_Rx_buffer[ESP8266_RX_BUFFER_SIZE];
static char ESP_Rx_buffer_shadow[ESP8266_RX_BUFFER_SIZE];    // shadow buffer to avoid

// static char ESP_Tx_buffer[ESP8266_TX_BUFFER_SIZE];        // Tx buffer 
static struct ESP_MSG_LIST ESP_Cmd_List;                     // command List variable 

static uint16_t extra_command_timeout = 0;           // extra command timeout for execution, this is used when wait for wifi connection.
static uint16_t buffR_length = 0;                    // global length of ESP_Rx_buffer (or index)
static uint16_t buffR_shadow_length = 0;             // global length of ESP_Rx_buffer_shadow (or index)
static uint16_t esp_timeout_counter = 0;             // counter for timeout
static uint8_t  rx_shadow_buffer_protected = 0;      // flag bit, record if this buffer is protected


/* ========= static function declaration =================  */
// ------------------ basic functions ---------------------
static void _esp8266_ioinit(void);
static void _esp8266_uartinit(void);
static void _esp8266_senddata(uint16_t byte);
static void _esp8266_sendstring(const char* __format, ...);
static void _esp8266_clearRxBuffer(void);
static void _esp8266_clearCmdList(void);

// --------- static call back function in receive message process -------

static uint8_t _esp8266_receive_msg_cb(void);
static uint8_t _esp8266_convert_msg_cb(void);


/* ==============  Interrupt functions  ====================*/
/// @brief read data in interruption function Into Rx Buffer 
/// @note  if the received char is "\\n", will automatically copy the Rx buffer to Rx_buffer_shadow
/// @param 
void USART3_IRQHandler(void){
    if (USART_GetITStatus(ESP_USART, USART_IT_RXNE) == SET){
        if (buffR_length < ESP8266_RX_BUFFER_SIZE){
            ESP_Rx_buffer[buffR_length] = (char)USART_ReceiveData(ESP_USART);
            // if the main program is spliting command, not do this 
            if (!rx_shadow_buffer_protected && ESP_Rx_buffer[buffR_length] == '\n' && buffR_length + buffR_shadow_length < ESP8266_RX_BUFFER_SIZE){
                strcpy(ESP_Rx_buffer_shadow + buffR_shadow_length, ESP_Rx_buffer);
                buffR_shadow_length = buffR_shadow_length + strlen(ESP_Rx_buffer);  // length of shadow buffer 
                memset(ESP_Rx_buffer,0, strlen(ESP_Rx_buffer));   // clear the buffer
                buffR_length = 0;
            }else{
                buffR_length++;
            }
        }else{
            // in this case:  1. the port never get a valid command or data, throw it out.
            //                2. throughout the port get valid command, RxBuffer_shadow is full
            memset(ESP_Rx_buffer, 0, strlen(ESP_Rx_buffer));
            buffR_length = 0;
        }
        USART_ClearITPendingBit(ESP_USART,USART_IT_RXNE);
    }
}

/* ============ static function definition ======================= */
/// @brief init ESP8266 gpio function
/// @param 
static void _esp8266_ioinit(void){
    RCC_APB2PeriphClockCmd(ESP_GPIO_Periph, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin =  ESP_Pin_EN | ESP_Pin_RST | ESP_Pin_GPIO0 | ESP_Pin_GPIO2;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ESP_GPIO, &GPIO_InitStruct);

    ESP_Set_RST(1);
    ESP_Set_EN(1);

    ESP_Set_GPIO0(1);
    ESP_Set_GPIO2(0);
}

/// @brief init USART and interruption (including io)
/// @param 
static void _esp8266_uartinit(void){
    // TXD and RXD cofiguration
    RCC_APB2PeriphClockCmd(ESP_GPIO_Periph, ENABLE);
    RCC_APB1PeriphClockCmd(ESP_USART_Periph, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin  = ESP_Pin_TXD;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ESP_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.GPIO_Pin  = ESP_Pin_RXD;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(ESP_GPIO, &GPIO_InitStruct);
    
    USART_DeInit(ESP_USART);
    USART_InitTypeDef USART_InitStruct;
    USART_StructInit(&USART_InitStruct);
    USART_InitStruct.USART_BaudRate = ESP_USART_Baudrate;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_Init(ESP_USART, &USART_InitStruct);

    // configure the uart interruption, and receive data in interruption
    USART_ITConfig(ESP_USART, USART_IT_RXNE, ENABLE);
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority  = 2;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    USART_Cmd(ESP_USART, ENABLE);  // Enable the USART;
}

/// @brief send data to usart 
/// @note  uart must be initialized correctly 
/// @param byte 
static void _esp8266_senddata(uint16_t byte){
	USART_SendData(ESP_USART, byte);
    while(USART_GetFlagStatus(ESP_USART, USART_FLAG_TXE) == RESET);
	// wait for data sent -> if we send before data transfer from TDR to Shift Register
}

/// @brief send string or command (use \r\n for end of AT command)
/// @param __format 
/// @param 
static void _esp8266_sendstring(const char* __format, ...){
	char ch[ESP8266_TX_BUFFER_SIZE];
	va_list arg;
	va_start(arg, __format);      // receive the parameter from format 
	vsprintf(ch, __format, arg);  
	va_end(arg); // release parameter list
    for (int i = 0; i!=strlen(ch); i++){
        _esp8266_senddata((uint16_t)ch[i]);
    }
}

/// @brief clear the ESP Rx Buffer and shadow Buffer 
/// @note  this also reset the Rx buffer index and Rx shadow buffer index
/// @param 
static void _esp8266_clearRxBuffer(void){
    memset(ESP_Rx_buffer, 0,strlen(ESP_Rx_buffer));
    memset(ESP_Rx_buffer_shadow, 0, strlen(ESP_Rx_buffer_shadow));
    buffR_length = 0;  // also reset the Receive index
    buffR_shadow_length = 0;
}

/// @brief clear the ESP_Cmd_List Buffer (this is a dynamic allocate buffer)
/// @param 
static void _esp8266_clearCmdList(void){
    for (int i = 0; i < ESP_Cmd_List.cmd_buffer_length; i++){
        if (ESP_Cmd_List.cmd_buffer[i]!=NULL){
            free(ESP_Cmd_List.cmd_buffer[i]);
            ESP_Cmd_List.cmd_buffer[i] = NULL;  // must set to NULL after free 
        }
    }
    ESP_Cmd_List.cmd_buffer_length = 0;
}


/// @brief this function is called automatically in receive message function 
/// @note  append the message in ESP_Rx_buffer_shadow into ESP_Cmd_List 
/// @note  split the recognized response string into pieces and storge into ESP_Cmd_List
/// @note  it always clear the ESP_Rx_buffer_shadow, **note the ESP_Cmd_List element should be freed after execute**
/// @return 0: exit normally (convert string into ESP_CMD_LIST successfully)
///         1: command list full (see ESP_CMD_BUFFER_Length)
static uint8_t _esp8266_convert_msg_cb(void){
    if (!strlen(ESP_Rx_buffer_shadow)) return 0;
    char *pch = strtok(ESP_Rx_buffer_shadow, "\r\n");  /* split the string into response */
    while(pch!=NULL){
        /* add the command into the list to be execute */
        if (ESP_Cmd_List.cmd_buffer_length == ESP_CMD_BUFFER_Length) return 1; // in this case,  command buffer already full
        // append the data at the end of ESP_Cmd_List:
        uint16_t idx = ESP_Cmd_List.cmd_buffer_length;
        
        if (ESP_Cmd_List.cmd_buffer[idx]!=NULL){// firstly, free the storge 
            free(ESP_Cmd_List.cmd_buffer[idx]);   
            ESP_Cmd_List.cmd_buffer[idx] = NULL;
        }
        ESP_Cmd_List.cmd_buffer[idx] = malloc((strlen(pch) + 1) * sizeof(char)); // the string end with "\0"
        // not take account if malloc failed
        strcpy(ESP_Cmd_List.cmd_buffer[idx], pch); // record the command into list
        pch = strtok(NULL,"\r\n");
        ESP_Cmd_List.cmd_buffer_length++;
    }
    // clear the buffer shadow for next message: 
    memset(ESP_Rx_buffer_shadow, 0, strlen(ESP_Rx_buffer_shadow));
    buffR_shadow_length = 0; 
    return 0;
}

/// @brief called in execute command, split the message of ESP_Rx_buffer_shadow to ESP_Cmd_List
/// @note  must receive "\\n" as Stop Sign for each command 
/// @param 
/// @return 0: exit no error;
///         1: Rx buffer full(this would clear Rx buffer);
/// @bug    fix the receive method
static uint8_t _esp8266_receive_msg_cb(void){
    if (!strlen(ESP_Rx_buffer_shadow)) return 0;   // exit normally (no command receive)
    // firsty, it will check if buffer full.
    if (strlen(ESP_Rx_buffer) == ESP8266_RX_BUFFER_SIZE){
        // this can't be full during the whole process, if fully its easy to lose data. 
        _esp8266_clearRxBuffer();
        return 1;  // will  return timeout failure, note buffer will be cleared in the interruption function 
    }
    /* splitting command, during splitting: protect shadow buffer*/
    rx_shadow_buffer_protected = 1;
    if (_esp8266_convert_msg_cb()) return 1;
    rx_shadow_buffer_protected = 0;
    return 0;
}

// ===================== public functions ========================== 
/// @brief ESP8266 Init Function, also use "AT" command to check if initialization is successful
/// @param  
/// @note   If just power on, delay about 1000 ms before call this init function.
/// @return ESP_RES_OK: Initialize Successfully
ESP_Error_t esp8266_Init(void){
    _esp8266_ioinit();
    _esp8266_uartinit();
    /*   initialize Command buffer  */
    ESP_Cmd_List.cmd_buffer_length = 0;
    for (int i = 0; i< ESP_CMD_BUFFER_Length; i++) {
        ESP_Cmd_List.cmd_buffer[i] = NULL;
    }
    // use AT command for test, if receive "OK", the initilization is success
    if (esp8266_sendcmd("AT\r\n","OK", NULL)!=ESP_RES_OK) return ESP_RES_INIT_ERR;
    return ESP_RES_OK;
}

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

/// this is the function to be called in the main loop, but in a cmd send function
///  it will wait message and do the logic. it will be added in the next version.
ESP_Error_t esp8266_waitmsg(void){
    /* add the message  handeler here*/
    return ESP_RES_OK;
}

/// @brief Reset esp8266 using RST pin 
/// @param 
/// @return ESP_RES_OK
ESP_Error_t esp8266_hardware_reset(void){
    Delay_ms(500);
    ESP_Set_RST(0);
    Delay_ms(500);
    ESP_Set_RST(1);
    return ESP_RES_OK;
}

/// @brief set the extra timeout used in the command function 
/// @param timeout extra delay when command execution in ms (not too big or wouldn't set succeed)
/// @note  remember to reset it to zero after execution
/// @return 
ESP_Error_t esp8266_extra_timeout_set(const uint16_t timeout){
    if (extra_command_timeout >= 65535-ESP8266_RESPONSE_TIMEOUT) return ESP_RES_PARA_INVALID;
    extra_command_timeout = timeout;
    return ESP_RES_OK;
}


