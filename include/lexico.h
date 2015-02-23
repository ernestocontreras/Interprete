/*
* Nombre : lexico.h
* Descripción : Archivo de cabecera para lexico.cpp
*/
/*Recibe una cadena de caracteres de algún buffer y
devuelve el apuntador al inicio de una lista de tokens ordenados
con forme al orden de entrada de izquierda a derecha
para iterar u obtener tokens uno a uno se puede utilizar la función
nextToken definida en token.h*/
Token* getTokensFromString(const char *cadena);
void finishMDD();
void clearLexico();
