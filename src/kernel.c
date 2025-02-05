/*bruh*/
/* Bare Bones, on OS Dev, im learning the basics of kernel development right now */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


#if defined(__linux__)
#error "not using cc"
#endif

#if !defined(__i386__)
#error "must be compiled with ix86-elf compiler"
#endif

// Consult VGA color palletes, in VGA mode, number of bits controls the number of colors so there is a whole palette out//  there ( We are in 4 bit mode )
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

// inline in C directs the cross compiler to make calls faster for this function, by intergrating function code, in code for its callers.
//
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg){
	return fg | bg << 4;
}
// 8 bit char | 4 bit bg color | 4 bit fg color
static inline uint16_t vga_entry(unsigned char vc, uint8_t color){
	return (uint16_t) vc | (uint16_t) color << 8;
}

// if doesnt work change to the other one
size_t strlen(const char* str){
	size_t len = 0; 
	while (*str != '\0'){
		len++;
		str++;
	}
	return len;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

// index is calculated in a unqiue way due to how vga bitmaps are made. we are referring to each bit of bitmap
void terminal_initialize(void)
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_RED,VGA_COLOR_CYAN);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT;y++)
	{
		for (size_t x = 0; x < VGA_WIDTH;x++)
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_color(uint8_t color)
{
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x ;
	terminal_buffer[index] = vga_entry(c,color);
}

// to implement scrolling, i rpobably gotta change this cause when termianl row hits max, it simply resets :(
void terminal_putchar(char c)
{
	terminal_putentryat(c,terminal_color, terminal_column, terminal_row);
	if (++terminal_column==VGA_WIDTH)
	{
		terminal_column==0;
		if (++terminal_row == VGA_HEIGHT)
		{
			terminal_row=0;
		}
	}
}

void terminal_write(const char* data, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		terminal_putchar(data[i]);
	}
}

void terminal_writestring(const char* data)
{
	terminal_write(data,strlen(data));
}

void kernel_main(void)
{
	terminal_initialize();
	terminal_writestring("Hello, Kernel World!\n");
}




