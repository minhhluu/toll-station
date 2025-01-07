#include <00_Library\main.h>
#include <00_Library\Delay.h>
#include <00_Library\UART_Mode_1.h>
#include <00_Library\LCD_1602.h>

//Naming the led pin P2
sbit LCD_RS = P2^6; 
sbit LCD_EN = P2^7;
sbit LCD_RW = P2^5;

#define LCD_DATA P0

sbit PWM_Pin = P2^0;

unsigned int T, Ton, Toff;
unsigned char Ton_H_Reload, Ton_L_Reload, Toff_H_Reload, Toff_L_Reload;

// cycle (unit - us)
void PWM_Init(unsigned int cycle)
{
    PWM_Pin = 1;
    
    TMOD &= 0xF0;       // Xoa di cac bit chon mode cua Timer0
    TMOD |= 0x01;       // Timer0 hoat dong o mode 1
    
    ET0 = 1;            // Cho phep ngat Timer0
    EA = 1;             // Cho phep ngat toan cuc
    
    T = cycle;
    
    Ton = T/2;          // Duty  cycle = 50%
    Toff = T - Ton;
    
    Ton_H_Reload = (65536-Ton)>>8;
    Ton_L_Reload = (65536-Ton)&0x00FF;
    
    Toff_H_Reload = (65536-Toff)>>8;
    Toff_L_Reload = (65536-Toff)&0x00FF;
    
    TH0 = Ton_H_Reload;
    TL0 = Ton_L_Reload;
}

void PWM_Start()
{
    TR0 = 1;            // Timer0 bat dau dem
}

void PWM_Stop()
{
    TR0 = 0;
}

// Duty cycle tu 0 den 100%
void PWM_Set_Duty(unsigned char duty)
{
    if (duty == 0)
    {
        PWM_Pin = 0;
        ET0 = 0;
    }
    else if (duty == 100)
    {
        PWM_Pin = 1;
        ET0 = 0;
    }
    else
    {
        Ton = ((unsigned long)T)*duty/100;
        Toff = T - Ton;
    
        Ton_H_Reload = (65536-Ton)>>8;
        Ton_L_Reload = (65536-Ton)&0x00FF;
    
        Toff_H_Reload = (65536-Toff)>>8;
        Toff_L_Reload = (65536-Toff)&0x00FF;
        
        ET0 = 1;
    }
}

void main()
{
    unsigned int a = 0, b;
    char t[16] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
    char character[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
    char i, j;
		char *messages[] = {"Vehicle 1       ", "Vehicle 2       ", "Vehicle 3       ", "Vehicle 4       ", "Access denied   "};
		int temp = 1;
		int cnt_motor = 0;
		
		//PWM_Init(1000);     // T = 1000us = 1ms

    
    // LCD Display
    LCD_RW = 0;
    Lcd_Init();
		Lcd_out(1,1, "RFID is: ");
    
   
    //P1 = 0x0C;
    P2_0 = 1;
    P2_1 = 1;
    
		P1 |= 0x03;
		

		while (1){
////////////////////////////////////////////////////////////////////////////////////////////////
			if (temp == 0){
					Lcd_out(1,1, "RFID is:        ");
					Lcd_out(2,1, "                ");
					temp = 1;
			}		
			if (temp == 1){
					
						if (P1_0 == 0) //sensor in
						{
							temp = 2; //co vat can
							//Lcd_out(2,1, "Vehicle Detected");
						} 
					} else if (temp == 2){
						
					// Khoi tao UART o che do 1, 9600 baud, 1 start, 8-bit du lieu, 1 bit stop
					LCD_RW = 0;
					Lcd_Init();
					UART_Init();
					temp = 3;
					Delay_ms(100);
					Lcd_out(1,1, "Vehicle Detected");
					Lcd_out(2,1, "Your card, pls  ");
					Delay_ms(100);
					
					
					} else if (temp == 3){
							
						if (UART_Data_Ready()) {
						i = UART_Read();
        
        // Validate character
        if ((i >= 'a' && i <= 'z') || (i >= 'A' && i <= 'Z') || 
            (i >= '0' && i <= '9') || i == ' ') {
            t[a++] = i; // Store and increment index
        }

        // When two characters are captured (bien dem, dem len tu 2 lan tro len, bit 1 la ki tu trong)
        if (a >= 2) {
            if (t[1] >= '1' && t[1] <= '4') {
                Lcd_out(2, 1, messages[t[1] - '1']);
								temp = 4;
            } else {
                Lcd_out(2, 1, messages[4]); // Default message (access denied)
								Delay_ms(3000);
								temp = 0;
            }
							a = 0; // Reset index
						}
					}
						
					} else if (temp ==4){
								Delay_ms(2000);
								PWM_Init(20000);     // T = 20000us = 20ms
								PWM_Set_Duty(0);
								PWM_Start();
									
								Delay_ms(1000);
								Lcd_Cmd(0x01);
								Lcd_out(2,1, "You may pass    ");

							
								// open the barrier
								PWM_Set_Duty(5);
								Delay_ms(3000);
								temp = 5;
						
					} else if (temp == 5){
							//close the barrier
						  if (P1_1 == 0) //sensor out
							{
                PWM_Set_Duty(10);
                Delay_ms(3000);
								temp = 6;
								EA = 0;
							}
								Delay_ms(100);
								Lcd_out(2,1, "Waiting...      ");
											
					} else if (temp == 6){
						//write Uart to 8051
							LCD_RW = 0;
							Lcd_Init();
							UART_Init();
							temp = 7;
							Delay_ms(100);

				
				} else if (temp == 7){
							UART_Write(t[1]);
							if (UART_Data_Ready()) {
									j = UART_Read();
									if (j == 'a'){
											Lcd_out(2,1, "Sending data... ");
											temp = 8;
//										SBUF = 0;
											Delay_ms(2000);
										}	
								}		
					} else if (temp == 8){
							UART_Write('b');
								if (UART_Data_Ready()) {
									j = UART_Read();
									if (j == 'c'){
											temp = 0;
											Lcd_out(1,1, "Data sent       ");
											Lcd_out(2,1,"successfully!   ");
											Delay_ms(3000);
										}	
					}
			}
	}
}

void T0_Overflow() interrupt 1
{
    PWM_Pin = !PWM_Pin;
    if (PWM_Pin == 0)
    {
        TH0 = Toff_H_Reload;
        TL0 = Toff_L_Reload;
    }
    else
    {
        TH0 = Ton_H_Reload;
        TL0 = Ton_L_Reload;
    }
        
}