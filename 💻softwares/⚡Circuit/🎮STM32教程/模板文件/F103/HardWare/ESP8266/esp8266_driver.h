#ifndef ESP8266_DRIVER_H
#define ESP8266_DRIVER_H

#ifdef __cplusplus
    extern "C"{
#endif //

#include "stm32f10x.h"
#include <stdio.h>
#include "Delay.h"

// ----- Command linear list type definition (easy to copy)-----
#define ESP_CMD_BUFFER_Length 30
typedef struct ESP_MSG_LIST{
    char* cmd_buffer[ESP_CMD_BUFFER_Length];
    uint16_t cmd_buffer_length;
}ESP_MSG_LIST;

typedef enum{
    ESP_RES_OK = 0,                // indicate the first 
    ESP_RES_INIT_ERR,              // initialization error
    ESP_RES_NO_AT_ERR,             // no AT command  error
    ESP_RES_PARA_INVALID,          // parameter invalid error
    ESP_RES_PARA_PARSE_FAIL,
    ESP_RES_CMD_EXEC_FAIL,         // execute command failed  
    ESP_RES_CMD_PROCESSING,      
    ESP_RES_CMD_NO_RESPONSE_ERR,   // response timeout 
    ESP_RES_RxBUFFER_FULL,         // receive buffer full
    ESP_RES_TxBUFFER_FULL          // send buffer full 
}ESP_Error_t;

typedef	enum
{
	ESP_WifiMode_Error                          =     0,
	ESP_WifiMode_Station                        =     1,
	ESP_WifiMode_SoftAp                         =     2,
	ESP_WifiMode_StationAndSoftAp               =     3,
}ESP_WifiMode_t;   // Wifi Mode Typedef,  _t means type(not a variable or function)

typedef enum
{
  ESP_WifiEncryptionType_Open                 =     0,
  ESP_WifiEncryptionType_WPA_PSK              =     2,
  ESP_WifiEncryptionType_WPA2_PSK             =     3,
  ESP_WifiEncryptionType_WPA_WPA2_PSK         =     4,
}ESP_WifiEncryptionType_t;   // Wifi Encryprtion Type



/*********************    Pin Definitions  *******************************/
// The Pin Definition 
#define ESP_GPIO_Periph    RCC_APB2Periph_GPIOB
#define ESP_GPIO           GPIOB
#define ESP_Pin_EN         GPIO_Pin_6   // PB6: EN 
#define ESP_Pin_RST        GPIO_Pin_7   // PB7: RST 
#define ESP_USART_Periph   RCC_APB1Periph_USART3    // USART3 for transmit (note: APB1 is used by default)
#define ESP_Pin_GPIO0      GPIO_Pin_0   // PB0: IO0
#define ESP_Pin_GPIO2      GPIO_Pin_1   // PB1: IO2
#define ESP_USART          USART3       
#define ESP_USART_Baudrate 115200       // 
#define ESP_Pin_TXD        GPIO_Pin_10  // PB10: TXD for master 
#define ESP_Pin_RXD        GPIO_Pin_11  // PB11: RXD for master 


/*********************** Buffer Size definition ************************/ 
#define ESP8266_RX_BUFFER_SIZE 256        // receive buffer. 
#define ESP8266_TX_BUFFER_SIZE 256        // for make command (command length shouldn't exceed this)
#define ESP8266_RESPONSE_TIMEOUT 1000     // wait message maximum time of 1000ms

#define ESP_CMD_RETRY_TIME    3     // If the response is not recognized, retry sending.



ESP_Error_t esp8266_Init(void);
// a command call back function should be given, if not use, give NULL;
ESP_Error_t esp8266_sendcmd(const char* cmd,const char* response, uint8_t (*cmd_function)(ESP_MSG_LIST));
ESP_Error_t esp8266_waitmsg(void);

ESP_Error_t esp8266_hardware_reset(void);
ESP_Error_t esp8266_extra_timeout_set(const uint16_t timeout);


#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // !ESP8266_DRIVER
