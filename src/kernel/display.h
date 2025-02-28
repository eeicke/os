#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "stdint.h"

//Text Colors
#define BLACK           0x0
#define BLUE            0x1
#define GREEN           0x2
#define CYAN            0x3
#define RED             0x4
#define MAGENTA         0x5
#define BROWN           0x6
#define GRAY            0x7
#define DARKGRAY        0x8
#define LIGHTBLUE       0x9
#define LIGHTGREEN      0xA
#define LIGHTCYAN       0xB
#define LIGHTRED        0xC
#define LIGHTMAGENTA    0xD
#define YELLOW          0xE
#define WHITE           0xF

#define MAKECOLOR(fg,bg) ((bg << 4) | fg)

extern int kprintf(char *, ...);
extern void kprint(char *);
extern void kputchar(char);
extern void kprintmem(uint8_t * start, uint32_t length);
int printSignedInteger(int32_t, uint32_t, uint32_t);
int printUnsignedInteger(uint32_t, uint32_t, uint32_t);
int printFloat(float, uint32_t, uint32_t);
void setTextColor(unsigned char);
unsigned char getTextColor();

#endif
