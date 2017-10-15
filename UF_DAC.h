/*
 * UF_DAC.h
 *
 * Created: 3/27/2015 1:34:56 AM
 *  Author: Bhoomi
 */ 


#ifndef UF_DAC_H_
#define UF_DAC_H_

/*//--------------------------------------
//   LTC1661 10-bit D/A interface
//
//   PIN1 = PortB.2     (/CS)
//   PIN2 = PortB.1     (Clock)
//   PIN3 = PortB.0     (Data)
//   PIN4 = VCC      (VREF)
//   PIN5 = OUTPUTA
//   PIN6 = VCC      (Power)
//   PIN7 = GND      (Ground)
//   PIN8 = OUTPUTB
//
//  Following Code Writes to the DAC via bit-banging.
//  Spi Modules are encouraged for greater efficiency.
//
//  Code by Diego Mesa - University of Florida
//
//----------------------------------------
*/

#include <avr/io.h>


#define DACDIR	DDRB	//Increase Readability
#define DACOUT	PORTB

void dac_write(unsigned char x)
{

	DACDIR |= 0x07;	//Sets bits 0-2 to be outputs on PortB
	DACOUT &= 0x00;
	DACOUT |= 0x04;	//Sets /CS to false (high)
	
	int output;
	char temp = x;
	char temp2 = x;
	signed char i;
	
	temp = temp>>4;
	temp = temp & 0x0F;	//Masking top 4 bits
	temp = temp | 0xF0;	//Inseting control code of F
	
	output = temp;
	output = output<<8;
	
	
	temp2 = temp2<<4;
	output = output | temp2;

	
	DACOUT &= 0xFB;	//Sets the /CS to true(low)
	for (i = 15; i > -1; i--) {

		DACOUT |= (output>>i & 0x01);
		DACOUT |= 0x02;
		DACOUT &= 0xFD;
		DACOUT &= 0xFE;
	}
	
	DACOUT |= 0x04;	//Sets the /CS to false(high)

}



#endif /* UF_DAC_H_ */