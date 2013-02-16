//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

// The raspberry pi firmware at the time this was written defaults
// loading at address 0x8000.  Although this bootloader could easily
// load at 0x0000, it loads at 0x8000 so that the same binaries built
// for the SD card work with this bootloader.  Change the ARMBASE
// below to use a different location.

#include "io.h"
#include "gpio.h"
#include "miniuart.h"

#define ARMBASE 0x8000

#define SYSTIMER_CS_ADDR  0x20003000
#define SYSTIMER_CLO_ADDR 0x20003004
#define SYSTIMER_C0_ADDR  0x2000300C
#define SYSTIMER_C1_ADDR  0x20003010
#define SYSTIMER_C2_ADDR  0x20003014
#define SYSTIMER_C3_ADDR  0x20003018
#define SYSTIMER_CS  __IO(SYSTIMER_CS_ADDR)
    #define M3 3
    #define M2 2
    #define M1 1
    #define M0 0
#define SYSTIMER_CLO __IO(SYSTIMER_CLO_ADDR)
#define SYSTIMER_C0  __IO(SYSTIMER_C0_ADDR) //the GPU may be using this one, steer clear
#define SYSTIMER_C1  __IO(SYSTIMER_C1_ADDR)
#define SYSTIMER_C2  __IO(SYSTIMER_C2_ADDR) //the GPU may be using this one, steer clear
#define SYSTIMER_C3  __IO(SYSTIMER_C3_ADDR)

#define INTERRUPT_BASE_ADDR 0x2000B000
#define INTERRUPT_BASICPEND_ADDR       (INTERRUPT_BASE_ADDR+0x200)
#define INTERRUPT_IRQPEND1_ADDR        (INTERRUPT_BASE_ADDR+0x204)
#define INTERRUPT_IRQPEND2_ADDR        (INTERRUPT_BASE_ADDR+0x208)
#define INTERRUPT_FIQCONTROL_ADDR      (INTERRUPT_BASE_ADDR+0x20C)
#define INTERRUPT_ENABLEIRQ1_ADDR      (INTERRUPT_BASE_ADDR+0x210)
#define INTERRUPT_ENABLEIRQ2_ADDR      (INTERRUPT_BASE_ADDR+0x214)
#define INTERRUPT_ENABLEBASICIRQ_ADDR  (INTERRUPT_BASE_ADDR+0x218)
#define INTERRUPT_DISABLEIRQ1_ADDR     (INTERRUPT_BASE_ADDR+0x21C)
#define INTERRUPT_DISABLEIRQ2_ADDR     (INTERRUPT_BASE_ADDR+0x220)
#define INTERRUPT_DISABLEBASICIRQ_ADDR (INTERRUPT_BASE_ADDR+0x224)
#define INTERRUPT_BASICPEND       __IO(INTERRUPT_BASICPEND_ADDR)
#define INTERRUPT_IRQPEND         __IO64(INTERRUPT_IRQPEND1_ADDR)
   #define IRQSYSTIMERC1 1
   #define IRQSYSTIMERC3 3
   #define IRQAUX        29
#define INTERRUPT_IRQPEND1        __IO(INTERRUPT_IRQPEND1_ADDR)
#define INTERRUPT_IRQPEND2        __IO(INTERRUPT_IRQPEND2_ADDR)
#define INTERRUPT_FIQCONTROL      __IO(INTERRUPT_FIQCONTROL_ADDR)
#define INTERRUPT_ENABLEIRQ       __IO64(INTERRUPT_ENABLEIRQ1_ADDR)
#define INTERRUPT_ENABLEIRQ1      __IO(INTERRUPT_ENABLEIRQ1_ADDR)
#define INTERRUPT_ENABLEIRQ2      __IO(INTERRUPT_ENABLEIRQ2_ADDR)
#define INTERRUPT_ENABLEBASICIRQ  __IO(INTERRUPT_ENABLEBASICIRQ_ADDR)
#define INTERRUPT_DISABLEIRQ1     __IO(INTERRUPT_DISABLEIRQ1_ADDR)
#define INTERRUPT_DISABLEIRQ2     __IO(INTERRUPT_DISABLEIRQ2_ADDR)
#define INTERRUPT_DISABLEBASICIRQ __IO(INTERRUPT_DISABLEBASICIRQ_ADDR)

extern void enable_irq ( void );

//------------------------------------------------------------------------
#define UART_MAXLEN 64
volatile char uart_buffer[UART_MAXLEN];
volatile int  uart_idx;
volatile int  uart_busy;
//-------------------------------------------------------------------------
void uart_handler ( void )
{
    //if nothing is modifying the buffer or index
    if(!uart_busy){
        //if we can...
        if((uart_idx < UART_MAXLEN) && (uart_buffer[uart_idx] != '\0'))
        {
            //send another character
            AUX_MU_IO_REG = uart_buffer[uart_idx];
            uart_idx++;
        }
        else
        {
            uart_buffer[0] = '\0';
            uart_idx = 0;
        }
    }
}
//------------------------------------------------------------------------
volatile unsigned int irq_counter;
volatile unsigned int interval;
//-------------------------------------------------------------------------
void timer_handler ( void )
{
    //clear/ack the interrupt
    SYSTIMER_CS |= (1<<M1);
    //"reset" the timer
    SYSTIMER_C1 = SYSTIMER_CLO + interval;

    //increment our counter
    irq_counter++;
}
void c_irq_handler ( void )
{
    //determine the source of the interrupt
    //if(SYSTIMER_CS & (1<<M1)) //if system timer 1 has gone off
    if(INTERRUPT_IRQPEND & (1<<IRQSYSTIMERC1)) //if system timer 1 has gone off
    {
        //if so, handle it
        timer_handler();
    }
    //else if(AUX_IRQ & (1<<MU_IRQEN)) //if the mini uart has gone off
    else if((INTERRUPT_IRQPEND & (1<<IRQAUX)) && (AUX_IRQ & (1<<MU_IRQEN))) //if the mini uart has gone off
    {
        //if so, handle it
        uart_handler();
    }
}
//------------------------------------------------------------------------
void systimer_init ( unsigned int ivl )
{
    //assuming ARM IRQs are disabled?
    //initialize our counter
    irq_counter = 0;
    //copy over the interval to use
    interval = ivl;
    //"reset" the timer
    SYSTIMER_C1 = SYSTIMER_CLO + interval;
    //clear any interrupts
    SYSTIMER_CS |= (1<<M1);
    //enable the interrupt for the timer
    INTERRUPT_ENABLEIRQ |= (1<<IRQSYSTIMERC1);
}
//------------------------------------------------------------------------
void iuartPrint(char *s)
{
    int i = 0;

    //wait until transmission is over
    while(uart_buffer[0] != '\0'){}
    //take the uart
    uart_busy = 1;

    //copy the string
    while((i < UART_MAXLEN) && (s[i] != '\0'))
    {
        uart_buffer[i] = s[i];
        i++;
    }

    //leave the uart
    uart_busy = 0;
    //start the transmission
    AUX_MU_IO_REG = uart_buffer[0];
}
void iuartPrintln(char *s)
{
    iuartPrint(s);
    iuartPrint("\r\n");
}
int notmain ( void )
{
    unsigned int ra;

    //make gpio pin tied to the led an output
    GPIOMODE(16, FSEL_OUTPUT); //led output
    GPIOSET(16); //led off

    //rely on the interrupt to measure time.
    ra=irq_counter;

    //systimer_init(0x00080000);
    systimer_init(1000000);
    uartInit();
    uartPutln("Booted!");

    //mini uart irq setup
    uart_buffer[0] = '\0';
    uart_idx = 0;
    uart_busy = 0;
    //AUX_MU_IER_REG |= (1<<ETBEI); //interrupt when transmit FIFO is empty

    enable_irq();
    while(1)
    {
        while(irq_counter==ra) continue;
        ra=irq_counter;

        GPIOCLR(16); //LED on

        while(irq_counter==ra) continue;
        ra=irq_counter;

        GPIOSET(16); //LED off
    }


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
