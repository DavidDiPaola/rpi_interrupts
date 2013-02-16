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
    //enable the interrupt for the timer
    INTERRUPT_ENABLEIRQ |= (1<<IRQSYSTIMERC1);
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
