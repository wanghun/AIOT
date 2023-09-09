/**************************************************************************************************************
 * altobeam iot at cmd file 
 *
 * Copyright (c) 2018, altobeam.inc   All rights reserved.
 * Permission is hereby granted for use on Altobeam IOT 6421 only,
 * in which case ,it is free to  redistribute it and/or modify
 *
*****************************************************************************************************************/

#include "app_AT_cmd.h"
#include <stdarg.h>
#include <string.h>

extern   void ATCmd_init_sys(void);
extern  void Command_Putc(uint8_t Data);

/*
uart transparent tx,
this function can't be close by hal_uart_dbg_close
*/
 void UART_TSPT_Sendbuff(char *str,int num)
{
    int i=0;

    for(i = 0; i<num;i++)
    {
        char ch;
        ch = *str;

        if (ch == '\n')
        {
            Command_Putc('\r');
        }
        //iot_printf("ch = %c \n",ch);

        Command_Putc(ch);

        str++;
        if (ch == '\n')
        {
            break;
        }
    }
}
/*
uart transparent string,
this function can't be close by hal_uart_dbg_close
*/
 void UART_TSPT_SendStr(const char *str)
{
    for (;;)
    {
        char ch;

        ch = *str;
        str++;

        if (ch == 0)
        {
            break;
        }

        if (ch == '\n')
        {
            Command_Putc('\r');

        }

        Command_Putc(ch);

    }
}

 void UART_SendString(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    //command_vprintf(fmt, args);
    //vprintf(fmt, args);
    va_end(args);
    //Command_SetPolling(0);

//    if(CMD_UART_INTR_NUM == CONSOLE_UART_INTR_NUM){
//        Console_SendDataInIrq();
//    }
}

 void UART_DIRECT_Sendbuff(char *buf, int num)
{
    int i=0;

    for(i = 0; i<num;i++)
    {
        char ch;
        ch = *buf;
        Command_Putc(ch);
        buf++;
    }

//    if(CMD_UART_INTR_NUM == CONSOLE_UART_INTR_NUM){
//        Console_SendDataInIrq();
//    }
}

 void UART_TSPT_SendData(char *header, int headerLen, char *data, int dataLen, char *tailer, int tailerLen)
{
    int i=0;
    for(i = 0; i<headerLen;i++)
    {
        Command_Putc(header[i]);
    }
    for(i = 0; i<dataLen;i++)
    {
        Command_Putc(data[i]);
    }
    for(i = 0; i<tailerLen;i++)
    {
        Command_Putc(tailer[i]);
    }

}

  void ATCmd_init(void)
{
#ifdef CONFIG_AT_CMD_SUPPORT
	ATCmd_init_sys();
#endif //CONFIG_AT_CMD_SUPPORT
}

  void ATCmd_loop(void)
{	
#ifdef CONFIG_AT_CMD_SUPPORT
	cli_main();
#endif //CONFIG_AT_CMD_SUPPORT
}
