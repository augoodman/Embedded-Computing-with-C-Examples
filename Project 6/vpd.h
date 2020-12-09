/*******************************************************************************
 * vpd.h
 *
 * SER486 Project 2
 * Fall 2020
 * Written By:  Andrew Goodman (augoodma)
 *
 * This file provides function declarations for implementing non-volatile vital
 * product data for the IIoT device.
 */

#ifndef VPD_H_INCLUDED
#define VPD_H_INCLUDED


/* structs */
typedef struct vpd_struct{
    char token[4];
    char model[12];
    char manufacturer[12];
    char serial_number[12];
    unsigned long manufacure_date;
    unsigned char mac_address[6];
    char country_of_origin[4];
    unsigned char checksum;
} vpd_struct;

/* variables */
static vpd_struct p_vpd_struct;
static vpd_struct* vpd = &p_vpd_struct;


/* Initializes vpd member data from the EEPROM. */
void vpd_init();

#endif // VPD_H_INCLUDED

