#ifndef __SYSTIMER_H
#define __SYSTIMER_H

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

void systimer_init ( unsigned int ivl );

void systimer_handler ( void );

unsigned int systimer_get( void );

#endif

/*
Copyright (c) 2012 David Welch dwelch@dwelch.com, 2013 David DiPaola

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
