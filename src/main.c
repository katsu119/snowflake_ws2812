#include <stc12.h>
#include "ws2812.h"
// 定义时钟频率

#define MAIN_Fosc 24000000L

/*
P3.3/INT1
P3.2/INT0
P3.1/TXD/T2
P3.0/RXD/T2CLKO/INT4

P5.4/CMP-/RTS/MCLKO
P5.5/CMP+

PCB Pin Connection
P5.5 DI
P3.1 TXD
P3.0 RXD
*/

/*
24bit MSB Fisrt send, GRB Format
*/

void delay_ms(unsigned int ms)
{
    unsigned int i;
    do
    {
        i = MAIN_Fosc / 13000;
        while (--i)
            ; //14T per loop
    } while (--ms);
}
/************UART Begin************/
volatile static char tx_busy = 0;
volatile static char rx_flag = 0;
SFR(T2H, 0xd6);
SFR(T2L, 0xd7);
void uart_init()//115200bps@33.1776MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR &= 0xFB;		//定时器2时钟为Fosc/12,即12T
	T2L = 0xFA;		//设定定时初值
	T2H = 0xFF;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2
    EA = 1;
    ES = 1;
}
void uart_send_byte(unsigned char c)
{
    tx_busy = 1;
    SBUF = c;
    while(tx_busy);
}
void uart_send_string(char *s)
{
    while(*s)
    {
        tx_busy=1; 
        SBUF = *s++;
        while(tx_busy);
    }
}

void uart_rx_handler(void) __interrupt 4
{
    if(TI == 1)
    {
        tx_busy = 0;
        TI = 0;
    }
    if(RI == 1)
    {
        rx_flag = 1;
        RI = 0;
    }
}
/************UART End************/

/************Timer0 Begin************/
void timer0_init()
{
    AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x33;		//设置定时初值
	TH0 = 0xF5;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
    EA = 1;
    ET0 = 1;
}
void timer0_handler(void) __interrupt 1
{

}
/************Timer0 End************/

RGB_t led_color[37];
inline unsigned char location(char arm, char idx)
{
    if (arm > 5 || idx > 5)
        return 36;
    else
        return (arm * 6) + idx;
}
void send_one_frame()
{
    EA = 0;
    for(char i = 0; i < 37; ++i)
        send_one_pixel(&led_color[i]);
    EA = 1;
    send_reset();
    
}

void ws_clear()
{
    for(char i = 0; i < 37; ++i)
    {
        led_color[i].b = 0;
        led_color[i].g = 0;
        led_color[i].r = 0;
    }
}
void rand_color(unsigned char brightness)
{
    if (brightness == 0)
        ws_clear();
    else
        for(char i = 0; i < 37; ++i)
        {
            led_color[i].b = (i * TL0 + 7) % brightness;
            led_color[i].g = (i * TL0 + 7) % brightness;
            led_color[i].r = (i * TL0 + 7) % brightness;
        }
}

void circle()
{
    for (char i = 0; i < 6; ++i)
    {
        led_color[location(i, 3)].r = 10;
    }
}




void main()
{
    uart_init();
    timer0_init();
    ws_clear();
    led_color[4].r = 10;
    while (1)
    {

        circle();
        rand_color(20);
        send_one_frame();
        delay_ms(10);
    }
}
