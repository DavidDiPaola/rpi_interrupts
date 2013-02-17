#include "miniuart.h"
#include "io.h"
#include "gpio.h"
#include "interrupt.h" //DEUBG?

void uartInit (void)
{
    int i;

    AUX_ENABLES |= (1<<MU_EN); //enable the mini uart
    AUX_MU_IER_REG &= ~((1<<ETBEI) | (1<<ERBFI)); //disable interrupts (ERBFI, ETBEI)
    AUX_MU_CNTL_REG &= ~((1<<CTSLEVEL) |  //disable hardware flow control
                         (1<<RTSLEVEL) |
                         (1<<RTSAUTO1) |
                         (1<<RTSAUTO0) |
                         (1<<CTSFLOWEN) |
                         (1<<RTSFLOWEN) |
                         (1<<TXEN) |      //disable transmitter and receiver
                         (1<<RXEN));
    AUX_MU_LCR_REG &= ~((1<<DLAB) | (1<<SETBREAK)); //disable divisor latch, disable break
    AUX_MU_LCR_REG |= ((1<<WLS1) | (1<<WLS0)); //8 data bits, 1 stop bit
    AUX_MU_MCR_REG &= ~(1<<RTS); //set RTS line low
    AUX_MU_IIR_REG |= ((1<<FIFOEN1)  | (1<<FIFOEN0) |  //enable FIFOs
                       (1<<FIFOCLR1) | (1<<FIFOCLR0)); //clear FIFOs
    AUX_MU_BAUD_REG = MU_BAUD(115200);

    //set GPIO pin 14 to Mini-UART TX mode
    GPIOMODE(14, FSEL_AF5);

    //set GPIO pin 15 to Mini-UART RX mode
    GPIOMODE(15, FSEL_AF5);

    //turn off pull-up/pull-down resistors on the TX pin
    GPIOPULLOF(14);

    //enable transmitter and receiver
    AUX_MU_CNTL_REG |= ((1<<TXEN) | (1<<RXEN));
}

char uartGetc(void)
{
    char ch = 0;

    //wait for a character to come in
    while((AUX_MU_LSR_REG & 0x01) == 0){}
    //get char
    ch = (AUX_MU_IO_REG & 0xFF);

    return ch;
}

void uartPutc( char ch )
{
    //wait for transmission to finish
    while((AUX_MU_LSR_REG & 0x20) == 0){}
    //send char
    AUX_MU_IO_REG = ch;
}

void uartPuts( char *s )
{
    while(*s){
        if( *s != '\n' ){
            uartPutc(*s);
        } else {
            uartPutc('\r');
            uartPutc('\n');
        }
        s++;
    }
}

void uartPutln( char *s )
{
    uartPuts(s);
    uartPuts("\n");
}

//DEBUG=============================================================================
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

#define UART_MAXLEN 64
static volatile char uart_buffer[UART_MAXLEN];
static volatile int  uart_idx;
static volatile int  uart_busy;
//static volatile int  uart_done;

void uart_handler ( void )
{
    //if nothing is modifying the buffer or index
    if(!uart_busy){
        //if we can...
        if((uart_idx < UART_MAXLEN) && (uart_buffer[uart_idx] != '\0'))
        {
            //uart_done = 0;
            //send another character
            AUX_MU_IO_REG = uart_buffer[uart_idx];
            uart_idx++;
        }
        else
        {
            uart_buffer[0] = '\0';
            uart_idx = 0;
            //uart_done = 1;
        }
    }
}

void iuartInit( void ){
    uart_buffer[0] = '\0';
    uart_busy = 1;
    //uart_done = 1;
    AUX_MU_IER_REG |= (1<<ETBEI); //interrupt when transmit FIFO is empty
    INTERRUPT_ENABLEIRQ1 |= (1<<IRQAUX); //enable the interrupt for AUX devices
}

void iuartPuts(char *s)
{
    int i = 0;

    //wait until transmission is over
    //while(!uart_done){}
    while(uart_buffer[0] != '\0'){}
    //take the uart
    uart_busy = 1;

    //copy the string
    while((i < UART_MAXLEN) && (s[i] != '\0'))
    {
        uart_buffer[i] = s[i];
        i++;
    }
    if(i >= UART_MAXLEN){
        i = UART_MAXLEN-1;
    }
    uart_buffer[i] = '\0';

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
    iuartPuts(s);
    iuartPuts("\r\n");
}

/*
Copyright (c) 2012 David Welch dwelch@dwelch.com, 2013 David DiPaola

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
