/*
* Nombre : token.h
* Descripción : Este archivo de cabecera define
* 	los distintos tokens que se pueden encontrar en el
* 	lenguaje como constantes simbólicas
*/
#define MAX_SIZE_CUERPO 100
/*
Enumeración que contiene la clasificación de los
tokens leídos por el analizador léxico para compartir con
el sintáctico*/
typedef enum{
	IDENTIFICADOR,
	FUNCION,
	PALABRA_RESERVADA,
	CADENA,
	NUMERO_ENTERO,
	NUMERO_DECIMAL,
	IGUALDAD,
	ASIGNACION,
	OPERADOR,
	DOSPUNTOS,
	CONSTANTE,
	COMENTARIO,
	PARIZQ, /*paréntesis izquierdo*/
	PARDER, /*paracentesis derecho*/
	NONE/*NONE para evitar declarar un token, equivale a nulo*/
}Tipo_Token;

typedef struct Token{
	Tipo_Token tipo_token;
	char cuerpo[MAX_SIZE_CUERPO];
	Token *sig;
}Token;

Token * newToken(Tipo_Token tipo_token,const char* cuerpo);
Token * nextToken(Token* token);
void clearTokens(Token* token);