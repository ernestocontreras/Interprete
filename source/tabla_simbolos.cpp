/*
* Nombre : tabla_simbolos.cpp
* Descripción : Este archivo gestiona el análisis
* sintáctico del interprete haciendo uso del analizador léxico.
* Taimen obedece a la gramática declarada en el archivo Grammar
* Las funciones que comienzan con (__) son privadas
* y no deben ser llamadas directamente desde el exterior
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabla_simbolos.h"
#include "error.h"
#define VAR(tabla) tabla->var
#define TRUE 1
#define FALSE 0
typedef struct Simbol{
	Variable *var;
	Simbol *sig;
}Simbol;
static Simbol *tabla = NULL;
static Simbol * newSimbol(Variable *var)
{
	Simbol *s = (Simbol*)malloc(sizeof(Simbol));
	if(s == NULL){
		ERROR_MESSAGE_EXIT(MEMORY_ERROR);
	}
	s->var = var;
	s->sig = NULL;
	return s;
}
Variable * newVariable()
{
	Variable *var = (Variable*)malloc(sizeof(Variable));
	if(var == NULL){
		ERROR_MESSAGE_EXIT(MEMORY_ERROR);
	}
	return var;
}
int existInTable(const char id[])
{
	Simbol * aux = tabla;
	while(aux != NULL){
		if(strcmp(VAR(aux)->id,id) == 0){
			return TRUE;
		}
		aux = aux->sig;
	}
	return FALSE;
}
Variable *searchInTable(const char id[])
{
	Simbol * aux = tabla;
	while(aux != NULL){
		if(strcmp(VAR(aux)->id,id) == 0){
			return aux->var;
		}
		aux = aux->sig;
	}
	return NULL;
}
void setInTable(Variable *var)
{
	if(!existInTable(var->id)){
		Simbol * nuevo = newSimbol(var);
		if(tabla == NULL){
			tabla = nuevo;
			return;
		}
		Simbol *aux = tabla;
		while(aux->sig != NULL){
			aux = aux->sig;
		}
		aux->sig = nuevo;
		return;
	}
}
void changeInTable(const char id[],Tipo_Dato tipo_dato,Valor value)
{
	if(existInTable(id)){
		Variable *var_actual = searchInTable(id);
		var_actual->tipo_dato = tipo_dato;
		var_actual->value = value;
	}
}
void deletInTable(const char id[])
{
	if(!existInTable(id))return;
	Variable *var_actual = searchInTable(id);
	
	Simbol * aux = tabla,*antes = tabla;
	if(strcmp(VAR(tabla)->id,id) == 0){
		tabla = tabla->sig;
		free(aux);
		return;
	}
	while(aux->sig != NULL && strcmp(VAR(aux)->id,id) != 0){
		antes = aux;
		aux = aux->sig;
	}
	antes->sig = aux->sig;
	free(aux);
}
void limpiarTabla(Simbol *tabla)
{
	if(tabla == NULL)return;
	limpiarTabla(tabla->sig);
	free(tabla);
	tabla = NULL;
}
void clearTablaSimbolos()
{
	limpiarTabla(tabla);
}
