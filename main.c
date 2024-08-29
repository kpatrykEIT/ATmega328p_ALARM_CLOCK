/*
 * ATmega328P_ALARM_CLOCK.c
 *
 * Created: 27.08.2024 15:19:22
 * Author : kpatrykEIT
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "hd44780.h"
#include "defines.h"
#include "LCD.h"

// ============== DEFINES ================= // 

#define SUBMIT_BUTTON	PD7
#define ADD_BUTTON		PD6
#define BUZZER			PB5
#define LED1			PB4
#define LED2			PB3
#define LED3			PB2
#define LED4			PB1
#define LED5			PB0

// ============== FUNCTIONS =============== //

void TIMER1_init(void);
void TIMER0_init(void);
void CLOCK_MENU(char* MENU_TEXT);

// ============== VARIABLES =============== //

uint8_t ADD_BUTTON_STATE		= 0;
uint8_t SUBMIT_BUTTON_STATE		= 0;

volatile uint8_t miliseconds	= 0;
volatile uint8_t seconds		= 0;
volatile uint8_t minutes		= 0;
volatile uint8_t hours			= 0;

volatile uint8_t endFlag		= 0;

volatile uint8_t msCounter		= 0;
volatile uint8_t secondsCounter = 0;

char msBufor[10];
char secBufor[10];
char minBufor[10];
char hourBufor[10];


// ======================================== //


int main(void)
{
	DDRD &= ~(1 << ADD_BUTTON) | ~(1 << SUBMIT_BUTTON);
	DDRB |=	 (1 << BUZZER) | (1 << LED1) | (1 << LED2) 
			 | (1 << LED3) | (1 << LED4) | (1 << LED5);	
			 
    lcd_init();
	TIMER0_init();
	TIMER1_init();
	sei();
	
    while (1) 
    {
		if( (PIND & _BV(SUBMIT_BUTTON)) == 0)
		{
			SUBMIT_BUTTON_STATE ++;
			_delay_ms(100);
		}
		
		switch(SUBMIT_BUTTON_STATE)
		{
 // ============== SET SECONDS =============== //	
 			
			case 0: 
				
				if( (PIND & _BV(ADD_BUTTON)) == 0)
				{
					seconds += 1;
					if(seconds > 59)
						seconds = 0;
				}
			CLOCK_MENU("***SET ALARM***");
			break;
			
 // ============== SET MINUTES =============== //

			case 1:	
				if( (PIND & _BV(ADD_BUTTON)) == 0)
				{
					minutes += 1;
					if(minutes > 59)
					minutes = 0;
					
				}
			CLOCK_MENU("***SET ALARM***");
			break;
			
 // ============== SET HOURS ================ //
			
			case 2:
				if( (PIND & _BV(ADD_BUTTON)) == 0)
				{
					hours += 1;
					if(hours > 23)
					hours = 0;
					
				}
			CLOCK_MENU("***SET ALARM***");
			break;
	
 // ============== SUBMIT =================== //
			
			case 3:
				if(seconds != 0 || hours != 0 || minutes != 0)
				{
					CLOCK_MENU("****RUNNING****");
					TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
					if(secondsCounter >= 0)
					{
						PORTB |= (1 << secondsCounter);
						if(secondsCounter == 5)
						{
							PORTB = 0;
							secondsCounter = 0;
						}
					}
					
				}
				
				if(endFlag == 1)
				{
					TCCR1B = 0;
					CLOCK_MENU("**TIME IS UP!**");
					for(int i =0; i<5; i++)
					{
						PORTB |= (1 << i);
					}
					TCCR0A |= (1 << WGM01);
					TCCR0B |= (1 << CS02) | (1 << CS00);
				}
				
			break;
			
 // ============== STATE ZERO =============== //
			
			default:
			SUBMIT_BUTTON_STATE = 0;
			seconds = 0;
			minutes = 0;
			hours	= 0;
			endFlag = 0;
			PORTB = 0;
			TCCR1B = 0;
			TCCR0B = 0;
			CLOCK_MENU("***SET ALARM***");
			break;
 // ========================================= //				
		}
 		
    }
}
void TIMER0_init(){
	OCR0A = 156; // 10ms
	TIMSK0 |= (1 << OCIE0A); 
}

void TIMER1_init()
{
	OCR1A = 156; // 10ms
	TIMSK1 |= (1 << OCIE1A);
}

void CLOCK_MENU(char* MENU_TEXT)
{
	lcd_goto(0,0);
	lcd_puttext(MENU_TEXT);
	sprintf(secBufor, "%d", seconds);
	sprintf(minBufor, "%d", minutes);
	sprintf(hourBufor, "%d", hours);
	lcd_goto(10,1);
	lcd_puttext(secBufor);
	lcd_goto(9,1);
	lcd_puttext(":");
	lcd_goto(6,1);
	lcd_puttext(":");
	lcd_puttext(minBufor);
	lcd_goto(4,1);
	lcd_puttext(hourBufor);
	_delay_ms(100);
	lcd_cls();
}

ISR(TIMER0_COMPA_vect)
{
	msCounter ++;
	if(msCounter == 50)
	{
		PORTB ^= (1 << BUZZER);
		msCounter = 0;
	}
}

ISR(TIMER1_COMPA_vect)
{
	miliseconds ++;
	msCounter ++;
	if(miliseconds == 100)
	{
		secondsCounter ++;
		seconds --;
		miliseconds = 0;	
	}
	
	if(minutes > 0 && seconds == 0)
	{
		seconds = 59;
		minutes --;
	}
	
	if(hours > 0 && minutes == 0 && seconds == 0)
	{
		hours--;
		minutes = 59;
		seconds = 59;
	}
	
	if(minutes == 0 && seconds == 0 && hours == 0)
	{
		endFlag = 1;
		secondsCounter = 0;
		
	}
}


