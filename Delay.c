#include "Delay.h"
#include <STC8G.H>

void Delay_ms(unsigned int ms)	//@11.0592MHz
{
	unsigned char data i, j;
    unsigned int k;
	
    for(k = 0; k < ms; k++)
    {
        i = 15;
        j = 90;
        do
        {
            while (--j);
        } while (--i);
    }
}