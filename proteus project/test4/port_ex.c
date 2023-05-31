
#include "main.h"

char SAs = 0x00;
char c;

void port_ex_init(){
	// настройка портов для HC165
	DDRD |= 1<<0; // порт D0 - выход clock - тактовый сигнал
	DDRD |= 1<<1; // порт D0 - Выход SHIELD - защелка
	//	DDRC |= 1<<5; // порт С5 - Выход SER
	DDRA &= ~(1<<1); // порт А1 - Вход сигнала
	PORTA |= 1<<1;
	DDRA &= ~(1<<2); // порт А2 - Вход сигнала
	PORTA |= 1<<2;
	
	DDRA = (1<<6);
	PORTA = (0<<6);
}

void fire_ledFlash(){
	PORTA |= (1<<6);
}

char Serial_Read_fire()				// читаем состояние HC165
{
	// формирование синхроимпульса на защелке
	PORTD &= ~(1<<1);
	PORTD |= 1<<1;

	// чтение логического уровня
	// на последовательном выходе Qh регистра HC165
	

	for (c = 0; c < 8; c++)
	{
		// чтение логического уровня
		// на последовательном выходе Qh регистра HC165
		// сдвигая предыдущие данные влево
		SAs <<= 1;
		
		if ( PINA & 1<<1 )
		{
			SAs |= 0x01;
		}
		// формирование синхроимпульса на входе SCK
		PORTD |= 1<<0;
		PORTD &= ~(1<<0);
		
	}

	return SAs;
}


void check_fire_Sensors()
{
	char fire_state = Serial_Read_fire();
	if(fire_state != 0b11111111)
	{
		fire_ledFlash();
	}
}
