#include <00_Library\main.h>
#include <00_Library\UART_Mode_1.h>

#if (FREQ_OSC != 11059200)
    #error "Thu vien UART_Mode_1 chi ho tro tan so hoat dong la 11059200"
#endif

void UART_Init()
{
    SM0 = 0;
    SM1 = 1; 					//Chon UART mode 1
    
    TMOD &= 0x0F; 			//0010 xxxx - Timer 1 hoat dong o che do 8-bits tu dong nap lai
    TMOD = 0x20;
    
    #if (BAUD_RATE == 9600)
    TH1 = 0xFD;				//Toc do baud 9600
    
    #elif (BAUD_RATE == 2400)
    TH1 = 0xF4;				//Toc do baud 2400
    
    #elif (BAUD_RATE == 1200)
    TH1 = 0xE8;				//Toc do baud 1200
    
    #elif (BAUD_RATE == 19200)
    TH1 = 0xFD;				//Toc do baud 19200
    PCON |= 0x80;
    #else
        #error "Toc do baud chi co the la 1 trong 4 so sau: 19200, 9600, 2400, 1200."
    #endif
    
    TR1 = 1; 					//Timer 1 se bat dau chay
    TI = 1;						//Ready to send du lieu
    REN = 1;					//Ready to receive du lieu
}

void UART_Write(char c)
{
    while (TI == 0);		//Neu viec gui ky tu truoc do chua hoan tat thi wait
    TI = 0;
    SBUF = c;
}

void UART_Write_Text(char *str)
{
    char i = 0;
    while(str[i] != 0)
    {
        UART_Write(str[i]);
        i++;
    }
}

char UART_Data_Ready()
{
    return RI;
}

char UART_Read()
{
    while (RI == 0);
		RI = 0;
    return SBUF;
}
