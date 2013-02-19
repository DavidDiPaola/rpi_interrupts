#include "systimer.h"
#include "io.h"
#include "interrupt.h"

static volatile unsigned int interval;
static volatile unsigned int counter;

void systimer_init ( unsigned int ivl )
{
    //assuming ARM IRQs are disabled?
    //initialize our counter
    counter = 0;
    //copy over the interval to use
    interval = ivl;
    //"reset" the timer
    SYSTIMER_C1 = SYSTIMER_CLO + interval;
    //clear any interrupts
    SYSTIMER_CS |= (1<<M1);
}

void systimer_handler ( void )
{
    //clear/ack the interrupt
    SYSTIMER_CS |= (1<<M1);
    //advance the timer comparison register to the next time we want an interrupt
    SYSTIMER_C1 += interval;

    //increment our counter
    counter++;
}

unsigned int systimer_get( void )
{
    return counter;
}

/*
Copyright (c) 2012 David Welch dwelch@dwelch.com, 2013 David DiPaola

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
