#ifndef __MINIUART_H
#define __MINIUART_H

#include "io.h"

//Auxiliary peripherals registers (see pg8 of the BCM2835 datasheet) (some are verified)
#define AUX_BASE_ADDR 0x20215000
#define AUX_IRQ     __IO(AUX_BASE_ADDR+0x00)
#define SPI1_IRQPEND 2
#define SPI0_IRQPEND 1
#define MU_IRQPEND   0
#define AUX_ENABLES __IO(AUX_BASE_ADDR+0x04)
#define SPI1_EN 2
#define SPI0_EN 1
#define MU_EN   0
//mini uart registers (mostly verified woking)
#define AUX_MU_IO_REG   __IO8(AUX_BASE_ADDR+0x40)
#define AUX_MU_IER_REG  __IO8(AUX_BASE_ADDR+0x44) //warning: datasheet may have errors on this
#define ETBEI 1
#define ERBFI 0
#define AUX_MU_IIR_REG  __IO8(AUX_BASE_ADDR+0x48) //warning: datasheet may have errors on this
//TODO: get real names for these
#define FIFOEN1  7
#define FIFOEN0  6
#define IID1     2 //when reading
#define IID0     1 //when reading
#define IID_MASK 0b11
#define FIFOCLR1 2 //when writing
#define FIFOCLR0 1 //when writing
#define IRQPEND  0
#define AUX_MU_LCR_REG  __IO8(AUX_BASE_ADDR+0x4C) //warning: datasheet may have errors on this
#define DLAB 7 //untested
#define SETBREAK 6 //untested TODO: get real name for this
#define WLS1 1 //the datasheet is actually wrong about bits 1 and 0
#define WLS0 0 //see above
#define AUX_MU_MCR_REG  __IO8(AUX_BASE_ADDR+0x50)
#define RTS 1 //untested
#define AUX_MU_LSR_REG  __IO8(AUX_BASE_ADDR+0x54)
#define TEMT 6 //untested
#define THRE 5 //untested
#define OE   1 //untested
#define DR   0
#define AUX_MU_MSR_REG  __IO8(AUX_BASE_ADDR+0x58) //untested
#define CTS 4 //datasheet says this is bit 5, but doesn't say what bit 4 does so I don't believe it
#define AUX_MU_SCRATCH  __IO8(AUX_BASE_ADDR+0x5C) //untested
#define AUX_MU_CNTL_REG __IO8(AUX_BASE_ADDR+0x60)
//TODO: get real names for these
#define CTSLEVEL 7 //untested
#define RTSLEVEL 6 //untested
#define RTSAUTO1 5 //untested
#define RTSAUTO0 4 //untested
#define CTSFLOWEN 3 //untested
#define RTSFLOWEN 2 //untested
#define TXEN 1
#define RXEN 0
#define AUX_MU_STAT_REG __IO32(AUX_BASE_ADDR+0x64) //untested
#define AUX_MU_BAUD_REG __IO16(AUX_BASE_ADDR+0x68)
#define MU_BAUD(x) (((250000000/(x))/8)-1)
//SPI 1 registers (not verified)
#define AUX_SPI0_CNTL0_REG __IO(AUX_BASE_ADDR+0x80)
#define AUX_SPI0_CNTL1_REG __IO(AUX_BASE_ADDR+0x84)
#define AUX_SPI0_STAT_REG  __IO(AUX_BASE_ADDR+0x88)
#define AUX_SPI0_IO_REG    __IO(AUX_BASE_ADDR+0x90)
#define AUX_SPI0_PEEK_REG  __IO(AUX_BASE_ADDR+0x94)
//SPI 2 registers (not verified)
#define AUX_SPI1_CNTL0_REG __IO(AUX_BASE_ADDR+0xC0)
#define AUX_SPI1_CNTL1_REG __IO(AUX_BASE_ADDR+0xC4)
#define AUX_SPI1_STAT_REG  __IO(AUX_BASE_ADDR+0xC8)
#define AUX_SPI1_IO_REG    __IO(AUX_BASE_ADDR+0xD0)
#define AUX_SPI1_PEEK_REG  __IO(AUX_BASE_ADDR+0xD4)

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
