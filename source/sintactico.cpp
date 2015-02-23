/*
* Nombre : sintactico.cpp
* Descripción : Este archivo gestiona el análisis
* sintáctico del interprete haciendo uso del analizador léxico.
* Taimen obedece a la gramática declarada en el archivo Grammar
* Las funciones que comienzan con (__) son privadas
* y no deben ser llamadas directamente desde el exterior
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entrada.h"
#include "token.h"
#include "lexico.h"
#include "error.h"
#include "sintactico.h"
#include "reserved_words.h"
#include "tabla_simbolos.h"
#include "solve_expresion.h"
/*Funciones Prototipo*/
void bloque();
void sentencia();
void asignacion();
void variable();
void expresion();
void termino();
void mas_terminos();
void factor();
void mas_factores();
void constante();
void impresion(int tipo_imp);
void errores(int error);

#define TRUE 1
#define FALSE 0

static Token* lista_token = NULL;
static char cad_imp[MAX_SIZE_CUERPO];
/*
Devuelve el siguiente Token en la lista de tokens apuntados por
lista_token y recorre la lista si la lista está vacia devuelve NULL****/
Token * nextToken()
{
	Token* next = lista_token;
	if(lista_token != NULL)lista_token = lista_token->sig;
	return next;
}
/*Reinserta un Token que no se ocupó*/
void reinsertToken(Token *token)
{
	token->sig = lista_token;
	lista_token = token;
}
/*Inicializa el análisis del programa*/
void initProgram(Token *tokens)
{
	lista_token = tokens;
	if(lista_token == NULL)return;
	sentencia();
}
void bloque() {
	sentencia();
} 
void sentencia() 
{ 
 	Token *token = NULL;
	token = nextToken();
	if(token == NULL)return;
	if (strcmp(CUERPO(token),PRINT) == 0
	|| strcmp(CUERPO(token),PRINTLN) == 0)
	{
		if(strcmp(CUERPO(token),PRINTLN) == 0)
			impresion(1);
		else impresion(0);
	}
	else if (isReservedWord(CUERPO(token)) == FALSE) { 
		reinsertToken(token); 
		asignacion(); 
	}else{
		printf("%s ",CUERPO(token) );
		errores(6);
	}
}
void asignacion() 
{  
	Token *token = NULL;
	Variable *var_nueva = NULL;
	Variable *aux = NULL;
	token = nextToken();
	var_nueva = newVariable();
	if(token == NULL)return;
	if(!isReservedWord(CUERPO(token))){
		if(TIPO(token) == IDENTIFICADOR){
			strcpy(var_nueva->id,CUERPO(token));
			token = nextToken();
			if(token == NULL)errores(3);
			if (TIPO(token) != ASIGNACION) errores(3); 
			token = nextToken();
			if(token == NULL)errores(7);
			switch(TIPO(token)){
				case CADENA:
					var_nueva->tipo_dato = STRING;
					strcpy(var_nueva->value.s,CUERPO(token)); 
				break;
				case NUMERO_ENTERO:
					var_nueva->tipo_dato = ENTERO;
					var_nueva->value.i = stringToInt(CUERPO(token));
				break;
				case NUMERO_DECIMAL:
					var_nueva->tipo_dato = FLOTANTE;
					var_nueva->value.f = stringToFloat(CUERPO(token));
				break;
				case IDENTIFICADOR:
					if (strcmp(CUERPO(token),NULO) == 0){
						deletInTable(var_nueva->id);
						return;
					}
					aux = searchInTable(CUERPO(token));
					if(aux == NULL)errores(8);
					var_nueva->tipo_dato = aux->tipo_dato;
					switch(aux->tipo_dato){
						case STRING:
							strcpy(var_nueva->value.s,aux->value.s); 
						break;
						case ENTERO:
							var_nueva->value.i = aux->value.i;
						break;
						case FLOTANTE:
							var_nueva->value.f = aux->value.f;
						break;
						default:errores(7);
					}
				break;
				default:errores(7);
			}
		}else{
			printf("%s ",CUERPO(token) );
			errores(5);
			return;
		}
	}else{
		printf("%s ",CUERPO(token) );
		errores(5);
	}
	setInTable(var_nueva);
}
char* quitarComillas(const char *cuerpo)
{
	int i =	0;
	cad_imp[i] = '\0';
	strcpy(cad_imp,cuerpo);
	for(i = 0; i < strlen(cad_imp)-2;i++){
		cad_imp[i] = cad_imp[i+1];
	}cad_imp[i] = '\0';
	return cad_imp;
} 
void impresion(int tipo_imp)
{
	Token *token = NULL;
	token = nextToken();
	switch(TIPO(token))
	{
		case IDENTIFICADOR :
			if(existInTable(CUERPO(token)))
			{
				Variable *var = searchInTable(CUERPO(token));	
				switch(var->tipo_dato)
				{
					case STRING:printf("%s",quitarComillas(var->value.s));break;
					case ENTERO:printf("%i",var->value.i);break;
					case FLOTANTE:printf("%lf",var->value.f);break;
					
					default :printf("%s\n","NULO");break;
				}
				if(tipo_imp)printf("\n");
			}else{
				ERROR_MESSAGE_LINE_EXIT(
					"No se ha establecido un valor para %s",CUERPO(token),numLine());
			}
		break;

		case CADENA :
			if(tipo_imp)printf("%s\n",quitarComillas(CUERPO(token)));
			else printf("%s",quitarComillas(CUERPO(token)));
		break;
		default : 
			printf("%s\n","NONE");break;
	}
}
void errores(int codigo) 
{ 
	printf("\nLINEA %i", numLine());
	switch (codigo) 
	{ 
		case 3 :printf(" :ESPERABA UN =\n");break; 
		case 4 :printf(" :ESPERABA UN )\n");break; 
		case 5 :printf(" :ESPERABA UN IDENTIFICADOR\n");break; 
		case 6 :printf(" :INSTRUCCION DESCONOCIDA\n");break; 
		case 7 :printf(" :ESPERABA UNA CONSTANTE\n");break;
		case 8 :printf(" :NO EXISTE EL IDENTIFICADOR\n");break;
		default: 
		printf(" :NO DOCUMENTADO\n"); 
	} 
	ERROR_EXIT();
}
