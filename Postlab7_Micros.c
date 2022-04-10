/*
* File: Post Lab7.c
* Author: Saby Andrade
*Descripción: Haga una función a la que se le envíe un número y devuelva en 3 variables los
valores en decimal.displays de 7 segmentos para desplegar el valor de los dichos registros, timer0
como interrupción.
 * Creado: 09/04/2022
*/
//Configuración #1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO
//oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)

#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT disabled and can
//be enabled by SWDTEN bit of the WDTCON register)

#pragma config PWRTE = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, 
//MCLR internally tied to VDD)

#pragma config CP = OFF // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = ON // Low Voltage Programming Enable bit (RB3 pin has digital I/O, 
//HV on MCLR must be used for programming)

//Configuración #2
#pragma config BOR4V = BOR40V // Brown-out Reset Selection bit (Brown-out Resetset to 4.0V)
#pragma config WRT = OFF // Flash Program Memory Self Write Enable bits (Write protection off)
        
        
//Librerías que se utilizarán
#include <xc.h>

//Esta librería se usa para los int que ocupan ocho bits
#include <stdint.h> 

//Se define el timer 0 que tendrá un valor de 177 que esto equivalará cada 5 mS
#define _tmr0_value 177 //177 Para 5ms

/* 
 * Variables
 */

uint8_t Sis_Muxeos;
uint8_t Sis_Centenas;
uint8_t Sis_Decenas;
uint8_t Sis_Unidades;


/* 
 * Prototipo de Funciones
 */

void setup(void);
int Set_Displays(int Numero);
void Sis_Decimales (void);


/* 
 * Interrupción
 */
void __interrupt() isr (void)
{
    //Interrupción del Timer 0
    //Comprueba si en el Timer0 hay overflow
    if (T0IF)                       
    {     
           //Se llama el determinado valor desde el arreglo para el display, todo dependerá que display despleguerá
        switch(Sis_Muxeos){            
            case(0x01): Sis_Muxeos = 0x02;
            break;
            
            case(0x02): Sis_Muxeos = 0x04;
            break;
            
            case(0x04): Sis_Muxeos = 0x01;
            break;
        }
        
        T0IF = 0;
        TMR0 = _tmr0_value;
    }
    //Interrupción que ocurre en el puerto B
    if (RBIF)                       
      //Comprueba si hay interrupción en ese puerto
    {
                //Interrupción en el puerto B pin 0
        if (RB0 == 0){
            //Si se presiona el botón incrementa el valor en el puerto A
            PORTA++;  
        }
                //Interrupción en el puerto B pin 1
        if (RB1 == 0){
            
            //Si se presiona el botón decrementa el valor en el puerto A
            PORTA--;
        }  
      RBIF = 0;  
    }       
    } 


/* 
 * Configuración "Main"
 */
void main(void){
    //Configuración Inicial
setup();
//Es el valor inicial para el muxeo
Sis_Muxeos = 0x01;
while(1)
{   
    //Se hace la conversión
Sis_Decimales(); 

//Es el muxeo y se muestra en los displays
switch(Sis_Muxeos){
    case(0x01):
        PORTD = 0;
        //se uso el puerto E el valor que se necesita para desplegar las centenas
        PORTE = 0x01;
        
        //Se muestra el valor
        Set_Displays(Sis_Centenas);   
        break;
        
    case(0x02):
        PORTD = 0;
        //se uso el puerto E el valor que se necesita para desplegar las decenas
        PORTE = 0x02;
        
        //Se muestra el valor
        Set_Displays(Sis_Decenas);
        break;
        
    case(0x04):
        PORTD = 0;
        //se uso el puerto E el valor que se necesita para desplegar las unidades
        PORTE = 0x04;
        
        //Se muestra el valor
        Set_Displays(Sis_Unidades);
        
        break;
}
}
}


/* 
 * Funciones
 */
void setup (void)
{
    //Configuración del reloj interno
    OSCCONbits.SCS = 1;
    OSCCONbits.IRCF2 = 0;
    OSCCONbits.IRCF1 = 0; // 125KHz se tiene en  Hz
    OSCCONbits.IRCF0 = 1;
    
    //Configuración de las entradas y salidas
    //Se limpia el puerto D0
    PORTDbits.RD0 = 0;
    //Se limpia el puerto D1
    PORTDbits.RD1 = 0;
    //Se limpia el puerto D2
    PORTDbits.RD2 = 0;
    //Se limpia el puerto D3
    PORTDbits.RD3 = 0;
    //Se limpia el puerto D4
    PORTDbits.RD4 = 0;
     //Se limpia el puerto D5
    PORTDbits.RD5 = 0;
    //Se limpia el puerto D6
    PORTDbits.RD6 = 0;
    //Se limpia el puerto D7
    PORTDbits.RD7 = 0;
    
    //Se limpia el puerto B0
    PORTBbits.RB0 = 3;
    //Se limpia el puerto B1
    PORTBbits.RB1 = 3;
    //Se limpia el puerto B2
    PORTBbits.RB2 = 3;
    //Se limpia el puerto B3
    PORTBbits.RB3 = 3;
    //Se limpia el puerto B4
    PORTBbits.RB4 = 3;
     //Se limpia el puerto B5
    PORTBbits.RB5 = 3;
    //Se limpia el puerto B6
    PORTBbits.RB6 = 3;
    //Se limpia el puerto B7
    PORTBbits.RB7 = 3;

    //Se limpia el puerto A0
    PORTAbits.RA0 = 0;
    //Se limpia el puerto A1
    PORTAbits.RA1 = 0;
    //Se limpia el puerto A2
    PORTAbits.RA2 = 0;
    //Se limpia el puerto A3
    PORTAbits.RA3 = 0;
    //Se limpia el puerto A4
    PORTAbits.RA4 = 0;
     //Se limpia el puerto A5
    PORTAbits.RA5 = 0;
    //Se limpia el puerto A6
    PORTAbits.RA6 = 0;
    //Se limpia el puerto A7
    PORTAbits.RA7 = 0;
 
    //Se limpia el puerto E0
    PORTEbits.RE0 = 0;
    //Se limpia el puerto E1
    PORTEbits.RE1 = 0;
    //Se limpia el puerto E2
    PORTEbits.RE2 = 0;
            
    ANSEL = 0; //configuración de los puertos
    ANSELH = 0;
    
    //Puerto D0 como salida
    TRISDbits.TRISD0 = 0;
    //Puerto D1 como salida
    TRISDbits.TRISD1 = 0;
    //Puerto D2 como salida
    TRISDbits.TRISD2 = 0; 
    //Puerto D3 como salida
    TRISDbits.TRISD3 = 0; 
    //Puerto D4 como salida
    TRISDbits.TRISD4 = 0;
    //Puerto D5 como salida
    TRISDbits.TRISD5 = 0;
    //Puerto D6 como salida
    TRISDbits.TRISD6 = 0; 
    //Puerto D7 como salida
    TRISDbits.TRISD7 = 0; 
    
    //Puerto B0 como salida
    TRISBbits.TRISB0 = 3;
    //Puerto B1 como salida
    TRISBbits.TRISB1 = 3;
    //Puerto B2 como salida
    TRISBbits.TRISB2 = 3; 
    //Puerto B3 como salida
    TRISBbits.TRISB3 = 3; 
    //Puerto B4 como salida
    TRISBbits.TRISB4 = 3;
    //Puerto B5 como salida
    TRISBbits.TRISB5 = 3;
    //Puerto B6 como salida
    TRISBbits.TRISB6 = 3; 
    //Puerto B7 como salida
    TRISBbits.TRISB7 = 3; 

    //Puerto A0 como salida
    TRISAbits.TRISA0 = 0;
    //Puerto A1 como salida
    TRISAbits.TRISA1 = 0;
    //Puerto A2 como salida
    TRISAbits.TRISA2 = 0; 
    //Puerto A3 como salida
    TRISAbits.TRISA3 = 0; 
    //Puerto A4 como salida
    TRISAbits.TRISA4 = 0;
    //Puerto A5 como salida
    TRISAbits.TRISA5 = 0;
    //Puerto A6 como salida
    TRISAbits.TRISA6 = 0; 
    //Puerto A7 como salida
    TRISAbits.TRISA7 = 0; 
 
    //Puerto E0 como salida
    TRISEbits.TRISE0 = 0;
    //Puerto E1 como salida
    TRISEbits.TRISE1 = 0;
    //Puerto E2 como salida
    TRISEbits.TRISE2 = 0; 
    
    
    //Se habilitan los Pullups en el Puerto B
    OPTION_REGbits.nRBPU = 0;
    IOCB = 0X0F;
    
    //Se configura los Pullups del Puerto B
    WPUB = 0X0F;
    
    
    //Se configura el Timer0
    OPTION_REGbits.T0CS = 0;// Configuramos (Fosc/4)
    OPTION_REGbits.PSA = 0; // PSA asignamos el prescaler al Timer0
    OPTION_REGbits.PS2 = 0; // prescaler 1:2
    OPTION_REGbits.PS1 = 0;
    OPTION_REGbits.PS0 = 0;
    TMR0 = _tmr0_value;
    
    
    //Se habilitan interrrupciones de Timer0 y las interrupciones del puerto B
    //INTCONbits.RBIF = 0;
    INTCONbits.T0IF = 0;
    //INTCONbits.RBIE = 1;
    INTCONbits.T0IE = 1;
    INTCONbits.GIE = 1;
}

//Es el convertidor de Binario a Decimal
void Sis_Decimales (void){
    int Variable_Temporal;
    Sis_Centenas = PORTA /100;
    Variable_Temporal = PORTA % 100;
    Sis_Decenas = Variable_Temporal /10;
    Sis_Unidades = Variable_Temporal % 10; 
} 
//Es la tabla de 7 segmentos, de un cátodo común
int Set_Displays(int Cantidad_Numeros){
    switch (Cantidad_Numeros){
        case(0): PORTD = (0b0111111);
        break;
        case(1): PORTD = (0b0000110);
        break;
        case(2): PORTD = (0b1011011);
        break;
        case(3): PORTD = (0b1001111);
        break;
        case(4): PORTD = (0b1100110);
        break;
        case(5): PORTD = (0b1101101);
        break;
        case(6): PORTD = (0b1111101);
        break;
        case(7): PORTD = (0b1111111);
        break;
        case(8): PORTD = (0b1111111);
        break;
        case(9): PORTD = (0b1100111);
        break;
    }  
}