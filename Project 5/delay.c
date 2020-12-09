/*******************************************************************************
 * delay.c
 *
 * SER486 Project 1
 * Fall 2020
 * Written By:  Andrew Goodman (augoodma)
 *
 * This file implements functions for SER486 timer-based delay library.
 *
 * Functions are:
 *    delay_init() - This private function initializes the delay timer.
 *
 *    delay_get(unsigned int num) - This function gets the current tick counter
 *                  value for the specified instance.
 *
 *    delay_set(unsigned int num, unsigned int msec) - This function sets the
 *                  counter limit and resets the count for the specified
 *                  instance.
 *
 *    unsigned delay_isdone(unsigned int num) - This function returns 1 if the
 *                  specified instance of the counter has reached its limit,
 *                  otherwise, returns 0.
 *
 *    __vector_14(void) - This function implements an interrupt handler with
 *                  vector 14.
 */

/* macros */
#define TCCR0A (*((volatile unsigned char *)0x44))
#define TCCR0B (*((volatile unsigned char *)0x45))
#define TIMSK0 (*((volatile unsigned char *)0x6E))
#define OCR0A  (*((volatile unsigned char *)0x47))
#define SREG   (*((volatile unsigned char *)0x5F))

/* variables */
static unsigned int count[2];
static unsigned int limit[2];
static unsigned char initialized = 0;

/* functions */
/*******************************************************************************
 * delay_init()
 *
 * This private function initializes the delay timer.
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
 void delay_init()
{
    //set compare for 1ms tick
    OCR0A = 0xF9;  //0b11111001//0d249
    //set ctc mode
    TCCR0A = 0x2;  //0b00000010
    //set clock divisor
    TCCR0B = 0x3;  //0b00000011
    //enable interrupts
    TIMSK0 = 0x2;  //0b00000010
    //stop further initialization
    initialized = 1;
}

/*******************************************************************************
 * delay_get(unsigned int num)
 *
 * This function gets the current tick counter value for the specified instance.
 *
 * arguments:
 *   unsigned - specifies instance
 *
 * returns:
 *   unsigned - current tick counter value
 *
 * changes:
 *   ****update later****
 */
unsigned int delay_get(unsigned int num)
{
    //check global interrupt state
    if(SREG & (1<<7))
    {
        //disable interrupts
        SREG &= ~(1<<7);
    }
    //get count value
    int c = count[num];
    //restore interrupts
    SREG |= 1<<7;
    return c;
}

/*******************************************************************************
 * delay_set(unsigned num, unsigned int msec)
 *
 * This function sets the counter limit and resets the count for the specified
 * instance.
 *
 * arguments:
 *   unsigned int - specifies instance
 *   unsigned int - counter limit
 *
 * returns:
 *   nothing
 *
 * changes:
 *   ****update later****
 */
void delay_set(unsigned int num, unsigned int msec)
{
    //initialize delay counter
    if(initialized == 0) delay_init();
    //check global interrupt state
    if(SREG & (1<<7)){
        //disable interrupts
        SREG &= ~(1<<7);
    }
    //set limit and clear count
    limit[num] = msec;
    count[num] = 0;
    //restore interrupts
    SREG |= 1<<7;
}

/*******************************************************************************
 * delay_isdone(unsigned int num)
 *
 * This function returns 1 if the specified instance of the counter has reached its
 * limit, otherwise, returns 0.
 *
 * arguments:
 *   unsigned int - specifies instance
 *
 * returns:
 *   unsigned - state of counter
 *
 * changes:
 *   ****update later****
 */
unsigned delay_isdone(unsigned int num)
{
    if(count[num] == limit[num]) return 1;
    else return 0;
}

/*******************************************************************************
 * __vector_14(void)
 *
 * This function implements an interrupt handler with vector 14.
 *
 * arguments:
 *   void
 *
 * returns:
 *   nothing
 *
 * changes:
 *   ****update later****
 */
void __vector_14(void) __attribute__ ((signal, used, externally_visible));
void __vector_14(void)
{
    if(count[0] < limit[0]) count[0]++;
    if(count[1] < limit[1]) count[1]++;
}
