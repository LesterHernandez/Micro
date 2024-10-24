#include "stm32f3xx.h"                  // Device header
#include "UART.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
//Funcion complementaria para ver el valor de los registros
extern int display(int x);  
//Funcion complementaria para cambiar el valor de un registro
extern int rmodify(int x,char y);
//Funciones complementarias para el call y run
extern void CALL(unsigned char *addr);
extern void RUN(unsigned char *addr);
//Funcion donde estara el bucle principal
void rutinaprincipal(void); 
char rx_data[64],delimitador[]=" ";//Delemitador " " es para cortar cada vez que hay un comando
uint8_t n;
uint8_t cmd_ready;
//Contador cuenta por que instruccion va en la terminal
uint8_t contador =0;
//Donde se almacenan las instrucciones
char *instruction = 0;
char *instruction1 = 0;
char *instruction2 = 0;
char *instruction3 = 0;
char *instruction4 = 0;
//arreglo donde se guarda lo que se quiere mandar al put_string
char buffer[64];

void miFuncion() { 
	sprintf(buffer, "\n\r Funcion \n\r"); 
	USART_putString(buffer);
	
}
//COMANDO RD
void rd(void){
	//Conteo de los 16 registros
	int momento = 0; 
	while (!(momento==16)){
		sprintf(buffer, "\n\rR%d : 0x%x \n\r", momento,display(momento)); 
		USART_putString(buffer);
		momento += 1;
	}
	
}

void rm(void){
	 //Puntero hacia el valor de instruction1
	 const char *cadena = instruction1;
	 //Encontrar la R
	 char *ptrR = strchr(cadena, 'R');
   // Verificar si después de 'R' hay un número
   char *ptrNumero = ptrR + 1; // Saltar la 'R'
   // Extraer el número y con atoi de una vez pasarlo a un int 
   int numero = atoi(ptrNumero);  // Convertir el número directamente 
	 //Llamar la funcion en .s
	 rmodify(numero,*instruction2);
	 sprintf(buffer, "\n\rRegistro cambiado: R%d  \n\r", numero);  			
   USART_putString(buffer); 
	//Reiniciar variables
	 instruction="";
	 instruction1="";
	 instruction2="";
	 instruction3="";
	 contador = 0;
	 cmd_ready = 0;
	//Llamar a la rutina
	 rutinaprincipal();
}

void md(){
	 //Hexadecimal a entero sin signo
	 unsigned long start_addr = strtoul(instruction1, NULL, 16);
   unsigned long end_addr = strtoul(instruction2, NULL, 16); 
	 //Castear las direcciones y asignarle el valor de la direccion
	 unsigned char *start = (unsigned char*)start_addr;
	 unsigned char *end =(unsigned char*)end_addr;
	 unsigned char* addr = start;
	 //Bucle para recorrer las direcciones
	 while (addr <= end) {
		 sprintf(buffer,"\n\rAddress: %p, Value: 0x%02x\n\r", (void*)addr, *addr); //Acceder a la direccion y el valor
     USART_putString(buffer); 
     addr++;
   } 
	
}

void mm(){
		
	  uint32_t direccion;
    uint32_t valor;
    uint32_t tamano = 1; 

    // Obtener la dirección de memoria
    direccion = strtoul(instruction1, NULL, 16);  
    

    // Obtener el valor a escribir
    valor = strtoul(instruction2, NULL, 16);  
	  

    // Obtener el tamaño opcional
    if (instruction3 != NULL) {
        tamano = atoi(instruction3); 
        if (tamano != 1 && tamano != 2 && tamano != 4) {
            USART_putString("Error: Tamano incorrecto. Usa 1, 2 o 4 bytes.\n\r"); 
        }
    }

    // Escribir el valor en la dirección indicada
    //USART_putString("r: Tamano incorrecto. Usa 1, 2 o 4 bytes.\n\r");
		if (tamano == 1) {
        *((volatile uint8_t *)direccion) = (uint8_t)valor;
    } else if (tamano == 2) {
        *((volatile uint16_t *)direccion) = (uint16_t)valor;
    } else if (tamano == 4) {
        *((volatile uint32_t *)direccion) = valor;
					
    }

    sprintf(buffer, "Memoria modificada en 0x%08X con valor 0x%X (%d bytes)\n\r", direccion, valor, tamano);
    USART_putString(buffer);

	
}
void bf(){

	uint32_t tempo;
	uint32_t inicio;
	uint32_t final;
	uint32_t valor;
	uint32_t tamano = 1;
	
	inicio = strtoul(instruction1, NULL, 16);
	
	final = strtoul(instruction2, NULL, 16);
	
	valor = strtoul(instruction3, NULL, 16);
	
	if(tamano != NULL){
		tamano = atoi(instruction4);
		if(tamano != 1 && tamano != 2 && tamano != 4){
			USART_putString("Error: Tamano incorrecto. Usa 1, 2 o 4 bytes.\n\r");
			return;
		}
	}
	
	if(tamano == 1){
	
		for(tempo = inicio;tempo <= final; tempo += tamano){
		
			memset((void*)tempo, (uint8_t) valor, tamano);
		}
		USART_putString("\n\rllenando byte a byte\n\r");
		
	}
	else if((tamano == 2) || (tamano == 4)){
		
			for(tempo = inicio; tempo <= final; tempo += tamano){
				switch(tamano){
				
					case 2:
						*(uint16_t*) tempo = (uint16_t)valor;
						USART_putString("\n\rllenando 2 byte\n\r");
						break;
					
					case 4:
					USART_putString("\n\rllenando 4 byte\n\r");	
					*(uint32_t*) tempo = (uint32_t) valor;
						break;
					
					default:
						USART_putString("\n\rnada por aqui\n\r");
					return;
					
				}
			}
				
		}
		else{
		
			USART_putString("\n\r aqui no se recibio nada\n\r");
		}
}
  
void run(void){ 
  unsigned long start_addr = strtoul(instruction1, NULL, 16);
	unsigned char *start = (unsigned char*)start_addr;
	unsigned char* addr = start; 	
	RUN(addr); 
	 
	 
}


void call(void){  
	unsigned long start_addr = strtoul(instruction1, NULL, 16);
	unsigned char *start = (unsigned char*)start_addr;
	unsigned char* addr = start;  
	CALL(addr); 
	 
}
void funcion(void){
	sprintf(buffer,"\n\rLa direccion de miFuncion es: %p\n\r", (void*)miFuncion); 
  USART_putString(buffer);
}
void rutinaprincipal(void){
	USART_putString(">> ");	
	while(1){
		if(cmd_ready){
			char *token = strtok(rx_data,delimitador);
			if(token != NULL){
				while(token != NULL){
					if(contador==0){
						instruction = token;
						contador +=1;
					}else if(contador==1){
						instruction1 = token;
						contador +=1;
					}else if(contador==2){
						instruction2 = token;
						contador +=1;
					}else if(contador==3){ 	
						instruction3 = token;
						contador +=1;
					}else if(contador==4){
						instruction4 = token;
						contador +=1;
					}
							
							
					 token = strtok(NULL, delimitador);
				}
						
			}
			if(!strcmp(instruction,"MD")){
				md();
			}else if(!strcmp(instruction,"MM")){
				mm();
			}else if(!strcmp(instruction,"RM")){
				rm();
							////////
			}else if(!strcmp(instruction,"BF")){
				bf();
							//////
			}else if(!strcmp(instruction,"RUN")){
				run();
			}else if(!strcmp(instruction,"CALL")){
				call();
			}else if(!strcmp(instruction,"RD")){ 
				rd();
			}else if(!strcmp(instruction,"Funcion")){
				funcion();
			}else{
				USART_putString("\n\rComando no encontrado!!\n\r\n\r");
			} 
				 
			//Reiniciar variables
			instruction="";
			instruction1="";
			instruction2="";
			instruction3="";
			contador = 0;
			USART_putString(">> ");	
			cmd_ready = 0;
	
		}
	} 
	
}
int main(void){ 
	USART_config(115200); 
	
	USART_putString("\n\r         Proyecto Final Microprocesadores 2024       \n\r");
	USART_putString("                    Programa Monitor                 \n\r");
	USART_putString("                      Grupo nose                     \n\r\n\r"); 
	rutinaprincipal();
}
