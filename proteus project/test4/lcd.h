
#ifndef LCD_H_
#define LCD_H_

#include "main.h"

//----------------------------------------
void LCD_ini(void);
void setpos(unsigned char x, unsigned y);
void str_lcd (char str1[]);
void clearlcd();
//----------------------------------------
#define e1    PORTC|=0b00001000 // ��������� ����� E � 1
#define e0    PORTC&=0b11110111 // ��������� ����� E � 0
#define rs1    PORTC|=0b00000100 // ��������� ����� RS � 1 (������)
#define rs0    PORTC&=0b11111011 // ��������� ����� RS � 0 (�������)
//----------------------------------------

#endif /* LCD_H_ */