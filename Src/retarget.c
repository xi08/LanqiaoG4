/*
 * Copyright (C) ARM Limited, 2006. All rights reserved.
 * 
 * This is a retargeted I/O example which implements the functions required
 * for communication through an UART. The implementation relies on two UART
 * functions which the user must provide (UART_write and UART_read) for 
 * sending and receiving single characters to and from the UART.
 *
 * See the "rt_sys.h" header file for complete function descriptions.
 */
 
#include <rt_sys.h>
#include "usart.h"
#ifdef __cplusplus
extern "C" {
#endif
 
#if !defined(__MICROLIB)

#if defined ( __CC_ARM )
#pragma import(__use_no_semihosting)
#elif defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
__asm (".global __use_no_semihosting\n\t");
#endif

#define DEFAULT_HANDLE 0x100;
 

char UART_read(void)
{
  return 0;
}

void UART_write(char ch)
{
  HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,-1);
}
 
 
/*
 * These names are special strings which will be recognized by 
 * _sys_open and will cause it to return the standard I/O handles, instead
 * of opening a real file.
 */
const char __stdin_name[] ="STDIN";
const char __stdout_name[]="STDOUT";
const char __stderr_name[]="STDERR";
 
/*
 * Open a file. May return -1 if the file failed to open. We do not require
 * this function to do anything. Simply return a dummy handle.
 */
FILEHANDLE _sys_open(const char * name, int openmode)
{
    return DEFAULT_HANDLE;  
}
 
/*
 * Close a file. Should return 0 on success or a negative value on error.
 * Not required in this implementation. Always return success.
 */
int _sys_close(FILEHANDLE fh)
{
    return 0; //return success
}
 
/*
 * Write to a file. Returns 0 on success, negative on error, and
 * the number of characters _not_ written on partial success.
 * `mode' exists for historical reasons and must be ignored.
 * 
 * This implementation sends a buffer of size 'len' to the UART.
 */
int _sys_write(FILEHANDLE fh, const unsigned char * buf,
               unsigned len, int mode)
{
    int i;
    for(i=0;i<len;i++) 
    {
        UART_write(buf[i]);
    
        // Fix for HyperTerminal    
        if(buf[i]=='\n') UART_write('\r');
    }
    
    return 0;   
}
 
/*
 * Read from a file. Can return:
 *  - zero if the read was completely successful
 *  - the number of bytes _not_ read, if the read was partially successful
 *  - the number of bytes not read, plus the top bit set (0x80000000), if
 *    the read was partially successful due to end of file
 *  - -1 if some error other than EOF occurred
 * This function receives a character from the UART, processes the character
 * if required (backspace) and then echo the character to the Terminal 
 * Emulator, printing the correct sequence after successive keystrokes.  
 */
int _sys_read(FILEHANDLE fh, unsigned char * buf,
              unsigned len, int mode)
{
    int pos=0;
    
    do {
        buf[pos]=UART_read();
        
        // Advance position in buffer
        pos++;
        
        // Handle backspace
        if(buf[pos-1] == '\b') 
        {
            // More than 1 char in buffer
            if(pos>1)
            {
                // Delete character on terminal
                UART_write('\b');
                UART_write(' ');
                UART_write('\b');         
                
                // Update position in buffer
                pos-=2;   
            }
            else if (pos>0) pos--; // Backspace pressed, empty buffer
        }
        else UART_write(buf[pos-1]); // Echo normal char to terminal
        
        
    }while(buf[pos-1] != '\r');
    
    buf[pos]= '\0'; // Ensure Null termination
 
    return 0;       
}
 
/*
 * Writes a character to the output channel. This function is used
 * for last-resort error message output.
 */
void _ttywrch(int ch)
{
    // Convert correctly for endianness change
    char ench=ch;
    
    UART_write(ench);
}
 
/*
 * Return non-zero if the argument file is connected to a terminal.
 */
int _sys_istty(FILEHANDLE fh)
{
    return 1; // no interactive device present
}
 
/*
 * Move the file position to a given offset from the file start.
 * Returns >=0 on success, <0 on failure. Seeking is not supported for the 
 * UART.
 */
int _sys_seek(FILEHANDLE fh, long pos)
{
    return -1; // error
}
 
/*
 * Flush any OS buffers associated with fh, ensuring that the file
 * is up to date on disk. Result is >=0 if OK, negative for an
 * error.
 */
int _sys_ensure(FILEHANDLE fh)
{
    return 0; // success
}
 
/*
 * Return the current length of a file, or <0 if an error occurred.
 * _sys_flen is allowed to reposition the file pointer (so Unix can
 * implement it with a single lseek, for example), since it is only
 * called when processing SEEK_END relative fseeks, and therefore a
 * call to _sys_flen is always followed by a call to _sys_seek.
 */
long _sys_flen(FILEHANDLE fh)
{
    return 0;
}
 
/*
 * Return the name for temporary file number sig in the buffer
 * name. maxlen is the maximum name length allowed.
 *
 * Return value is 0 on success, or negative on failure (similarly to
 * Unix system calls, and also matching the SYS_TMPNAM semihosting
 * call). If this function returns failure, it need not write anything
 * into the output buffer at all.
 */
int _sys_tmpnam2(char * name, int sig, unsigned maxlen)
{
    return -1; // fail, not supported
}
 
/*
 * Terminate the program, passing a return code back to the user.
 * This function may not return.
 */
void _sys_exit(int returncode)
{
    while(1) {};
}
#else

#if defined(__CC_ARM)
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#elif defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#elif defined(__GNUC__)
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#error Not Supported
#endif

PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, -1);
  return ch;
}

#endif
 
#ifdef __cplusplus
}
#endif