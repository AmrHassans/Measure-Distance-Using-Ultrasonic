/*
 ================================================================================================
 Name        : measure_distance_ultrasonic.c
 Author      : Amr Hassan
 Description : system to measure the distance using ultrasonic sensor HC-SR04
 Date        : 14/10/2021
 ================================================================================================
 */

#include<avr/io.h>
#include "gpio.h"
#include "lcd.h"
#include "icu.h"
#include "ultrasonic.h"
#include "std_types.h"

uint16 reading = 0; /* variable to store reading during running */

int main(void)
{
	/* Enable Global Interrupt I-Bit */
	SREG = (1<<7);

	/* Initialize both the LCD and ICU driver */
	LCD_init();
	Ultrasonic_init();


	LCD_displayStringRowColumn(0, 0,"Distance =    cm");/* Permanent text display  */

	while(1)
	{
		LCD_moveCursor(0,11);

		reading = Ultrasonic_readDistance();/*store reading coming from sensor*/

		if(reading >= 100)
		{
			LCD_intgerToString(reading);
		}
		else
		{
			/* display the reading of distance on LCD screen */
			LCD_intgerToString(reading);

			/*to avoid errors on LCD */
			LCD_displayCharacter(' ');
		}
	}
}
