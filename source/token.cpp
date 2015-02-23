/*
* Nombre : token.cpp
* Descripción : Gestiona la entrada de datos al
* 	interprete
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "token.h"
#include "error.h"

/*Crea un nuevo token y lo devuelve como apuntador*/
Token * newToken(Tipo_Token tipo_token,const char* cuerpo)
{
	Token *nuevo = (Token *)malloc(sizeof(Token));
	if(nuevo == NULL){
		printf("s\n",MEMORY_ERROR);
		return NULL;
	}
	nuevo->tipo_token = tipo_token;
	strcpy(nuevo->cuerpo , cuerpo);
	nuevo->sig = NULL;
	return nuevo;
}
/*Libera la memoria de todos los tokens apuntados a partir
del token ingresado, trabajos de limpieza*/
void clearTokens(Token* token)
{
	if(token == NULL)return;

	clearTokens(token->sig);

	token->sig = NULL;
	free(token);
}
