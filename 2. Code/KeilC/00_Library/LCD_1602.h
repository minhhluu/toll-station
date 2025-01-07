#ifndef _LCD1602_H_
#define _LCD1602_H_

void Lcd_Cmd(unsigned char cmd);

void Lcd_Chr_Cp(char c); //Current Position
    
//Display a letter at row, col
void Lcd_Chr(unsigned char row, unsigned char col, char c);
    
void Lcd_out_Cp(char *str);

void Lcd_out(unsigned char row, unsigned char col, char *str);

void Lcd_Init();

#endif