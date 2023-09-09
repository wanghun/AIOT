/**************************************************************************************************************
 * altobeam iot at cmd file 
 *
 * Copyright (c) 2018, altobeam.inc   All rights reserved.
 * Permission is hereby granted for use on Altobeam IOT 6421 only,
 * in which case ,it is free to  redistribute it and/or modify
 *
*****************************************************************************************************************/

#include "app_AT_cmd.h"
#include "at_def.h"


extern void Console_SetPolling(int Polling);


 char ConvertHexChar(char ch)
{
	if (ch >= '0' && ch <= '9')
		return ch - '0';
	if (ch >= 'a' && ch <= 'f')
		return ch - 'a' + 10;
	if (ch >= 'A' && ch <= 'F')
		return ch - 'A' + 10;
	return -1;
}

 int hex2byte(const char *hex)
{
	int a, b;
	a = ConvertHexChar(*hex++);
	if (a < 0)
		return -1;
	b = ConvertHexChar(*hex++);
	if (b < 0)
		return -1;
	return (a << 4) | b;
}


/**
 * hexstr2bin - Convert ASCII hex string into binary data
 * @hex: ASCII hex string (e.g., "01ab")
 * @buf: Buffer for the binary data
 * @len: Length of the text to convert in bytes (of buf); hex will be double
 * this size
 * Returns: 0 on success, -1 on failure (invalid hex string)
 */
 int hexstr2bin(uint8_t *buf, const char *hex, size_t len)
{
	size_t i;
	int a;
	const char *ipos = hex;
    uint8_t *opos = (uint8_t *)buf;

	for (i = 0; i < len; i += 2) {
		a = hex2byte(ipos);
		if (a < 0)
			return -1;
		*opos++ = a;
		ipos += 2;
	}
	return 0;
}

 int str2mac(char *macstr, unsigned char *buf)
{
	unsigned char  i = 0;
	char tmp = 0;
	for(i =0; i< 5; i++)
	{
		tmp = ConvertHexChar(*macstr++);
		if(tmp < 0)
		{
			return -1;
		}
		buf[i] = tmp*16;
		tmp = ConvertHexChar(*macstr++);
		if(tmp < 0)
		{
			return -1;
		}
		buf[i] = buf[i]+tmp;
		macstr++;
	}
	tmp= ConvertHexChar(*macstr++);
	if(tmp < 0)
	{
		return -1;
	}
	buf[5] = tmp*16;
	tmp = ConvertHexChar(*macstr);
	if(tmp < 0)
	{
		return -1;
	}
	buf[5] = buf[5]+tmp;
	return 0;
}

 int str2mac_new(char *macstr, unsigned char *buf)
{
	unsigned char  i = 0;
	char tmp = 0;
	for(i =0; i< 6; i++)
	{
		tmp = ConvertHexChar(*macstr++);
		if(tmp < 0)
		{
			return -1;
		}
		buf[i] = tmp*16;
		tmp = ConvertHexChar(*macstr++);
		if(tmp < 0)
		{
			return -1;
		}
		buf[i] = buf[i]+tmp;
	}
	return 0;
}



   char * CmdLine_SkipSpace(char * line)
{
    char ch;
	int loop =0;
	

    /* escape white space */
    ch = line[0];
    while(ch != 0)
    {
        /* CmdLine_GetLine() has already replaced '\n' '\r' with 0 */
        if ( (ch == ' ') || (ch == ',') || (ch == '\t') || (ch == '='))
        {
            line++;
            ch = line[0];
			//闂冨弶顒汚T cmd 鐡掑﹦鏅�?	    	loop++;
			if(loop > 1600)
				break;
			
            continue;
        }
        break;
    }
    return line;
}

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
   char * CmdLine_GetToken(char ** pLine)
{
    char *    str;
    char *    line;
    char ch;
	int loop =0;
	
    line = *pLine;

    /* escape white space */
    ch = line[0];
    while(ch != 0)
    {
        /* CmdLine_GetLine() has already replaced '\n' '\r' with 0 */
        if ( (ch == ' ') || (ch == ',') || (ch == '\t') || (ch == '='))
        {
            line++;
            ch = line[0];
			//闂冨弶顒汚T cmd 鐡掑﹦鏅�?	    	loop++;
			if(loop > 1600)
				break;
			
            continue;
        }
        break;
    }

    str = line;
    while(ch != 0)
    {
        if ( (ch == ' ') || (ch == ',') || (ch == '\t') || (ch == '='))
        {
            line[0] = 0;
            /* CmdLine_GetLine() has replaced '\n' '\r' with 0, so we can do line++ */
            line++;
            break;
        }
        line++;
        ch = line[0];
		//闂冨弶顒汚T cmd 鐡掑﹦鏅�?    	loop++;
		if(loop > 1600)
			break;
    }

    *pLine = line;

    return str;
}
/*get string between " " */
char * CmdLine_GetToken_String(char ** ppLine)
{
	
	char *	  str;
	char *	  pLine;
	int index = 0;
	int first_flag =0;
	
	pLine = *ppLine;
	
	
	pLine = CmdLine_SkipSpace(pLine);
	if(pLine[0] == '"'){
		first_flag=1;
		index=1;
		str = &pLine[1];
		while(pLine[index] != 0)
		{
			if (pLine[index] == '"')
			{
				first_flag++;
				//if (first_flag == 1)
				//{
				//	str = &pLine[index + 1];
				//}else
				//{
					pLine[index] = 0;
					index++;
					break;
				//}
			}
			index++;
		}
	}
		
	if (first_flag == 2)
		pLine += index;
	else{
		str = CmdLine_GetToken(&pLine);
	}
	*ppLine=pLine;
	
	return str;
}


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
   int CmdLine_GetHex(char **pLine, uint32_t *pDword)
{
    char *  str;
    char *  str0;
    int     got_hex;
    uint32_t  d = 0;

    str = CmdLine_GetToken(pLine);
    if (str[0] == 0)
    {
        return FALSE;
    }

    str0 = str;
    got_hex = FALSE;
    for (;;)
    {
        char    ch;

        ch = str[0];
        if (ch == 0)
        {
            break;
        }
        if (ch >= '0' && ch <= '9')
        {
            d = (d<<4) | (ch - '0');
        }
        else if (ch >= 'a' && ch <= 'f')
        {
            d = (d<<4) | (ch - 'a' + 10);
        }
        else if (ch >= 'A' && ch <= 'F')
        {
            d = (d<<4) | (ch - 'A' + 10);
        }
        else
        {
            got_hex = FALSE;
            break;
        }
        got_hex = TRUE;
        str++;
    }
    if (got_hex)
    {
        *pDword = d;
    }
    else
    {
        //DBG_Printf("Invalid hexdecimal: %s\n", str0);
    }

    return got_hex;
}


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
    int CmdLine_GetInteger(char **pLine, uint32_t *pDword)
{
    char *  str;
    char *  str0;
    int     got_int;
    uint32_t  d = 0;
	int     negative = 0;

    str = CmdLine_GetToken(pLine);
    if (str[0] == 0)
    {
        return FALSE;
    }

    str0 = str;
    got_int = FALSE;
    for (;;)
    {
        char    ch;

        ch = str[0];
        if (ch == 0)
        {
            break;
        }

        if(ch == '-')
        {
            negative = 1;
            str++;
            continue;
        }

        if (ch >= '0' && ch <= '9')
        {
            d = d*10 + (ch - '0');
            got_int = TRUE;
            str++;
        }
        else
        {
            got_int = FALSE;
            break;
        }
    }

    if(negative == 1)
    {
        d = -d;
    }

    if (got_int)
    {
        *pDword = d;
    }
    else
    {
        //DBG_Printf("Invalid unsigned decimal: %s\n", str0);
    }

    return got_int;
}


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
** DESCRIPTION    Read an signed decimal integer from a cli.
**
**************************************************************************/
    int CmdLine_GetSignInteger(char **pLine, int *pDword)
{
    char *  str;
    char *  str0;
    int     got_int;
	int negativeFlag = 0;
    int  d = 0;

    str = CmdLine_GetToken(pLine);
    if (str[0] == 0)
    {
        return FALSE;
    }

    str0 = str;
    got_int = FALSE;
    for (;;)
    {
        char    ch;

        ch = str[0];
        if (ch == 0)
        {
            break;
        }
		if((ch == '-') && (str0 == str))
		{
			negativeFlag = -1;
            str++;
		}else if (ch >= '0' && ch <= '9')
        {
            d = d*10 + (ch - '0');
            got_int = TRUE;
            str++;
        }
        else
        {
            got_int = FALSE;
            break;
        }
    }
    if (got_int)
    {
    	if (negativeFlag < 0)
        	*pDword = d * negativeFlag;
    	else
    		*pDword = d;	
    }
    else
    {
        //DBG_Printf("Invalid unsigned decimal: %s\n", str0);
    }

    return got_int;
}



/**************************************************************************
**
** NAME        CmdLine_GetDecimalFraction_x10
**
** PARAMETERS:  *pLine - the current line location to parse.
**
** RETURNS:        TRUE if the next token is an unsigned decimal integer.
**           
**              *pDword - the decimal fraction returned. Unchanged if return FALSE.
**              *pLine - next line location to parse.
**
** DESCRIPTION    Read an unsigned decimal fraction from a cli.
**
**************************************************************************/
    int CmdLine_GetDecimalFraction_x10(char **pLine, uint32_t *pDword)
{
    char *  str;
    char *  str0;
    int     got_int;
    uint32_t  d = 0;
    uint32_t  hasDot = 0;

    str = CmdLine_GetToken(pLine);
    if (str[0] == 0)
    {
        return FALSE;
    }

    str0 = str;
    got_int = FALSE;
    for (;;)
    {
        char    ch;

        ch = str[0];
        if (ch == 0)
        {
            break;
        }
        if (ch >= '0' && ch <= '9')
        {
            d = d*10 + (ch - '0');
            got_int = TRUE;
            str++;
        }
		else if(ch == '.')
		{
            str++;
			hasDot = 1;
			continue;
		}
        else
        {
            got_int = FALSE;
            break;
        }
    }
    if (got_int)
    {
    	if(hasDot == 0)
        	*pDword = d*10;
		else
			*pDword = d;
    }
    else
    {
        //DBG_Printf("Invalid unsigned decimal Fraction: %s\n", str0);
    }

    return got_int;
}

// int parse_mac_cmd(char *pLine, unsigned char *buf)
//{
//	char * str;
//	unsigned char str_len = 0;
//	int ret = 0;
//	if (!memcmp("ADDR",pLine,strlen("ADDR")))
//	{
//		str = CmdLine_GetToken(&pLine);
//		str = CmdLine_GetToken(&pLine);
//		str_len = (unsigned char)strlen(str);
//		if(str_len < 17)
//		{
//			return -1;
//		}
//		ret = str2mac(str,buf);

//	}
//	else
//	{
//		return -1;
//	}
//	return ret;
//}
#define AT_CMD_SET_RETURN_STATUS 1
	
 void UART_Send_status(int ret)
{   
	char *strTmp0  = "+OK\n";
	char *strTmp1  = "+ERR\n";	
	char *strTmp2  = "+UNSUPPORT\n";	

	if(ret == 0)
	{
		UART_SendString(strTmp0);
	}
	else if(ret == HAL_ERR_NOT_PRESENT)
	{
		UART_SendString(strTmp2);
	}else
	{
		UART_SendString(strTmp1);
	}
}

