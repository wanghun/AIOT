/*
 * command.h
 *
 *  Created on: 2019-1-9
 *      Author: NANXIAOFENG
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include <stdint.h>


#define TRUE	    1
#define FALSE	   0

//#include "lmac_os.h"
#if defined(CONFIG_ALI_OS) || defined(CONFIG_TUYA_YUN)


/*
 *  In buffering mode, the sender should check the buffer space before printing the line
 *  CMD_TX_BUF_SIZE must be power of 2
 */
#ifndef CMD_TX_BUF_SIZE
#define CMD_TX_BUF_SIZE            64 // good for printing 3 lines
#endif

/***********************************************************************/
/***                        Constants                                ***/
/***********************************************************************/
/* If a script input file is used, a comment will be striped immediately.
 * Assuming a raw command has 25 bytes, 512 buffer size can handle 20 commands.
 * Yuo may define CONSOLE_RX_BUF_SIZE in sysconfig.h. It must be power of 2.
*/
#ifndef CMD_RX_BUF_SIZE
#define CMD_RX_BUF_SIZE            128
#endif
#else //free rtos CONFIG_ALI_OS
/*
 *  In buffering mode, the sender should check the buffer space before printing the line
 *  CMD_TX_BUF_SIZE must be power of 2
 */
#ifndef CMD_TX_BUF_SIZE
#define CMD_TX_BUF_SIZE            512 // good for printing 3 lines
#endif

#ifdef CONFIG_SPI_SLAVE_FUNC
#ifndef SPI_CMD_TX_BUF_SIZE
#define SPI_CMD_TX_BUF_SIZE            4096 // good for printing 3 lines
#endif
#endif

/***********************************************************************/
/***                        Constants                                ***/
/***********************************************************************/
/* If a script input file is used, a comment will be striped immediately.
 * Assuming a raw command has 25 bytes, 512 buffer size can handle 20 commands.
 * Yuo may define CONSOLE_RX_BUF_SIZE in sysconfig.h. It must be power of 2.
*/
#if defined(CONFIG_8266_AT_CMD) || defined(CONFIG_SHENGTENG)
#ifndef CMD_RX_BUF_SIZE
#define CMD_RX_BUF_SIZE            4096
#endif
#else	//CONFIG_8266_AT_CMD
#ifndef CMD_RX_BUF_SIZE
#define CMD_RX_BUF_SIZE            2048
#endif
#endif	//CONFIG_8266_AT_CMD
#endif  //#ifdef CONFIG_ALI_OS

typedef struct CMD_LOCAL_S
{
    int                 Polling;
    volatile  uint32_t     TxGet;
    uint32_t              TxPut;
    uint8_t            TxBuffer[CMD_TX_BUF_SIZE];

    volatile  uint32_t     RxPut;
    uint32_t              RxGet;
    uint8_t               LastChar;
    uint8_t               Padding[3];
    uint8_t               RxBuffer[CMD_RX_BUF_SIZE];
}CMD_LOCAL;

/*****************************************************************************/
/* UART Function Declarations                                                */
/*****************************************************************************/

void Command_Putc(uint8_t Data);
unsigned Command_TxBufferSpace(void);
int Command_ReadRxFifo(char *pChar);
int Command_ClearRxFifo();
void Command_SetPolling(int Polling);
int Command_RxFifoFull();
void Command_WriteRxFifo(char Data);
int Command_PeekRxFifo(char *pChar);
int Command_RxFifoEmpte();
void Command_ReadDataInIrq(void);
void Command_Init(void);
void Command_SendDataInIrq(void); 
void Command_SetIndication(void);
#endif /* COMMAND_H_ */
