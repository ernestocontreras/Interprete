/*
* Nombre : entrada.cpp
* Descripción : Gestiona la entrada de datos al
* interprete con funciones que hacen el trabajo
* mas fácil, las funciones que comiencen con (__) son
* funciones locales a este archivo y NO deberán ser llamadas
* directamente desde algún otro archivo, en su lugar existen 
* funciones que se encargaran de hacer
* estas llamadas.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entrada.h"
#include "error.h"
#define TRUE 1
#define FALSE 0
static Entrada buffer = CONSOLA;/*Valor por defecto, se puede cambiar con setFile*/
static FILE *file = NULL;/*apunta al archivo abierto actualmente*/
static char linea_leida [MAX_SIZE_LINE];
static int numero_linea = 0;/*esta variable va a contener el numero
de lineas leídas por el interprete y podrá ser usada por otros módulos con
el fin de informes de error solo llamando al método numLine()*/
/*
Si se manda a llamar correctamente
el buffer de entrada se define como fichero */
int setFile(const char * name)
{
	file = fopen(name,"r");
	if(file == NULL){
		printf("%s ",name );
		ERROR_MESSAGE_EXIT(FILE_ERROR);
	}
	buffer = ARCHIVO;
	return TRUE;
}
/*Funciones de uso interno para readLine()*/
static char* __readFromFile()
{
	linea_leida[0] = '\0';/*reiniciamos la cadena*/
	if(!feof(file))
	{
		fgets(linea_leida,MAX_SIZE_LINE+1,file);

		/*se coloca el nulo al final de la cadena
		para hacerla compatible con la biblioteca
		string.h ya que fgets no lo coloca*/
		/*si la cadena se capturó con un salto de linea este se reemplaza
		con el carácter nulo*/
		if(linea_leida[ strlen(linea_leida) - 1 ] == '\n')
		{
			linea_leida[ strlen(linea_leida) - 1 ] = '\0';
		}else{/*si no entonces se pone al final de la cadena*/
			linea_leida[ strlen(linea_leida)] = '\0';
		}
		if (linea_leida != NULL)numero_linea++;

		return linea_leida;
	}else{
		// printf("FIN DE ARCHIVO\n");//solo para depurar
		fclose(file);
		return NULL;
	}
}
static char* __readFromConsole()
{
	//gets(linea_leida);
	fgets(linea_leida,MAX_SIZE_LINE+1,stdin);
	if(strcmp(linea_leida,"fin")){
		return NULL;
	}
	if (linea_leida != NULL)numero_linea++;

	return linea_leida;
}/*Fin de las funciones de uso interno*/
/*
Lee una linea del buffer indicado en la variable
buffer y la almacena en "linea_leida" que se devuelve como un
apuntador tipo char*/
char* readLine(){
	if (buffer == CONSOLA)
	{
		return __readFromConsole();
	}else{
		if (buffer == ARCHIVO)
		{
			return __readFromFile();
		}
	}
}
int numLine(){
	return numero_linea;
}
