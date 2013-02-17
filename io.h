#ifndef __BCM2835IO_H
#define __BCM2835IO_H

#include "stdint.h" //for uint32_t, etc

/*******************************************************************************
 * Macros that make I/O device registers look like global volatile ints
 *
 * Addresses that the ARM can use directly are in 0x20nnnnnn (see IO_PHY_ADDR)
 * Addresses that the DMA engine can use are in 0x7Ennnnnn (see IO_BUS_ADDR)
 ******************************************************************************/
//for I/O devices when using the physical address space (0x20000000 - 0x20FFFFFF)
#define IO_PHY_ADDR(x) (((x)&0x00FFFFFF) | 0x20000000)
//for I/O devices when using the bus address space (0x7E000000 - 0x7EFFFFFF)
#define IO_BUS_ADDR(x) (((x)&0x00FFFFFF) | 0x7E000000)

//for using I/O devices just like a global variable
#define __IO64(x) (*((volatile uint64_t*)(IO_PHY_ADDR(x)))) //requires little-endian mode?
#define __IO32(x) (*((volatile uint32_t*)(IO_PHY_ADDR(x))))
#define __IO16(x) (*((volatile uint16_t*)(IO_PHY_ADDR(x)))) //requires little-endian mode
#define __IO8(x)  (*((volatile uint8_t*)(IO_PHY_ADDR(x)))) //requires little-endian mode
#define __IO(x)   __IO32(x)

#endif

/*
Copyright (c) 2013 David DiPaola

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
