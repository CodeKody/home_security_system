
#include "main.h"

char SAs = 0x00;
char c;

void port_ex_init(){
	// ��������� ������ ��� HC165
	DDRD |= 1<<0; // ���� D0 - ����� clock - �������� ������
	DDRD |= 1<<1; // ���� D0 - ����� SHIELD - �������
	//	DDRC |= 1<<5; // ���� �5 - ����� SER
	DDRA &= ~(1<<1); // ���� �1 - ���� �������
	PORTA |= 1<<1;
	DDRA &= ~(1<<2); // ���� �2 - ���� �������
	PORTA |= 1<<2;
	
	DDRA = (1<<6);
	PORTA = (0<<6);
}

void fire_ledFlash(){
	PORTA |= (1<<6);
}

char Serial_Read_fire()				// ������ ��������� HC165
{
	// ������������ �������������� �� �������
	PORTD &= ~(1<<1);
	PORTD |= 1<<1;

	// ������ ����������� ������
	// �� ���������������� ������ Qh �������� HC165
	

	for (c = 0; c < 8; c++)
	{
		// ������ ����������� ������
		// �� ���������������� ������ Qh �������� HC165
		// ������� ���������� ������ �����
		SAs <<= 1;
		
		if ( PINA & 1<<1 )
		{
			SAs |= 0x01;
		}
		// ������������ �������������� �� ����� SCK
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
