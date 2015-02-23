/* error.cpp: Maneja los errores*/
#include <stdlib.h>
#include "error.h"
#include "token.h"
#include "lexico.h"
#include "tabla_simbolos.h"
/*Limpia la memoria recervada*/
void exitProgram(){
	finishMDD();
	clearLexico();
	clearTablaSimbolos();
	exit(0);
}
