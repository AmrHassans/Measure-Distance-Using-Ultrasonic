 /*******************************************************************************\
 *Module: Ultrasonic Sensor                                                    *
 *File Name: ultrasonic.c                                                      *
 *Description: Source file for the AVR Ultrasonic Sensor driver                *
 *Author: Amr Hassan                                                           *
\*******************************************************************************/



#include <avr/io.h>
#include <util/delay.h>
#include"gpio.h"
#include "icu.h"
#include"common_macros.h"
#include "ultrasonic.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

uint8 g_edgeCount = 0;
uint16 g_timeHigh = 0; /*T1*/
uint16 g_timePeriod = 0;/*T2*/
uint16 g_timePeriodPlusHigh = 0;/*T3 = T1+T2*/

uint32 distance = 0;
uint32 timer = 0;

/* Create configuration structure for ICU driver */

Icu_ConfigType ICU_CONFIG= {F_CPU_8,RISING};
/* set the Microcontroller with frequency 8Mh nd to detect the raising
edge as the first edge. */


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void Ultrasonic_init(void)
{
	Icu_init(&ICU_CONFIG);
	Icu_setCallBack(Ultrasonic_edgeProcessing);

	GPIO_setupPinDirection(PORTB_ID, PIN5_ID, PIN_OUTPUT);
	GPIO_writePin(PORTB_ID, PIN5_ID, LOGIC_LOW);
}



void Ultrasonic_Trigger(void)
{
	GPIO_writePin(PORTB_ID, PIN5_ID, LOGIC_HIGH);
	_delay_us(10);
	GPIO_writePin(PORTB_ID, PIN5_ID, LOGIC_LOW);
}


uint16 Ultrasonic_readDistance(void)
{
	Ultrasonic_Trigger();
	if(g_edgeCount == 2)
	{
		/* calculate the distance */
		distance = (float)(g_timeHigh / 58.8);


		/*to overcome the ultrasonic sensor defects ; this equations is used*/
		if (distance >= 2 && distance < 60)
		{
			distance = distance + 1;
		}
		else if (distance >= 60 && distance < 133)
		{
			distance = distance + 2;
		}
		else if (distance >= 133 && distance < 207)
		{
			distance = distance + 3;
		}
		else if (distance >= 207 && distance < 280)
		{
			distance = distance + 4;
		}
		else if (distance >= 280 && distance < 353)
		{
			distance = distance + 5;
		}
		else if (distance >= 353 && distance <= 400)
		{
			distance = distance + 6;
		}
		g_edgeCount = 0;
	}

	return distance;

}


void Ultrasonic_edgeProcessing(void)
{

	g_edgeCount++;

	if(g_edgeCount == 1)
	{
		/*Clear the timer counter register to start measurements from the first detected rising edge*/
		Icu_clearTimerValue();

		/* Detect falling edge */
		Icu_setEdgeDetectionType(FALLING);
	}

	else if(g_edgeCount == 2)
	{
		/* Store the High time (T2) value */
		g_timeHigh = Icu_getInputCaptureValue();

		/* Detect rising edge */
		Icu_setEdgeDetectionType(RISING);
	}
}
