/*
	Interprete.cpp
	Descripción:Programa principal
*/
#include <stdio.h>
#include <string.h>
#define TRUE 1
#define FALSE 0
#define FLAG_LEXEMAS "-lex"
/*Librerías necesarios del interprete*/
#include "entrada.h"
#include "token.h"
#include "lexico.h"
#include "sintactico.h"
#include "tabla_simbolos.h"
#include "opcion_lexemas.cpp"
int main(int argc, char const *argv[])
{
	char *aux = NULL;
	int lexemas = FALSE;
	Token *tokens = NULL;
	if(argc <= 1){
		printf("Modo de uso :Interprete <fichero> <opciones>\n");
		//return 0;
	}
	if(argc >= 3){
		if(strcmp(FLAG_LEXEMAS,argv[2]) == 0)
		lexemas = TRUE;
	}
	//setFile(argv[1]);
	setFile("codigo.code");
	aux = readLine();
	while(aux != NULL){
		tokens = getTokensFromString(aux);
		if(lexemas)printLexema(tokens);
		initProgram(tokens);
		aux = readLine();
	}
	return 0;
}