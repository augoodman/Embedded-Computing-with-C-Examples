/**
* File:   main.c
* SER 486
* Homework 2
*
* @author Andrew Goodman (augoodma)
* @version 2020.10.20
*/

////////////////////////////////////////////////////////////////////////////////
//MACROS
#define PORTB (*((volatile char *)0x24))
#define PINB (*((volatile char *)0x23))
#define DELAY_COUNT 64000

////////////////////////////////////////////////////////////////////////////////
//FOWARD DELCARATIONS
void delay();

/////////////////////////////////////////////////////////////////////////////////
/**
 * Program entry point.
 */
int main(void) {
    PORTB = 0x0;
    PORTB |=  (0x1<<1);
    while(1) {
        PORTB ^=  (0x1<<1);
        PINB = 0x2;
        delay();
    }
}

/**
 * Triggers a delay.
 */
void delay(){
    volatile unsigned int i;
    for(i=0;i<DELAY_COUNT;i++){}
}
