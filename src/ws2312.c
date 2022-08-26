#include "ws2812.h"
void send_one_pixel(void *p)
{
    unsigned char *ptr = p;
    for(char i = 0; i < 3; ++i)
    {
        char tmp = ptr[i];
        for(char j = 0; j < 8; ++j)
        {
            if(tmp & 0x80)
                DI_H
            else
                DI_L
            tmp <<= 1;
        }
    }
}
void send_reset()
{
    DI = 0;
	unsigned char i, j;
	for(i=0;i<50;i++)		//此处33Mhz时延时65us
	{
		for(j=0;j<20;j++)
		{
			;
		}
	}
}