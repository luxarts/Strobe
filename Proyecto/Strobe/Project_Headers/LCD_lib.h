/********************************************
 * Librería para el control de LCD 16x2
 * 			del MCU HC9S08 SH8
 * 
 * 		Creada por bacelolucas@gmail.com
 * 			www.github.com/luxarts
 * 				07-12-2017 
 * Instrucciones: Este archivo va en la carpeta "Project_Headers" del proyecto
 ********************************************/
#ifndef LCD_LIB_H_
#define LCD_LIB_H_

#define D7	PTBD_PTBD2
#define D6	PTBD_PTBD3
#define D5	PTCD_PTCD0
#define D4	PTCD_PTCD1
#define E	PTCD_PTCD3
#define RS	PTBD_PTBD4

void LCD_begin(void);
void LCD_clear(void);
void LCD_cmd(unsigned char);
void LCD_char(char);
void LCD_print(char *);
void LCD_setCursor(unsigned char, unsigned char);
void LCD_home(void);
void LCD_printInt(long);

void delayMicroseconds(unsigned int);
void delayLCD(unsigned int);
void delay(unsigned int);

#endif /* LCD_LIB_H_ */
