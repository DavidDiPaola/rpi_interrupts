#ifndef __VIC_H
#define __VIC_H

/*
    See vic.c for inportant notes.
*/

typedef volatile void(*vect_t)(void);

//see the table page 113 of the BCM2835 Arm Peripherals datasheet for vector numbers
#define VECT_SYSTIMERM1 1  //triggered when the system timer matches the C1 register
#define VECT_PL011      57 //triggered when the PL011 asserts IRQ status

void vic_init(void);

void vic_register(int vect_num, vect_t handler);

void vic_deregister(int vect_num);

void vic_irq_handler(void);

#endif //__VIC_H
