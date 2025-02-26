#ifndef _IOPORT_H
#define _IOPORT_H

void OutByte(unsigned int port, unsigned char val);
void OutWord(unsigned int port, unsigned short val);
void OutDword(unsigned int port, unsigned int val);

unsigned char InByte(unsigned int port);
unsigned short InWord(unsigned int port);
unsigned int InDword(unsigned int port);

#endif
