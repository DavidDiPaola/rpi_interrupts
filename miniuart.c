#include "miniuart.h"
#include "io.h"
#include "gpio.h"

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
