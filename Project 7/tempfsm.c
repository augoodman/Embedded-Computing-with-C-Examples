/*******************************************************************************
 * tempfsm.c
 *
 * SER486 Project 3
 * Fall 2020
 * Written By:  Andrew Goodman (augoodma)
 *
 * This file provides implementation for temperature finite state machine
 * functions.
 *
 * Functions are:
 *    void tempfsm_update(int current, int hicrit, int hiwarn, int locrit, int
 *                  lowarn) - Updates the state of the temperature sensor finite
 *                  state machine (provides hysteresis).  Sends alarms and
 *                  updates the led blink based on state transitions.
 *
 *    void tempfsm_reset() - Resets the state machine to the initial state
 *                  (normal).
 *
 *    void tempfsm_init() - Initializes the finite state machine.
 */

/* includes */
#include "tempfsm.h"
#include "log.h"
#include "temp.h"
#include "process_packet.h"
#include "alarm.h"
#include "config.h"

/* macros */
#define NORM1    1
#define WARN_HI1 2
#define CRIT_HI  3
#define WARN_HI2 4
#define NORM3    5
#define WARN_LO1 6
#define CRIT_LO  7
#define WARN_LO2 8
#define NORM2    9

 /* variables */
static unsigned char state;
static int t;
static int twarn_h;
static int twarn_l;
static int tcrit_h;
static int tcrit_l;

 /* functions */
/*******************************************************************************
 * void tempfsm_update(int current, int hicrit, int hiwarn, int locrit, int
 * lowarn)
 *
 * Updates the state of the temperature sensor finite state machine (provides
 * hysteresis).  Sends alarms and updates the led blink based on state
 * transitions.
 *
 * arguments:
 *   unsigned int - specifies the current temp.
 *   unsigned int - specifies the high critical threshold.
 *   unsigned int - specifies the high warning threshold.
 *   unsigned int - specifies the low critical threshold.
 *   unsigned int - specifies the low warning threshold.
 *
 * returns:
 *   nothing
 *
 * changes:
 *   ****update later****
 */
void tempfsm_update(int current, int hicrit, int hiwarn, int locrit, int lowarn)
{
    t = current;
    tcrit_h = hicrit;
    twarn_h = hiwarn;
    tcrit_l = locrit;
    twarn_l = lowarn;
    switch(state)
    {
    case NORM1:
        if (t >= twarn_h)
        {
            alarm_send(EVENT_HI_WARN);
            state = WARN_HI1;
            break;
        }
        if (t <= twarn_l)
        {
            alarm_send(EVENT_LO_WARN);
            state = WARN_LO1;
            break;
        }
        break;
    case WARN_HI1:
        if (t >= tcrit_h)
        {
            alarm_send(EVENT_HI_ALARM);
            state = CRIT_HI;
            break;
        }
        if (t < twarn_h)
        {
            state = NORM3;
            break;
        }
        break;
    case CRIT_HI:
        if (t < tcrit_h)
        {
            state = WARN_HI1;
            break;
        }
        break;
    case WARN_HI2:
        if (t >= tcrit_h)
        {
            state = CRIT_HI;
            break;
        }
        if (t < twarn_h)
        {
            state = NORM3;
            break;
        }
        break;
    case NORM3:
        if (t >= twarn_h)
        {
            state = WARN_HI1;
            break;
        }
        if (t <= twarn_l)
        {
            alarm_send(EVENT_LO_WARN);
            state = WARN_LO1;
            break;
        }
        break;
    case WARN_LO1:
        if (t <= tcrit_l)
        {
            alarm_send(EVENT_LO_ALARM);
            state = CRIT_LO;
            break;
        }
        if (t > twarn_l)
        {
            state = NORM2;
            break;
        }
        break;
    case CRIT_LO:
        if (t > tcrit_l)
        {
            state = WARN_LO2;
            break;
        }
        break;
    case WARN_LO2:
        if (t <= tcrit_l)
        {
            state = CRIT_LO;
            break;
        }
        if (t > twarn_h)
        {
            state = NORM2;
            break;
        }
        break;
    case NORM2:
        if (t >= twarn_h)
        {
            state = WARN_HI1;
            alarm_send(EVENT_HI_WARN);
            break;
        }
        if (t <= twarn_l)
        {
            state = WARN_LO1;
            break;
        }
        break;
    default:
        tempfsm_reset();
    }
}

/*******************************************************************************
 * void tempfsm_reset()
 *
 * Resets the state machine to the initial state (normal).
 *
 * arguments:
 *   none
 *
 * returns:
 *   nothing
 *
 * changes:
 *   ****update later****
 */
void tempfsm_reset()
{
    state = NORM1;
}

/*******************************************************************************
 * void tempfsm_init()
 *
 * Initializes the finite state machine.
 *
 * arguments:
 *   none
 *
 * returns:
 *   nothing
 *
 * changes:
 *   ****update later****
 */
void tempfsm_init()
{
    state = NORM1;
}
