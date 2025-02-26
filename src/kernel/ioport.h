#ifndef _IOPORT_H
#define _IOPORT_H

void outByte(unsigned int port, unsigned char val);
void outWord(unsigned int port, unsigned short val);
void outDoubleWord(unsigned int port, unsigned int val);
unsigned int inDoubleWord(unsigned int port);
unsigned short inWord(unsigned int port);

#endif
