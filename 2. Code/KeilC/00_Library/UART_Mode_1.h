#ifndef _UARTMODE1_H_
#define _UARTMODE1_H_

// Khoi tao UART Mode 1
void UART_Init();

// Gui 1 ky tu ra UART
void UART_Write(char c);

// Gui 1 chuoi ra UART
void UART_Write_Text(char *str);

// Kiem tra xem co nhan duoc byte du lieu khong
// Tra ve 1: Da nhan duoc
// Tra ve 0: Chua nhan duoc
char UART_Data_Ready();

// Doc byte nhan duoc tu UART
char UART_Read();

//void UART_Read_Text(char *str);

#endif