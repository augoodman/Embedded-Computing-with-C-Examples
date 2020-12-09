/*******************************************************************************
 * main.c
 *
 * SER486 Project 1
 * Fall 2020
 * Written By:  Andrew Goodman (augoodma)
 *
 * This file provides a main program entry point for SER486 project 1.
 */

/* includes */
#include "delay.h"
#include "led.h"
#include "timer1.h"
#include "rtc.h"
#include "uart.h"
#ifdef RELEASE
    #define RELEASE_FLAG 1
#else
    #define RELEASE_FLAG 0
#endif

/* functions */
/*******************************************************************************
 * main(void)
 *
 * This function provides a main program entry point for SER486 project 1.
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
    //init uart, led, rtc
    uart_init();
    led_init();
    rtc_init();
    //set led blink pattern to "--- -.-"
    led_set_blink("--- -.-");
    //init rtc date/time to "01/01/2019 00:00:00"
    rtc_set_by_datestr("01/01/2019 00:00:00");
    //print assignment and author info to console
    uart_writestr("SER 486 Project 1 - Andrew Goodman\n\r");
    //get baseline performance
    signed long c1 = 0;
    if(RELEASE_FLAG) delay_set(1, 10000);
    else delay_set(1, 300);
    while (!delay_isdone(1))
    {
        c1++;
    }
    //measure performance with led update
    signed long c2 = 0;
    if(RELEASE_FLAG) delay_set(1, 10000);
    else delay_set(1, 300);
    while (!delay_isdone(1))
    {
        led_update();
        c2++;
    }
    //display results
    uart_writedec32(c1);
    uart_writestr(" ");
    uart_writedec32(c2);
    uart_writestr("\r\n");
    while(1)
    {
        //update led blink FSM each iteration (every 500ms)
        if(RELEASE_FLAG) delay_set(1, 500);
        else delay_set(1, 50);
        if(!delay_isdone(1))
        {
            uart_writestr(rtc_get_date_string());
            uart_writechar('\r');
        }
    }
}
