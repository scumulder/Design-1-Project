/*
 * SPI_sine.h
 *
 * Created: 4/26/2015 5:12:49 PM
 *  Author: Bhoomi
 */ 


#ifndef SPI_SINE_H_
#define SPI_SINE_H_

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

int sinewave[50] = {0x200,0x240,0x27F,0x2BC,0x2F6,0x32C,0x35E,0x38A,0x3AF,0x3CE,0x3E6,0x3F6,0x3FE,0x3FE,0x3F6,0x3E6,0x3CE,0x3AF,0x38a,0x35E,0x32C,0x2F6,0x2BC,0x27F,0x240,0x200,0x1BF,0x180,0x143,0x109,0xD3,0xA1,0x75,0x50,0x31,0x19,0x9,0x1,0x1,0x9,0x19,0x31,0x50,0x75,0xA1,0xD3,0x109,0x143,0x180,0x1BF};


void SPI_MasterTransmit(int cData)
{
	int cDataH;
	int cDataL;
	cDataH = cData;
	cDataH >>= 6;
	cDataH &= 0x0F;
	cDataH |= 0xF0;
	cDataL = cData;
	cDataL &= 0x3F;
	cDataL <<=2;
	SPDR0 = cDataH;
	while (!(SPSR0 & 0x80));
	SPDR0 = cDataL;
	while (!(SPSR0 & 0x80));
	//SPDR0 = cData; /* Start transmission */
	//while(!(SPSR0 & (1<<SPIF0))); /* Wait for transmission complete */
}

void output_sine()
{
	for(int x=0; x<50; x++)
	{
		PORTB &= 0xEF;		//start reading. Enable /SS
		SPI_MasterTransmit(sinewave[x]);
		PORTB |= 0x10;		//command to stop reading. Disable /SS
		_delay_ms(2);
	}
}


#endif /* SPI_SINE_H_ */