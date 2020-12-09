/*******************************************************************************
 * vpd.c
 *
 * SER486 Project 2
 * Fall 2020
 * Written By:  Andrew Goodman (augoodma)
 *
 * This file provides implementation for non-volatile vital product data for the
 * IIoT device.
 *
 * Functions are:
 *    void vpd_init() - Initializes config member data from the EEPROM.
 *
 *    int vpd_is_data_valid() - Returns 1 (true) if the token and checksum is valid.
 *                  Otherwise, returns 0.
 *
 *    void vpd_write_defaults() - Called by init() to write the config “factory
 *                  defaults” to eeprom.
 */

/* includes */
#include "eeprom.h"
#include "util.h"
#include "vpd.h"

/* forward declarations */
int vpd_is_data_valid();
void vpd_write_defaults();

/* variables */
static vpd_struct defaults_struct;
static vpd_struct* defaults = &defaults_struct;

/* functions */
/*******************************************************************************
 * void vpd_init()
 *
 * Initializes vpd member data from the EEPROM.
 *
 * arguments:
 *   none
 *
 * returns:
 *   nothing
 *
 * changes:
 *   EEPROM
 */
void vpd_init(){
    // while busy, wait
    while(eeprom_isbusy()){}
    // initialize vpd data with eeprom.readbuf()
    eeprom_readbuf(0x0, (unsigned char *)defaults, sizeof(*defaults) - 1);
    if(!vpd_is_data_valid()){
        vpd_write_defaults();
        eeprom_readbuf(0x0, (unsigned char *)defaults, sizeof(*defaults) - 1);
    }
}

/*******************************************************************************
 * vpd_int is_data_valid()
 *
 * Returns 1 (true) if the token and checksum is valid. Otherwise, returns 0.
 *
 * arguments:
 *   none
 *
 * returns:
 *   int - validation boolean
 *
 * changes:
 *   none
 */
int vpd_is_data_valid(){
    int result = 1;
    if(vpd->token[0] != 'S' || vpd->token[1] != 'E' || vpd->token[2] != 'R')
        result = 0;
    if(!is_checksum_valid((unsigned char *)vpd, sizeof(*vpd)))
        result = 0;
    return result;
}

/*******************************************************************************
 * void write_defaults()
 *
 * Called by init() to write the “factory defaults” to eeprom.
 *
 * arguments:
 *   none
 *
 * returns:
 *   nothing
 *
 * changes:
 *   EEPROM
 */
void vpd_write_defaults(){
    defaults->token[0] = 'S';
    defaults->token[1] = 'E';
    defaults->token[2] = 'R';
    defaults->token[3] = '\0';
    defaults->model[0] = 'A';
    defaults->model[1] = 'n';
    defaults->model[2] = 'd';
    defaults->model[3] = 'r';
    defaults->model[4] = 'e';
    defaults->model[5] = 'w';
    defaults->model[6] = '\0';
    defaults->manufacturer[0] = 'G';
    defaults->manufacturer[1] = 'o';
    defaults->manufacturer[2] = 'o';
    defaults->manufacturer[3] = 'd';
    defaults->manufacturer[4] = 'm';
    defaults->manufacturer[5] = 'a';
    defaults->manufacturer[6] = 'n';
    defaults->manufacturer[7] = '\0';
    defaults->serial_number[0] = '4';
    defaults->serial_number[1] = '2';
    defaults->serial_number[2] = '\0';
    defaults->manufacure_date = 0;
    defaults->mac_address[0] = (0x41 & 0xFE);
    defaults->mac_address[1] = 0x6E;
    defaults->mac_address[2] = 0x64;
    defaults->mac_address[3] = 0x47;
    defaults->mac_address[4] = 0x6F;
    defaults->mac_address[5] = 0x6F;
    defaults->country_of_origin[0] = 'U';
    defaults->country_of_origin[1] = 'S';
    defaults->country_of_origin[2] = 'A';
    defaults->country_of_origin[3] = '\0';
    defaults->checksum = 0;
    update_checksum((unsigned char *)defaults, sizeof(*defaults));
    eeprom_writebuf(0x0, (unsigned char *)defaults, sizeof(*defaults));
}
