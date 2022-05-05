#ifndef __UTIL_H
#define __UTIL_H

static inline void enable_irq()
{
	__asm__ volatile("sti");
}

static inline void disable_irq()
{
	__asm__ volatile("cli");
}

static inline void halt()
{
	__asm__ volatile("hlt");
}

#endif