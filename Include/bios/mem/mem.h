#ifndef __MEM_H_
#define __MEM_H_

#include "bios/utils/stddef.h"
#include "bios/utils/stdint.h"

void init_mem(unsigned int begin, unsigned int end);
void *kmalloc(size_t size);
void *krealloc(void *p, uint32_t size);
void kfree(void *location);

#endif //__MEM_H_