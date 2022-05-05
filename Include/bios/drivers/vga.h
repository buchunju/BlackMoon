#ifndef VGA_DRIVER
#define VGA_DRIVER

#define VIDEO_MEMORY      0xB8000

#define VGA_ROWS          25
#define VGA_COLUMNS       80
#define VGA_CTRL_REGISTER 0x3d4
#define VGA_DATA_REGISTER 0x3d5
#define VGA_OFFSET_LOW    0x0f
#define VGA_OFFSET_HIGH   0x0e



enum VGA_COLORS {
	BLACK      	= 0x0,
	BLUE       	= 0x1,
	GREEN		= 0x2,
	CYAN		= 0x3,
	RED 		= 0x4,
	MAGENTA		= 0x5,
	BROWN		= 0x6,
	LIGHT_GREY	= 0x7,
	DARK_GREY	= 0x8,
	LIGHT_BLUE	= 0x9,
	LIGHT_GREEN	= 0xa,
	LIGHT_CYAN	= 0xb,
	LIGHT_RED	= 0xc,
	PINK		= 0xd,
	YELLOW		= 0xe,
	WHITE		= 0xf
};

void init_console();
void clear_screen();
void put_string(const char *data);
void set_color(char color);
void backspace();
void set_cursor(unsigned int offset);
void put_at(char character, unsigned int offset);
void put_char(char c);
void disable_cursor();
void enable_cursor();

int  printk(const char *fmt, ...);
int  sprintf(char *str, char *fmt, ...);

unsigned int  scroll(unsigned int offset);
unsigned int  get_cursor();
unsigned int  get_row(unsigned int offset);
unsigned int  get_offset(unsigned int row, unsigned int column);
unsigned int  nl_offset(unsigned int offset);

char get_color(char foreground, char background);

#endif //VGA_DRIVER
