/*
 *	LCD interface for MPLAB XC8 compiler	
 *
 *	This code will interface to a standard LCD controller
 *	like the Hitachi HD44780. It uses it in 4 bit mode, delay mode, with
 *	the hardware connected as follows (the standard 14 or 16 pin
 *	LCD connector is used).
 *	
 *	Default Settings:
 *	LCD D4-D7 data pins are connected to PORTB pins RB0-RB3 
 *	LCD EN pin is connected to PORTB pin RB4 
 *      LCD RS pin is connected to PORTB pin RB5
 *	LCD_RW pin is connected to ground
 *      Fosc=4 MHz
 *	
 *  To use these routines:
 *  Include lcd.h in main source file
 *  Add lcd.c to project source files
 *  Choose LCD_EN, LCD_RS and LCD_Dx pins in the #defines below
 *  Set up the used port I/O to digital outputs (ANSEL, ADCON, TRIS).
 *  Call lcd_init(), then call other routines as required.
 *
 */
#include <xc.h>
#include "lcd.h"

#define	 _XTAL_FREQ 4000000

#define LCD_D4 LATBbits.LATB2			//LCD Databit D4
#define LCD_D5 LATBbits.LATB3			//LCD Databit D5
#define LCD_D6 LATBbits.LATB4			//LCD Databit D6
#define LCD_D7 LATBbits.LATB5			//LCD Databit D7

#define LCD_EN LATCbits.LATC3			//LCD Enable
#define	LCD_RS LATCbits.LATC2			//LCD Register Select

#define	LCD_STROBE() {LCD_EN=1; __delay_us(1); LCD_EN=0;}
 
/* Write a byte to the LCD in 4 bit mode */
void lcd_write(unsigned char c)
{	if(c & 0x80) LCD_D7=1; else LCD_D7=0;
	if(c & 0x40) LCD_D6=1; else LCD_D6=0;
	if(c & 0x20) LCD_D5=1; else LCD_D5=0;
	if(c & 0x10) LCD_D4=1; else LCD_D4=0;
	LCD_STROBE();
	if(c & 0x08) LCD_D7=1; else LCD_D7=0;
	if(c & 0x04) LCD_D6=1; else LCD_D6=0;
	if(c & 0x02) LCD_D5=1; else LCD_D5=0;
	if(c & 0x01) LCD_D4=1; else LCD_D4=0;
	LCD_STROBE();
	__delay_us(50);
}
void lcd_char(unsigned char cgram_addr){
    LCD_RS = 0;
    lcd_write(0x40+cgram_addr);
    LCD_RS=1;
}


/*Clear and home the LCD*/
void lcd_clear(void)
{	LCD_RS = 0;
	lcd_write(0x01);
	LCD_RS=1;
	__delay_ms(2);
}


/* Write a string constant to the LCD */
void lcd_writesc(const char * s)
{	while(*s != '\0')
		lcd_write(*s++);
}


/* Write a string variable to the LCD */
void lcd_writesv( char * s)
{	while(*s != '\0')
		lcd_write(*s++);
}


/* Go to the specified position*/
void lcd_goto(unsigned char pos)
{	LCD_RS = 0;
	lcd_write(0x80+pos);
	LCD_RS=1;
}


/* Write a command to the LCD*/
void lcd_cmd(unsigned char command)
{	LCD_RS = 0;
	lcd_write(command);
	LCD_RS=1;
}


/* Initialise the LCD - put into 4 bit mode */
void lcd_init(void)
{	LCD_RS = 0;	
	LCD_EN = 0;
	__delay_ms(40);		// Wait 40ms after power applied
	LCD_D4 = 1;		
	LCD_D5 = 1;
 	LCD_D6 = 0;		
	LCD_D7 = 0; 
	LCD_STROBE();	
	__delay_ms(6);		// Wait 6ms
	LCD_STROBE();	
	__delay_ms(2);
	LCD_STROBE();	
	__delay_ms(2);
	LCD_D4 = 0;		// Set 4 bit mode
	LCD_STROBE();	
	__delay_ms(2);
	lcd_cmd(0b00101000);	// Set interface length 4-bit, 2-line, 5x8 dots display
	lcd_cmd(0b00001100);	// Display On, Cursor OFF, Cursor Blink OFF
	lcd_cmd(0b00000110);	// Set entry Mode
	lcd_clear();		// Clear screen

        lcd_goto(0);				// Select first line position 0
        lcd_writesc("b r:  % 00:00:00");
        lcd_goto(0x40);
        lcd_writesc(" r:   % R:  %  ");
        lcd_goto(0x01);
        lcd_write(0xEF);
        lcd_goto(0x40);
        lcd_write(0xE1);
        lcd_goto(0x07);
        lcd_write(0xFF);
        lcd_goto(0x47);
        lcd_write(0xFF);
}
