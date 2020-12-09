/********************************************************
 * main.c
 *
 * SER486 Assignment 4
 * Fall 2020
 * Written By:  Andrew Goodman (augoodma)
 *
 * This file provides a main program entry point for
 * SER486 homework assignment 3.
 */

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include "simpledelay.h"
#include "simpleled.h"
#include "temp.h"
#include "uart.h"

////////////////////////////////////////////////////////////////////////////////
//MACROS
#ifdef RELEASE
    #define RELEASE_FLAG 1
#else
    #define RELEASE_FLAG 0
#endif

/////////////////////////////////////////////////////////////////////////////////
//FUNCTIONS
/********************************************************
 * main(void)
 *
 * This function provides a main program entry point for
 * SER486 homework assignment 4.
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
 * NOTE: Return statement unreachable, so it is not used.
 */
int main(void)
{
    //init led
    led_init();
    //init uart
    uart_init();
    //init temp
    temp_init();
    //use uart to output class, hw4, name
    uart_writestr("SER 486 HW4 – Andrew Goodman\n\r");
    //infinite loop to display temp every sec
    while(1){
        if(RELEASE_FLAG)
            delay(1000);
        else
            delay(75);
        temp_start();
        temp_is_data_ready();
        uart_writedec32((unsigned int) temp_get());
        uart_writestr("\n\r");
    }
}
