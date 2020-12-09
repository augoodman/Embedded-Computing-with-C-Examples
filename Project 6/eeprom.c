/*******************************************************************************
 * eeprom.c
 *
 * SER486 Project 2
 * Fall 2020
 * Written By:  Andrew Goodman (augoodma)
 *
 * This file implements a hardware device design pattern to encapsulate
 * reading/writing of the ATMEGA 328P EEPROM.
 *
 * Functions are:
 *    void eeprom_writebuf(unsigned int addr, unsigned char* buf, unsigned char size) -
 *                  Places the data (specified by buf and size) into the write
 *                  buffer for later writing to the EEPROM.
 *
 *    void eeprom_readbuf(unsigned int addr, unsigned char* buf, unsigned char size) -
 *                  Reads a specified amount of data (s) from the EEPROM
 *                  starting at a specified address (addr) and places it in
 *                  the specified buffer (buf).
 *
 *    int eeprom_isbusy() - Returns 0 if write_busy is 0, otherwise, returns 1.
 *
 *    void __vector_22() - Enabled when writebuf() places new data in the write
 *                  buffer, this ISR sends one byte at a time to the EEPROM.
 */

/* includes */
#include "eeprom.h"

/* macros */
#define BUFSIZE 64
#define EEAR (*((volatile unsigned int  *)0x41))
#define EEDR (*((volatile unsigned char *)0x40))
#define EECR (*((volatile unsigned char *)0x3F))

/* forward declarations */
void eeprom_unlock();

/* variables */
static unsigned char writebuf[BUFSIZE];
static unsigned char bufidx;
static unsigned char writesize;
static unsigned int  writeaddr;
static volatile unsigned char write_busy;

/* functions */
/*******************************************************************************
 * void eeprom_writebuf(unsigned int addr, unsigned char* buf, unsigned char size)
 *
 * Places the data (specified by buf and size) into the write buffer for later
 * writing to the EEPROM.
 *
 * arguments:
 *   unsigned int - specifies the next EEPROM address to write data to.
 *   unsigned char* - a write buffer
 *   unsigned char - size of the data to be written
 *
 * returns:
 *   nothing
 *
 * changes:
 *   EECR
 */
void eeprom_writebuf(unsigned int addr, unsigned char* buf, unsigned char size){
    // while busy, wait
    while(eeprom_isbusy()){}
    // initialize write
    writeaddr = addr;
    write_busy = 1;
    bufidx = 0;
    // copy buf[0:size-1] to writebuf[0:size-1]
    int i;
    for(i = 0; i < size - 1; i++)
        writebuf[i] = buf[i];
    writesize = size;
    // enable eeprom interrupts
    EECR |= (1<<3);
}

/*******************************************************************************
 * void eeprom_readbuf(unsigned int addr, unsigned char* buf, unsigned char size)
 *
 * Reads a specified amount of data (s) from the EEPROM starting at a specified
 * address (addr) and places it in the specified buffer (buf).
 *
 * arguments:
 *   unsigned int - specifies the next EEPROM address to read data from.
 *   unsigned char* - a read buffer
 *   unsigned char - size of the data to be read
 *
 * returns:
 *   nothing
 *
 * changes:
 *   none
 */
void eeprom_readbuf(unsigned int addr, unsigned char* buf, unsigned char size){
    // while busy, wait
    while(eeprom_isbusy()){}
    // read characters from eeprom starting at addr and place in buff[0:size-1]
    int i;
    for(i = 0; i < size; i++){
        EEAR = addr + i;
        EECR |= (1<<0);
        buf[i] = EEDR;
    }
}

/*******************************************************************************
 * int eeprom_isbusy()
 *
 * Returns 0 if write_busy is 0, otherwise, returns 1.
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
int eeprom_isbusy() {
    return (write_busy == 1);
}

#pragma GCC push_options
#pragma GCC optimize ("Os")
/*******************************************************************************
 * void __vector_22()
 *
 * Enabled when writebuf() places new data in the write buffer, this ISR sends
 * one byte at a time to the EEPROM.
 *
 * arguments:
 *   void
 *
 * returns:
 *   nothing
 *
 * changes:
 *   EEAR, EECR, EEDR
 */
void __vector_22(void) __attribute__ ((signal, used, externally_visible));
void __vector_22(void){
    if(bufidx < writesize){
        while(EECR & (1<<1));
        EEAR = writeaddr;
        EEDR = writebuf[bufidx];
        /* Write logical one to EEMPE */
        EECR |= (1<<2);
        /* Start eeprom write by setting EEPE */
        EECR |= (1<<1);
        //uart_writehex8(((unsigned char*)writeaddr));
        writeaddr++;
        bufidx++;
    }
    else{
        //EECR = 0;
        EECR &= ~(1<<3);
        write_busy = 0;
    }
}
#pragma GCC pop_options
