/**************************************************************************************************************
 * altobeam iot user app file 
 *
 * Copyright (c) 2018, altobeam.inc   All rights reserved.
 * Permission is hereby granted for use on Altobeam IOT 6421 only,
 * in which case ,it is free to  modify it
 *
*****************************************************************************************************************/


#include "cli.h"
#include "app_AT_cmd.h"
#include "command.h"
#include <stdio.h>
#include <string.h>

//if enable uart echo back
#if defined(CONFIG_UART_ECHO)
uint32_t Uart_TSPT_Echo_flag = 1;
#else //CONFIG_UART_ECHO
uint32_t_t Uart_TSPT_Echo_flag = 0;
#endif //CONFIG_UART_ECHO

/***********************************************************************/
/***               Internally Visible Static Data                    ***/
/***********************************************************************/
typedef struct CMDLINE_LOCAL_S
{
    unsigned            InpCount;
	unsigned            getCount;
//#ifdef CONFIG_ANGEL
//	char                Line[1024];
//#else
    char                Line[240];
//#endif
    struct cli_cmd_struct *cmds;
}CMDLINE_LOCAL; 

PRIVATE void cli_help(char *arg);
PRIVATE  void CmdLine_ReadMemory(char *arg);
PRIVATE  void CmdLine_WriteMemory(char *arg);

PRIVATE    CMDLINE_LOCAL CmdLine = { 0 };

PRIVATE struct cli_cmd_struct GenericCommands[] =
{
	{.cmd ="help",	.fn = cli_help,		.next = (void*)0 },		
	{.cmd ="AT+rmem",	  /* "rmem [h_addr][d_size]",  */ .fn = CmdLine_ReadMemory ,		.next = (void*)0},
	{.cmd ="AT+wmem",	  /* "wmem [h_addr][h_data]",  */  .fn = CmdLine_WriteMemory,		.next = (void*)0},
};

extern void ATCmd_init(void);
void cli_main(void)
{
    cli_add_cmds(&GenericCommands[0],sizeof(GenericCommands)/sizeof(GenericCommands[0]));
    ATCmd_init();
    Command_ReadDataInIrq();
    //cli_help(NULL);
	while (1) {	
		CmdLine_ProcessingInput();
     }

}

/**/
 void CmdLine_SetEvent()
{
    //OS_APP_SCHED_IRQ();
}

/**************************************************************************
**
** NAME            CmdLine_GetLine
**
** PARAMETERS:    None
**
** RETURNS:        The line read. NULL - if a full line is not ready.
**
** DESCRIPTION    Read cli
**
**************************************************************************/
char last_r_n=0;
 char * CmdLine_GetLine(void)
{
    unsigned    Count;
	
    Count = CmdLine.InpCount;
    while(Count < sizeof(CmdLine.Line))
    {
        char Ch;

		if (!Command_ReadRxFifo(&Ch))
		{
			break;
		}

		if ( (Ch == '\r') || (Ch == '\n') || (Ch == 0x1B) || (Count == (sizeof(CmdLine.Line)-1)) )
		{
			CmdLine.Line[Count] = 0;
			CmdLine.InpCount = 0;
			last_r_n = Ch;

			if(Uart_TSPT_Echo_flag == 1)
            {
#ifdef QT_RUN
                printf(">");
                printf(CmdLine.Line);
                printf("\n");
#else
				UART_TSPT_SendStr(">");
				UART_TSPT_SendStr(CmdLine.Line);
				UART_TSPT_SendStr("\n");
#endif  //QT_RUN
			}
			CmdLine.getCount = Count;
			return CmdLine.Line;
		}

        CmdLine.Line[Count] = Ch;
        Count++;
    }
    CmdLine.InpCount = Count;

    return NULL;
}



//static struct cli_cmd_struct *cmds;

PRIVATE void cli_help(char *arg)
{
    if (Uart_TSPT_Echo_flag)
	{
		struct cli_cmd_struct *cmd = CmdLine.cmds;

		UART_TSPT_SendStr( "\nSupported commands:\n");
        do {
        #ifdef QT_RUN
            printf(cmd->cmd);
            printf("\n");
        #else
            UART_TSPT_SendStr(cmd->cmd);
            UART_TSPT_SendStr("\n");
        #endif //QT_RUN
			cmd = cmd->next;

		} while (cmd);
	}
	return;
}



  void cli_add_cmd(struct cli_cmd_struct *cmd)
{
	cmd->next = CmdLine.cmds;
	CmdLine.cmds = cmd;
}

 void cli_add_cmds(struct cli_cmd_struct *cmds, int len)
{
	int i;

	for (i = 0; i < len; i++)
		cli_add_cmd(cmds++);
}

  int cli_process_cmd(char * Line)
{  
	struct cli_cmd_struct *p = CmdLine.cmds;
    char *      Token;
    //iot_printf("cli_process_cmd \n");
    Token = CmdLine_GetToken(&Line);

    if (Token[0] == 0)
    {
		if(CmdLine.getCount > 0){
			UART_TSPT_SendStr( "+ERR\n");
		}
        return -1;
    }

	while (p) {

        if (strcmp(Token, p->cmd) == 0){
			Line = CmdLine_SkipSpace(Line);
			p->fn(Line);
            //Command_SetIndication();
			return 1;
		}

		p = p->next;
	}

	UART_TSPT_SendStr( "+ERR\n");
    //DBG_Printf( "Unknown cmd: %s\n", Token);
	return 0;
}



/**************************************************************************
**
** NAME     CmdLine_ProcessingInput
**
** PARAMETERS:
**
** RETURNS:
**
** DESCRIPTION  Processing input and execute commands in non-IRQ context.
**
**************************************************************************/
  void CmdLine_ProcessingInput(void)
{
	//not powersave 20s,when AT cmd 
    //CmdLine_DelayPowerSave();

    /* Don't know how much we are going to print. So set polling.
          We are in non-IRQ context. It is OK to poll - other event handlers
          are suspended anyway.
       */
    //Command_SetPolling(TRUE);

    for (;;)
    {
        char * Line;
		//char *pdata;
		//DBG_Printf("%s uart_net.get_tx_data_falg:%d \n",__func__, uart_net.get_tx_data_falg);

		 Line = CmdLine_GetLine();
      	 if(Line == NULL)
       	 {            
            break;
         }
         cli_process_cmd(Line);			

    }    
	
    //Command_SetPolling(FALSE);
}


/**************************************************************************
**
** NAME            CmdLine_ReadMemory
**
** PARAMETERS:    None
**
** RETURNS:
**
** DESCRIPTION    Process "mem" command
**
**************************************************************************/
 PRIVATE  void CmdLine_ReadMemory(char *arg)
{
    uint32_t ReadMemAddr;
    uint32_t ReadMemSize;
    CmdLine_GetHex(&arg, &ReadMemAddr);
    CmdLine_GetInteger(&arg, &ReadMemSize);
    if (ReadMemSize == 0)
    {
        ReadMemSize = 256;
    }
	if(ReadMemSize > 1024){
		ReadMemSize = 1024;
	}
    ReadMemAddr &= (~0x03);
//    DBG_Printf("Memory at %08X:\n", ReadMemAddr);
//    DBG_PrintDwords((uint32_t *)ReadMemAddr, ReadMemSize);
	UART_Send_status(0);
}


/**************************************************************************
**
** NAME            CmdLine_WriteMemory
**
** PARAMETERS:    None
**
** RETURNS:
**
** DESCRIPTION    Process "fill" command
**
**************************************************************************/
 PRIVATE  void CmdLine_WriteMemory(char *arg)
{
    uint32_t  Addr;

    if (CmdLine_GetHex(&arg, &Addr))
    {
        uint32_t  Data;

        Addr &= (~0x03);
        if (CmdLine_GetHex(&arg, &Data))
        {
            *(uint32_t *)Addr = Data;
            //DBG_Printf("  [%08x] <= %08x\n", Addr, Data);
			UART_Send_status(0);
            return;
        }
    }
	UART_Send_status(0);
}


