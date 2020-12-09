/*******************************************************************************
 * util.c
 *
 * SER486 Project 2
 * Fall 2020
 * Written By:  Andrew Goodman (augoodma)
 *
 * This file implements various utility functions for SER486.
 *
 * Functions are:
 *    update_checksum(unsigned char *data, unsigned int dsize) - Update the
 *                  checksum (last byte) of the specifed data so that the sum
 *                  of all the bytes will be zero.
 *
 *    is_checksum_valid(unsigned char *data, unsigned int dsize) - Returns 1 if
 *                  the sum of all the bytes in the data structure is 0.
 *                  Otherwise, returns 0.
 */

/* includes */
#include "util.h"

/* functions */
/*******************************************************************************
 * update_checksum(unsigned char *data, unsigned int dsize)
 *
 * Update the checksum (last byte) of the specifed data so that the sum of all
 * the bytes will be zero.
 *
 * arguments:
 *   unsigned char* - a pointer to the data structure to update the checksum
 *   unsigned int - the size of the data structure
 *
 * returns:
 *   nothing
 *
 * changes:
 *   none
 */
void update_checksum(unsigned char *data, unsigned int dsize){
    unsigned char i;
    unsigned sum = 0;
    for (i = 0; i < dsize - 1; i++)
        sum += ((unsigned char *)data)[i];
    data[dsize - 1] = 0 - sum;
}

/*******************************************************************************
 * is_checksum_valid(unsigned char *data, unsigned int dsize)
 *
 * Returns 1 if the sum of all the bytes in the data structure is 0. Otherwise,
 * returns 0.
 *
 * arguments:
 *   unsigned char* - a pointer to the data structure to validate
 *   unsigned int - the size of the data structure
 *
 * returns:
 *   nothing
 *
 * changes:
 *   none
 */
int is_checksum_valid(unsigned char *data, unsigned int dsize){
    unsigned char i;
    unsigned sum = 0;
    for (i = 0; i < dsize; i++)
        sum += ((unsigned char *)data)[i];
    return (sum == 0);
}
