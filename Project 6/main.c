/*******************************************************************************
 * main.c
 *
 * SER486 Project 2
 * Fall 2020
 * Written By:  Andrew Goodman (augoodma)
 *
 * This file provides a main program entry point for SER486 project 2.
 */

/* includes */
#include "config.h"
#include "delay.h"
#include "eeprom.h"
#include "led.h"
#include "log.h"
#include "rtc.h"
#include "timer1.h"
#include "uart.h"
#include "util.h"
#include "vpd.h"
#define VPD    ((unsigned int) 0)
#define CONFIG ((unsigned int) 0X040)

/* functions */
/*******************************************************************************
 * main(void)
 *
 * This function provides a main program entry point for SER486 project 2.
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
    //init uart, config, led, log, rtc, vpd
    uart_init();
    config_init();
    led_init();
    log_init();
    rtc_init();
    vpd_init();
    int dumped = 0;
    //set led blink pattern to "--- -.-"
    led_set_blink("--- -.-");
    //init rtc date/time to "01/01/2019 00:00:00"
    rtc_set_by_datestr("01/01/2019 00:00:00");
    //print assignment and author info to console
    uart_writestr("SER 486 Project 2 - Andrew Goodman\n\r");
    //get model, manufacturer and token
    eeprom_readbuf(VPD, (unsigned char *)vpd, sizeof(*vpd) - 1);
    eeprom_readbuf(CONFIG, (unsigned char *)config, sizeof(*config) - 1);
    uart_writestr(vpd->model);
    uart_writestr("\n\r");
    uart_writestr(vpd->manufacturer);
    uart_writestr("\n\r");
    uart_writestr(config->token);
    uart_writestr("\n\r");
    //set use static ip to 1
    config->use_static_ip = '1';
    eeprom_writebuf(CONFIG, (unsigned char *)config, sizeof(*config));
    //clear event log and add records: 0xAA, 0xBB, 0xCC
    log_clear();
    log_add_record(0xAA);
    log_add_record(0xBB);
    log_add_record(0xCC);
    while(1){
        led_update();
        log_update();
        config_update();
        if ((!eeprom_isbusy()) && (!dumped)) {
            dump_eeprom(0,0x100);
            dumped=1;
        }
    }
}

