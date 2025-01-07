#include <00_Library\main.h>

void Delay_ms(unsigned int t)
{
    unsigned int x, y;
    for (x = 0; x < t; x++)
    {
            for (y = 0; y <123; y++); //Delay 1ms
    }
}