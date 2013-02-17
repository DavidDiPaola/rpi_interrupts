#ifndef __MINIUART_H
#define __MINIUART_H

#include "io.h"

//see page175 of the Broadcom Datasheet for more information
#define PL011_BASE_ADDR   0x20201000
#define PL011_DR     __IO(PL011_BASE_ADDR+0x00) //Data Register
    #define PL011_DR_OE 11 //set to 1 on overrun error
    #define PL011_DR_BE 10 //set to 1 on break condition
    #define PL011_DR_PE 9  //set to 1 on parity error
    #define PL011_DR_FE 8  //set to 1 on framing error
    #define PL011_DATA_MASK 0xFF
#define PL011_RSRECR __IO(PL011_BASE_ADDR+0x04) //Receive Status Register/Error Clear Register
    //writing clears error states
    #define PL011_RSRECR_OE 3 //set to 1 on overrun error
    #define PL011_RSRECR_BE 2 //set to 1 on break condition
    #define PL011_RSRECR_PE 1 //set to 1 on parity error
    #define PL011_RSRECR_FE 0 //set to 1 on framing error
#define PL011_FR     __IO(PL011_BASE_ADDR+0x18) //Flag Register
    #define PL011_FR_RI   8 //set to 1 when ring indicator pin is low
    #define PL011_FR_TXFE 7 //set to 1 when TX FIFO/register is empty
    #define PL011_FR_RXFF 6 //set to 1 when RX FIFO/register is full
    #define PL011_FR_TXFF 5 //set to 1 when TX FIFO/register is full
    #define PL011_FR_RXFE 4 //set to 1 when RX FIFO/register is empty
    #define PL011_FR_BUSY 3 //set to 1 when UART is transmitting data
    #define PL011_FR_DCD  2 //set to 1 when DCD pin is low
    #define PL011_FR_DSR  1 //set to 1 when DSR pin is low
    #define PL011_FR_CTS  0 //set to 1 when CTS pin is low
#define PL011_ILPR   __IO(PL011_BASE_ADDR+0x20) //IrDA Low-Power Counter Register ("not in use")
#define PL011_IBRD   __IO(PL011_BASE_ADDR+0x24) //Integer Baud Rate Register
    #define PL011_IBRD_MASK 0xFFFF
#define PL011_FBRD   __IO(PL011_BASE_ADDR+0x28) //Fractional Baud Rate Register
    #define PL011_FBRD_MASK 0b111111
#define PL011_LCRH   __IO(PL011_BASE_ADDR+0x2C) //Line Control Register
    #define PL011_LCRH_SPS  7 //
    #define PL011_LCRH_WLEN 5 //2bit, sets word length
        #define PL011_LCRH_WLEN_8BIT 0b11
        #define PL011_LCRH_WLEN_7BIT 0b10
        #define PL011_LCRH_WLEN_6BIT 0b01
        #define PL011_LCRH_WLEN_5BIT 0b00
    #define PL011_LCRH_FEN  4 //set to 1 to enable TX and RX FIFOs
    #define PL011_LCRH_STP2 3 //set to 1 to send 2 stop bits
    #define PL011_LCRH_EPS  2 //set to 1 for even parity, 0 for odd
    #define PL011_LCRH_PEN  1 //set to 1 to enable parity
    #define PL011_LCRH_BRK  0 //set to 1 to force the TX pin low
#define PL011_CR     __IO(PL011_BASE_ADDR+0x30) //Control Register
    #define PL011_CR_CTSEN  15 //set to 1 to enable CTS hardware flow control
    #define PL011_CR_RTSEN  14 //set to 1 to enable RTS hardware flow control
    #define PL011_CR_OUT2   13 //set to 1 to set out2 to 0
    #define PL011_CR_OUT1   12 //set to 1 to set out1 to 0
    #define PL011_CR_RTS    11 //set to 1 to set the RTS pin low
    #define PL011_CR_DTR    10 //set to 1 to set the DTR pin low
    #define PL011_CR_RXE    9  //set to 1 to enable receiving
    #define PL011_CR_TXE    8  //set to 1 to enable transmitting
    #define PL011_CR_LBE    7  //set to 1 to enable loopback
    #define PL011_CR_SIRLP  2  //sets SIR IrDA mode (unused?)
    #define PL011_CR_SIREN  1  //enables SIR IrDA mode (unused?)
    #define PL011_CR_UARTEN 0  //set to 1 to enable the UART
#define PL011_IFLS   __IO(PL011_BASE_ADDR+0x34) //Interrupt FIFO Level Select Register
    #define PL011_IFLS_RXIFLSEL 3 //3bit, RX FIFO fill interrupt threshold
        #define PL011_IFLS_RXIFLSEL_EIGHTH        0b000
        #define PL011_IFLS_RXIFLSEL_QUARTER       0b001
        #define PL011_IFLS_RXIFLSEL_HALF          0b010
        #define PL011_IFLS_RXIFLSEL_THREEQUARTERS 0b011
        #define PL011_IFLS_RXIFLSEL_SEVELEIGHTS   0b100
    #define PL011_IFLS_TXIFLSEL 0 //3bit, TX FIFO fill interrupt threshold
        #define PL011_IFLS_TXIFLSEL_EIGHTH        0b000
        #define PL011_IFLS_TXIFLSEL_QUARTER       0b001
        #define PL011_IFLS_TXIFLSEL_HALF          0b010
        #define PL011_IFLS_TXIFLSEL_THREEQUARTERS 0b011
        #define PL011_IFLS_TXIFLSEL_SEVELEIGHTS   0b100
#define PL011_IMSC   __IO(PL011_BASE_ADDR+0x38) //Interrupt Mask Set/Clear Register
    #define PL011_IMSC_OEIM   10 //overrun error mask bit, if 1: this interrupt is enabled
    #define PL011_IMSC_BEIN   9  //break error mask bit, if 1: this interrupt is enabled
    #define PL011_IMSC_PEIM   8  //parity error mask bit, if 1: this interrupt is enabled
    #define PL011_IMSC_FEIM   7  //framing error mask bit, if 1: this interrupt is enabled
    #define PL011_IMSC_RTIM   6  //receive timeout mask bit, if 1: this interrupt is enabled
    #define PL011_IMSC_TXIM   5  //transmit interrupt mask bit, if 1: this interrupt is enabled
    #define PL011_IMSC_RXIM   4  //receive interrupt mask bit, if 1: this interrupt is enabled
    #define PL011_IMSC_DSRMIM 3  //DSR interrupt mask bit, if 1: this interrupt is enabled
    #define PL011_IMSC_DCDMIM 2  //DCD interrupt mask bit, if 1: this interrupt is enabled
    #define PL011_IMSC_CTSMIM 1  //CTS interrupt mask bit, if 1: this interrupt is enabled
    #define PL011_IMSC_RIMIM  0  //RI interrupt mask bit, if 1: this interrupt is enabled
#define PL011_RIS    __IO(PL011_BASE_ADDR+0x3C) //Raw Interrupt Status Register
    //read only
    #define PL011_RIS_OERIS   10 //overrun error interrupt status
    #define PL011_RIS_BERIS   9  //break error interrupt status
    #define PL011_RIS_PERIS   8  //parity error interrupt status
    #define PL011_RIS_FERIS   7  //framing error interrupt status
    #define PL011_RIS_RTRIS   6  //receive timeout interrupt status
    #define PL011_RIS_TXRIS   5  //transmit interrupt status
    #define PL011_RIS_RXRIS   4  //receive interrupt status
    #define PL011_RIS_DSRRMIS 3  //DSR interrupt status
    #define PL011_RIS_DCDRMIS 2  //DCD interrupt status
    #define PL011_RIS_CTSRMIS 1  //CTS interrupt status
    #define PL011_RIS_RIRMIS  0  //RI interrupt status
#define PL011_MIS    __IO(PL011_BASE_ADDR+0x40) //Masked Interrupt Status Register
    //read only
    #define PL011_MIS_OEMIS   10 //overrun error interrupt status
    #define PL011_MIS_BEMIS   9  //break error interrupt status
    #define PL011_MIS_PEMIS   8  //parity error interrupt status
    #define PL011_MIS_FEMIS   7  //framing error interrupt status
    #define PL011_MIS_RTMIS   6  //receive timeout interrupt status
    #define PL011_MIS_TXMIS   5  //transmit interrupt status
    #define PL011_MIS_RXMIS   4  //receive interrupt status
    #define PL011_MIS_DSRMMIS 3  //DSR interrupt status
    #define PL011_MIS_DCDMMIS 2  //DCD interrupt status
    #define PL011_MIS_CTSMMIS 1  //CTS interrupt status
    #define PL011_MIS_RIMMIS  0  //RI interrupt status
#define PL011_ICR    __IO(PL011_BASE_ADDR+0x44) //Interrupt Clear Register
    //write only
    #define PL011_ICR_OEIC   10 //overrun error interrupt clear
    #define PL011_ICR_BEIC   9  //break error interrupt clear
    #define PL011_ICR_PEIC   8  //parity error interrupt clear
    #define PL011_ICR_FEIC   7  //framing error interrupt clear
    #define PL011_ICR_RTIC   6  //receive timeout interrupt clear
    #define PL011_ICR_TXIC   5  //transmit interrupt clear
    #define PL011_ICR_RXIC   4  //receive interrupt clear
    #define PL011_ICR_DSRMIC 3  //DSR interrupt clear
    #define PL011_ICR_DCDMIC 2  //DCD interrupt clear
    #define PL011_ICR_CTSMIC 1  //CTS interrupt clear
    #define PL011_ICR_RIMIC  0  //RI interrupt clear
#define PL011_DMACR  __IO(PL011_BASE_ADDR+0x48) //DMA Control Register (unsupported)
#define PL011_ITCR   __IO(PL011_BASE_ADDR+0x80) //Test Control Register
#define PL011_ITIP   __IO(PL011_BASE_ADDR+0x84) //Integration Test Input Register
#define PL011_ITOP   __IO(PL011_BASE_ADDR+0x88) //Integration Test Output Register
#define PL011_TDR    __IO(PL011_BASE_ADDR+0x8C) //Test Data Register
#define PL011_BAUD_INT(x) (3000000 / (16 * (x)))
#define PL011_BAUD_FRAC(x) (int)((((3000000.0 / (16.0 * (x)))-PL011_BAUD_INT(x))*64.0)+0.5) //9600 baud may be slightly off with this calcualtion

void uartInit (void);

char uartGetc(void);

void uartPutc( char ch );

void uartPuts( char *s );

void uartPutln( char *s );

//DEBUG=============================================================================
void uartPut4(unsigned int n);

void uartPut32(unsigned int q);

void uart_handler ( void );

void iuartInit(void);

void iuartPuts(char *s);

void iuartPutln(char *s);

#endif
