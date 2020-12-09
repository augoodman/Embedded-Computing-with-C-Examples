/********************************************************
 * output.c
 *
 * SER486 Assignment 4
 * Spring 2018
 * Written By:  Doug Sandy (instructor)
 * Modified By:
 *
 * this file implements functions associated with SER486
 * homework assignment 4.  The procedures implemented
 * provide led and console output support for debugging
 * of future SER 486 assignments.
 *
 * functions are:
 *    writestr(char *)  - a function to write an ascii
 *                        null-terminated string to the
 *                        avr atmega 328P uart port.
 *                        (instructor provided)
 *
 *    writehex8(unsigned char) - a function to write the
 *                        hexadecimal representation of
 *                        an 8-bit unsigned integer to
 *                        avr atmega 328P uart port.
 *
 *    writehex16(unsigned int) - a function to write the
 *                        hexadecimal representation of
 *                        a 16-bit unsigned integer to
 *                        avr atmega 328P uart port.
 *
 *    blink_led(char *) - a function to send a specific
 *                        blink pattern to the development
 *                        board's user-programmable LED.
 *
 *    delay(unsigned int) - delay code execution for
 *                        the specified amount of milliseconds.
 *                        (instructor provided)
 */

 #include "hw4lib.h"

 /* macro definitions used by delay() */
 #define MSEC_PER_SEC     1000
 #define CLOCKS_PER_LOOP  16
 #define LOOPS_PER_MSEC   ((F_CPU/MSEC_PER_SEC)/CLOCKS_PER_LOOP)
 #define PORTB            (*((volatile unsigned char *)0x24))
 #define PINB             (*((volatile unsigned char *)0x23))
 #ifdef RELEASE
    #define RELEASE_FLAG 1
 #else
    #define RELEASE_FLAG 0
 #endif

/**********************************
 * delay(unsigned int msec)
 *
 * This code delays execution for for
 * the specified amount of milliseconds.
 *
 * arguments:
 *   msec - the amount of milliseconds to delay
 *
 * returns:
 *   nothing
 *
 * changes:
 *   nothing
 *
 * NOTE: this is not ideal coding practice since the
 * amount of time spent in this delay may change with
 * future hardware changes.  Ideally a timer should be
 * used instead of a set of for loops.  Timers will be
 * taught later in the semester.
 */
void delay(unsigned int msec)
{
    volatile unsigned i,j ;  /* volatile prevents loops from being optimized away */

    /* outer loop, loops for specified number of milliseconds */
    for (i=0; i<msec; i++) {
        /* inner loop, loops for 1 millisecond delay */
        for (j=0;j<LOOPS_PER_MSEC;j++) {}
    }
}


/**********************************
 * writestr(char * str)
 *
 * This code writes a null-terminated string
 * to the UART.
 *
 * arguments:
 *   str - a pointer to the beginning of the
 *         string to be printed.
 *
 * returns:
 *   nothing
 *
 * changes:
 *   the state of the uart transmit buffer will
 *   be changed by this function.
 *
 * NOTE: uart_init() should be called this function
 *   is invoked for the first time or unpredictable
 *   UART behavior may result.
 */
void writestr(char * str)
{
    unsigned int i;

    /* loop for each character in the string */
    for (i=0; str[i]!=0;i++) {
        /* output the character to the UART */
        uart_writechar(str[i]);
    }
}

/************************************************************
* STUDENT CODE BELOW THIS POINT
*************************************************************/

/********************************************************
 * writehex8(unsigned char num)
 *
 * This function outputs an 8 bit input to the console
 * in its ASCII hexadecimal representation.
 *
 * arguments:
 *   num - the hexadecimal value to be converted to
 *   ASCII.
 *
 * returns:
 *   nothing
 *
 * changes:
 *   No direct state changes.
 */
void writehex8(unsigned char num)
{
    unsigned char temp = num;
    char hex[] = "0123456789ABCDEF";
    int i, ii;
    for(i = 0; temp >= 16; i++)
    {
        temp -= 16;
    }
    uart_writechar(hex[i]);
    temp = num<<4;
    for(ii = 0; temp >= 16; ii++)
    {
        temp -= 16;
    }
    uart_writechar(hex[ii]);
}

/********************************************************
 * writehex16(unsigned int num)
 *
 * This function outputs an 16 bit input to the console
 * in its ASCII hexadecimal representation.
 *
 * arguments:
 *   num - the hexadecimal value to be converted to
 *   ASCII.
 *
 * returns:
 *   nothing
 *
 * changes:
 *   No direct state changes.
 */
void writehex16(unsigned int num)
{
    writehex8(num>>8);
    writehex8(num);
}

/********************************************************
 * void blink_led(char *msg)
 *
 * This function signals a Morse Code string using the
 * LED.
 *
 * arguments:
 *   msg - the Morse Code string to be signaled.
 *
 * returns:
 *   nothing
 *
 * changes:
 *   The states of PINB and PORTB will be changed by
 *   this function.
 */
void blink_led(char *msg)
{
    PORTB |= 0x2;
    int i = 0;
    if(RELEASE_FLAG == 1){
        while(msg[i] != '\0'){
            if(msg[i] == '-'){
                PINB = 0x2;
                delay(750);
                PINB = 0x2;
                delay(100);
            }
            if(msg[i] == '.'){
                PINB = 0x2;
                delay(250);
                PINB = 0x2;
                delay(100);
            }
            if(msg[i] == ' '){
                delay(1000);
            }
            i++;
        }
    }
    else{
        while(msg[i] != '\0'){
            if(msg[i] == '-'){
                PINB = 0x2;
                delay(40);
                PINB = 0x2;
                delay(5);
            }
            if(msg[i] == '.'){
                PINB = 0x2;
                delay(15);
                PINB = 0x2;
                delay(5);
            }
            if(msg[i] == ' '){
                delay(50);
            }
            i++;
        }
    }
}
