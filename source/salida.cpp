/*
* Nombre : salida.cpp
* Descripción : Gestiona la salida de 
* datos del interprete
*/
#include "stdio.h"
#define PROMPT "$>"/*para el modo interactivo*/
/*Imprime el promt en la consola*/
void prompt(){	
	printf("%3s ",PROMPT);
}
void promptL(int linea){
	printf("%3i%s ",linea,PROMPT);
}
