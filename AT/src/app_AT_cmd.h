/**************************************************************************************************************
 * altobeam iot at cmd file 
 *
 * Copyright (c) 2018, altobeam.inc   All rights reserved.
 * Permission is hereby granted for use on Altobeam IOT 6421 only,
 * in which case ,it is free to  redistribute it and/or modify
 *
*****************************************************************************************************************/

#ifndef _APP_AT_CMD_H_
#define _APP_AT_CMD_H_

#include "cli.h"
#include <stdint.h>
#include <stdbool.h>

#if(CHIP_TYPE >= CHIP_HERA_A)
#define MAX_GPIO_PIN_NUM 28
#else
#define MAX_GPIO_PIN_NUM 23
#endif
#define FLASH_SAVA_FLAG  1
#define FLASH_UNSAVA_FLAG  0
#define UN_USED_PARAMS (void)

#ifndef TSPT_RX_BUF_SIZE
#if defined(CONFIG_ALI_OS) || defined(CONFIG_TUYA_YUN)
#define TSPT_RX_BUF_SIZE      64
#else
#ifdef CONFIG_8266_AT_CMD
#define TSPT_RX_BUF_SIZE      4096 
#else
#define TSPT_RX_BUF_SIZE      2048 
#endif
#endif 
#endif

struct iot_ip_param{
      uint32_t static_ipaddr;
      uint32_t static_ipmask;
      uint32_t static_gwaddr;
};

enum ap_touch_monit_event{
	AP_TOUCH_DONE_EVNT,
	AP_TOUCH_END,
	AP_TOUCH_TIMEOUT
};

#define MAX_AP_TOUCH_MSG_LEN 128
#define MAX_AP_TOUCH_NOICE_LEN 4
#define MAX_AP_BSSID_LEN 17 /*aa:bb:cc:dd:ee:ff*/
enum{
AP_TOUCH_TYPE_AP_NOTIFY,	
AP_TOUCH_TYPE_NO_NOTIFY,
};
#define APTOUCH_AP_SSID "ATBM_AP"

struct ap_touch_monit_task_msg{
	enum ap_touch_monit_event event;
    uint8_t param1[MAX_AP_TOUCH_MSG_LEN];
};

extern  struct flash_boot_volatile_section iot_boot_volatile_sect;
extern  struct flash_boot_const_section iot_boot_const_sect;
extern  struct iot_ip_param ip_params_user_set;

/*wifi config*/
extern struct wifi_configure* hmac_cfg;
extern struct wifi_configure *softap_cfg;


/*********************************************************/
/*********************************************************/

void AT_SmartConfigStart(char *Args);
void UART_TSPT_SendStr(const char *str);
void UART_SendString(const char *fmt, ...);
void UART_Send_status(int ret);
int hexstr2bin(  uint8_t *buf, const char *hex, size_t len);
int str2mac(char *macstr, unsigned char *buf);
void AT_WDisConnect(char *pLine);
char * get_country_str(int country_id);	
int parse_channel_no_cmd(char *pLine, int *channel_no);
int CmdLine_GetSignInteger(char **pLine, int *pDword);
char * CmdLine_SkipSpace(char * line);
char * CmdLine_GetToken_String(char ** ppLine);
int CmdLine_GetInteger(char **pLine, uint32_t *pDword);

int atbm_wifi_ap_touch_start(int type);
void atbm_wifi_ap_touch_Stop(bool sync);

int AT_SmartStart_Proc(char *Args);
void AT_SmartStop_Proc(char *Args);

#endif /*_APP_AT_CMD_H_*/
