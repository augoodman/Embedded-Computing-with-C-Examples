/*******************************************************************************
 * util.c
 *
 * SER486 Project 3
 * Fall 2020
 * Written By:  Andrew Goodman (augoodma)
 *
 * This file provides implementation for various utility functions for SER486.
 *
 * Functions are:
 *    int update_tcrit_hi(int value) - Updates the configuration tcrit_hi limit
 *                  with the specified value. This function is called by the
 *                  packet command parser.
 *
 *    int update_twarn_hi(int value) - Updates the configuration twarn_hi limit
 *                  with the specified value. This function is called by the
 *                  packet command parser.
 *
 *    int update_tcrit_lo(int value) - Updates the configuration tcrit_lo limit
 *                  with the specified value. This function is called by the
 *                  packet command parser.
 *
 *    int update_twarn_lo(int value) - Updates the configuration twarn_lo limit
 *                  with the specified value. This function is called by the
 *                  packet command parser.
 */

/* includes */
#include "util.h"
#include "process_packet.h"
#include "config.h"

 /* functions */
/*******************************************************************************
 * int update_tcrit_hi(int value)
 *
 * Updates the configuration tcrit_hi limit with the specified value. This
 * function is called by the packet command parser.
 *
 * arguments:
 *   int - tcrit_hi limit
 *
 * returns:
 *   nothing
 *
 * changes:
 *   ****update later****
 */
int update_tcrit_hi(int value)
{
    if(value > config.hi_warn && value <= 0x3FF) return 0;
    return 1;
}

/*******************************************************************************
 * int update_twarn_hi(int value)
 *
 * Updates the configuration twarn_hi limit with the specified value. This
 * function is called by the packet command parser.
 *
 * arguments:
 *   int - twarn_hi limit
 *
 * returns:
 *   nothing
 *
 * changes:
 *   ****update later****
 */
int update_twarn_hi(int value)
{
    if(value > config.lo_warn && value < config.hi_alarm) return 0;
    return 1;
}

/*******************************************************************************
 * int update_tcrit_lo(int value)
 *
 * Updates the configuration tcrit_lo limit with the specified value. This
 * function is called by the packet command parser.
 *
 * arguments:
 *   int - tcrit_lo limit
 *
 * returns:
 *   nothing
 *
 * changes:
 *   ****update later****
 */
int update_tcrit_lo(int value)
{
    if(value < config.lo_warn) return 0;
    return 1;
}

/*******************************************************************************
 * int update_twarn_lo(int value)
 *
 * Updates the configuration twarn_lo limit with the specified value. This
 * function is called by the packet command parser.
 *
 * arguments:
 *   int - twarn_lo limit
 *
 * returns:
 *   nothing
 *
 * changes:
 *   ****update later****
 */
int update_twarn_lo(int value)
{
    if(value > config.lo_alarm && value < config.hi_warn) return 0;
    return 1;
}
