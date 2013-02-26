//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

// The raspberry pi firmware at the time this was written defaults
// loading at address 0x8000.  Although this bootloader could easily
// load at 0x0000, it loads at 0x8000 so that the same binaries built
// for the SD card work with this bootloader.  Change the ARMBASE
// below to use a different location.

#include "io.h"
#include "gpio.h"
#include "biguart.h"
#include "interrupt.h"
#include "systimer.h"
#include "stdint.h"
#include "vic.h"

#define ARMBASE 0x10000

extern void enable_cpu_irq ( void );
extern void disable_cpu_irq ( void );

void led_toggle(void){
    if(GPIOREAD(16)){
        GPIOCLR(16);
    }else{
        GPIOSET(16);
    }
}

void c_irq_handler ( void )
{
    vic_irq_handler();
}

int notmain ( void )
{
    unsigned int ra, rb;

    //make gpio pin tied to the led an output
    GPIOMODE(16, FSEL_OUTPUT); //led output
    GPIOCLR(16); //led on

    //initialize interrupts
    vic_init();

    //register handlers and enable IRQs
    register_irq(VECT_SYSTIMERM1, systimer_handler);
    register_irq(VECT_PL011, uart_handler);
    enable_irq(VECT_PL011);
    enable_irq(VECT_SYSTIMERM1);

    //initialize hardware
    systimer_init(1000000); //go off once a second
    uartInit();
    uartPutln("Booted!");

    //test interrupts fully
    GPIOSET(16); //led off
    enable_cpu_irq();
    //enable(); //wish this worked...
    iuartInit();
    for(;;){
        iuartPutln("Interrupts!");
        enable_irq(VECT_SYSTIMERM1);
        disable_irq(VECT_SYSTIMERM1);
        if(ra != (rb = systimer_get()))
        {
            ra = rb;
            led_toggle();
        }
    }

    return(0);
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
//
// Copyright (c) 2012 David Welch dwelch@dwelch.com, 2013 David DiPaola dsd3275@rit.edu
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------
