/*******************************************************************************
 * led.c
 *
 * SER486 Project 1
 * Fall 2020
 * Written By:  Andrew Goodman (augoodma)
 *
 * This file implements functions for SER486 led library.
 *
 * Functions are:
 *     void led_set_blink(char*) - This function sets the Morse Code blink
 *                  message.
 *
 *     void led_update() - This function updates the blink FSM.
 */

 /* macros */
#include "led.h"
#include "delay.h"
#define  PHASE1 1
#define  PHASE2 2
#ifdef RELEASE
    #define RELEASE_FLAG 1
#else
    #define RELEASE_FLAG 0
#endif

 /* variables */
static char* blink_msg;
static unsigned int blink_pos;
static unsigned char blink_state;

 /* functions */
/*******************************************************************************
 * led_set_blink(char* str)
 *
 * This function sets the Morse Code blink message.
 *
 * arguments:
 *   char* - message to be blinked in Morse Code
 *
 * returns:
 *   nothing
 *
 * changes:
 *   ****update later****
 */
void led_set_blink(char* msg){
    //initialize message
    blink_msg = msg;
    blink_pos = 0;
    //reset FSM
    blink_state = PHASE1;
    delay_set(0,0);
    led_off();
}

/*******************************************************************************
 * led_update()
 *
 * This function updates the blink FSM.
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
void led_update(){
    //check for message
    if(blink_msg != 0)
        if(delay_isdone(0) == 1){
            //update FSM
            switch(blink_state){
            case PHASE1:
                if(blink_msg[blink_pos] != ' '){
                    if(blink_msg[blink_pos] == '-'){
                        if(RELEASE_FLAG) delay_set(0, 750);
                        else delay_set(0, 45);
                        while (!delay_isdone(0)){
                            led_on();
                        }
                        led_off();
                        blink_state = PHASE2;
                   }
                    else if(blink_msg[blink_pos] == '.'){
                        if(RELEASE_FLAG) delay_set(0, 250);
                        else delay_set(0, 15);
                        while (!delay_isdone(0)){
                            led_on();
                        }
                        led_off();
                        blink_state = PHASE2;
                    }
                }
                else{
                    if(blink_msg[blink_pos + 1] != 0)
                        blink_pos++;
                    else blink_pos = 0;
                    if(RELEASE_FLAG) delay_set(0, 1000);
                    else delay_set(0, 60);
                }
                break;
            case PHASE2:
                if(blink_msg[blink_pos + 1] != 0)
                    blink_pos++;
                else blink_pos = 0;
                if(RELEASE_FLAG) delay_set(0, 100);
                else delay_set(0, 5);
                blink_state = PHASE1;
                break;
            }
        }
}
