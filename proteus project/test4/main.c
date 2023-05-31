#include "main.h"


#define keypin PINB

char read_val = '\0';
char srt[10] = "", pass[] = "1234";
int count, check_pass = 0, checker = 0, safe = 0;

void port_ini(void){
	DDRB = 0x00;
	PORTB = 0xff;
	DDRC = 0xff;
	PORTC = 1<<1;

	GICR|=(0<<INT1) | (1<<INT0) | (0<<INT2);
	MCUCR=(0<<ISC11) | (0<<ISC10) | (1<<ISC01) | (1<<ISC00);
	MCUCSR=(0<<ISC2);
	GIFR=(0<<INTF1) | (1<<INTF0) | (0<<INTF2);
	
	sei();
}

void enter_password(){
	if (count == PASSWORD_LENGTH){
		if (strcmp(srt, pass) == 0){
			check_pass = 1;
			safe = 1;
			clearlcd();
			str_lcd("Password OK");
			} 
			else{
			clearlcd();
			str_lcd("Wrong!");
		}
		_delay_ms(500);
		clearlcd();
		if (check_pass == 0){
			safe = 0;
			clearlcd();
			setpos(0,1);
			str_lcd("Enter password");
		}
		memset(srt, 0, sizeof(srt));
		count = 0;
	}
	
}


ISR(INT0_vect){
	read_val = 0x0f & keypin;
	switch (read_val){
	case 0x00:
	
	if (count > 0){
		clearlcd();
		memset(srt, 0, sizeof(srt));
		count = 0;
	}
	
	break;
	case 0x01:
	if(count < PASSWORD_LENGTH){
		srt[count] = '9';
		count++;
		
	}
	else{
		srt[count] = '\0';
	}
	break;
	case 0x02:
	if(count < PASSWORD_LENGTH){
		srt[count] = '8'; 
		count++;
	}
	else{
		srt[count] = '\0'; 
	}
	break;
	case 0x03:
	if(count < PASSWORD_LENGTH){
		srt[count] = '7';
		count++;
	}
	else{
		srt[count] = '\0'; 
	}
	break;
	case 0x04:
		if(count < PASSWORD_LENGTH){
			srt[count] = 'A';
			count++;
		}
	break;
	case 0x05:
	if(count < PASSWORD_LENGTH){
		srt[count] = '6';
		count++;
	}
	else{
		srt[count] = '\0'; 
	}
	break;
	case 0x06:
	if(count < PASSWORD_LENGTH){
		srt[count] = '5';
		count++;
	}
	else{
		srt[count] = '\0';
	}
	break;
	case 0x07:
	if(count < PASSWORD_LENGTH){
		srt[count] = '4';
		count++;
	}
	else{
		srt[count] = '\0';
	}
	break;
	case 0x08:
	if(count < PASSWORD_LENGTH){
		srt[count] = 'B'; 
		count++;
	}
	else{
		srt[count] = '\0';
	}
	break;
	case 0x09:
	if(count < PASSWORD_LENGTH){
		srt[count] = '3';
		count++;
	}
	else{
		srt[count] = '\0'; 
	}
	break;
	case 0x0a:
	if(count < PASSWORD_LENGTH){
		srt[count] = '2'; 
		count++;
	}
	else{
		srt[count] = '\0';
	}
	break;
	case 0x0b:
	if(count < PASSWORD_LENGTH){
		srt[count] = '1';
		count++;
	}
	else{
		srt[count] = '\0';
	}
	break;
	case 0x0c:
	if(count < PASSWORD_LENGTH){
		srt[count] = 'C';
		count++;
	}
	else{
		srt[count] = '\0';
	}
	break;
	case 0x0d:
	if(count < PASSWORD_LENGTH){
		check_pass = 0;
	}
	else{
		srt[count] = '\0'; 
	}
	break;
	case 0x0e:
	if(count < PASSWORD_LENGTH){
		srt[count] = '0';
		count++;
	}
	else{
		srt[count] = '\0';
	}
	break;
	case 0x0f:
	if(count < PASSWORD_LENGTH){
		PORTA &= ~(1<<6);
		PORTC &= ~(1 << PORTC0);
		checker = 0;
	}
	else{
		srt[count] = '\0'; 
	}
	break;
}
	setpos(1,1); clearlcd(); 
	if (check_pass == 0){
		safe = 0;
	str_lcd(srt);
	}
	if (check_pass == 0){
		safe = 0;
		setpos(0,1);
		str_lcd("Enter password");
		enter_password();
	}
		if (check_pass == 1){
			safe = 1;
		str_lcd( "Welcome Home!" );
	}
}

void move_sensor(){
	if(safe == 0){
	if((PINB & (1 << PORTB4)) == 0){
		checker = 1;
		}
	}
}

void window_sensor(){
	if(safe == 0){
	if((PINB & (1 << PORTB5)) == 0){
		checker = 1;
	}
	if((PINB & (1 << PORTB6)) == 0){
		checker = 1;
		}
	}
}

void door_sensor(){
	if(safe == 0){
	if((PINB & (1 << PORTB7)) == 0){
		checker = 1;
	}
	if((PINC & (1 << PORTC1)) == 0){
		checker = 1;
		}
	}
}

void security_ledFlash(){
	if(checker == 1){
		PORTC |= (1 << PORTC0);
	}
}

int main(void){
	port_ini();
	LCD_ini();
	port_ex_init();
	while(1){
	move_sensor();
	door_sensor();
	window_sensor();
	check_fire_Sensors();
	security_ledFlash();
	}
}