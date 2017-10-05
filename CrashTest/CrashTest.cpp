/*
 * CrashTest.cpp
 *
 * Created: 11.07.2017 10:30:49
 *  Author: user
 */ 

#define F_CPU 14745600UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define TRANSISTORS_DDR DDRC
#define TRANSISTORS_PORT PORTC
#define TRANSISTOR1_OUT 0
#define TRANSISTOR2_OUT 1

#define BUTTON_DDR DDRA
#define BUTTON_PIN PINA
#define BUTTON_PORT PORTA
#define BUTTON_IN 2

uint16_t timer0 = 0;
bool IsButtonPressed = false;

ISR(TIMER0_OVF_vect)
{
	if((BUTTON_PIN & (1 << BUTTON_IN)) == (1 << BUTTON_IN))
	{
		timer0++;
	}
	else if((BUTTON_PIN & (1 << BUTTON_IN)) == (0 << BUTTON_IN))
	{
		timer0 = 0;
	}
	if(timer0 > 10)
	{
		IsButtonPressed = true;
		timer0 = 0;
	}
}

void InitPorts(void)
{
	TRANSISTORS_DDR |= (1 << TRANSISTOR1_OUT)|(1 << TRANSISTOR2_OUT);
	TRANSISTORS_PORT &= ~(1 << TRANSISTOR1_OUT)|(1 << TRANSISTOR2_OUT);
	BUTTON_DDR &= ~(1 << BUTTON_IN);
}

void InitTimer0(void)
{
	TCCR0B = (1 << CS02)|(0 << CS01)|(1 << CS00);
	TIMSK0 = (1 << TOIE0);
}

int main(void)
{
	_delay_ms(100);
	InitPorts();
	InitTimer0();
	
	sei();

	while(1)
	{
		_delay_ms(100);
		if(IsButtonPressed == true)
		{
			for(uint8_t i = 0; i < 10; i++)
			{
				//open
				TRANSISTORS_PORT &= ~(1 << TRANSISTOR1_OUT);
				TRANSISTORS_PORT |= (1 << TRANSISTOR2_OUT);
				_delay_ms(30000);
				TRANSISTORS_PORT &= ~(1 << TRANSISTOR1_OUT);
				TRANSISTORS_PORT &= ~(1 << TRANSISTOR2_OUT);
				_delay_ms(5000);
				
				//close
				TRANSISTORS_PORT &= ~(1 << TRANSISTOR2_OUT);
				TRANSISTORS_PORT |= (1 << TRANSISTOR1_OUT);
				_delay_ms(30000);
				TRANSISTORS_PORT &= ~(1 << TRANSISTOR1_OUT);
				TRANSISTORS_PORT &= ~(1 << TRANSISTOR2_OUT);
				_delay_ms(5000);
			}
			
			IsButtonPressed = false;
		}
	}
}