/*******************************************************************************
 * config.c
 *
 * SER486 Project 2
 * Fall 2020
 * Written By:  Andrew Goodman (augoodma)
 *
 * This file provides implementation for non-volatile configuration data for the
 * IIOT device.
 *
 * Functions are:
 *    void config_init() - Initializes config member data from the EEPROM.
 *
 *    void config_update() - If config has been modified and eeprom is not busy, write
 *                  all of the configuration data to the eeprom write buffer.
 *
 *    void config_set_modified() - Set the modified flag.
 *
 *    int config_is_data_valid() - Returns 1 (true) if the token and checksum is valid.
 *                  Otherwise, returns 0.
 *
 *    void config_write_defaults() - Called by init() to write the config “factory
 *                  defaults” to eeprom.
 */

/* includes */
#include "eeprom.h"
#include "util.h"
#include "config.h"

/* forward declarations */
int config_is_data_valid();
void config_write_defaults();

/* variables */
static config_struct defaults_struct;
static config_struct* defaults = &defaults_struct;

static unsigned char modified;

/* functions */
/*******************************************************************************
 * void config_init()
 *
 * Initializes config member data from the EEPROM.
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
void config_init(){
    // while busy, wait
    while(eeprom_isbusy()){}
    // initialize config with eeprom.readbuf()
    eeprom_readbuf(0x040, (unsigned char *)defaults, sizeof(*defaults) - 1);
    if(!config_is_data_valid()){
        config_write_defaults();
        eeprom_readbuf(0x040, (unsigned char *)defaults, sizeof(*defaults) - 1);
        modified = 0;
    }
}

/*******************************************************************************
 * void config_update()
 *
 * If config has been modified and eeprom is not busy, write all of the
 * configuration data to the eeprom write buffer.
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
void config_update(){
    // while busy, wait
    while(eeprom_isbusy()){}
    if(modified){
        update_checksum((unsigned char *)config, sizeof(*config));
        eeprom_writebuf(0x040, (unsigned char *)config, sizeof(*config) - 1);
        modified = 0;
    }
}

/*******************************************************************************
 * void config_set_modified()
 *
 * Set the modified flag.
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
void config_set_modified(){
    modified = 1;
}

/*******************************************************************************
 * config_int is_data_valid()
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
int config_is_data_valid(){
    int result = 1;
    if(config->token[0] != 'A' || config->token[1] != 'S' || config->token[2] != 'U')
        result = 0;
    if(!is_checksum_valid((unsigned char *)config, sizeof(*config)))
        result = 0;
    return result;

}

/*******************************************************************************
 * void config_write_defaults()
 *
 * Called by init() to write the config “factory defaults” to eeprom.
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
void config_write_defaults(){
    defaults->token[0] = 'A';
    defaults->token[1] = 'S';
    defaults->token[2] = 'U';
    defaults->token[3] = '\0';
    defaults->hi_alarm = 0x3FF;
    defaults->hi_warn = 0x3FE;
    defaults->lo_alarm = 0x0000;
    defaults->lo_warn = 0x0001;
    defaults->use_static_ip = '0';
    defaults->static_ip[0] = 0xC0;
    defaults->static_ip[1] = 0xA8;
    defaults->static_ip[2] = 0x1;
    defaults->static_ip[3] = 0x64;
    defaults->checksum = 0;
    update_checksum((unsigned char *)defaults, sizeof(*defaults));
    eeprom_writebuf(0x040, (unsigned char *)defaults, sizeof(*defaults));
}
