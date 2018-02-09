/*******************************************************
 * 			   		Projecto: Strobe
 * 				 para el MCU HC9S08 SH8
 * 
 * Creado por bacelolucas@gmail.com y ivanabmm@gmail.com
 * 			  www.github.com/luxarts/strobe
 * 					   03-02-2018 
 * 
 *******************************************************/
#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "LCD_lib.h"
#include "SCI_lib.h"

#define	MINGRUESO 		1
#define MAXGRUESO		50
#define	MINFINO 		1
#define MAXFINO			2000
#define REFRESH_RATE	2
#define LED				PTAD_PTAD3

//Prototipos
void CLK_begin(void);
int map(int,int,int,int,int);
unsigned char analogRead(void);
void proceso1(unsigned int);
void proceso2(unsigned int);
void proceso3(unsigned int);

//Variables globales
int fino=0;
int grueso=0;

//Programa principal
void main(void) {
	CLK_begin();
	PTADD_PTADD3=1;
	LED=0;				//Led de status
	
	Serial_begin(9600); //Inicia el puerto serie a 9600bps
	Serial_flush();		//Vacia el buffer de recepciñon del puerto serie
	EnableInterrupts;	//Habilita las interrupciones
	LCD_begin();		//Inicializa el LCD 16x2
	
	//Presentación
	LCD_clear();
	LCD_print("+--------------+");
	LCD_setCursor(0,1);
	LCD_print("|    STROBE    |");
	delay(300);
	LCD_clear();
	LCD_print("|    STROBE    |");
	LCD_setCursor(0,1);
	LCD_print("|  Hecho  por  |");
	delay(300);
	LCD_clear();
	LCD_print("|  Hecho  por  |");
	LCD_setCursor(0,1);
	LCD_print("|   Bacelo.L   |");
	delay(300);
	LCD_clear();
	LCD_print("|   Bacelo.L   |");
	LCD_setCursor(0,1);
	LCD_print("| Mlinarevic.I |");
	delay(300);	
	LCD_clear();
	LCD_print("| Mlinarevic.I |");
	LCD_setCursor(0,1);
	LCD_print("+--------------+");
	delay(700);
	LCD_clear();
	LCD_print("G=");
	LCD_setCursor(0,1);
	LCD_print("Total=");
	
	LED=1;
	//Loop
	for(;;){
		proceso1(1);			//Lee el buffer de recepción del puerto serie
		proceso2(10); 			//Lee el ADC y envia el dato por el puerto serie
		proceso3(REFRESH_RATE);	//Actualización de la información en el LCD
	}
}
/**********************************************************************************
 * Descripción: Comprueba si llegaron nuevos datos por el puerto serie y los guarda
 * Entrada: Intervalo de ciclos entre ejecución
 * Devuelve: Nada
 **********************************************************************************/
void proceso1(unsigned int intervalo){
	static unsigned int ciclos=0;
	if(ciclos==intervalo){
		ciclos=0;
		if(Serial_available()){//Si hay algo en el buffer del SCI
			grueso=Serial_read();//Saca el primer dato del buffer del SCI
			grueso=map(grueso,0,255,MINGRUESO,MAXGRUESO);
		}
	}
	else{
		ciclos++;
	}
}
/**********************************************************************************
 * Descripción: Lee el ADC, envia el dato por el puerto serie y lo ubica en el rango
 * Entrada: Intervalo de ciclos entre ejecución
 * Devuelve: Nada
 **********************************************************************************/
void proceso2(unsigned int intervalo){
	static unsigned int ciclos=0;
	unsigned char i;
	unsigned int prom=0;
	
	if(ciclos==intervalo){
		ciclos=0;
		for(i=10;i!=0;i--){ //Toma varias muestras y las acumula
			prom+=analogRead();
		}
		fino=prom/10; //Calcula el promedio
		Serial_write(fino);	//Envía la lectura del potenciometro
		fino=map(fino,255,0,MINFINO,MAXFINO); //Potenciómetro invertido (min 255, max 0)
	}
	else{
		ciclos++;
	}
}
/**********************************************************************************
 * Descripción: Actualiza la información en el display LCD
 * Entrada: Intervalo de ciclos entre ejecución
 * Devuelve: Nada
 **********************************************************************************/
void proceso3(unsigned int intervalo){
	static unsigned int ciclos=0;
	if(ciclos==intervalo){
		ciclos=0;
		LCD_setCursor(2,0);
		LCD_printInt(grueso);//Muestra el ajuste grueso
		LCD_print("ms ");
		LCD_print("F=");
		LCD_printInt(fino);	//Muestra el ajuste fino
		LCD_print("us     ");
		LCD_setCursor(6,1);
		LCD_printInt(grueso*1000+fino);//Muestra el total
		LCD_print("us     ");
	}
	else{
		ciclos++;
	}
}
/**********************************************************************************
 * Descripción: Convierte un valor entre dos rangos a un valor entre otros rangos
 * Entrada: Valor a convertir, entrada mínima, entrada máxima, salida mínima, salida máxima
 * Devuelve: Valor convertido a rango
 **********************************************************************************/
int map(int var, int in_min, int in_max, int out_min, int out_max){
	int resultado=0;
	long dummy=var-in_min;			//255-0=255
	dummy=dummy*(out_max-out_min);	//255*(2000-1)=509745
	dummy=dummy/(in_max-in_min);	//509745/(255-0)=1999
	dummy=dummy+out_min;			//1999+1=2000
	resultado=dummy;
	return resultado;
}

unsigned char analogRead(void){
	ADCCFG=0b01000000;	//High speed, fbus/4, = 2MHz, short sample,  8 bits, bus clock
	ADCSC2=0;			//Software trigger, no compare
	ADCSC1=0b00001010;	// Entrada para ADC: AD10 (pin del SH8 - PTAD10)       
	APCTL2=0b00000100;	// Habilito pin para ADC: AD1 (pata 19 del SH8- PTAD0)
	while(ADCSC1_COCO==0);//Espera aque se active COnversion COmplete

	return ADCRL;
}
/**********************************************************************************
 * Descripción: Inicializa el clock
 * Entrada: Nada
 * Devuelve: Nada
 * Clock interno: 31250Hz, Multiplicador: 1024
 * Divisor: 2, Salida: 16MHz, FBUS = 16MHz/BDIV = 8MHz
 **********************************************************************************/
void CLK_begin(void){
	SOPT1=0x02;
	//------------------------------------------------------------------------
	// Inicialización el sistema de clock
	//------------------------------------------------------------------------
	// Registro	ICSC1 (Internal Clock Source Control register 1): 0x04 (reset)
	// bit 7-6   -CLKS:	00  		Clock Source Select, 
	//		                        00: salida FLL, 
	//		                        01: referencia interna, 
	//		                        10: referencia externa, 
	//		                        11: reservado
	// bit 5-4-3 -RDIV:	000	        Reference Divisor 	
	//		                        000:/1,	 001;/2,  010:/4,  011:/8,	
	//		                        100:/16, 101:/32, 110:/64, 111:/128
	// bit 2     -IREFS: 1			Internal Reference Select 
	//		                        0: externo, 
	//                              1: interno
	// bit 1     -IRCLKEN: 0		Internal Reference Clock Enable (RTC CLK)	
	//		                        0: inactivo, 
	//                              1: activo
	// bit 0     -IREFSTEN:	0		Internal Reference STOP Enable
	//		                        0: deshabilitado, 
	//                              1: habilitado
	//	
	//------------------------------------------------------------------------
	
	ICSC1 = 0b00000100;				//clock interno con FLL habilitado

	//------------------------------------------------------------------------
	// Registro	ICSC2 (Internal Clock Source Control register 2): 0x40 (reset)
	// 
	// bit 7-6    -BDIV: 01	        FBUS Divisor,	
	//		                        00: /1, 
	//                              01: /2, 
	//                              10: /4, 
	//                              11: /8
	// bit 5      -RANGE: 0		    External Oscillator Range 
	//		                        0: externo baja frecuencia 
	//                              1: externo alta frecuencia 
	// bit 4      -HGO: 0			High Frequency Gain 	
	//		                        0: config.ext.oscilador para baja potencia	
	//		                        1: config.ext.oscilador para ganancia alta
	// bit 3      -LP: 0			Low Power Selection,	
	//		                        0: FLL no habilitado para modo bypass,	
	//		                        1: FLL deshabilitado en modo bypass,	
	//                                 a menos que BDM es activo	
	// bit 2      -EREFS: 0			External Reference Selection 
	//		                        0: clock externo, 
	//                              1: oscilador externo
	// bit 1      -ERCLKEN: 0		External Reference Clock Enable (RTC-ADC)	
	//		                        0: inactivo,	
	//                              1: activo
	// bit 0      -EREFSTEN; 0	    External Reference STOP Enable 
	//		                        0: dehabilitado,	
	//                              1: habilitado			
	//------------------------------------------------------------------------
	
	ICSC2 = 0b01000000;	        	//FBus/2 = 8MHz

	//------------------------------------------------------------------------
	// Copia el valor de TRIM de fábrica ubicado en la memoría no volátil al
	// área alta de registros de la MCU
	//------------------------------------------------------------------------
	
	ICSSC = NVFTRIM;       			//copia NVFTRIM -> ICSSC
	ICSTRM = NVICSTRM;     			//copia NVICSTRM -> ICSTRM	
}
