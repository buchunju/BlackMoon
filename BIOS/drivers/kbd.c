#include "bios/drivers/kbd.h"
#include "bios/drivers/vga.h"
#include "bios/utils/ports.h"
#include "bios/utils/isr.h"


// static char key_buffer[256];

// const char *sc_name[] = {
// 	"ERROR", "ESC", "1", "2", "3", "4", "5", "6","7", "8", "9", "0", "-", "=", "BACKSPACE", "TAB",
// 	"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]", "ENTER", "LCTRL",
// 	"A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`", "LSHIFT", "\\",
// 	"Z", "X", "C", "V", "B", "N", "M", ",", ".", "/", "RSHIFT", "KEYPAD *", "LALT", "SPACE" 
// };


const char *sc_ascii_lower[] = {
	"?", "?", "1", "2", "3", "4", "5", "6","7", "8", "9", "0", "-", "=", "?", "?",
	"q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "]", "?", "?",
	"a", "s", "d", "f", "g", "h", "j", "k", "l", ";", "'", "`", "?", "\\",
	"z", "x", "c", "v", "b", "n", "m", ",", ".", "/", "?", "?", "?", " " 
};

// const char *sc_ascii_upper[] = {
// 	"?", "?", "!", "@", "#", "$", "%", "^","&", "*", "(", ")", "_", "+", "?", "?",
// 	"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "{", "}", "?", "?",
// 	"A", "S", "D", "F", "G", "H", "J", "K", "L", ":", "\"", "~", "?", "|",
// 	"Z", "X", "C", "V", "B", "N", "M", "<", ">", "?", "?", "?", "?", " " 
// };

void print_code(unsigned char code)
{
	if(code > SC_KEYS) return;

	switch(code)
	{
		case BACKSPACE:
			backspace();
			break;
		case ENTER:
			put_char('\n');
			break;
		default:
			char letter = *sc_ascii_lower[(int) code];
			put_char(letter);
			break;
	}
}

static void kbd_callback(registers_t regs)
{
	unsigned char code = 0;

	if(inb(0x64) & 1)
		code = inb(0x60);
	else
		return;

	if (code & 0x80) {
		/* Special keyboard chars */
	} else {
		print_code(code);
	}
}

void init_keyboard()
{
	register_interrupt_handler(IRQ1, &kbd_callback);
}