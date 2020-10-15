/**
* File:   main.c
* SER 486
* Homework 1
*
* @author Andrew Goodman (augoodma)
* @version 2020.10.15
*/

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include "uart.h"

////////////////////////////////////////////////////////////////////////////////
//FOWARD DELCARATIONS
void writestr(char* str);
void print_bits(int num_bits);

/////////////////////////////////////////////////////////////////////////////////
/**
 * Program entry point.
 */
int main(void)
{
    char* str1 = "Hello World from Atmega328\n";
    char* str2 = "SER486 -- Homework Assignment 1\n";
    char* str3 = "Andrew Goodman\n";
    char* str4 = "char size = ";
    char* str5 = "int size = ";
    char* str6 = "long size = ";
    char* str7 = " bits\n";
    int char_size = 8 * sizeof(char);
    int int_size = 8 * sizeof(int);
    int long_size = 8 * sizeof(long);

    uart_init();

    writestr(str1);
    writestr(str2);
    writestr(str3);
    writestr(str4);
    print_bits(char_size);
    writestr(str7);
    writestr(str5);
    print_bits(int_size);
    writestr(str7);
    writestr(str6);
    print_bits(long_size);
    writestr(str7);

    while(1);
    return 0;
}

/**
 * Repeatedly calls a char printing function to print a string.
 *
 * @param str the string to be printed.
 */
void writestr(char* str){
    int i = 0;
    while(str[i] != '\0'){
        uart_writechar(str[i]);
        i++;
    }
}

/**
 * A helper function for converting bit numbers into a printable string.
 *
 * @param num_bits an integer to be converted to string.
 */
void print_bits(int num_bits){
    switch(num_bits){
    case 8:
        writestr("8");
        break;
    case 16:
        writestr("16");
        break;
    case 24:
        writestr("24");
        break;
    case 32:
        writestr("32");
        break;
    case 40:
        writestr("40");
        break;
    case 48:
        writestr("48");
        break;
    case 56:
        writestr("56");
        break;
    case 64:
        writestr("64");
        break;
    default:
        writestr("too many");
    }
}
