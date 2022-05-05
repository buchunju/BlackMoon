#include "bios/mem/mem.h"
#include "bios/drivers/vga.h"
#include "bios/utils/string.h"

#include <stdbool.h>

void not_enough_memory();

struct node_t {
	size_t size;
	bool  used;
	struct node_t *next;
};

static unsigned int mem_start = 0;
static unsigned int mem_end = 0;
static struct node_t mem_pool[512];
static unsigned int current_pool = 0;

void init_mem(unsigned int begin, unsigned int end)
{
	current_pool = 0;
	mem_start = begin;
	mem_end   = end;

	struct node_t new_node =  {
		.size = end - begin,
		.used = false, 
	};

	mem_pool[0] = new_node;
	current_pool += 1;

}

void *kmalloc(size_t size)
{
	unsigned int location = mem_start;

	if(current_pool == 1)
	{
		if(mem_pool[0].size < size)
			not_enough_memory(size);

		struct node_t new_node = {
			.size = size,
			.used = true,
		};

		mem_pool[0].size -= size;
		mem_pool[current_pool] = new_node;

		current_pool += 1;

		location += mem_pool[0].size;

		return (void *)location;
 	}
 	else
 	{
 		for(int n = 0; n < current_pool; n++)
 		{
 			if(!mem_pool[n].used && mem_pool[n].size >= size)
 			{
 				struct node_t new_node = {
 					.size = size,
 					.used = true,
 				};

 				for(int m = current_pool - 1; m > n + 1; m--)
 				{
 					mem_pool[m + 1] = mem_pool[m];
 				} 

 				mem_pool[n].size -= size;
 				mem_pool[n + 1] = new_node;

 				current_pool += 1;

 				for(int i = 0; i <= n; i++)
 					location += mem_pool[i].size;


 				return (void *)location;

 			}
 		}

 		not_enough_memory(size);
 	}

	return NULL;
}


void kfree(void *location)
{


	unsigned int loc = (unsigned int)location;
	int calc_loc = mem_start;

	for(int i = 1; i < current_pool;i++)
	{
		calc_loc += mem_pool[i - 1].size;
		if(calc_loc == loc)
		{
			// memset(location,'\0', mem_pool[i].size);
			mem_pool[i].used = false;
			break;
		}
	}

	if(current_pool >= 2)
	for(int m = 1; m < current_pool; m++)
	{
		if(!mem_pool[m].used && !mem_pool[m -1].used)
		{
			mem_pool[m-1].size += mem_pool[m].size;
			
			for(int n = m; n + 1 < current_pool; n++)
			{
				mem_pool[n] = mem_pool[n + 1];
			}
			current_pool -= 1;
		}
	}
}

void *krealloc(void *p, uint32_t size)
{
	if(p == NULL)
		return kmalloc(size);

	void *newpos = kmalloc(size);
	memcpy(newpos, p, size);
	kfree(p);

	return newpos;
}


void not_enough_memory(size_t size)
{
	printk("PANIK:   There is not enough memory: %d", size);
	for(;;);
}