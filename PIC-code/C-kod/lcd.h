/*
 *	LCD header file
 *	See lcd.c for more info
 *
*/

/* Clear and home the LCD */
extern void lcd_clear(void);

/* Write a string constant to the LCD */
extern void lcd_writesc(const char * s);

/* Write a string variable to the LCD */
extern void lcd_writesv(char * s);

/* Go to the specified position */
extern void lcd_goto(unsigned char pos);
	
/* Intialize the LCD - call before anything else */
extern void lcd_init(void);

/* Write a command to the LCD*/
extern void lcd_cmd(unsigned char command);

/* Write a character to the LCD */
extern void lcd_write(unsigned char c);

/* Choose CG-RAM adres */
extern void lcd_char(unsigned char cgram_addr);

/* Skriver ut ramp-värde på disp*/
extern void print_ramp_val(char flag, unsigned int signal, unsigned int potentiometer);

/* Skriver ut Low-battery symbol på disp*/
extern void print_battery(char comp_val, char* blink);

/* Skapa nya symboler*/
void create_char(void);

/* Skriver ut tiden på displayen*/
void printTime(char sekunder, char minuter, char timmar);

/* Skriver ut är och börvärden på disp*/
void print_bor_ar(unsigned int pot_Val, unsigned int sensor_Val);



