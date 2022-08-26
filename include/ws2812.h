#ifndef _WS2812_H_
#define _WS2812_H_

__sbit  __at(0xc8+5) P5_5;
#define NOP() __asm NOP __endasm
#define DI P5_5
// 0
#define D400ns()   {NOP();NOP();NOP();NOP();\
                    NOP();NOP();NOP();NOP();}
#define D850ns()   {NOP();NOP();NOP();NOP();\
                    NOP();NOP();NOP();NOP();\
                    NOP();NOP();NOP();NOP();\
                    NOP();NOP();NOP();NOP();\
                    }
// 1
#define D800ns()   {NOP();NOP();NOP();NOP();\
                    NOP();NOP();NOP();NOP();\
                    NOP();NOP();NOP();NOP();\
                    NOP();NOP();NOP();NOP();\
                    NOP();}
#define D450ns()   {NOP();NOP();NOP();NOP();\
                    NOP();NOP();\
                    }
#define DI_H {DI=1; D800ns(); DI=0; D450ns();}
#define DI_L {DI=1; D400ns(); DI=0; D850ns();}

typedef struct
{
    unsigned char g;
    unsigned char r;
    unsigned char b;
} RGB_t;

void send_one_pixel(void *rgb);
void send_reset();
#endif