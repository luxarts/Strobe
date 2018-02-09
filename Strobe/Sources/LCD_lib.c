/********************************************
 * Librería para el control de LCD 16x2
 * 			del MCU HC9S08 SH8
 * 
 * 		Creada por bacelolucas@gmail.com
 * 			www.github.com/luxarts
 * 				07-12-2017 
 * Instrucciones: Este archivo va en la carpeta "Sources" del proyecto
 ********************************************/
#include "LCD_lib.h"
#include <derivative.h>

void LCD_begin(void){
	PTBDD_PTBDD2=1;
	PTBDD_PTBDD3=1;
	PTCDD_PTCDD0=1;
	PTCDD_PTCDD1=1;
	PTCDD_PTCDD3=1;
	PTBDD_PTBDD4=1;
	
	//Modo 4 bits
	RS=0;
	D7=0;
	D6=0;
	D5=1;
	D4=1;		//DL: 1 = 8 bit, 0 = 4 bit
	delayLCD(50);
	E=1;
	delayLCD(50);
	E=0;
	delayLCD(4500);
	RS=0;
	D7=0;
	D6=0;
	D5=1;
	D4=1;//DL: 1 = 8 bit, 0 = 4 bit
	delayLCD(50);
	E=1;
	delayLCD(50);
	E=0;
	delayLCD(1000);
	RS=0;
	D7=0;
	D6=0;
	D5=1;
	D4=1;//DL: 1 = 8 bit, 0 = 4 bit
	delayLCD(50);
	E=1;
	delayLCD(50);
	E=0;
	delayLCD(100);
	
	RS=0;
	D7=0;
	D6=0;
	D5=1;
	D4=0;//DL: 1 = 8 bit, 0 = 4 bit
	delayLCD(50);
	E=1;
	delayLCD(50);
	E=0;
	delayLCD(100);
	//Confirma modo
	LCD_cmd(0x28);//DL=1, N=1, F=0 (4 bit, dos lineas, fuente 5x7)
	//Limpia el LCD
	LCD_clear();
	//Display on, cursor on, cursor blink
	LCD_cmd(0x0C);
}
void LCD_clear(void){
	LCD_cmd(0x01);
}
void LCD_cmd(unsigned char cmd){
	RS=0;
	D7=(cmd>>7)&1;
	D6=(cmd>>6)&1;
	D5=(cmd>>5)&1;
	D4=(cmd>>4)&1;
	delayLCD(20);
	E=1;
	delayLCD(20);
	E=0;
	delayLCD(300);
	
	D7=(cmd>>3)&1;
	D6=(cmd>>2)&1;
	D5=(cmd>>1)&1;
	D4=cmd&1;
	delayLCD(20);
	E=1;
	delayLCD(20);
	E=0;
	delayLCD(300);
}
void LCD_char(char caracter){
	RS=1;
	D7=(caracter>>7)&1;
	D6=(caracter>>6)&1;
	D5=(caracter>>5)&1;
	D4=(caracter>>4)&1;
	delayLCD(20);
	E=1;
	delayLCD(20);
	E=0;
	delayLCD(300);
	
	D7=(caracter>>3)&1;
	D6=(caracter>>2)&1;
	D5=(caracter>>1)&1;
	D4=caracter&1;
	delayLCD(20);
	E=1;
	delayLCD(20);
	E=0;
	delayLCD(300);
}
void LCD_setCursor(unsigned char col, unsigned char row){
	if(row)row=0x40;//Fila 1 = bit 6 en 1
	if(col>40 && !row)col=(col-40)|0x40;//Si se pasa de columna sigue escribiendo abajo
	LCD_cmd(row + col | 0x80);
}
void LCD_home(void){
	LCD_cmd(0x02);
}
void LCD_print(char *string){
	static unsigned char charCount, chars;
	charCount=0;
	while(string[charCount]!='\0')charCount++;//Cuenta la cantidad de caracteres
	
	for(chars=0;chars<charCount;chars++){
		LCD_char(string[chars]);
	}
}
void LCD_printInt(long n){
	char str[9];
	unsigned char charCount=0;
	unsigned char digits=0,i;
	int x;
	if (n<0){
		str[0]='-';
		charCount++;
		n=-n;
	}
	
	if (n<10)digits=1;
	else if (n<100)digits=2;
	else if (n<1000)digits=3;
	else if (n<10000)digits=4;
	else if (n<100000)digits=5;
	else if (n<1000000)digits=6;
	else if (n<10000000)digits=7;
	else if (n<100000000)digits=8;
	
	for (i=digits; i>0 ; i--){
		switch (i){
			case 8: x=n%100000000/10000000;
			break;
			case 7: x=n%10000000/1000000;
			break;
			case 6: x=n%1000000/100000;
			break;
			case 5: x=n%100000/10000;
			break;
			case 4: x=n%10000/1000;
			break;
			case 3: x=n%1000/100;
			break;
			case 2: x=n%100/10;
			break;
			case 1: x=n%10;
		}
		str[charCount]=x+'0';
		charCount++;
	}
	str[charCount]='\0';
	LCD_print(str);
}

void delayLCD(unsigned int time){
	static unsigned int cycles,mult;
	for(mult=time;mult>0;mult--){
		for(cycles=1;cycles>0;cycles--);
	}
}

void delayMicroseconds(unsigned int time){
	static unsigned int cycles,mult;
	for(mult=time;mult>0;mult--){
		for(cycles=2;cycles>0;cycles--);
	}
}
void delay(unsigned int time){
	static unsigned int mult;
	for(mult=time;mult>0;mult--){
		delayMicroseconds(125);
	}
}
