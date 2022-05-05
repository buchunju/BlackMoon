#ifndef PORTS_H
#define PORTS_H

unsigned char inb(unsigned short port);
unsigned short ins(unsigned short _port);
void insm(unsigned short port, unsigned char * data, unsigned long size);
void outb(unsigned short port, unsigned char data);
void outs(unsigned short _port, unsigned short _data);
void outsm(unsigned short port, unsigned char * data, unsigned long size);

#endif //PORTS_H
