/********************************************
 * Librería para el control del módulo SCI
 * 			del MCU HC9S08 SH8
 * 
 * 		Creada por bacelolucas@gmail.com
 * 			www.github.com/luxarts
 * 				16-05-2017 
 * Instrucciones: Este archivo va en la carpeta "Sources" del proyecto
 ********************************************/
#include "SCI_lib.h"
#include <derivative.h>
#include <hidef.h>

//Variables globales
char rxBuffer[MAXRX];
//char txBuffer[100];

unsigned char rxIndex=0,rxReadIndex=0;

//Funciones
/**********************************************************************************
 * Descripción: Interrupción cuando se recibe un dato por el SCI
 * Entrada: Nada
 * Devuelve: Nada
 **********************************************************************************/
interrupt 17 void Vscirx_ISR(void){
	PTAD_PTAD0=1;
	SCIS1_RDRF = 0;						//Limpia bandera de recepción
	rxIndex=0;							//Resetea el index del buffer
	while(rxBuffer[rxIndex]!=0 && rxIndex<MAXRX)rxIndex++;//Busca el primer espacio vacío
	if(rxIndex>=MAXRX-1){  				//Si se llenó el buffer
		SCIC2_RIE = 0;					//Deshabilitar la interrupción (evita sobreescribir el buffer)
	}
	else{
		rxBuffer[rxIndex] = SCID;		//Guarda el caracter en el buffer
	}
}

/**********************************************************************************
 * Descripción: Cuenta la cantidad de caracteres sin leer
 * Entrada: Nada
 * Devuelve: Cantidad de caracteres sin leer en el buffer de recepción
 **********************************************************************************/
unsigned char Serial_available(void){
	unsigned char total=0;
	while(rxBuffer[total]!=0 && total<MAXRX)total++;	//Cuenta la cantidad de caracteres hasta el '\0' sin incluirlo
	return(total);
}

/**********************************************************************************
 * Descripción: Lee el primer caracter del buffer de recepción y lo elimina
 * Entrada: Nada
 * Devuelve: Próximo caracter para leer o -1 si no hay mas caracteres en el buffer
 **********************************************************************************/
char Serial_read(void){
	static char dato;
	unsigned char x;
	dato=rxBuffer[0];			//Lee el primer caracter del buffer
	SCIC2_RIE = 1;	//Habilita interrupción del Rx
	if(dato!=0){				//Si es un caracter
		for(x=0;x<MAXRX-1;x++){	//Reordena el buffer (elimina el primer caracter y mueve todos uno hacia la izquierda) 
			rxBuffer[x]=rxBuffer[x+1];
		}
		rxBuffer[MAXRX-1]=0;			//Pone un '\0' al final del vector
		return (dato);	
	}
	else{
		return (-1); //-1 = no hay mas caracteres para leer
	}
}

/**********************************************************************************
 * Descripción: Lee el primer caracter del buffer de recepción sin eliminarlo
 * Entrada: Nada
 * Devuelve: Próximo caracter para leer o -1 si no hay mas caracteres en el buffer
 **********************************************************************************/
char Serial_peek(void){
	char dato;
	dato=rxBuffer[0];			//Lee el primer caracter del buffer
	SCIC2_RIE = 1;	//Habilita interrupción del Rx
	if(dato!=0){				//Si es un caracter
		return (dato);	
	}
	else{
		return (-1); //-1 = no hay mas caracteres para leer
	}
}

/**********************************************************************************
 * Descripción: Limpia el buffer de recepción
 * Entrada: Nada
 * Devuelve: Nada
 **********************************************************************************/
void Serial_flush(void){
	unsigned char i;
	for(i=0;i<MAXRX;i++)rxBuffer[i]=0;//Limpia el buffer
}

/**********************************************************************************
 * Descripción: Inicializa el puerto serie (modulo SCI)
 * Entrada: Baudrate (9600-19200-38400)
 * Devuelve: Nada
 **********************************************************************************/
void Serial_begin(unsigned int baudrate){
	DisableInterrupts;
	SCIC1=0b00000000;	//Bit 7:	LOOPS = 0
						//Bit 6:	SCISWAI = 0
						//Bit 5:	RSRC = 0
						//Bit 4:	M = 0 (8 bit mode)
						//Bit 3:	WAKE = 0
						//Bit 2:	ILT = 0
						//Bit 1:	PE = 0 (Parity disabled)
						//Bit 0:	PT = 0 (Parity type, even-odd)
	
	SCIC2=0b00101100;	//Bit 7:	TIE = 0 (Tx interrupt disabled)
						//Bit 6:	TCIE = 0 (Tx complete interrupt disabled)
						//Bit 5:	RIE = 1 (Rx interrupt enabled)
						//Bit 4:	ILIE = 0
						//Bit 3:	TE = 1 (Transmiter enabled)
						//Bit 2:	RE = 1 (Receiver enabled)
						//Bit 1:	RWU = 0
						//Bit 0:	SBK = 0

	SCIC3=0b00000000;	//Bit 7:	R8 = 0
						//Bit 6:	T8 = 0
						//Bit 5:	TXDIR = 0
						//Bit 4:	TXINV = 0
						//Bit 3:	ORIE = 0
						//Bit 2:	NEIE = 0
						//Bit 1:	FEIE = 0
						//Bit 0:	PEIE = 0
	
	SCIBDH=0b00000000;	//Bit 7:	LBKDIE = 0
						//Bit 6:	RXEDGIE = 0
						//Bit 5:	Null (0)
						//Bit 4-0:	SBR(12:8) =
						//Cuando 1 < SBR < 8191 -> BaudRate = BUSCLK/(16xSBR)
						//SBR = BUSCLK/(16xBaudrate)
						//52 ~= 8MHz/(16x9600)
	switch(baudrate){
		case 9600:		SCIBDL=52;
		break;
		case 19200:		SCIBDL=26;
		break;
		case 38400:		SCIBDL=13;
		break;
		default:		SCIBDL=0;	//Desactivado
		break;
	}
	Serial_flush();
}

/**********************************************************************************
 * Descripción: Espera a recibir un byte y lo devuelve (polling) 
 * Entrada: Nada
 * Devuelve: Char recibido
 **********************************************************************************/
/*char Serial_read(void){
	while(!SCIS1_RDRF);
	return SCID;
}*/

/**********************************************************************************
 * Descripción: Espera a que esté disponible la transmisión y envía un byte (polling)
 * Entrada: Nada
 * Devuelve: Byte a enviar
 **********************************************************************************/
void Serial_write(unsigned char dato){
	while(SCIS1_TDRE == 0);
	SCID = dato;
}

/**********************************************************************************
 * Descripción: Lee una cadena de caracteres y la envía por el puerto SCI mediante
 * 				la función Serial_write() (polling)
 * Entrada: Puntero a la cadena de caracteres
 * Devuelve: Nada
 **********************************************************************************/
/*void Serial_println(char * txBuffer){
	int txIndex=0;
	while(txBuffer[txIndex]!=0){
		
	}
}*/
