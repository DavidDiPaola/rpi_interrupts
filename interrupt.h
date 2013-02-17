#ifndef __INTERRUPT_H
#define __INTERRUPT_H

#include "io.h"

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
   #define IRQUART       57
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

#endif

/*
Copyright (c) 2013 David DiPaola

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
