#ifndef __VIC_H
#define __VIC_H

/*
    See vic.c for inportant notes.
*/

#include "interrupt.h"
//typedef volatile void(*vect_t)(void);

//see the table page 113 of the BCM2835 Arm Peripherals datasheet for vector numbers
#define VECT_SYSTIMERM1 1  //triggered when the system timer matches the C1 register
#define VECT_PL011      57 //triggered when the PL011 asserts IRQ status

void vic_init(void);

inline int vic_enable_interrupts( void )
{
    int ret;


    // Check if interrupts are already enabled. If so, return 0

    __asm__("mrs r0, cpsr;"
            "and %[ret], r0, #0x80;"
            "bic r0, r0, #0x80;"
            "msr cpsr_c, r0" 
            : [ret]"=r" (ret) : : "r0");

    return ret;
}

inline int vic_disable_interrupts(void)
{
    int ret;

    __asm__("mrs r0, cpsr; "
            "and %[ret], r0, #0x80;"
            "orr r0, r0, #0x80;" 
            "msr cpsr_c, r0" 
	    : [ret]"=r" (ret) 
	    : 
	    : "r0");

    return ret;
}

irqmask vic_get_irqmask(void);

void vic_set_irqmask(irqmask im);

void vic_enable_irq(int vect_num);

void vic_disable_irq(int vect_num);

//void vic_register_irq(int vect_num, vect_t handler);
void vic_register_irq(int vect_num, irq_handler handler);

void vic_deregister_irq(int vect_num);

void vic_irq_handler(void);

#endif //__VIC_H
