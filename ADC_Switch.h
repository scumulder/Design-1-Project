/*
 * ADC_Switch.h
 *
 * Created: 4/16/2015 12:12:07 AM
 *  Author: Bhoomi
 */ 


#ifndef ADC_SWITCH_H_
#define ADC_SWITCH_H_

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>
#include "SPI_sine.h"

void getTEMP()
{	
	switch (ADMUX) //ADMUX will start with channel A0 as stated in the initializations

		{
			case 0x40: //get reading from channel 0/PA0 connected to TMP36 IC sensor
				_delay_ms(500);
				double val1 = ADC; 	              //return temp; stored 10-bit ADC value
				double voltage1 = ((val1*5)/ 1024) - 0.5;  
				double celS =  voltage1 * 100.0;
				double fahrS = (celS*1.8) + 32;   
	
				char temp2[10];
				dtostrf(fahrS, 3, 0, temp2);
				lcd_goto(2,1);
				lcd_string("IC Sensor:"); 
				lcd_goto(2,12);
				lcd_string(temp2); //Display temperature in farhS
				lcd_goto(2,15);
				lcd_char(0xB2); //Display the degree sign
				lcd_string("F"); //Display unit F
				
				while(fahrS>82)  //if temperature goes above 82 F, output a sine wave via SPI to turn on the speaker
					{
						output_sine();  //output sine wave
						//Keep checking what the current temperature is
						double val4 = ADC; 	            
						double voltage4 = ((val4*5)/ 1024) - 0.5;
						celS =  voltage4 * 100.0;
						fahrS = (celS*1.8) + 32;
					}
									
				ADMUX = 0x41;
			break;
		
			case 0x41: //get reading from channel 1/PA1 connected to thermistor
				_delay_ms(500);
		
				double val2 = ADC; 	               //return temp; stored 10-bit ADC value
				double voltage2 = (val2*5/ 1023);  // Get a voltage value. 1023 because it's 10-bit range is 0-1023 and 5 because that's the Vref

				/*****CALCULATE THE VARYING THERMISTOR VALUE******/
				double Rx = ((voltage2*10000)/(5-voltage2));   //Using the voltage divider equation
				//find the value of thermistor with the help of a 10kohm resistor.

				/****TO CHECK IF THE RESISTANCE READING IS CORRECT****/
			
				/*char temp5[10];
				dtostrf(Rx, 9, 0, temp5);
				lcd_goto(1,1);
				lcd_string(temp5);
				lcd_goto(1, 10);
				lcd_char(0xDE); //Print the omega symbol*/
				
				double celT = -26.51*log(Rx) + 268.03; //calculate temperature in celsius based on resistance
				double fahrT = (celT * 1.8) + 32; //convert to Fahrenheit
				
				char temp6[10];
				dtostrf(fahrT, 3, 0, temp6);
				lcd_goto(1,1);
				lcd_string("Thermistor:");
				lcd_goto(1,12);
				lcd_string(temp6); //Display temperature in fahrT
				lcd_goto(1,15);
				lcd_char(0xB2); //Display the degree sign
				lcd_string("F"); //Display unit F
				
				ADMUX = 0x40;
			break;
	}
}

#endif /* ADC_SWITCH_H_ */