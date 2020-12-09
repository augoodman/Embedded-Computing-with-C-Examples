/*******************************************************************************
 * eeprom.h
 *
 * SER486 Project 2
 * Fall 2020
 * Written By:  Andrew Goodman (augoodma)
 *
 * This file provides function declarations for implementing a hardware device
 * design pattern to encapsulate reading/writing of the ATMEGA 328P EEPROM.
 */

#ifndef EEPROM_H_INCLUDED
#define EEPROM_H_INCLUDED

/* Places the data (specified by buf and size) into the write buffer for later
* writing to the EEPROM.
*/
void eeprom_writebuf(unsigned int addr, unsigned char* buf, unsigned char s);

/* Reads a specified amount of data (s) from the EEPROM starting at a
* specified address (addr) and places it in the specified buffer (buf).
*/
void eeprom_readbuf(unsigned int addr, unsigned char* buf, unsigned char s);

/* Returns 0 if write_busy is 0, otherwise, returns 1. */
int eeprom_isbusy();

#endif // EEPROM_H_INCLUDED

