#include "main.h"
#include <stdio.h>
#include <string.h>



sbit LCD_RS = P2^6; //Naming the led pin P0.0
sbit LCD_EN = P2^7;
sbit LCD_RW = P2^5;

#define LCD_DATA P0

unsigned char led_num[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};
unsigned char led_pos[] = {0, 0x8f, 0x4f, 0x2f, 0x1f};

void Delay_ms(unsigned int t)
{
	unsigned int x, y;
	for (x = 0; x < t; x++)
	{
		for (y = 0; y <123; y++); //Delay 1ms
	}
}

void Lcd_Cmd(unsigned char cmd)
{
	LCD_RS = 0;
	LCD_DATA = cmd;
	LCD_EN = 0;
	LCD_EN = 1;
	if (cmd <= 0x02)
	{
		Delay_ms(2);
	}
	else
	{
		Delay_ms(1);
	}
}

void Lcd_Chr_Cp(char c) //Current Position
{
	LCD_RS = 1;
	LCD_DATA = c;
	LCD_EN = 0;
	LCD_EN = 1;
	Delay_ms(1);
}

//Display a letter at row, col
void Lcd_Chr(unsigned char row, unsigned char col, char c)
{
	unsigned char cmd;
	//Move pointer to the position
	cmd = (row == 1? 0x80:0xC0) + col - 1;
//	if (row == 1)
//	{
//		cmd = 0x80 + col - 1;
//	}
//	else
//	{
//		cmd = 0xC0 + col - 1;
//	}
	
	Lcd_Cmd(cmd);
	
	//Display a letter
	Lcd_Chr_Cp(c);
}

void Lcd_out_Cp(char *str)
{
	unsigned char i = 0;
	while (str[i] != 0)
	{
		Lcd_Chr_Cp(str[i]);
		i++;
	}
}

void Lcd_out(unsigned char row, unsigned char col, char *str)
{
	unsigned char cmd;
	
	//Move pointer to the position
	cmd = (row == 1? 0x80:0xC0) + col - 1;
	
	Lcd_Cmd(cmd);
	
	//Display a word
	Lcd_out_Cp(str);
}

void Lcd_Init()
{
	Lcd_Cmd(0x30);
	Delay_ms(5);
	Lcd_Cmd(0x30);
	Delay_ms(1);
	Lcd_Cmd(0x30);
	
	Lcd_Cmd(0x38); //So dong hien thi la 2, font 5x8;
	
	Lcd_Cmd(0x01);
	Lcd_Cmd(0x0C);
}

void main()
{
	unsigned int a, b, c;
	string t[6] = "";
	c = 0123456;
	//Delete contain in LCD
	//Lcd_Cmd(0x01);
	t[6] = "Hel";
	//Turn on LCD and turn on pointer
	//Lcd_Cmd(0x0E);
	
	//Turn on LCD and turn off pointer
	//Lcd_Cmd(0x0C);
	
	//Display word A in LCD
	//Lcd_Chr_Cp('A');
	P0 = 0x00;
	LCD_RW = 0;
	
	Lcd_Init();
	
	//Lcd_out(1,1, t[6]);
	Lcd_Chr(1,1, t[6]);
	printf("this is char: %s", t[6]);
	
	
	//Display string "Hello World"
	//Lcd_out_Cp("Hello world");
	
	//Lcd_Cmd(0xC4); //Move pointer to second line.
	//Lcd_Chr_Cp('A');
	
//	Lcd_out(1,1,"I'm coming ...");
//	Lcd_out(2,1,"8=========0-----");
//	Delay_ms(1000);
//	Lcd_Cmd(0x01);
//	Lcd_out(1,1,"I'm coming ...");
//	Lcd_out(2,1,"8========0");
//	Delay_ms(500);
//	Lcd_Cmd(0x01);
//	Lcd_out(1,1,"I'm coming ...");
//	Lcd_out(2,1,"8=======0");
//	Delay_ms(500);
//	Lcd_Cmd(0x01);
//	Lcd_out(1,1,"I'm coming ...");	
//	Lcd_out(2,1,"8======0");
//	Delay_ms(500);
//	Lcd_Cmd(0x01);
//	Lcd_out(1,1,"I'm coming ...");	
//	Lcd_out(2,1,"8=====0");	
//	Delay_ms(500);
//	Lcd_Cmd(0x01);
//	Lcd_out(1,1,"I'm coming ...");	
//	Lcd_out(2,1,"8====0");
//	Delay_ms(500);
//	Lcd_Cmd(0x01);
//	Lcd_out(1,1,"I'm coming ...");	
//	Lcd_out(2,1,"8===0");
//	Delay_ms(500);
//	Lcd_Cmd(0x01);
//	Lcd_out(1,1,"I'm coming ...");	
//	Lcd_out(2,1,"8==0");
//	Delay_ms(500);
//	Lcd_Cmd(0x01);
//	Lcd_out(1,1,"I'm coming ...");	
//	Lcd_out(2,1,"8=0");
	while(1)
	{
//		for (a = 0; a < 90000; a++)
//			{
//				b = (a/24)%60;
//				c = a/25/60;
//				P2 = 0x8f;
//				P0 = led_num[b%10];
//				Delay_ms(11);
//				
//				P2 = 0x4f;
//				P0 = led_num[b/10];
//				Delay_ms(10);

//				P2 = 0x2f;
//				P0 = led_num[c%10];
//				Delay_ms(10);
//				
//				P2 = 0x1f;
//				P0 = led_num[c/10];
//				Delay_ms(10);
//			}
	}
}
