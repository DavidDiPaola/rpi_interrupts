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
