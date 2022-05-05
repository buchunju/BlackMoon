#include "bios/utils/pit.h"
#include "bios/utils/isr.h"
#include "bios/utils/ports.h"
#include "bios/drivers/vga.h"
#include "bios/utils/string.h"


#define PIT_NATURAL_FREQ 1193180

#define PIT_DATA0 0x40
#define PIT_DATA1 0x41
#define PIT_DATA2 0x42
#define PIT_COMMAND 0x43

#define DRAW_MODULO 10
uint8_t draw_modcount = 0;

long long tick = 0;

#define BUFFSIZE 77

static void timer_callback(registers_t regs)
{
    draw_modcount = (draw_modcount + 1) % DRAW_MODULO;

    if(draw_modcount == 0)
    {
        tick++;

    }
}

void init_timer(uint32_t frequency)
{
    printk("Initializing PIT timer\n");
    register_interrupt_handler(IRQ0, &timer_callback);

    uint32_t divisor;
    if(frequency)
        divisor = PIT_NATURAL_FREQ / frequency;
    else
        divisor = 0;
    /*
      http://wiki.osdev.org/Programmable_Interval_Timer#I.2FO_Ports

      Channel  Access Mode        Operating Mode    BCD/Binary
      0x36 == 0011 0110 == 00       11 (lobyte/hibyte) 011 (square Wave) 0 (16-bit bin

      )
    */
    outb(PIT_COMMAND, 0x36);

    //Chop freq up into bytes and send to data0 port
    uint8_t low = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

    outb(PIT_DATA0, low);
    outb(PIT_DATA0, high);
}
