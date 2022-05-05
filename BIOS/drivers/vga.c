#include "bios/drivers/vga.h"
#include "bios/utils/ports.h"
#include "bios/utils/string.h"

#include <stdarg.h>

static char console_color = 0x0f;

void clear_screen()
{
	for(unsigned int n = 0; n < (VGA_COLUMNS * VGA_ROWS) ; n++)
	{
		*(unsigned short *)(VIDEO_MEMORY + (n * 2)) = 0x0f20;
	}

	set_cursor(get_offset(0,1));
}

void init_console()
{
	clear_screen();
	console_color = get_color(WHITE, BLACK);
}

int printk(const char *fmt, ...)
{
	va_list ap;
	int d, x;
	char c, *s;
	const char *p;
	void *pi;
	char tmp_buf[16];

	va_start(ap, fmt);
	for (p = fmt; *p; p++) {
		if (*p != '%') {
			put_char(*p);
			continue;
		}
		switch (*++p) {
		case 's':              /* string */
			s = va_arg(ap, char *);
			put_string(s);
			break;
		case 'd':              /* int */
			d = va_arg(ap, int);
			itoa(d, tmp_buf);
			put_string(tmp_buf);
			break;
		case 'x':		/* hex */
			x = va_arg(ap, int);
			htoa(x, tmp_buf);
			put_string(tmp_buf);
			break;
		case 'p':		/* pointer */
			pi = va_arg(ap, void *);
			if (pi) {
				htoa((int) pi, tmp_buf);
				put_string(tmp_buf);
			} else {
				put_string("NULL");
			}
			break;
		case 'c':              /* char */
			/* need a cast here since va_arg only
			   takes fully promoted types */
			c = (char) va_arg(ap, int);
			put_char(c);
			break;
		}
	}
	va_end(ap);
	return 0;
}

void put_string(const char *data)
{
	unsigned int offset = get_cursor();
	unsigned int index  = 0;
	while(data[index]  != '\0') {
		if(offset >= (VGA_ROWS * VGA_COLUMNS * 2))
		{
			offset = scroll(offset);
		}

		if(data[index] == '\n')
		{
			offset = nl_offset(offset);
		}
		else {
			put_at(data[index], offset);
			offset += 2;
		}

		index++;
	}
	set_cursor(offset);
}

void set_color(char color)
{
	console_color  = color;
}

void put_char(char c)
{
	unsigned int offset = get_cursor();
	if(c == '\n')
		offset = nl_offset(offset);
	else
		put_at(c, offset);

	set_cursor(offset + 2);
}

void backspace()
{
	unsigned int offset = get_cursor();

	offset -= 2;

	put_at(' ', offset);

	set_cursor(offset);
}

unsigned int scroll(unsigned int offset)
{
	memcpy(
		(char *) (get_offset(1, 0) + VIDEO_MEMORY),
		(char *) (get_offset(0, 0) + VIDEO_MEMORY),
		VGA_COLUMNS * (VGA_ROWS - 1) * 2 );

	for(int col = 0; col < VGA_COLUMNS; col++)
	{
		put_at(' ', get_offset(VGA_ROWS - 1, col));
	}

	return offset - 2 * VGA_COLUMNS;
}

unsigned int get_cursor()
{
	outb(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);

	unsigned int offset = inb(VGA_DATA_REGISTER) << 8;
	outb(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
	offset += inb(VGA_DATA_REGISTER);

	return offset * 2;
}

void set_cursor(unsigned int offset)
{
	offset /= 2;
	outb(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
	outb(VGA_DATA_REGISTER, (unsigned char) (offset >>  8));
	outb(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
	outb(VGA_DATA_REGISTER, (unsigned char) (offset & 0xff));
}

void put_at(char character, unsigned int offset)
{
	unsigned char *video_mem = (unsigned char*) VIDEO_MEMORY;
	video_mem[offset]        = character;
	video_mem[offset + 1]    = console_color;
}

unsigned int  get_row(unsigned int offset)
{
	return (offset / (2 * VGA_COLUMNS) );
}

unsigned int  get_offset(unsigned int row, unsigned int column)
{
	return 2 * ((row * VGA_COLUMNS) + column ) ;
}

unsigned int  nl_offset(unsigned int offset)
{
	return get_offset(get_row(offset) + 1, 0);
}

void disable_cursor()
{
	outb(VGA_CTRL_REGISTER, 0x0A);
	outb(VGA_DATA_REGISTER, 0x20);
}

void enable_cursor()
{
	outb(VGA_CTRL_REGISTER, 0x0A);
	outb(VGA_DATA_REGISTER, (inb(VGA_DATA_REGISTER) & 0xC0) | 0);

	outb(VGA_CTRL_REGISTER, 0x0B);
	outb(VGA_DATA_REGISTER, (inb(VGA_DATA_REGISTER) & 0xE0) | 0);
}

char get_color(char foreground, char background)
{
	return (background << 4) | foreground; 
}

int sprintf(char *str, char *fmt, ...) {
    va_list argp;
    va_start(argp, fmt);

    char *p;
    for(p = fmt; *p != 0; p++) {
        if(*p != '%') {
            *str++ = *p;
            continue;
        }
        p++; // Skip the %
        int i;
        char *s;
        switch(*p) {
        case 'c':
            i = va_arg(argp, int);
            *str++ = i;
            break;
        case 's':
            s = va_arg(argp, char *);
            while(*s) {
                *str++ = *s++;
            }
            break;
        case 'd':
            i = va_arg(argp, int);
            char decbuff[13]; // At most 12 decimal places for 32 bit int.
            char *dec = itos(i, decbuff, 13);
            while(*dec) {
                *str++ = *dec++;
            }
            break;
        case 'x':
            i = va_arg(argp, int);
            for(int j = 28; j >= 0; j-=4)
            {
                *str++ = hex_char(i>>j);
            }
            break;
        case '%':
            *str++ = '%';
            break;
        default:
            *str++ = '%';
            *str++ = *p;
            break;
        }
    }
    *str++ = 0;
    return 0;
}