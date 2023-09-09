/**************************************************************************************************************
 * altobeam iot at cmd file 
 *
 * Copyright (c) 2018, altobeam.inc   All rights reserved.
 * Permission is hereby granted for use on Altobeam IOT 6421 only,
 * in which case ,it is free to  modify it
 *
*****************************************************************************************************************/


#include "app_AT_cmd.h"
#include <stdio.h>

 void AT_reboot(char *Args)
{
	UN_USED_PARAMS(*Args);
}

 void AT_GetVer(char *Args)
{	
	UN_USED_PARAMS(*Args);
	char strTmp[64] = {0};	

//	CmdLine_GetPlatformVar(strTmp, sizeof(strTmp));
//	UART_SendString("%s", strTmp);
    printf("VER 3.3.3\n");
}


 void AT_GetSdkVer(char *Args)
{	
	UN_USED_PARAMS(*Args);
	char strTmp[64] = {0};	

    //sprintf(strTmp, "SDK_VER:%s,TIME:%s %s\n", IOT_SDK_VER, __DATE__, __TIME__);
    //UART_SendString("%s", strTmp);
    printf("SDK_VER 3.3.3\n");
#ifdef CONFIG_SHENGTENG
	UART_Send_status(0);
#endif
}



extern const char SYS_SwTitle[];
 void AT_GetSysStatus(char *Args)
{	
	UN_USED_PARAMS(*Args);
	
//	UART_SendString("\n\n%s\n",SYS_SwTitle);
//	UART_SendString("atbm_iot SDK version %s\n",IOT_SDK_VER);
//	UART_SendString("flash map size %dMbyte\n",FLASH_SIZE);
//	UART_SendString("OS	: freeRTOS\n");
//	UART_SendString("wifi	: 11n 1X1 20M\n");
//    AT_WifiStatus(NULL);
//	UART_SendString("\ncpu	: 160Mhz\n");
//	UART_SendString("memory total: %d byte\n",sys_mem_total_size_get());
//	UART_SendString("memory free: %d byte\n",hal_sys_mem_free_size_get());
//	UART_SendString("TCPIP	: LWIP\n");
//	UART_SendString("lwip config: lwipopts.h\n");
}

/*
use this AT cmd must ,get sntp  one time

*/


 void AT_Help(char *Args)
{	
	UN_USED_PARAMS(*Args);
	char strTmp[64] = {0};	

//	CmdLine_GetPlatformVar(strTmp, sizeof(strTmp));
//	UART_SendString("%s\n", strTmp);
}


struct cli_cmd_struct ATCommands_System[] =
{
	{.cmd ="AT+HELP",			.fn = AT_Help,			 .next = (void*)0},
	{.cmd ="AT+GET_VER",		.fn = AT_GetVer,		 .next = (void*)0},
    {.cmd ="AT+GET_SDK_VER",	.fn = AT_GetSdkVer,		 .next = (void*)0},
};



void ATCmd_init_sys(void)
{
	 cli_add_cmds(ATCommands_System,sizeof(ATCommands_System)/sizeof(ATCommands_System[0]));
 
}
