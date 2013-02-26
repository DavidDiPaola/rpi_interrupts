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

extern void enable_irq ( void );
extern void disable_irq ( void );

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
    unsigned int ra, rb, rc;

    //make gpio pin tied to the led an output
    GPIOMODE(16, FSEL_OUTPUT); //led output
    GPIOCLR(16); //led on

    //initialize interrupts
    vic_init();
    vic_register(VECT_SYSTIMERM1, systimer_handler);
    vic_register(VECT_PL011, uart_handler);
    vic_enable(VECT_PL011);
    vic_enable(VECT_SYSTIMERM1);

    //initialize hardware
    systimer_init(1000000); //go off once a second
    uartInit();
    uartPutln("Booted!");
    //uartPut32(0x12345678);

    //test the UART's interrupt bit
    /*
    iuartInit();
    GPIOSET(16); //led off
    PL011_DR = '0'; //fill the transmit FIFO
    PL011_DR = '1';
    PL011_DR = '2';
    PL011_DR = '3';
    PL011_DR = '4';
    PL011_DR = '5';
    PL011_DR = '6';
    PL011_DR = '7';
    PL011_DR = '8'; //characters past (and including) this one aren't printed...
    PL011_DR = '9';
    PL011_DR = 'A';
    PL011_DR = 'B';
    PL011_DR = 'C';
    PL011_DR = 'D';
    PL011_DR = 'E';
    PL011_DR = 'F';
    ra = INTERRUPT_IRQPEND2; //get the upper 32bits of IRQ pending
    while((rb = INTERRUPT_IRQPEND2) == ra){} //wait for IRQ pending to change
    rc = PL011_RIS; //get raw interrupt status
    uartPutln("");
    uartPut32(ra); //print the old value of IRQ pending
    uartPut32(rb); //print the new value
    uartPut32(rc); //print the PL011 interrupt status
    for(;;){} //chill
    */

    //test the UART with an interrupt
    /*
    GPIOSET(16); //led off
    iuartInit();
    enable_irq();
    PL011_DR = '1'; //fill the transmit FIFO
    PL011_DR = '2';
    PL011_DR = '3';
    PL011_DR = '4';
    PL011_DR = '5';
    PL011_DR = '6';
    PL011_DR = '7';
    PL011_DR = '8';
    for(;;){} //only do interrupts
    */

    //test interrupts fully
    GPIOSET(16); //led off
    enable_irq();
    iuartInit();
    //PL011_DR = '?';
    for(;;){
        iuartPutln("Interrupts!");
        vic_enable(VECT_SYSTIMERM1);
        vic_disable(VECT_SYSTIMERM1); //test deregistration. this line prevents the LED from blinking.
        if(ra != (rb = systimer_get()))
        {
            ra = rb;
            led_toggle();
        }
    }

    //test system timer AND UART
    /*
    while(1)
    {
        iuartPutln("Interrupts!");
    }*/
    /*
        while(systimer_get() == ra) continue;
        ra = systimer_get();

        GPIOCLR(16); //LED on

        while(systimer_get() == ra) continue;
        ra = systimer_get();

        GPIOSET(16); //LED off
    }*/


    return(0);
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
//
// Copyright (c) 2012 David Welch dwelch@dwelch.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------
