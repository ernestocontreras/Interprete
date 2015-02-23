/*
* Nombre : entrada.h
* Descripción : Archivo de cabecera para entrada.cpp
*/
#define MAX_SIZE_LINE 100/*longitud de linea leída*/
#define TRUE 1
#define FALSE 0
/*
Actualmente solo se cuenta con entrada desde
dos flujos; Consola y Archivo.*/
typedef enum{
	CONSOLA,ARCHIVO
}Entrada;

/*
Lee una linea del buffer indicado en la variable
buffer y la almacena en "linea_leida" que se devuelve como un
apuntador tipo char*/
char* readLine();
int setFile(const char * name);
int numLine();