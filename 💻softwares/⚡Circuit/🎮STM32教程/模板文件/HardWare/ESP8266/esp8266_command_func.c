/* 
 *    file: esp8266_command_func.c 
 *    author: FriedParrot 
 *    date: 2024-06-18 
 *    
 *    
 *  1. This file include command function, command call back function 
 *     and the infomation struct 
 * 
*/
#include  "esp8266_command_func.h"
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

/// ============== Declaration of static  variables ==================

static ESP_VERSION_INFO_t Esp8266_version_infomation;
static ESP_STATION_INFO_t Esp8266_station_infomation;

static ESP_WIFI_STATE_t   Esp8266_Wifi_state;
static ESP_TCP_IP_STATE_t Esp8266_TCPIP_state;

/// ======== Declaration of static functions and callback functions ===========

static uint8_t string_startwith(const char* str, const char* str_start);
static uint8_t string_startmatch(const char* str, const char* str_start, char** str_output, uint16_t length);
static uint8_t string_join_command(char** str_dest, const char* start_str,...);

// ----------------general function call back --------- 

static uint8_t _esp_cmd_checkversion_cb(ESP_MSG_LIST list);

// ----------------wifi function call back ------------ 

static uint8_t _esp_cmd_wifi_get_mode_cb(ESP_MSG_LIST list);
static uint8_t _esp_cmd_wifi_get_cipaddress_cb(ESP_MSG_LIST list);

// ---------------- tcp -ip function call back --------
static uint8_t _esp_cmd_tcp_get_cipmux_cb(ESP_MSG_LIST list);
static uint8_t _esp_cmd_tcp_get_ciptimout_cb(ESP_MSG_LIST list);

/// ========================= static functions  definition  ======================

/// @brief if string is start with str_start, return 0, or return 1
/// @param str string to match 
/// @param str_start the string should start with this str.
/// @return 0: succeed;  1: failed
static uint8_t string_startwith(const char* str, const char* str_start){
    return  strncmp(str, str_start, strlen(str_start)) == 0 ? 0: 1;
}

/// @brief match the start of string, and from just after match place, colloect string of specified length 
/// @param str        string to match 
/// @param str_start  the string should start with this str.
/// @param str_output the address of the output string, it collect the parameter after the matched str_start
/// @param length     length to copy(if too large, will copy until string end)
/// @return 0: match string succeed and result is written into str_output. 
///         1: match failed
/// @warning free the str_output must  after use it.
static uint8_t string_startmatch(const char* str, const char* str_start, char** str_output, uint16_t length){
    if (*str_output != NULL) {
        free(*str_output);
        *str_output = NULL;
    }
    uint16_t s = strlen(str_start);
    if (s + length > strlen(str)) length = strlen(str) - s;  // exceed 
    if (s + length > strlen(str) || strncmp(str, str_start, s) != 0) {
        // failed to compare the start string:
        return 1;
    }
    (*str_output) = malloc((length + 1) * sizeof(char));
    strncpy((*str_output), str+s, length);
    return 0;
}

/// @brief concatnate each parameter of the command to the start command, 
///        default deliminator is ","; 
///        "\\r\\n" will be automatically add to the str.
/// @param str_dest  address of the destination string 
/// @param start_str start 
/// @param other parameters should all be of type char* to concatenate command. 
/// @warning str_dest must be freed after use it 
/// @return 0: make command succeed 
///         1: command exceed ESP8266_TX_BUFFER_SIZE and str_dest would be NULL
static uint8_t string_join_command(char** str_dest, const char* start_str,...){
    if (str_dest!=NULL){
        free((*str_dest));
        (*str_dest) = NULL;
    }
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
            free((*str_dest)); (*str_dest) = NULL; // str_dest would not be NULL here
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

/// ================== esp8266 command call back function ======================= 
/// @brief callback function for get the version info of esp8266 chip 
/// @param list 
/// @return see esp8266_sendcmd in sep8266_driver.c 
static uint8_t _esp_cmd_checkversion_cb(ESP_MSG_LIST list){
    /* 
    response format:
        AT version:1.7.1.0(Jul 15 2019 16:58:04)
        SDK version:3.0.1(78a3e33)
        compile time:Feb 14 2020 09:19:42
    */
    for (int i = 0; i < list.cmd_buffer_length; i++){
        // free storge before calling 
        if (!string_startwith(list.cmd_buffer[i], "AT version:")){ // if return 0, match succeed. 
            // check other two command for match:
            if (string_startmatch(list.cmd_buffer[i],"AT version:", &Esp8266_version_infomation.AT_version, 7) || \
                string_startmatch(list.cmd_buffer[i+1],"SDK version:", &Esp8266_version_infomation.SDK_version, 5) || \
                string_startmatch(list.cmd_buffer[i+2],"compile time:", &Esp8266_version_infomation.Compile_time, 12)){
                    return 1;
                }
            return 0; // all check complete
        }
    }
    return 1;
}

/// ------------------- Wifi Command Call back function -------------- 

/// @brief callback function for get wifi mode 
/// @param list 
/// @return 
static uint8_t _esp_cmd_wifi_get_mode_cb(ESP_MSG_LIST list){
    for (int i = 0; i < list.cmd_buffer_length; i++){
        if (!string_startwith(list.cmd_buffer[i],"+CWMODE:")){
            char *mode_res = NULL;
            if (string_startmatch(list.cmd_buffer[i],"+CWMODE:", &mode_res, 1)) return 1;
            Esp8266_Wifi_state.mode = atoi(mode_res);
            free(mode_res); mode_res = NULL;
            return 0;
        }
    }
    return 1;
}

/// @brief call back function for getting the station address 
/// @param list 
/// @return 
static uint8_t _esp_cmd_wifi_get_cipaddress_cb(ESP_MSG_LIST list){
    for (int i = 0; i < list.cmd_buffer_length; i++){
        if (!string_startwith(list.cmd_buffer[i], "+CIPSTA:ip:")){
            if (string_startmatch(list.cmd_buffer[i],  "+CIPSTA:ip:", &Esp8266_station_infomation.ip, 20) || \
                string_startmatch(list.cmd_buffer[i+1],"+CIPSTA:gateway:", &Esp8266_station_infomation.gateway, 20) || \
                string_startmatch(list.cmd_buffer[i+2],"+CIPSTA:netmask:", &Esp8266_station_infomation.netmask, 20))
                return 1;
            return 0;
        }
    }
    return 1;
}

/// -------------------  TCP-IP Command callbacks ------------- 

/// @brief callback funciton for get cipmux mode 
/// @param list 
/// @return 
static uint8_t _esp_cmd_tcp_get_cipmux_cb(ESP_MSG_LIST list){
    for (int i = 0; i < list.cmd_buffer_length; i++){
        if (!string_startwith(list.cmd_buffer[i],"+CIPMUX:")){
            char *mux_res = NULL;
            if (string_startmatch(list.cmd_buffer[i],"+CIPMUX:",&mux_res,1)) return 1;
            Esp8266_TCPIP_state.cipmux = atoi(mux_res);
            free(mux_res);
            return 0;
        }
    }
    return 1;
}

/// @brief callback function for CIPSTO command 
/// @param list 
/// @return 
static uint8_t _esp_cmd_tcp_get_ciptimout_cb(ESP_MSG_LIST list){
    char *timeout_str = NULL;
    for (int i = 0; i < list.cmd_buffer_length; i++){
        if (!string_startwith(list.cmd_buffer[i],"+CIPSTO:")){
            if (string_startmatch(list.cmd_buffer[i],"+CIPSTO:", &timeout_str, 4)) return 1;  // maximum 7200
            Esp8266_TCPIP_state.ciptimeout = atoi(timeout_str);
            if (timeout_str!=NULL){
                free(timeout_str);
                timeout_str = NULL;
            }
            return 0;
        }
    }
    return 1;
}

/// ===================== ESP8266 AT General Command Functions ===================== 
/// @brief Check the information version of 
/// @param info_struct ESP_VERSION_INFO_t type, receive the infomation result
/// @return see ESP_Error_t
ESP_Error_t esp8266_cmd_checkversioninfo(ESP_VERSION_INFO_t *info_struct){
    ESP_Error_t result = esp8266_sendcmd("AT+GMR\r\n", "OK", _esp_cmd_checkversion_cb);
    if (result!=ESP_RES_OK) return result;
    if (info_struct!=NULL){
        // pointer redirection
        info_struct->AT_version   = Esp8266_version_infomation.AT_version;
        info_struct->SDK_version  = Esp8266_version_infomation.SDK_version;
        info_struct->Compile_time = Esp8266_version_infomation.Compile_time;
    }
    return  result;
}

/// @brief Set Echo enable or Echo Disable
/// @param enable 
/// @return 
ESP_Error_t esp8266_cmd_set_echoEnable(const uint8_t enable){
    ESP_Error_t result;
    if (!enable){
        result =  esp8266_sendcmd("ATE0\r\n","OK", NULL);
    }
    else{
        result = esp8266_sendcmd("ATE1\r\n","OK", NULL);
    }
    return result;
}

/// @brief Software Reset Command for esp8266
/// @param 
/// @return see ESP_Error_t
ESP_Error_t esp8266_cmd_software_reset(void){
    ESP_Error_t result = esp8266_sendcmd("AT+RST\r\n","OK",NULL);
    return result;
}

/// @brief Restore Factory Settings 
/// @param  
/// @return see ESP_Error_t 
ESP_Error_t esp8266_cmd_restore_factory_settings(void){
    ESP_Error_t result = esp8266_sendcmd("AT+RESTORE\r\n","OK", NULL);
    Delay_ms(1000);
    return result;
}

/// @brief Set sleep mode of ESP8266 
/// @param sleepmode 0: disable sleep, 1: Modern-sleep DTIM mode, 
///                  2: Light-sleep mode, 3:modem-sleep listen interval mod 
/// @note  when enter sleep mode, the chip would not always respond to command 
/// @return 
ESP_Error_t esp8266_cmd_set_sleepmode(uint8_t sleepmode){
    ESP_Error_t result;
    if (sleepmode > 3) return ESP_RES_PARA_INVALID;
    char buff[10];
    sprintf(buff, "%d", sleepmode);

    char* cmd = NULL;
    if (string_join_command(&cmd, "AT+SLEEP=", buff, NULL)) return ESP_RES_TxBUFFER_FULL;
    result = esp8266_sendcmd(cmd, "OK", NULL);
    if (cmd!=NULL){
        free(cmd); cmd = NULL;
    }
    return result;
}


/// ------------------------ ESP8266 WIFI Command Functions --------------------------

/// @brief set wifi mode of esp8266 device
/// @param mode: 0 : no wifi mode 1 : station mode  2 : SoftAP  mode  3: SoftAP+Station Mode 
/// @param auto_connect auto_connect mode, default is 1 
/// @return 
ESP_Error_t esp8266_cmd_wifi_set_mode(uint8_t mode){
    ESP_Error_t  result;
    if (mode > 3) return ESP_RES_PARA_INVALID;
    char md[10];
    sprintf(md, "%d",mode);
    
    char* cmd = NULL;
    if (string_join_command(&cmd, "AT+CWMODE=",md, NULL)) return ESP_RES_TxBUFFER_FULL;
    result = esp8266_sendcmd(cmd,"OK", NULL);
    if (cmd!=NULL){
        free(cmd); cmd = NULL;
    }
    return result;
}

/// @brief check the wifi mode of esp8266 device
/// @param mode 0 : no wifi mode 1 : station mode  2 : SoftAP  mode  3: SoftAP+Station Mode
/// @return 
ESP_Error_t esp8266_cmd_wifi_get_mode(uint8_t *mode){
    ESP_Error_t result = esp8266_sendcmd("AT+CWMODE?\r\n", "OK", _esp_cmd_wifi_get_mode_cb);
    if (result == ESP_RES_OK && mode!=NULL){
        (*mode) = Esp8266_Wifi_state.mode;
    }
    return result;
}

/// @brief Join AP connection 
/// @param no_param if set to 1, try to join the last wifi, ssid and pwd would be ignored.
/// @param ssid     Wifi name 
/// @param pwd      password 
/// @return 
ESP_Error_t esp8266_cmd_wifi_set_joinAP(uint8_t no_param, const char* ssid, const char* pwd){
    ESP_Error_t result;
    char* cmd = NULL;
    
    esp8266_extra_timeout_set(9000);  // wait for 10s for wifi connection
    if (no_param){
        result = esp8266_sendcmd("AT+CWJAP\r\n", "OK", NULL);
    }else{
        if (string_join_command(&cmd, "AT+CWJAP=",ssid, pwd, NULL)) {
            esp8266_extra_timeout_set(0);
            return ESP_RES_TxBUFFER_FULL;
        }
        result = esp8266_sendcmd(cmd, "OK", NULL);
        if (cmd!=NULL) {
            free(cmd); cmd = NULL;
        }
    }
    esp8266_extra_timeout_set(0);
    return result;
}

/// @brief quit the connection to the current AP 
/// @param  
/// @return 
ESP_Error_t esp8266_cmd_wifi_quitAP(void){ 
    // AT+CWQAP 
    return esp8266_sendcmd("AT+CWQAP\r\n", "OK", NULL);
}

/// @brief query the ip address of the ESP station 
/// @param station_addr  output station address structure
/// @return 
ESP_Error_t esp8266_cmd_wifi_get_cipaddress(ESP_STATION_INFO_t *station_addr){
    ESP_Error_t result;
    result = esp8266_sendcmd("AT+CIPSTA?\r\n", "OK", _esp_cmd_wifi_get_cipaddress_cb);
    if (result == ESP_RES_OK && station_addr!=NULL){
        station_addr -> ip = Esp8266_station_infomation.ip;
        station_addr -> gateway = Esp8266_station_infomation.gateway;
        station_addr -> netmask = Esp8266_station_infomation.netmask;
    }
    return result;
}

/// ------------------------ ESP8266 TCP-IP Commad Functions -------------

/// @brief open or disable the cip multiple connection mode 
/// @param mux_mode 0: single connection; 1: multiple connection 
/// @note  MUX can be 1 only when CIPMODE = 0 
/// @return 
ESP_Error_t esp8266_cmd_tcp_set_cipmux(uint8_t mux_mode){
    ESP_Error_t result;
    if (mux_mode > 1) return ESP_RES_PARA_INVALID;

    char mm[10];  sprintf(mm, "%d",mux_mode);
    char* cmd = NULL;
    if (string_join_command(&cmd, "AT+CIPMUX=", mm, NULL)) return ESP_RES_TxBUFFER_FULL;
    
    result = esp8266_sendcmd(cmd, "OK", NULL);
    if (cmd!=NULL){
        free(cmd); cmd = NULL;
    }
    return result;
}

/// @brief get the  cip multiple connection mode state
/// @param mux_mode  mux_mode 0: single connection; 1: multiple connection 
/// @return 
ESP_Error_t  esp8266_cmd_tcp_get_cipmux(uint8_t *mux_mode){
    ESP_Error_t result = esp8266_sendcmd("AT+CIPMUX?\r\n","OK", _esp_cmd_tcp_get_cipmux_cb);
    if (result == ESP_RES_OK && mux_mode!=NULL){
        (*mux_mode) = Esp8266_TCPIP_state.cipmux;
    }
    return result;
}

/// @brief delete or create TCP server 
/// @param mode close server
/// @param port if mode is 1, set port to listen; 
///        when mode is 0, 0: shutdown the server and keep existing connections.
///                        1: shutdown the server and close all connections.
/// @return 
ESP_Error_t esp8266_cmd_tcp_set_tcpServer(uint8_t mode,  uint16_t port){
    // AT+CIPSERVER=1,8080
    ESP_Error_t result;
    if (mode > 1 ) return ESP_RES_PARA_INVALID;
    
    char buf1[10], buf2[10];
    sprintf(buf1, "%d" ,mode); sprintf(buf2, "%d", port);
    
    char* cmd = NULL;
    if (string_join_command(&cmd, "AT+CIPSERVER=", buf1, buf2, NULL))
        return ESP_RES_TxBUFFER_FULL;
    result = esp8266_sendcmd(cmd, "OK", NULL);
    if(cmd!=NULL) {
        free(cmd); cmd = NULL;
    }
    return result;
}

/// @brief Send Data in the Normal Transmission Mode or Wi-Fi Passthrough Mode
/// @param link_id      id of the connected device(for single connection, set link_id as NULL
/// @param length       length of th 
/// @param remote_host  (if not UDP connection, set to NULL)
/// @param remote_port  (if not UDP connection, set to NULL)
/// @param msg          message 
/// @note ; if enter wifi-passthrough mode, set length as 0;
/// @bug (not finished!): single connection mode and passthrough mode haven't been tested!
/// @return 
ESP_Error_t esp8266_cmd_tcp_cipsend(const char* link_id, uint16_t length,const char* remote_host,const char* remote_port,const char* msg){
    ESP_Error_t result;
    if (length > ESP8266_TX_BUFFER_SIZE) return ESP_RES_PARA_INVALID;
    
    char *cmd = NULL;
    if (link_id == NULL){ // in single connection mode, 
        if (length == 0){
            // Enter the Wi-Fi Passthrough Mode 
            result = esp8266_sendcmd("AT+CIPSEND\r\n","OK",NULL);
        }
        else{
            char buf1[10]; sprintf(buf1, "%d", length);
            if (string_join_command(&cmd, "AT+CIPSEND=", buf1, NULL)) 
                return ESP_RES_TxBUFFER_FULL;
            result = esp8266_sendcmd(cmd,"OK", NULL);
        }
    }else{  // for mux=1 transmittion mode or UDP connection.
        char buf1[10]; sprintf(buf1, "%d", length);
        if (string_join_command(&cmd,"AT+CIPSEND=",link_id,buf1,remote_host,remote_port,NULL))
            return ESP_RES_TxBUFFER_FULL;
        result = esp8266_sendcmd(cmd,"OK",NULL);
    }
    if (cmd!=NULL){
        free(cmd); cmd = NULL;
    }
    if (result!=ESP_RES_OK) return result;
    if (msg!=NULL){
        result = esp8266_sendcmd(msg, "SEND OK", NULL);
    }
    return ESP_RES_OK;
}

/// @brief TCP_IP timeout query
/// @param timeout_res pointer to the result (if not needed use NULL)
/// @return 
ESP_Error_t esp8266_cmd_tcp_get_ciptimout(uint16_t* timeout_res){
    ESP_Error_t result = esp8266_sendcmd("AT+CIPSTO?\r\n","OK", _esp_cmd_tcp_get_ciptimout_cb);
    if (result == ESP_RES_OK){
        if (timeout_res!=NULL) (*timeout_res) = Esp8266_TCPIP_state.ciptimeout;
    }
    return result;
}

