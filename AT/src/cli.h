#ifndef __CLI_H__
#define __CLI_H__


///#include "cmd_line.h"
#include <stdint.h>

#define PRIVATE static

struct cmd_struct{

	const char *cmd;
	int (*fn)(const char *argv);
	struct cmd_struct *next;
};

/***********************************************************************/
/***                        Data Types and Constants                 ***/
/***********************************************************************/




typedef struct CLI_COMMAND_DEF_S
{
    const char *    Name;
    const char *    HelpLine;
    /* See CmdLine_ReadMemory(), CmdLine_ReadMemory() and CmdLine_Dump() for example.
    */
    void            (*Handler)(char *Args);
}CLI_COMMAND_DEF;

struct cli_cmd_struct{

	char *cmd;
	void (*fn)(char *args);
	struct cli_cmd_struct *next;
};

extern void cli_add_cmd(struct cli_cmd_struct *cmd);
extern void cli_add_cmds(struct cli_cmd_struct *cmds, int len);

/***********************************************************************/
/***                        Public Functions                         ***/
/***********************************************************************/

/**************************************************************************
**
** NAME        CmdLine_GetToken
** 
** PARAMETERS:    *pLine -    current line location to parse.
**
** RETURNS:        the token located. It never be NULL, but can be "\0" 
**              *pLine - next line location to parse.              
**
** DESCRIPTION    Locate the next token from a cli.
**
**************************************************************************/
  char * CmdLine_GetToken(char ** pLine);


/**************************************************************************
**
** NAME        CmdLine_GetHex
** 
** PARAMETERS:  *pLine - the current line location to parse.
**
** RETURNS:        TRUE if the next token is a hexdecimal integer.
**              *pDword - the integer returned. Unchanged if return FALSE.
**              *pLine - next line location to parse.              
**
** DESCRIPTION    Read a hexdecimal integer from a cli.
**
**************************************************************************/
  int CmdLine_GetHex(char ** pLine, uint32_t *pDword);

/**************************************************************************
**
** NAME        CmdLine_GetInteger
** 
** PARAMETERS:  *pLine - the current line location to parse.
**
** RETURNS:        TRUE if the next token is an unsigned decimal integer.
**              *pDword - the integer returned. Unchanged if return FALSE.
**              *pLine - next line location to parse.              
**
** DESCRIPTION    Read an unsigned decimal integer from a cli.
**
**************************************************************************/
int CmdLine_GetInteger(char ** pLine, uint32_t *pDword);
void CmdLine_ProcessingInput(void);

int cli_process_cmd(char * Line);
void cli_add_cmd(struct cli_cmd_struct *cmd);
void cli_add_cmds(struct cli_cmd_struct *cmds, int len);
void cli_main(void);

#endif /* __CLI_H__ */
