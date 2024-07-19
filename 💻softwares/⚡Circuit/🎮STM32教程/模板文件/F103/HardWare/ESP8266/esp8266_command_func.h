#ifndef ESP8266_COMMAND_FUNC_H
#define ESP8266_COMMAND_FUNC_H

#ifdef __cplusplus
    extern "C"{
#endif

#include "stm32f10x.h"
#include "esp8266_driver.h"
#include <stdio.h>
#include "string.h"

// -------- esp8266 Infomation struct typedef ---------------------
typedef struct{   // AT + GMR 
    char* AT_version;
    char* SDK_version;
    char* Compile_time;
} ESP_VERSION_INFO_t;

typedef struct{  // AT + CIFSR 
    char* APIP;
    char* APMAC;
    char* STAIP;
    char* STAMAC;
} ESP_LOCAL_ADDRESS_INFO_t;

typedef struct{  // AT + CIPSTA  
    char* ip;
    char* gateway;
    char* netmask;
} ESP_STATION_INFO_t;

// ----------------------- state struct -------------------
typedef struct{
    uint8_t sleepmode;
} ESP_STATE_t;

typedef struct{
    uint8_t mode;
} ESP_WIFI_STATE_t;

typedef struct{
    uint8_t  cipmux;
    uint16_t ciptimeout;
} ESP_TCP_IP_STATE_t;

// --------------------- general functions ---------------------

ESP_Error_t esp8266_cmd_checkversioninfo(ESP_VERSION_INFO_t *info_struct); // AT+GMR?
ESP_Error_t esp8266_cmd_set_echoEnable(const uint8_t enable);              // AT+ATEx
ESP_Error_t esp8266_cmd_software_reset(void);                              // AT+RST
ESP_Error_t esp8266_cmd_restore_factory_settings(void);                    // AT+RESTORE
ESP_Error_t esp8266_cmd_set_sleepmode(uint8_t sleepmode);                  // AT+SLEEP=

// ---------------------- Wifi functions ------------------------

ESP_Error_t esp8266_cmd_wifi_get_mode(uint8_t *mode);  // AT+CWMODE? 
ESP_Error_t esp8266_cmd_wifi_set_mode(uint8_t mode);   // AT+CWMODE=
ESP_Error_t esp8266_cmd_wifi_set_joinAP(uint8_t no_param, const char* ssid, const char* pwd);  // AT+CWJAP=
ESP_Error_t esp8266_cmd_wifi_quitAP(void);             // AT+CWQAP
ESP_Error_t esp8266_cmd_wifi_get_cipaddress(ESP_STATION_INFO_t *station_addr);

// --------------------- TCP_IP functions ----------------------

ESP_Error_t esp8266_cmd_tcp_set_cipmux(uint8_t mux_mode);   // AT+CIPMUX=
ESP_Error_t esp8266_cmd_tcp_get_cipmux(uint8_t *mux_mode);  // AT+CIPMUX?
ESP_Error_t esp8266_cmd_tcp_set_tcpServer(uint8_t mode,  uint16_t port);  // AT+CIPSERVER=
ESP_Error_t esp8266_cmd_tcp_cipsend(const char* link_id, uint16_t length,const char* remote_host,const char* remote_port,const char* msg); // AT+CIPSEND (may bug)
ESP_Error_t esp8266_cmd_tcp_get_ciptimout(uint16_t* timeout_res); // AT+CIPSTO?

#ifdef __cplusplus
    }
#endif

#endif // !ESP8266_COMMAND_FUNC_H

