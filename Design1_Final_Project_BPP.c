/*
 * Design1_Final_Project_BPP.c
 *
 * Created: 3/30/2015 11:07:14 PM
 *  Author: Bhoomi
 *  Comments: Here's what the following code does:
              1. Gets the voltage from the output of the TMP36 temperature sensor using the ADC channel A0 on the ATmega324P.
			  2. Converts that voltage into fahrenS and celS values and displays them on an LCD
			  3. If the value of the temperature crosses X the DAC will output a signal
 
    *****Calculations for sine wave generation*****
	
	e.g. For sin[x]
	x= number of samples and it's range is 0 to (x-1)
	Required SAMPLE RATE to produce desired output frequency = output frequency x no. of samples
	
	Maximum output frequency = (max. sample rate for 10-bit DAC) / (max. no. of samples for 10-bit DAC)
	
	where Max. sample rate = 
	max. number of samples = 1024
 
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#define F_CPU 2000000      // Processor speed for delay functions 1MHz
#include "LCD.h"
#include "ADC_Switch.h"
#include "SPI_sine.h"
#include <stdlib.h>

///////////////////////////////FUNCTIONS DECLARATIONS///////////////////////////////
void ADC_INIT();
void SPI_MasterINIT();
void LCD_INIT();

///////////////////////////////////MAIN FUNCTION////////////////////////////////////
int main(void)
{
	DDRC |= 0x00;                           // Set all the pins on PORTD as inputs
	
 	LCD_INIT();  //Initialize the LCD
	ADC_INIT();  //Initialize the ADC
	SPI_MasterINIT(); //Initialize the SPI

	while(1)
    {
        getTEMP();
    }
}

/////////////////////////////////FUNCTION DEFINITIONS////////////////////////////////

void LCD_INIT()  //This function is pretty much copied and pasted from the example code given
{
	/*
	******FOR LCD: Pins 3:0 on PORTD of MCU connected to DB7:4 of LCD
	******RS->PORTD6, R/W->GND, E->PORTD7
	*/
	
	DDRD |= 0xCF;
	lcd_command(0x33); //Initialize LCD Driver
	lcd_command(0x32); //Four bit mode
	lcd_command(0x2C); //2 Line Mode
	lcd_command(0x0C); //Display On, Cursor Off, Blink Off Change to 0x0F if cursor is desired
	lcd_command(0x01); //Clear Screen, Cursor Home
}

void ADC_INIT() //Configure the ADC
{
	DDRA |= (0<<PORTA0) | (0<<PORTA1);        //Set pin 0 and pin 1 on PORTA as inputs
	ADMUX = 0b01000000;         //Enable ADLAR, ADC channel 0
	ADCSRA = 0xE4;              //Enable the ADC, auto triggering, pre-scaler of 16, and interrupt flag.
	
	//ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADATE);    //Enable the ADC //ADC Interrupt enable //For pre-scaler of 16, ADPS2:0 = 100
 	//ADMUX |= (1<<ADLAR) | (1<<REFS0);   //Enable Left Adjust Result //Use AVCC as voltage reference
	//ADCSRA |= 1<<ADSC; //Start conversion

	_delay_ms(5);
}

void SPI_MasterINIT()
{
	DDRB  = (1<<4)|(1<<5)|(1<<7);//0xB0;  //Set MOSI and SCK as output
	SPCR0 = (1<<SPE0)|(1<<MSTR0); /* Enable SPI, Master, set clock rate fclk/4 */
	SPSR0 = 0x00;
}