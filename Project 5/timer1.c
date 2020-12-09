/*******************************************************************************
 * timer1.c
 *
 * SER486 Project 1
 * Fall 2020
 * Written By:  Andrew Goodman (augoodma)
 *
 * This file implements functions for SER486 timer1 library.
 *
 * Functions are:
 * void timer1_init() - This function initializes timer1 for a 1 second
 *                  periodic tick.
 *
 * unsigned long timer1_get() - This function returns the tick count value.
 *
 * void timer1_clear() - This function clears the value of the tick counter.
 */

/* macros */
#define TCCR1A (*((volatile unsigned char *)0x80))
#define TCCR1B (*((volatile unsigned char *)0x81))
#define TCCR1C (*((volatile unsigned char *)0x82))
#define TIMSK1 (*((volatile unsigned char *)0x6F))
#define OCR1A  (*((volatile unsigned int  *)0x88))
#define SREG   (*((volatile unsigned char *)0x5F))

/* variables */
static unsigned long count;

 /* functions */
/*******************************************************************************
 * void timer1_init()
 *
 * This function initializes timer1 for a 1 second periodic tick.
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
void timer1_init()
{
    //set compare for 1s tick
    OCR1A =  0x3D08;//0b0011110100001000//0d15624
    //set ctc mode
    TCCR1A = 0x0;   //0b00000000
    //set clock divisor
    TCCR1B = 0xD;   //0b00001101
    //enable interrupts on output compare 1
    TIMSK1 = 0x2;   //0b00000010
}

/*******************************************************************************
 * unsigned long timer1_get()
 *
 * This function returns the tick count value.
 *
 * arguments:
 *   none
 *
 * returns:
 *   unsigned long - current tick count value
 *
 * changes:
 *   ****update later****
 */
unsigned long timer1_get()
{
    //check global interrupt state
    if(SREG & (1<<7))
    {
        //disable interrupts
        SREG &= ~(1<<7);
    }
    //get count value
    int c = count;
    //restore interrupts
    SREG |= 1<<7;
    return c;
}

/*******************************************************************************
 * void timer1_clear()
 *
 * This function clears the value of the tick counter.
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
void timer1_clear()
{
    //check global interrupt state
    if(SREG & (1<<7))
    {
        //disable interrupts
        SREG &= ~(1<<7);
    }
    //reset count
    count = 0;
    //restore interrupts
    SREG |= 1<<7;
}

/*******************************************************************************
 * __vector_11(void)
 *
 * This function implements an interrupt handler with vector 11.
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
void __vector_11(void) __attribute__ ((signal, used, externally_visible));
void __vector_11(void)
{
    count++;
}
