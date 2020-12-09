/********************************************************
 * main.c
 *
 * SER486 Assignment 3
 * Fall 2020
 * Written By:  Andrew Goodman
 * Modified By:
 *
 * this file provides a main program entry point for
 * SER486 homework assignment 3.
 */

#include "hw4lib.h"
#include "output.h"

/********************************************************
 * main(void)
 *
 * This function provides a main program entry point for
 * SER486 homework assignment 3.
 *
 * arguments:
 *   void
 *
 * returns:
 *   int - return code
 *
 * changes:
 *   No direct state changes.
 *
 * NOTE: Return statement was unreachable, so it was
 * removed.
 */
int main(void)
{
    uart_init();
    writestr("SER486 HW3 - Andrew Goodman\n\r");
    writehex8(0x0A);
    writestr("\n\r");
    writehex16(0xC0DE);
    writestr("\n\r");
    blink_led("--- -.-");
    while(1);
}
