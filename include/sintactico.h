/*
* Nombre : sintactico.h
* Descripción : Cabecera de sintactico.cpp
*/
#define SIG(token) token->sig
#define TIPO(token) token->tipo_token
#define CUERPO(token) token->cuerpo
#define PRINT "imprime"
#define PRINTLN "imprimeln"
void initProgram(Token *tokens);
void errores(int codigo);
