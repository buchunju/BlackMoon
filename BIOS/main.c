#include "bios/drivers/vga.h"
#include "bios/drivers/kbd.h"
#include "bios/utils/string.h"
#include "bios/utils/utils.h"
#include "bios/utils/stdio.h"
#include "bios/utils/pic.h"
#include "bios/utils/pit.h"
#include "bios/utils/gdt.h"
#include "bios/utils/idt.h"
#include "bios/fs/fat32.h"
#include "bios/mem/mem.h"

void _dark_entry()
{
	// clear bios messages
	clear_screen();
	// set vga colors
	set_color(get_color(GREEN, BLACK));
	// We are here
	printk("DARK MOON\n");

	//new IDT
	printk("Installed IDT\n");
	init_gdt();

	disable_irq();

	// Setup interupts
	printk("Setup IDT\n");
	init_idt();

	enable_irq();

 	// start timer
	printk("Start timer.\n");
	init_timer(100);

	// disable_irq();
	remap_pic();

	// 100MB RAM
	init_mem(0x100000, 0x1500000);
	printk("Initialized MMU\n");

	// printk("Initialized Filesystem\n");
	// master_fs = makeFilesystem("");
    	// if(master_fs == NULL) {
 //        	printk("Failed to create fat32 filesystem. Disk may be corrupt.\n");
       		// halt();
    	// }

   	printk("Initialized keyboard\n");
   	init_keyboard();

    	// clear_screen();

    	// FILE *f = fopen("/boot/test.txt", NULL);

    	// #define BCOUNT 1000
    	// uint8_t c[BCOUNT];

    	// if(f) {
 //        	printk("READING:.................................\n");
        	// int count = 0, total = 0;
 //        	while((count = fread(&c, BCOUNT, 1, f)), count > 0) {
 //            		for(int i = 0; i < count; i++) {
 //                		printk("%c", c[i]);
 //            		}
 //            		total += count;
 //        	}
 //        	fclose(f);
 //        	printk("Read: %d bytes.\n", total);
 //    	}
 //    	else {
 //        	printk("File not found. Continuing.\n");
 //    	}

    	// if(strcmp(c, "hahaha") == 0)
    	// 	printk("WOrked\n");


	// hang forever
	for(;;);
}
