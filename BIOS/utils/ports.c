#include "bios/utils/ports.h"

unsigned char inb(unsigned short port)
{
	unsigned char ret;
	__asm__  volatile(" inb %0, %1" : "=a" (ret) : "Nd" (port));

	return ret;
}

void outb(unsigned short  port, unsigned char data)
{
	__asm__  volatile("outb %1, %0" :: "a" (data), "Nd" (port));
}

unsigned short ins(unsigned short _port) {
	unsigned short rv;
	asm volatile ("inw %0, %1" : "=a" (rv) : "Nd" (_port));
	return rv;
}

void outs(unsigned short _port, unsigned short _data) {
	asm volatile ("outw %1, %0" : : "a" (_data), "Nd" (_port));
}


/*
 * Output multiple sets of shorts
 */
void outsm(unsigned short port, unsigned char * data, unsigned long size) {
	asm volatile ("rep outsw" : "+S" (data), "+c" (size) : "d" (port));
}

/*
 * Input multiple sets of shorts
 */
void insm(unsigned short port, unsigned char * data, unsigned long size) {
	asm volatile ("rep insw" : "+D" (data), "+c" (size) : "d" (port) : "memory");
}
