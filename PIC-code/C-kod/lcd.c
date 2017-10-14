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

/* Skriver ut ramp-värdet på displayen*/
void print_ramp_val(char flag, unsigned int signal, unsigned int potentiometer){
           //Rampvärde

        char disp_rampsignal = (char)(signal/10.23);
        if(disp_rampsignal < 10) {
            lcd_goto(0x4A);
            lcd_writesc(" ");
            lcd_goto(0x4B);
            lcd_write(0x30+disp_rampsignal);
        }
        else if(disp_rampsignal < 100) {
            lcd_goto(0x4A);
            lcd_writesc(" ");
            lcd_goto(0x4A);
            lcd_write(0x30+(disp_rampsignal/10));
            lcd_goto(0x4B);
            lcd_write(0x30+(disp_rampsignal%10));
        }
}

void print_battery(char comp_val, char* blink){
    //Battery full
        if(comp_val == 0) {
            lcd_goto(0x4F);
            lcd_write(0x00);
        }

        //Battery low ***********
        if(comp_val == 1) {
            if(*blink%2 == 1){
                lcd_goto(0x4F);
                lcd_write(0x01);
                if(*blink >10)
                    *blink = 1;
            }
            if(*blink%2 == 0){
                lcd_goto(0x4F);
                lcd_write(0x02);
            }
        }
}

//Skapar nya symboler ----------------------------------------------------------
void create_char(void){
    //Battery Full
    lcd_char(0x00);
    lcd_write(0b01110);
    for(int i=1; i<=7; i++){
        lcd_char(0x00+i);
        lcd_write(0b11111);
    }//*************

    //Battery Low 1
    lcd_char(0x08);
    lcd_write(0b01110);
    for(int i=1; i<6; i++){
        lcd_char(0x08+i);
        lcd_write(0b10001);
    }
    for(int i=6; i<8; i++){
        lcd_char(0x08+i);
        lcd_write(0b11111);
    }//*************

    //Battery Low 2
    lcd_char(0x10);
    lcd_write(0b01110);
    for(int i=1; i<7; i++){
        lcd_char(0x10+i);
        lcd_write(0b10001);
    }
    lcd_char(0x17);
    lcd_write(0b11111);
    //*************

    //Rampfunktion
    lcd_char(0x18);
    lcd_write(0x00);
    lcd_char(0x19);
    lcd_write(0x00);
    lcd_char(0x1A);
    lcd_write(0b00011);
    lcd_char(0x1B);
    lcd_write(0b00110);
    lcd_char(0x1C);
    lcd_write(0b01100);
    lcd_char(0x1D);
    lcd_write(0b11000);
    lcd_char(0x1E);
    lcd_write(0x00);
    lcd_char(0x1F);
    lcd_write(0x00);


}

/*Printar ut tiden på Displayen */
void printTime(char sekunder, char minuter, char timmar)
{
    int i = 0;
    int var = 0;
    int pos1 = 0, pos2 = 0;
    for(i=0; i<3; i++){
        switch(i){
                case 0:
                    var = timmar;
                    pos1 = 0x08;
                    pos2 =0x09;
                    break;
                case 1:
                    var = minuter;
                    pos1 = 0x0B;
                    pos2= 0x0C;
                    break;
                case 2:
                    var = sekunder;
                    pos1 = 0x0E;
                    pos2 = 0x0F;
                    break;
        }
        if(var < 10) { //Skriv ut '0' och sen ental
            lcd_goto(pos1);
            lcd_write('0');
            lcd_goto(pos2);
            lcd_write(0x30+var);
        }
        else { //Skriv ut först tiotal och sedan ental
            lcd_goto(pos1);
            lcd_write(0x30+(var/10));
            lcd_goto(pos2);
            lcd_write(0x30+(var%10));
        }
    }
}

void print_bor_ar(unsigned int pot_Val, unsigned int sensor_Val){
            //Skriva börvärde
        char set_Val = (char)(pot_Val/10.23);
        if(set_Val < 10) {
            lcd_goto(0x04);
            lcd_writesc(" ");
            lcd_goto(0x05);
            lcd_write(0x30+set_Val);
        }
        else if(set_Val < 100) {
            lcd_goto(0x04);
            lcd_writesc(" ");
            lcd_goto(0x04);
            lcd_write(0x30+(set_Val/10));
            lcd_goto(0x05);
            lcd_write(0x30+(set_Val%10));
        }

        //Skriva ärvärde
        char act_Val = (char)(sensor_Val/10.23);
        if(act_Val < 10) {
            lcd_goto(0x44);
            lcd_writesc(" ");
            lcd_goto(0x45);
            lcd_write(0x30+act_Val);
        }
        else if(act_Val < 100) {
            lcd_goto(0x44);
            lcd_writesc(" ");
            lcd_goto(0x44);
            lcd_write(0x30+(act_Val/10));
            lcd_goto(0x45);
            lcd_write(0x30+(act_Val%10));
        }
}