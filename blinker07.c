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
#include "interrupt.h"
#include "systimer.h"

#define ARMBASE 0x8000

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
    //advance the timer comparison register to the next time we want an interrupt
    SYSTIMER_C1 += interval;

    //increment our counter
    irq_counter++;
}
void c_irq_handler ( void )
{
    //get a copy of the IRQ pending register so we don't have to read it multiple times
    unsigned long long int irqs = INTERRUPT_IRQPEND;

    //determine the source of the interrupt
    //if(SYSTIMER_CS & (1<<M1)) //if system timer 1 has gone off
    if(irqs & (1<<IRQSYSTIMERC1)) //if system timer 1 has gone off
    {
        //if so, handle it
        //GPIOSET(16); //led ff
        timer_handler();
    }
    else if(AUX_IRQ & (1<<MU_IRQEN)) //if the mini uart has gone off
    //else if((irqs & (1<<IRQAUX)) && (AUX_IRQ & (1<<MU_IRQEN))) //if the mini uart has gone off
    {
        //if so, handle it
        //uart_handler();
        //GPIOCLR(16); //led on
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

    //initialize the index
    uart_idx = 0;

    //leave the uart
    uart_busy = 0;
    //start the transmission
    AUX_MU_IO_REG = uart_buffer[0];
    //uartPutln(uart_buffer);
}
void iuartPutln(char *s)
{
    iuartPrint(s);
    iuartPrint("\r\n");
}
void uartPut4(unsigned int n){
    n &= 0xF;

    switch(n){
        case 0x0:
            uartPutc('0');
            break;
        case 0x1:
            uartPutc('1');
            break;
        case 0x2:
            uartPutc('2');
            break;
        case 0x3:
            uartPutc('3');
            break;
        case 0x4:
            uartPutc('4');
            break;
        case 0x5:
            uartPutc('5');
            break;
        case 0x6:
            uartPutc('6');
            break;
        case 0x7:
            uartPutc('7');
            break;
        case 0x8:
            uartPutc('8');
            break;
        case 0x9:
            uartPutc('9');
            break;
        case 0xA:
            uartPutc('A');
            break;
        case 0xB:
            uartPutc('B');
            break;
        case 0xC:
            uartPutc('C');
            break;
        case 0xD:
            uartPutc('D');
            break;
        case 0xE:
            uartPutc('E');
            break;
        case 0xF:
            uartPutc('F');
            break;
    }
}
void uartPut32(unsigned int q){
    int i;

    uartPuts("0x");
    for(i=0; i<8; i++){
        uartPut4(q >> ((7-i)*4));
    }
    uartPuts("\n");
}
int notmain ( void )
{
    unsigned int ra, rb;

    //make gpio pin tied to the led an output
    GPIOMODE(16, FSEL_OUTPUT); //led output
    GPIOSET(16); //led off

    //rely on the interrupt to measure time.
    ra=irq_counter;

    //systimer_init(0x00080000);
    systimer_init(1000000);
    uartInit();
    //uartPutln("Booted!");
    //uartPut32(0x12345678);

    //mini uart irq setup
    uart_buffer[0] = '\0';
    uart_busy = 0;
    //AUX_MU_IER_REG |= (1<<1); //interrupt when transmit FIFO is empty
    //INTERRUPT_ENABLEIRQ |= (1<<IRQAUX); //enable the interrupt for AUX devices

    //test the UART's interrupt bit
    /*
    GPIOSET(16); //led off
    AUX_MU_IO_REG = '1'; //fill the transmit FIFO
    AUX_MU_IO_REG = '2';
    AUX_MU_IO_REG = '3';
    AUX_MU_IO_REG = '4';
    AUX_MU_IO_REG = '5';
    AUX_MU_IO_REG = '6';
    AUX_MU_IO_REG = '7';
    AUX_MU_IO_REG = '8';
    //while((AUX_IRQ & (1<<MU_IRQEN)) == 0){} //wait for the mini uart bit to be set
    ra = INTERRUPT_IRQPEND; //get the lower 32bits of IRQ pending
    while((rb = INTERRUPT_IRQPEND) == ra){} //wait for IRQ pending to change
    uartPut32(ra); //print the old value of IRQ pending
    uartPut32(rb); //print the new value
    for(;;){} //chill

    while( //while...
        ((INTERRUPT_IRQPEND & (1<<IRQAUX)) == 0) && //the AUX IRQ bit is not set and...
        ((AUX_IRQ & (1<<MU_IRQEN)) == 0) //the Mini-UART IRQ bit is not set...
    ){} //wait
    GPIOCLR(16); //led on
    */

    //test the UART with an interrupt
    GPIOSET(16); //led off
    enable_irq();
    /*
    AUX_MU_IO_REG = '1'; //fill the transmit FIFO
    AUX_MU_IO_REG = '2';
    AUX_MU_IO_REG = '3';
    AUX_MU_IO_REG = '4';
    AUX_MU_IO_REG = '5';
    AUX_MU_IO_REG = '6';
    AUX_MU_IO_REG = '7';
    AUX_MU_IO_REG = '8';
    */
    //iuartPutln("Booted!");
    //for(;;){} //only do interrupts

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
