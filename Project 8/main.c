/*******************************************************************************
 * main.c
 *
 * SER486 Final Project
 * Fall 2020
 * Written By:  Andrew Goodman (augoodma)
 *
 * This file provides a main program entry point for SER486 final project.
 */

/* includes */
#include "config.h"
#include "delay.h"
#include "dhcp.h"
#include "led.h"
#include "log.h"
#include "rtc.h"
#include "spi.h"
#include "uart.h"
#include "vpd.h"
#include "temp.h"
#include "socket.h"
#include "alarm.h"
#include "wdt.h"
#include "tempfsm.h"
#include "eeprom.h"
#include "ntp.h"
#include "w51.h"
#include "signature.h"

/* macros */
#define HTTP_PORT       8080	/* TCP port for HTTP */
#define SERVER_SOCKET   0

/* enums */
enum state
{
    SOCKET_EST,
    GET_DEV_RESP,
    PUT_DEV_RESP,
    PUT_CONF_RESP,
    DEL_LOG_RESP,
    FLUSH,
    ERROR_RESP
} state;

/* globals */
int current_temperature = 75;
const char* get_device  = "GET /device";
const char* put_device  = "PUT /device?reset=";
const char* put_config  = "PUT /device/config?";
const char* del_log     = "DELETE /device/log";
const char* get_200     = "HTTP/1.1 200 OK\nContent-Type: application/vnd.api+json\nConnection: close\r\n";
const char* p_d_200     = "HTTP/1.1 200 OK\nConnection: close\r\n";
const char* gpd_400     = "HTTP/1.1 400 Resource not found\nConnection: close\r\n";
const char* crlf        = "\r\n";
enum state;
unsigned char buf;
int num, num_logs, error=0;


/* functions */
/*******************************************************************************
 * main(void)
 *
 * This function provides a main program entry point for SER486 final project.
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
   /* Initialize the hardware devices
    * uart
    * led
    * vpd
    * config
    * log
    * rtc
    * spi
    * temp sensor
    * W51 Ethernet controller
    * tempfsm
    */
    uart_init();
    led_init();
    vpd_init();
    config_init();
    log_init();
    rtc_init();
    spi_init();
    temp_init();
    W5x_init();
    tempfsm_init();

   /* sign the assignment
    * Asurite is the first part of your asu email (before the @asu.edu
    */
    signature_set("goodman","andrew","augoodma");

    /* configure the W51xx ethernet controller prior to DHCP */
    unsigned char blank_addr[] = {192,168,1,1};
    W5x_config(vpd.mac_address, blank_addr, blank_addr, blank_addr);

    /* loop until a dhcp address has been gotten */
    while (!dhcp_start(vpd.mac_address, 60000UL, 4000UL)) {}
    uart_writestr("local ip: ");uart_writeip(dhcp_getLocalIp());

    /* configure the MAC, TCP, subnet and gateway addresses for the Ethernet controller*/
    W5x_config(vpd.mac_address, dhcp_getLocalIp(), dhcp_getGatewayIp(), dhcp_getSubnetMask());

	/* add a log record for EVENT_TIMESET prior to synchronizing with network time */
	log_add_record(EVENT_TIMESET);

    /* synchronize with network time */
    ntp_sync_network_time(5);

    /* add a log record for EVENT_NEWTIME now that time has been synchronized */
    log_add_record(EVENT_NEWTIME);

    /* start the watchdog timer */
    wdt_init();

    /* log the EVENT STARTUP and send and ALARM to the Master Controller */
    log_add_record(EVENT_STARTUP);
    alarm_send(EVENT_STARTUP);

    /* request start of test if 'T' key pressed - You may run up to 3 tests per
     * day.  Results will be e-mailed to you at the address asurite@asu.edu
     */
    check_for_test_start();

    /* start the first temperature reading and wait 5 seconds before reading again
    * this long delay ensures that the temperature spike during startup does not
    * trigger any false alarms.
    */
    temp_start();
    delay_set(0,5000);
    //unsigned char* buf = 0;
    while (1) {
        /* reset  the watchdog timer every loop */
        wdt_reset();
        /* update the LED blink state */
        led_update();
        /* if the temperature sensor delay is done, update the current temperature
        * from the temperature sensor, update the temperature sensor finite state
        * machine (which provides hysteresis) and send any temperature sensor
        * alarms (from FSM update).
        */

        if(temp_is_data_ready())
        {
            /* read the temperature sensor */
            current_temperature=temp_get();
            /* update the temperature fsm and send any alarms associated with it */
            tempfsm_update(current_temperature,config.hi_alarm,config.hi_warn,config.lo_alarm,config.lo_warn);
            /* restart the temperature sensor delay to trigger in 1 second */
            delay_set(0,1000);
        }
        /*if the server socket is closed */
        if(socket_is_closed(SERVER_SOCKET))
        {
            /* if socket is closed, open it in passive (listen) mode */
            socket_open(SERVER_SOCKET,HTTP_PORT);
            socket_listen(SERVER_SOCKET);
            state = SOCKET_EST;
        }
        /* if there is input to process */
        if (socket_is_established(SERVER_SOCKET))
        {
            /* parse and process any pending commands */
            switch(state)
            {
            case SOCKET_EST:
                if(socket_recv_compare(SERVER_SOCKET,get_device))
                {
                    state=GET_DEV_RESP;
                }
                 if(socket_recv_compare(SERVER_SOCKET,put_device))
                {

                    state=PUT_DEV_RESP;

                }
                else if(socket_recv_compare(SERVER_SOCKET,put_config))
                {

                    state=PUT_CONF_RESP;
                }
                else if(socket_recv_compare(SERVER_SOCKET,del_log))
                {
                    state=DEL_LOG_RESP;
                }
                break;
            case GET_DEV_RESP:
                num_logs = log_get_num_entries();
                unsigned long i,time = 0;
                unsigned char event_num = 0;
                socket_writestr(SERVER_SOCKET, get_200);
                socket_writestr(SERVER_SOCKET, crlf);
                socket_writechar(SERVER_SOCKET,'{');
                socket_writequotedstring(SERVER_SOCKET,"vpd");
                socket_writestr(SERVER_SOCKET,":{");
                socket_writequotedstring(SERVER_SOCKET,"model");
                socket_writechar(SERVER_SOCKET,':');
                socket_writequotedstring(SERVER_SOCKET,vpd.model);
                socket_writechar(SERVER_SOCKET,',');
                socket_writequotedstring(SERVER_SOCKET,"manufacturer");
                socket_writechar(SERVER_SOCKET,':');
                socket_writequotedstring(SERVER_SOCKET,vpd.manufacturer);
                socket_writechar(SERVER_SOCKET,',');
                socket_writequotedstring(SERVER_SOCKET,"serial_number");
                socket_writechar(SERVER_SOCKET,':');
                socket_writequotedstring(SERVER_SOCKET,vpd.serial_number);
                socket_writechar(SERVER_SOCKET,',');
                socket_writequotedstring(SERVER_SOCKET,"manufacture_date");
                socket_writechar(SERVER_SOCKET,':');
                socket_writedate(SERVER_SOCKET,vpd.manufacture_date);
                socket_writechar(SERVER_SOCKET,',');
                socket_writequotedstring(SERVER_SOCKET,"mac_address");
                socket_writechar(SERVER_SOCKET,':');
                socket_write_macaddress(SERVER_SOCKET,vpd.mac_address);
                socket_writechar(SERVER_SOCKET,',');
                socket_writequotedstring(SERVER_SOCKET,"country_code");
                socket_writechar(SERVER_SOCKET,':');
                socket_writequotedstring(SERVER_SOCKET,vpd.country_of_origin);
                socket_writestr(SERVER_SOCKET,"},");
                socket_writequotedstring(SERVER_SOCKET,"tcrit_hi");
                socket_writechar(SERVER_SOCKET,':');
                socket_writedec32(SERVER_SOCKET,config.hi_alarm);
                socket_writechar(SERVER_SOCKET,',');
                socket_writequotedstring(SERVER_SOCKET,"twarn_hi");
                socket_writechar(SERVER_SOCKET,':');
                socket_writedec32(SERVER_SOCKET,config.hi_warn);
                socket_writechar(SERVER_SOCKET,',');
                socket_writequotedstring(SERVER_SOCKET,"tcrit_lo");
                socket_writechar(SERVER_SOCKET,':');
                socket_writedec32(SERVER_SOCKET,config.lo_alarm);
                socket_writechar(SERVER_SOCKET,',');
                socket_writequotedstring(SERVER_SOCKET,"twarn_lo");
                socket_writechar(SERVER_SOCKET,':');
                socket_writedec32(SERVER_SOCKET,config.lo_warn);
                socket_writechar(SERVER_SOCKET,',');
                socket_writequotedstring(SERVER_SOCKET,"temperature");
                socket_writechar(SERVER_SOCKET,':');
                socket_writedec32(SERVER_SOCKET,current_temperature);
                socket_writechar(SERVER_SOCKET,',');
                socket_writequotedstring(SERVER_SOCKET,"state");
                socket_writechar(SERVER_SOCKET,':');
                socket_writequotedstring(SERVER_SOCKET,"NORMAL");
                socket_writestr(SERVER_SOCKET,"},");
                socket_writequotedstring(SERVER_SOCKET,"log");
                socket_writestr(SERVER_SOCKET,":[");
                for(i = 0; i < num_logs-1; i++)
                {
                    log_get_record(i,&time,&event_num);
                    socket_writechar(SERVER_SOCKET,'{');
                    socket_writequotedstring(SERVER_SOCKET,"timestamp");
                    socket_writechar(SERVER_SOCKET,':');
                    socket_writedate(SERVER_SOCKET,time);
                    socket_writechar(SERVER_SOCKET,',');
                    socket_writequotedstring(SERVER_SOCKET,"event");
                    socket_writechar(SERVER_SOCKET,':');
                    socket_writedec32(SERVER_SOCKET,event_num);
                    socket_writechar(SERVER_SOCKET,',');
                }
                log_get_record(num_logs-1,&time,&event_num);
                socket_writechar(SERVER_SOCKET,'{');
                socket_writequotedstring(SERVER_SOCKET,"timestamp");
                socket_writechar(SERVER_SOCKET,':');
                socket_writedate(SERVER_SOCKET,time);
                socket_writechar(SERVER_SOCKET,',');
                socket_writequotedstring(SERVER_SOCKET,"event");
                socket_writechar(SERVER_SOCKET,':');
                socket_writedec32(SERVER_SOCKET,event_num);
                socket_writestr(SERVER_SOCKET, "}]}");
                socket_writestr(SERVER_SOCKET, crlf);
                state=SOCKET_EST;
                socket_disconnect(0);
                break;
            case PUT_DEV_RESP:
                if(socket_recv_compare(SERVER_SOCKET,"\"true\""))
                {
                    socket_writestr(SERVER_SOCKET, p_d_200);
                    socket_writestr(SERVER_SOCKET, crlf);
                    state=SOCKET_EST;
                    socket_disconnect(0);
                    while(1);
                }
                else if(socket_recv_compare(SERVER_SOCKET,"\"false\""))
                {
                    socket_writestr(SERVER_SOCKET, p_d_200);
                    socket_writestr(SERVER_SOCKET, crlf);
                    state=SOCKET_EST;
                    socket_disconnect(0);
                }
                else
                {
                    state=FLUSH;
                }
            case PUT_CONF_RESP:
                if(socket_recv_compare(SERVER_SOCKET,"tcrit_hi="))
                {
                    socket_recv_int(SERVER_SOCKET,&num);
                    if(num>config.hi_warn)
                    {
                        config.hi_alarm = num;
                        tempfsm_update(current_temperature,config.hi_alarm,config.hi_warn,config.lo_alarm,config.lo_warn);
                        socket_writestr(SERVER_SOCKET, p_d_200);
                        socket_writestr(SERVER_SOCKET, crlf);
                        state=SOCKET_EST;
                        socket_disconnect(0);
                    }
                    else state=FLUSH;
                }
                else if(socket_recv_compare(SERVER_SOCKET,"twarn_hi="))
                {
                    socket_recv_int(SERVER_SOCKET,&num);
                    if(num>config.lo_warn && num<config.hi_alarm)
                    {
                        config.hi_warn = num;
                        tempfsm_update(current_temperature,config.hi_alarm,config.hi_warn,config.lo_alarm,config.lo_warn);
                        socket_writestr(SERVER_SOCKET, p_d_200);
                        socket_writestr(SERVER_SOCKET, crlf);
                        state=SOCKET_EST;
                        socket_disconnect(0);
                    }
                    else state=FLUSH;
                }
                else if(socket_recv_compare(SERVER_SOCKET,"tcrit_lo="))
                {
                    socket_recv_int(SERVER_SOCKET,&num);
                    if(num<config.lo_warn)
                    {
                        config.lo_alarm = num;
                        tempfsm_update(current_temperature,config.hi_alarm,config.hi_warn,config.lo_alarm,config.lo_warn);
                        socket_writestr(SERVER_SOCKET, p_d_200);
                        socket_writestr(SERVER_SOCKET, crlf);
                        state=SOCKET_EST;
                        socket_disconnect(0);
                    }
                    else state=FLUSH;
                }
                else if(socket_recv_compare(SERVER_SOCKET,"twarn_lo="))
                {
                    socket_recv_int(SERVER_SOCKET,&num);
                    if(num>config.lo_alarm && num<config.hi_warn)
                    {
                        config.lo_warn = num;
                        tempfsm_update(current_temperature,config.hi_alarm,config.hi_warn,config.lo_alarm,config.lo_warn);
                        socket_writestr(SERVER_SOCKET, p_d_200);
                        socket_writestr(SERVER_SOCKET, crlf);
                        state=SOCKET_EST;
                        socket_disconnect(0);
                    }
                    else state=FLUSH;
                }
                else state=FLUSH;
                break;
            case DEL_LOG_RESP:
                log_clear();
                socket_writestr(SERVER_SOCKET, p_d_200);
                socket_writestr(SERVER_SOCKET, crlf);
                socket_disconnect(0);
                state=SOCKET_EST;
                break;
            case FLUSH:
                socket_flush_line(SERVER_SOCKET);
                state=ERROR_RESP;
                break;
            case ERROR_RESP:
                socket_writestr(SERVER_SOCKET, gpd_400);
                socket_writestr(SERVER_SOCKET, crlf);
                socket_disconnect(0);
                state=SOCKET_EST;
                break;
            default:
                state = FLUSH;
            }
        }
        /* otherwise... */
        else
        {
            /* update any pending log write backs */
            log_update();
            /* update any pending config write backs */
            config_update();
        }
    }
	return 0;
}
