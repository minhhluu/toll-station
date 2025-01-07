#include <00_Library\main.h>
#include <00_Library\LCD_1602.h>
#include <00_Library\Delay.h>

sbit LCD_RS = P2^6; //Naming the led pin P0.0
sbit LCD_EN = P2^7;
sbit LCD_RW = P2^5;

#define LCD_DATA P0

void Lcd_Cmd(char cmd)
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
    char cmd;
    //Move pointer to the position
    cmd = (row == 1? 0x80:0xC0) + col - 1;
//		if (row == 1)
//		{
//			cmd = 0x80 + col - 1;
//		}
//		else
//		{
//			cmd = 0xC0 + col - 1;
//		}
    
    Lcd_Cmd(cmd);
    
    //Display a letter
    Lcd_Chr_Cp(c);
}

void Lcd_out_Cp(char *str)
{
    char i = 0;
    while (str[i] != 0)
    {
        Lcd_Chr_Cp(str[i]);
        i++;
    }
}

void Lcd_out(unsigned char row, unsigned char col, char *str)
{
    char cmd;
    
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