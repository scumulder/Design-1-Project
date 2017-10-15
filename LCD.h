/*
 * LCD.h
 *
 * Created: 4/26/2015 5:16:00 PM
 *  Author: Bhoomi
 */ 


#ifndef LCD_H_
#define LCD_H_

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

void lcd_command(char cmd) //This function is pretty much copied and pasted from the example code given
{
	char temp = cmd;
	PORTD=0;
	_delay_ms(10);
	cmd = ( (cmd & 0xF0) >> 4) | 0x80; //Write Upper Nibble (RS=0) E --> 1
	PORTD = cmd;
	_delay_ms(10);
	cmd ^= 0x80; //E --> 0
	PORTD = cmd;
	_delay_ms(10);
	cmd = temp;
	cmd = ( (cmd & 0x0F) ) | 0x80; //Write Lower Nibble (RS=0) E --> 1
	PORTD = cmd;
	_delay_ms(10);
	cmd ^= 0x80; //E --> 0
	PORTD = cmd;
	_delay_ms(7);
}

void lcd_char(char data)  //This function is pretty much copied and pasted from the example code given
{
	char temp = data;
	PORTD = 0x40;
	_delay_ms(10);
	data = ( (data & 0xF0) >> 4) | 0xC0; //Write Upper Nibble (RS=1) E --> 1
	PORTD = data;
	_delay_ms(10);
	data ^= 0x80; // E --> 0
	PORTD = data;
	_delay_ms(10);
	data = temp;
	data = ( (data & 0x0F) ) | 0xC0; //Write Lower Nibble (RS=1) E --> 1
	PORTD = data;
	_delay_ms(10);
	data ^= 0x80; //E --> 0
	PORTD = data;
	_delay_ms(7);
}

void lcd_string(char *data) //this function is used to print strings onto the LCD
{
	while(*data!= 0)
	{
		lcd_char(*data);
		data++;
	}
	//lcd_command(0x02); //Clear Screen, Cursor Home
	//_delay_ms(50);
}

void lcd_goto(unsigned char y, unsigned char x)
{
	unsigned char firstAddress[] = {0x80,0xC0,0x94,0xD4};
	
	lcd_command(firstAddress[y-1] + x-1);
	_delay_ms(10);
}



#endif /* LCD_H_ */