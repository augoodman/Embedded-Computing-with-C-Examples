/*******************************************************************************
 * wdt.c
 *
 * SER486 Project 3
 * Fall 2020
 * Written By:  Andrew Goodman (augoodma)
 *
 * This file provides implementation for various utility functions for SER486.
 *
 * Functions are:
 *    void wdt_init() - Initializes the watchdog timer for a 2 second timeout
 *                  and interrupt+reset mode.
 *
 *    void wdt_reset() - Resets the watchdog timer so that it does not time out.
 *
 *    void wdt_force_restart() - Forces a system restart (reset only, no
 *                  interrupt) by disabling the watchdog interupt and waiting
 *                  for a watchdog timeout.
 *
 *    void __vector_6() - This function is enabled when the watchdog is
 *                  initialized. When invoked, the function turns on the LED,
 *                  adds a EVENT_WDT to the system event log and  attempts to
 *                  write back any modified log and configuration information
 *                  before  the second stage of the watchdog timer forces a
 *                  system reset.
 */

 /* includes */
#include "wdt.h"
#include "led.h"
#include "log.h"
#include "config.h"

/* macros */
#define MCUSR  (*((volatile unsigned char *)0x54))
#define SREG   (*((volatile unsigned char *)0x5F))
#define WDTCSR (*((volatile unsigned char *)0x60))
#define WDIE   6
#define WDCE   4
#define WDE    3
#define WDP2   2
#define WDP1   1
#define WDP0   0


 /* functions */
/*******************************************************************************
 * void wdt_init()
 *
 * Initializes the watchdog timer for a 2 second timeout and interrupt+reset
 * mode.
 *
 * arguments:
 *   none
 *
 * returns:
 *   nothing
 *
 * changes:
 *   WDTCSR
 *   SREG
 */
void wdt_init()
{
    //disable interrupts
    WDTCSR = 0;
    //watchdog reset
    wdt_reset();
    //start timed sequence
    WDTCSR |= (1<<WDCE) | (1<<WDE);
    //set new prescaler time out value (256k/2s)
    WDTCSR = (1<<WDIE) | (1<<WDE) | (1<<WDP2) | (1<<WDP1) | (1<<WDP0);
    //enable interrupts
    //SREG |= (1<<7);
    //WDTCSR |= (1<<WDE) | (1<<WDIE);
}

#pragma GCC push_options
#pragma GCC optimize ("Os")
/*******************************************************************************
 * void wdt_reset()
 *
 * Resets the watchdog timer so that it does not time out.
 *
 * arguments:
 *   none
 *
 * returns:
 *   nothing
 *
 * changes:
 *   none
 */
void wdt_reset()
{
    __builtin_avr_wdr();
}
#pragma GCC pop_options

/*******************************************************************************
 * wdt_force_restart()
 *
 * Forces a system restart (reset only, no interrupt) by disabling the watchdog
 * interupt and waiting for a watchdog timeout.
 *
 * arguments:
 *   none
 *
 * returns:
 *   nothing
 *
 * changes:
 *   ****update later****
 */
void wdt_force_restart()
{
    //disable interrupts
    WDTCSR = 0;
    //wait
    while(1);
}

/*******************************************************************************
 * void __vector_6()
 *
 * This function is enabled when the watchdog is initialized. When invoked, the
 * function turns on the LED, adds a EVENT_WDT to the system event log and
 * attempts to write back any modified log and configuration information before
 * the second stage of the watchdog timer forces a system reset.
 *
 * arguments:
 *   void
 *
 * returns:
 *   nothing
 *
 * changes:
 *   ****update later****
 */
void __vector_6(void) __attribute__ ((signal, used, externally_visible));
void __vector_6(void)
{
    int i;
    //turn on led
    led_on();
    //add watchdog timeout log entry
    log_add_record(EVENT_WDT);
    //write modified log entries to eeprom w/o isr
    for(i = 0; i < 16; i++)
        log_update_noisr();
    //write config data to eeprom w/o isr
    config_update_noisr();
}
