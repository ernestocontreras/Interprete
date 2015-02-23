/*
* Nombre : error.h
* Descripción : Archivo de cabecera para error.cpp
*/
void exitProgram();

#define MEMORY_ERROR "Error en peticion de memoria"
#define FILE_ERROR "Archivo no encontrado"

#define ERROR_MESSAGE_LINE_EXIT(format,string,line) printf("\n%s %3i : ","LINEA",line );\
	printf(format,string);putchar('\n'); exitProgram()
#define ERROR_MESSAGE_EXIT(string) printf("%s",string); exitProgram()
#define EXIT_EROOR_LINE(linea) printf("%s %3i : ","LINEA",linea);exitProgram()
#define ERROR_EXIT() exitProgram()
