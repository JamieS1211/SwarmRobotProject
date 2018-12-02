/*
 * File:   interrupts.h
 * Author: Jamie
 *
 * Created on 30 November 2018, 16:45
 */

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

// Pages 97 - 110 in data sheet

/*
 * Function sets up all interrupt settings.
 */
void interrupts_Setup(void);



/*
 * Function should be called on an interrupt.
 * Contains code for each type of interrupt and clears interrupt after handling.
 */
void __interrupt() interrupts_Event(void);

#endif //INTERRUPTS_H