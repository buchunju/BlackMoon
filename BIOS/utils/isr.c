/**
 * This file contains the minimal C code for dispatching
 * interrupts to handlers.
 */

#include "bios/utils/isr.h"
#include "bios/utils/idt.h"
#include "bios/utils/string.h"
#include "bios/utils/pic.h"
#include "bios/utils/ports.h"
#include "bios/utils/stdint.h"
#include "bios/drivers/vga.h"
#include "bios/utils/utils.h"

isr_handler_t interrupt_handlers[256];

void register_interrupt_handler(uint8_t interrupt, isr_handler_t handler)
{
    interrupt_handlers[interrupt] = handler;
}

void isr_handler(registers_t regs)
{
    if(regs.int_no == GENERAL_PROTECTION_FAULT)
    {
        printk("General Protection Fault. Code: %d", regs.err_code);
        printk("General Protection Fault!");
        halt();
    }

    if(interrupt_handlers[regs.int_no])
    {
        //printk("Handling %d!\n", regs.int_no);
        interrupt_handlers[regs.int_no](regs);
        //printk("Returning!\n");
    }
//    else {
//        printk("Got ISR.\n");
//        PANIC("Unhandled ISR.\n");
//    }
}


void irq_handler(registers_t regs)
{
    //If int_no >= 40, we must reset the slave as well as the master
    if(regs.int_no >= 40)
    {
        //reset slave
        outb(SLAVE_COMMAND, PIC_RESET);
    }

    outb(MASTER_COMMAND, PIC_RESET);

    if(interrupt_handlers[regs.int_no])
    {
        interrupt_handlers[regs.int_no](regs);
    }
//    else {
//        if(regs.int_no == 33) return;
//        printk("Got IRQ.\n");
//        char buff[1024];
//        sprintk(buff, "Unhandled IRQ %d\n", regs.int_no);
//        PANIC(buff);
//    }
}
