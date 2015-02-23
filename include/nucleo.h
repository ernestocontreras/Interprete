/*
* Este archivo de cabecera describe las principales funcionalidades
* del interprete como los tipos de datos.
*/
#include <stdlib.h>
#define MAX_ID 100
#define MAX_LINE 100
typedef enum{
	CARACTER,ENTERO,FLOTANTE,DOBLE,STRING,
	TIPO_NULO
}Tipo_Dato;
typedef union{
	char c;
	int i;
	float f;
	double d;
	long l;
	char s[MAX_LINE];
}Valor;
typedef struct{
	char id[MAX_ID];
	Tipo_Dato tipo_dato;
	Valor value;
}Variable;
