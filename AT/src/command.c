/**************************************************************************************************************
 * altobeam iot user app file 
 *
 * Copyright (c) 2018, altobeam.inc   All rights reserved.
 * Permission is hereby granted for use on Altobeam IOT 6421 only,
 * in which case ,it is free to  modify it
 *
*****************************************************************************************************************/

#include "command.h"
#include <string.h>

/***********************************************************************/
/***                Internally Visible Functions                     ***/
/***********************************************************************/

extern void CmdLine_SetEvent(void);


int CMD_UART_Send(uint8_t Data)
{
    return 0;
}

int CMD_UART_Recv(uint8_t * pData)
{
    return 0;
}

CMD_LOCAL Command;
/**************************************************************************
**
** NAME     UART_Recv
**
** PARAMETERS:
**
** RETURNS:     The data read.
**
** DESCRIPTION  Read a byte from the UART port.
**
** NOTES:       None.
**************************************************************************/
void Command_Init(void)
{   
    //iot_printf("Command_Init \n");
    memset(&Command,0,sizeof(Command));
	Command.Polling = 1;
}

 int Command_ClearRxFifo()
{
    Command.RxGet=0;
    Command.RxPut=0;
    return TRUE;
}

/**************************************************************************
**
** NAME     Command_Putc
**
** PARAMETERS:  Data
**
** RETURNS:
**
** DESCRIPTION  Send a byte to the UART port.
**
** NOTES:       None.
**************************************************************************/
 void Command_Putc(uint8_t Data)
{
/*(
    if (Command.Polling)
    {
    	while(!CMD_UART_Send(Data))
        {
        }
    }
    else
*/	
    {
        uint32_t  TxPut;
        TxPut = Command.TxPut;
        if ((TxPut - Command.TxGet) < CMD_TX_BUF_SIZE)
        {
            Command.TxBuffer[TxPut & (CMD_TX_BUF_SIZE -1)] = Data;
            TxPut++;
            Command.TxPut = TxPut;
            if (Data == '\n' || Data == 0x1b)
            {
                Command_SendDataInIrq();
            }
        }
    }
	Command_SetPolling(Command.Polling);
}

/**************************************************************************
**
** NAME         Command_SendDataInIrq
**
** PARAMETERS:  void
**
** RETURNS:     void
**
** DESCRIPTION  Write to the tx fifo, if data is avaiable.
**
**************************************************************************/
 void Command_SendDataInIrq(void)
{

    uint32_t  TxGet;

    TxGet = Command.TxGet;
    for (;;)
    {
        if (TxGet == Command.TxPut)
        {
            break;
        }

        if (!CMD_UART_Send(Command.TxBuffer[TxGet & (CMD_TX_BUF_SIZE -1)]))
        {
            break;
        }

        TxGet++;
    }
    Command.TxGet = TxGet;
}


/**************************************************************************
**
** NAME         Command_SetPolling
**
** PARAMETERS:  Polling: TRUE/FALSE
**
** RETURNS:     void
**
** DESCRIPTION  Turn on/off polling mode.
**
**************************************************************************/

enum CMD_POLL_TYPE{
	COMMAND_NO_POLL 		= 0,
	COMMAND_MAY_NOT_POLL 	= 1,
	COMMAND_MAY_POLL 		= 2,
	COMMAND_MUST_POLL	 	= 3,
};

void Command_SetPolling(int Polling)
{
	

    
}

/**************************************************************************
**
** NAME         Cmmand_TxBufferSpace
**
** PARAMETERS:
**
** RETURNS:     TX buffer space
**
** DESCRIPTION  Get TX buffer space.
**              In non-polling mode, sending more characters than this
**              number may cause the character lost.
**
**************************************************************************/
unsigned Command_TxBufferSpace(void)
{
    unsigned TxPending;

    TxPending = Command.TxPut - Command.TxGet;

    return CMD_TX_BUF_SIZE - TxPending;
}

/**************************************************************************
**
** NAME     Command_ReadDataInIrq
**
** PARAMETERS:
**
** RETURNS:
**
** DESCRIPTION  Receive data in the UART IRQ context.
**
** NOTES:       None.
**************************************************************************/
 void Command_ReadDataInIrq(void)
{
	//iot_printf("Command_ReadDataInIrq\n");
	unsigned RxPut;

    RxPut = Command.RxPut;

#ifdef QT_RUN
    uint8_t Data = '\n';
    strcpy((char*)Command.RxBuffer,"help\r\n");
    Command.LastChar = Data;
    //Command.RxBuffer[RxPut & (CMD_RX_BUF_SIZE -1)] = (uint8_t)Data;
    RxPut = strlen((char*)Command.RxBuffer);
    Command.RxPut = RxPut;

#else

    int i, k = 0;
    static unsigned pre_RxPut;
    static unsigned back_flag = 0;
    static unsigned is_null_flag = 0;

    while((int32_t)(RxPut - Command.RxGet) < CMD_RX_BUF_SIZE)
	{
        uint8_t Data;

		if (!CMD_UART_Recv(&Data))
		{
			break;
		}

		Command.LastChar = Data;
        Command.RxBuffer[RxPut & (CMD_RX_BUF_SIZE -1)] = (uint8_t)Data;
		RxPut++;
		Command.RxPut = RxPut; 
		if((Data == '\n') || (Data == '\r'))
		{
			//iot_printf(" CmdLine_SetEvent---\n");
			CmdLine_SetEvent();
        }
	}
#endif //QT_RUN
	//fix when at cmd too fast bug,
//    if((int32_t)(Command.RxPut - Command.RxGet)>=(CMD_RX_BUF_SIZE-4)){

//	}
}


/**************************************************************************
**
** NAME            Command_ReadRxFifo
**
** PARAMETERS:    None
**
** RETURNS:        TRUE if a char is available.
**
** DESCRIPTION    Read RX FIFO
**
**************************************************************************/
int Command_ReadRxFifo(char *pChar)
{
    unsigned RxGet;

    RxGet = Command.RxGet;
    if (RxGet == Command.RxPut)
    {
        return FALSE;
    }
    *pChar = (char)Command.RxBuffer[RxGet & (CMD_RX_BUF_SIZE -1)];
    Command.RxBuffer[RxGet & (CMD_RX_BUF_SIZE -1)] = (uint8_t)0;
    RxGet++;
    Command.RxGet = RxGet;

    return TRUE;
}
int Command_PeekRxFifo(char *pChar)
{

    unsigned RxGet;

    RxGet = Command.RxGet;
    if (RxGet == Command.RxPut)
    {
        return FALSE;
    }
    *pChar = (char)Command.RxBuffer[RxGet & (CMD_RX_BUF_SIZE -1)];
    return TRUE;
}

int Command_RxFifoEmpte()
{
	if (Command.RxGet == Command.RxPut)
		return 1;
	else
		return 0;
}

 int Command_RxFifoFull()
{
	 if((int)(Command.RxPut - Command.RxGet) < CMD_RX_BUF_SIZE){
		return 0;
	 }
	 else {
		return 1;
	 }

}

 void Command_WriteRxFifo(char Data)
{
   if(Command_RxFifoFull()==0){
        Command.RxBuffer[Command.RxPut & (CMD_RX_BUF_SIZE -1)] = (uint8_t)Data;
	    Command.RxPut++;
    }
}

 void Command_SetIndication(void)
{

}
