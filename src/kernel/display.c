//Text Mode 16 Colors:
//black(0) blue(1) green(2) cyan(3) red(4) magenta(5) brown(6) light gray(7) dark gray(8)
//light blue(9) light green(10) light cyan(11) light red(12) light megenta(13) yellow(14) 
//white(15)

#define NULL 0

unsigned char * VIDEOBUFFER = (unsigned char *)0xb8000;
const unsigned char WIDTH = 80;
const unsigned char HEIGHT = 25;
const unsigned char BYTESPERCHAR = 2;
unsigned int currentX = 0;
unsigned int currentY = 0;

void kprintWithColor(char *message, unsigned char color)
{
    if (message == NULL)
        return;

    for (;*(message) != '\0'; ++message)
    {
        if (currentX >= WIDTH)
        {
            ++currentY;
            currentX = 0;
        }

        if (currentY >= HEIGHT)
        {                 
            //scroll up                   
            for (int i = 0; i < (HEIGHT * WIDTH) * BYTESPERCHAR; ++i)
            {
                if (i < ((HEIGHT - 1) * (WIDTH * BYTESPERCHAR)))
                    VIDEOBUFFER[i] = VIDEOBUFFER[i + (WIDTH * BYTESPERCHAR)];
                else
                    VIDEOBUFFER[i] = '\0';                    
            }

            --currentY;
        }

        if (*(message) == '\n')
        {
            currentX = 0;
            ++currentY;            
            continue;
        }

        if (*(message) == '\r')
        {
            currentX = 0;
            continue;
        }

        unsigned int actualLocation = currentY * (WIDTH * BYTESPERCHAR) + (currentX * BYTESPERCHAR);

        VIDEOBUFFER[actualLocation] = *message;
        VIDEOBUFFER[actualLocation + 1] = color;

        ++currentX;     
    }
}

void kprint(char *message)
{
    kprintWithColor(message, 0x07);
}