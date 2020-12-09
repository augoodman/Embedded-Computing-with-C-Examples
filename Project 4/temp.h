/********************************************************
 * temp.h
 *
 * this file provides function declarations for SER486
 * homework assignment 4.  The procedures implemented
 * provide temp sensor initialization and temp reading.
 */
#ifndef TEMP_H_INCLUDED
#define TEMP_H_INCLUDED

void temp_init();
unsigned int temp_is_data_ready();
void temp_start();
signed int temp_get();

#endif // TEMP_H_INCLUDED
