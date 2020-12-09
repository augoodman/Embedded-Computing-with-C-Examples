/********************************************************
 * temp.c
 *
 * SER486 Assignment 4
 * Fall 2020
 * Written By:  Andrew Goodman (augoodma)
 *
 * This file implements functions associated with SER486
 * homework assignment 4.  The procedures implemented
 * provide temp sensor initialization and temp reading.
 *
 * Functions are:
 *    temp_init() - Initializes the Atmega328P analog to
 *                  digital convertor (ADC) to measure the
 *                  temperature sensor input channel with
 *                  a reference voltage of 1.1V.
 *
 *    temp_is_data_ready() - Reads the status of the
 *                  Atmega328P ADC to determine if the
 *                  most recent conversion has completed.
 *
 *    temp_start() - Start the conversion of the Atmega328P
 *                  ADC.
 *
 *    temp_get() -  Reads the value of the most recent
 *                  conversion form the Atmega328P ADC.
 */

////////////////////////////////////////////////////////////////////////////////
//MACROS
 #define DEGREES(R)       ((R * 101) / 100) - 273
 #define ADCL             (*((volatile unsigned char *)0x78))
 #define ADCH             (*((volatile unsigned char *)0x79))
 #define ADCSRA           (*((volatile unsigned char *)0x7A))
 #define ADMUX            (*((volatile unsigned char *)0x7C))

 /////////////////////////////////////////////////////////////////////////////////
 //FUNCTIONS
/********************************************************
 * temp_init()
 *
 * Initializes the Atmega328P analog to digital convertor
 * (ADC) to measure the temperature sensor input channel
 * with a reference voltage of 1.1V.
 *
 * arguments:
 *   none
 *
 * returns:
 *   nothing
 *
 * changes:
 *   ****update later****
 *
 * NOTE: ****update later****
 */
void temp_init()
{
    //set ADCSRA::ADEN to enable ADC and clear remaining bits
    ADCSRA = 0x80;      //10000000
    //set ADCSRA::ADPS[2:0] for divisor of 64
    ADCSRA |= 0x6;      //10000110
    //set ADMUX::REGS[1:0] to select internal 1.1V voltage reference
    ADMUX |= 0xC0;      //11bbbbbb
    //clear ADMUX::ADLAR
    ADMUX &= (~(1<<5)); //110bbbbb
    //set ADMUX::MUX[3:0] to 1000b (0x8) to select temp sensor from input channel
    ADMUX |= 0x8;       //110b1bbb
    ADMUX |= (~(0x7));  //110b1000
    ADMUX &= (~(0x20));
}

/********************************************************
 * temp_is_data_ready()
 *
 * Reads the status of the Atmega328P ADC to determine if
 * the most recent conversion has completed.
 *
 * arguments:
 *   none
 *
 * returns:
 *   unsigned int - 1 if conversion
 *   completed, otherwise 0.
 *
 * changes:
 *   ****update later****
 *
 * NOTE: ****update later****
 */
unsigned int temp_is_data_ready()
{
    //poll ADSC until it reads 0 (conversion complete)
    unsigned int i = ADCSRA & 0x40;
    while(i != 0)
        i = ADCSRA & 0x40; //bit 6
    return i;
}

/********************************************************
 * temp_start()
 *
 * Start the conversion of the Atmega328P ADC.
 *
 * arguments:
 *   none
 *
 * returns:
 *   nothing
 *
 * changes:
 *   ****update later****
 *
 * NOTE: ****update later****
 */
void temp_start()
{
    ////set ADCSRA::ADSC to start conversion
    ADCSRA |= 0x40;     //b1bbbbbb
}

/********************************************************
 * temp_get()
 *
 * Reads the value of the most recent conversion form the
 * Atmega328P ADC.
 *
 * arguments:
 *   none
 *
 * returns:
 *   signed int - temperature in degrees Celsius.
 *
 * changes:
 *   No direct state changes.
 */
signed int temp_get()
{
    //read ADCL, then ADCH for results/to update ADC (gets 10 bit value, convert to 16 bits)
    signed long int adcl = ADCL;
    signed long int adch = ADCH;
    signed long int reading = (adch<<8) | adcl;
    return (signed int) DEGREES(reading);
}
