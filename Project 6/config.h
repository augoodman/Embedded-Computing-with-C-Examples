/*******************************************************************************
 * config.h
 *
 * SER486 Project 2
 * Fall 2020
 * Written By:  Andrew Goodman (augoodma)
 *
 * This file provides function declarations for implementing non-volatile
 * configuration data for the IIOT device.
 */

#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

/* structs */
typedef struct config_struct{
    char token[4];
    unsigned int hi_alarm;
    unsigned int hi_warn;
    unsigned int lo_alarm;
    unsigned int lo_warn;
    char use_static_ip;
    unsigned char static_ip[4];
    unsigned char checksum;
} config_struct;

/* variables */
config_struct p_config_struct;
static config_struct* config = &p_config_struct;

/* Initializes config member data from the EEPROM. */
void config_init();

/* If config has been modified and eeprom is not busy, write all of the
* configuration data to the eeprom write buffer.
*/
void config_update();

/* Set the modified flag. */
void config_set_modified();

#endif // CONFIG_H_INCLUDED
