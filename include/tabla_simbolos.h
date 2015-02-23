/*
* Nombre : tabla_simbolos.h
* Descripción : Archivo de cabecera para
* tabla_simbolos.cpp
* 
*/
#include "nucleo.h"
Variable* newVariable();
int existInTable(const char id[]);
Variable* searchInTable(const char id[]);
void setInTable(Variable *var);
void changeInTable(const char id[],Tipo_Dato tipo_dato,Valor value);
void deletInTable(const char id[]);
void clearTablaSimbolos();
