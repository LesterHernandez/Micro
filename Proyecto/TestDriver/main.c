#include "stm32f3xx.h"                  // Device header
#include "UART.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

extern int display(int x); 
extern int re0(); 
extern int rmodify(int x,char y);
void rutinaprincipal(void);
void modificarMemoria(void);
char rx_data[64],delimitador[]=" ";
uint8_t n;
uint8_t cmd_ready;
uint8_t contador =0;
char *instruction = 0;
char *instruction1 = 0;
char *instruction2 = 0;
char *instruction3 = 0;
char *instruction4 = 0;
char buffer[64];

void miFuncion() { 
	sprintf(buffer, "\n\r Funcion \n\r"); 
	USART_putString(buffer);
	
}

void rd(void){
	int momento = 0; 
	
	momento++;
	while (!(momento==16)){
		sprintf(buffer, "\n\rR%d : 0x%x \n\r", momento,display(momento)); 
		USART_putString(buffer);
		momento += 1;
	}
	
}

void rm(void){
	 const char *cadena = instruction1;
	char *ptrR = strchr(cadena, 'R');
     

    // Verificar si después de 'R' hay un número
    char *ptrNumero = ptrR + 1; // Saltar la 'R'
     

    // Extraer el número (considerando que puede tener varios dígitos)
    int numero = atoi(ptrNumero);  // Convertir el número directamente 
	  rmodify(numero,*instruction2);
		sprintf(buffer, "\n\rR: 0x%d %c \n\r", numero,*instruction2); 
	  USART_putString(buffer);
	  sprintf(buffer, "\n\rR%d : 0x%x \n\r", numero,display(numero)); 
					
    USART_putString(buffer); 
	cmd_ready = 0;
		rutinaprincipal();
	
		
    
		 
	
}

void md(){
		
	  unsigned long start_addr = strtoul(instruction1, NULL, 16);
    unsigned long end_addr = strtoul(instruction2, NULL, 16); 
	  unsigned char *start = (unsigned char*)start_addr;
	  unsigned char *end =(unsigned char*)end_addr;
	  unsigned char* addr = start;
	  while (addr <= end) {
			  sprintf(buffer,"\n\rAddress: %p, Value: 0x%02x\n\r", (void*)addr, *addr); 
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

typedef void (*func_ptr)(void); 
void run(void){
	/*// Obtener la dirección de la función 
	unsigned long start_addr = strtoul(instruction1, NULL, 16);
	unsigned char *addr = (unsigned char*)start_addr;
	sprintf(buffer,"\n\rAddress: %p\n\r", (void*)addr); 
  USART_putString(buffer);*/
  unsigned long start_addr = strtoul(instruction1, NULL, 16);
	unsigned char *start = (unsigned char*)start_addr;
	unsigned char* addr = start;  
	sprintf(buffer,"\n\rLa direccion de miFuncion es: %p\n\r", (void*)addr); 
  USART_putString(buffer);	
	void (*ptr)() = miFuncion;
  ptr();	
	//08001d25
	 
}


void call(void){ 
	/*unsigned long start_addr = strtoul(instruction1, NULL, 16);
	unsigned char *addr = (unsigned char*)start_addr;
	sprintf(buffer,"\n\rAddress: %p\n\r", (void*)addr); 
  USART_putString(buffer); */
	unsigned long start_addr = strtoul(instruction1, NULL, 16);
	unsigned char *start = (unsigned char*)start_addr;
	unsigned char* addr = start;  
	sprintf(buffer,"\n\rLa direccion de miFuncion es: %p\n\r", (void*)addr); 
  USART_putString(buffer);	
	void (*ptr)() = miFuncion;
  ptr();
	
	//08001d25
	 
}
void funcion(void){
	sprintf(buffer,"\n\rLa direccion de miFuncion es: %p\n\r", (void*)miFuncion); 
  USART_putString(buffer);
}
void rutinaprincipal(void){
	USART_putString(">> ");	
	while(1){
		if(cmd_ready){
			if(!strcmp(rx_data,"xD")){ 
				
				rd();
				   
			}else{
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
							sprintf(buffer, "\n\rR%d : 0x%x \n\r", 0,re0()); 
	            USART_putString(buffer);
							rd();
						}else if(!strcmp(instruction,"Funcion")){
							funcion();
						}else{
							USART_putString("\n\rComando no encontrado!!\n\r\n\r");
						} 
				 
			}
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
