/********************************************
 * Librería para el control del módulo SCI
 * 			del MCU HC9S08 SH8
 * 
 * 		Creada por bacelolucas@gmail.com
 * 			www.github.com/luxarts
 * 				16-05-2017 
  * Instrucciones: Este archivo va en la carpeta "Project_Headers" del proyecto
 ********************************************/
#ifndef SCI_LIB_H_
#define SCI_LIB_H_

//Definiciones
#define CR		0x0d
#define LF		0x0a
#define MAXRX	20 //Tamaño del buffer

//Prototipos
void Serial_begin(unsigned int); //Inicializa el puerto serie a la velocidad dada
char Serial_read(void); //Lee el primer byte del buffer	
char Serial_peek(void);	//Lee el primer byte del buffer sin eliminarlo
void Serial_write(char); //Envía un byte por el puerto
unsigned char Serial_available(void); //Devuelve la cantidad de bytes sin leer en el buffer
void Serial_flush(void); //Limpia todo el buffer

#endif /* SERIAL_LIB_H_ */
