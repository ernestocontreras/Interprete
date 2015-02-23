/*
* Nombre : mddcore.cpp
* Descripción : Este archivo gestiona una
* Máquina Discriminatoria Determinista, la crea en memoria y
* la inicializa para ser usada por otro archivo
* Las funciones que comienzan con (__) son privadas
* y no deben ser llamadas directamente desde el exterior
*/
/*
* Modificado : 6/11/2013
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "error.h"
#define TRUE 1
#define FALSE 0
/*
Expresiones que definirán el avance y cambio de estados en la
maquina*/
typedef enum Tipo_Expresion{ 
	LETRA,
	NUMERO,
	PUNTO,
	MAS,
	MENOS,
	ASTERISCO,
	DIAGONAL,
	IGUAL,
	COMILLAS,
	GUION_BAJO,
	EXPDOSPUNTOS,
	EXPPARIZQ,
	EXPPARDER,
	COMODIN /*Acepta cualquier expresión*/
}Tipo_Expresion;
typedef enum Tipo_Estado{ FINAL , INTERMEDIO }Tipo_Estado;/*Estados que existen en la maquina*/
/*Creamos un contenedor de estados*/
typedef struct Expresion{
	//void (*__addExpresion(Expresion *e,Tipo_Expresion tipo_expresion));
	Tipo_Expresion *list;/*que expresion acepta*/
	int size_list;
}Expresion;
/*Define cada nodo de estado en la maquina*/
typedef struct Estado{
	Tipo_Estado tipo_estado;
	Tipo_Expresion *tipo_expresion;/*que expresion acepta*/
	Tipo_Token tipo_token;/*que tipo de token puede generar en caso
		de ser un estado final, si es un estado intermedio se tiene que definir
		como NONE*/
	int size_expresion;
	Estado *sig;/*Apunta al siguiente estado en el camino*/
}Estado;
/*Maneja todos los caminos, estado elemental o cero*/
typedef struct{
	/*Caminos desde el nodo inicial*/
	int numero_caminos;
	Estado **camino;
}Inicial;
/*Declaración del apuntador al inicio de la maquina*/
static Inicial *estado_inicial = NULL;/*apunta al estado inicial*/
static Estado *camino = NULL;
static Expresion *expresion = NULL;
static int iniciado = FALSE;/*El estado en el que está la maquina*/
/*Contara el numero de caminos que se han definido para la maquina*/
static Expresion * __newExpresion(){
	Expresion *e = (Expresion*)malloc(sizeof(Expresion));
	if(e == NULL){
		printf("s\n",MEMORY_ERROR);
		exit(1);
	}
	e->size_list = 0;
	e->list = NULL;
	return e;
}
static void __addExpresion(Tipo_Expresion tipo_expresion){
	Tipo_Expresion *tmp = NULL;
	tmp = (Tipo_Expresion*)realloc(expresion->list,
		(expresion->size_list + 1) * sizeof(Tipo_Expresion));
	if(tmp == NULL){
		printf("s\n",MEMORY_ERROR);
		exit(1);
	}
	expresion->list = tmp;
	expresion->list[ expresion->size_list ] = tipo_expresion;
	(expresion->size_list)++;
	return;
}
/*Crea el nodo inicial de la maquina*/
static Inicial * __getInicial()
{
	Inicial *nuevo = (Inicial*)malloc(sizeof(Inicial));
	if(nuevo == NULL){
		printf("s\n",MEMORY_ERROR);
		exit(1);
	}
	/*se limpian los apuntadores*/
	nuevo->camino = NULL;
	nuevo->numero_caminos = 0;
	return nuevo;
}
/*
* Crea un Nodo de tipo Estado y lo pone en memoria
* tipo_estado : Que estado representa FINAL o INTERMEDIO
* tipo_expresion : Que expresiones acepta
* tipo_token : Si es final que token generaría
*/
static Estado* __nuevoEstado(Tipo_Estado tipo_estado,Tipo_Token tipo_token){

	Estado *nuevo = (Estado*)malloc(sizeof(Estado));
	if(nuevo == NULL){
		printf("s\n",MEMORY_ERROR);
		exit(1);
	}
	nuevo->tipo_estado = tipo_estado;
	nuevo->size_expresion = expresion->size_list;
	nuevo->tipo_expresion = expresion->list;
	nuevo->tipo_token = tipo_token;
	nuevo->sig = NULL;
	expresion->list  = NULL;
	expresion->size_list = 0;
	return nuevo;
}
/*
Insertar un Nodo de tipo Estado al final
del camino apuntado por "camino", si el camino esta vació se inserta
al inicio*/
static void __add(Tipo_Estado tipo_estado,Tipo_Token tipo_token)
{
	Estado *nuevo = __nuevoEstado(tipo_estado,tipo_token);
	if(camino == NULL){
		camino = nuevo;
		return;
	}
	Estado *aux = camino;
	while(aux->sig != NULL)
	{
		aux = aux->sig;
	}
	aux->sig = nuevo;
}
/*
Enlaza un camino ingresado a la primer posición que este
libre en el arreglo de apuntadores  miembro de "estado_inicial"*/
static void __enlazar()
{
	if(camino == NULL)return;
	Estado **temporal = NULL;
	temporal = (Estado**)realloc(estado_inicial->camino,
						(estado_inicial->numero_caminos+1) * sizeof(Estado));
	if(temporal == NULL){
		printf("s\n",MEMORY_ERROR);
		exit(1);
	}
	estado_inicial->camino = temporal;
	estado_inicial->camino[estado_inicial->numero_caminos] = camino;
	camino = NULL;
	estado_inicial->numero_caminos++;
	return;
}
/*****************************************************************************
Función principal : Se encarga de inicializar la maquina poniendo
en memoria los elementos a utilizar y definiendo los caminos
y sus enlaces, así como su comportamiento, por seguridad solo se puede
mandar a llamar una sola vez, dos o mas veces no surtirán efecto en la maquina
********************************************************************************/
void startMDD()
{
	if(iniciado)return;/*Ya esta iniciada la maquina*/
	iniciado = TRUE;
	estado_inicial = __getInicial();
	expresion = __newExpresion();
	/*
	* La Funcion __addExpresion(tipo_expresion) acepta un elemento de la 
	*	enumeración Tipo_Expresion y lo almacena como tipo de expresión aceptada
	*	para el próximo estado (la proxima vez que se llame a __add() ).
	* La función __add(tipo_estado,tipo_token) acepta:
	* 	Tipo de estado : FINAL o INTERMEDIO
	* 	Tipo de token : Puede ser cualquier elemento de la enumeración
	*	Tipo_Token definida en token.cpp e indica que token puede emitir el estado
	*	en caso de ser final, en caso de ser un estado intermedio este parámetro habrá 
	* 	que definirse como NONE.
	* La Funcion __enlazar() agrega el camino definido al estado_inicial.
	*
	* Para crear cada camino de evaluación habrá que definir primero cada nodo de
	* estado en el orden que se evaluará, para esto previamente se debe indicar que 
	* expresión o expresiones aceptara Ejemplo.
	* Si se quiere un camino con un solo nodo que reconozca números y letras,
	* ademas que sea un estado final y que emita un identificador el código es:
	* 	__addExpresion(NUMERO);
	* 	__addExpresion(LETRA);
	* 	__add( FINAL, IDENTIFICADOR);
	* 	__enlazar();
	*/
	/********Creación de los caminos de la maquina********/
	/*
		Camino 1:
		|Final : letra| ->  |Final : numero|
		Puede emitir:
		PALABRA_RESERVADA,FUNCION_DEL_LENGUAJE,
		FUNCION_DE_USUARIO,IDENTIFICADOR*/
	__addExpresion(LETRA);
	__add( FINAL,IDENTIFICADOR);
	__addExpresion(NUMERO);
	__addExpresion(LETRA);
	__addExpresion(GUION_BAJO);
	__add( FINAL, IDENTIFICADOR);

	__enlazar();
	/*
		Camino 2:
		|Final : numero| -> |Intermedio : punto| -> |Final : numero|
		Puede emitir:
		NUMERO_ENTERO , NUMERO_DECIMAL*/
	__addExpresion(NUMERO);
	__add( FINAL,NUMERO_ENTERO);

	__addExpresion(PUNTO);
	__add( INTERMEDIO,NONE);

	__addExpresion(NUMERO);
	__add( FINAL,NUMERO_DECIMAL);

	__enlazar();
	/*
		Camino 3:
		|Final : igual|
		Puede emitir:
		ASIGNACION*/
	__addExpresion(IGUAL);
	__add( FINAL,ASIGNACION);
	__enlazar();
	/*
		Camino 4:
		|Intermedio : comillas| -> |Intermedio : comodin| -> |Final : comillas|
		Puede emitir:
		CADENA*/
	__addExpresion(COMILLAS);
	__add( INTERMEDIO,NONE);

	__addExpresion(COMODIN);
	__add( INTERMEDIO,NONE);

	__addExpresion(COMILLAS);
	__add( FINAL,CADENA);

	__enlazar();
	/*
		Camino 5:
		|Final : mas|
		Puede emitir:
		OPERADOR*/
	__addExpresion(MAS);
	__add( FINAL,OPERADOR);
	__enlazar();
	/*
		Camino 6:
		|Final : menos|
		Puede emitir:
		OPERADOR*/
	__addExpresion(MENOS);
	__add( FINAL,OPERADOR);
	__enlazar();
	/*
		Camino 7:
		|Final : asterisco|
		Puede emitir:
		OPERADOR*/
	__addExpresion(ASTERISCO);
	__add( FINAL,OPERADOR);
	__enlazar();
	/*
		Camino 8:
		|Final : dos puntos|
		Puede emitir:
		DOSPUNTOS*/
	__addExpresion(EXPDOSPUNTOS);
	__add( FINAL,DOSPUNTOS);
	__enlazar();
	/*
		Camino 9:
		|Final : paréntesis izquierdo|
		Puede emitir:
		PAR_IZQ*/
	__addExpresion(EXPPARIZQ);
	__add( FINAL,PARIZQ);
	__enlazar();
	/*
		Camino 10:
		|Final : paréntesis derecho|
		Puede emitir:
		PAR_DER*/
	__addExpresion(EXPPARDER);
	__add( FINAL,PARDER);
	__enlazar();
	/*
		Camino 11:
		|Final : diagonal| -> |Intermedio : asterisco| -> |Intermedio : comodin|
		-> |Intermedio : asterisco| -> |Final : diagonal|
		Puede emitir:
		OPERADOR, COMENTARIO*/
	__addExpresion(DIAGONAL);
	__add( FINAL,OPERADOR);

	__addExpresion(ASTERISCO);
	__add( INTERMEDIO,NONE);

	__addExpresion(COMODIN);
	__add( INTERMEDIO,NONE);

	__addExpresion(ASTERISCO);
	__add( INTERMEDIO,NONE);

	__addExpresion(DIAGONAL);
	__add( FINAL,COMENTARIO);

	__enlazar();
}/***fin de la funcion startMDD()***/

/*
Función para liberar la memoria de un camino
usando recursividad*/
static void __liberarCamino(Estado *camino)
{
	if(camino == NULL)return;/*caso base*/

	__liberarCamino(camino->sig);
	
	camino->tipo_expresion = NULL;
	free(camino->tipo_expresion);
	camino->sig = NULL;
	free(camino);
}
/*
Función que se encarga de liberar la memoria creada por startMDD()*/
void finishMDD()
{
	if(estado_inicial == NULL)return;
	int i = 0;
	for (i = 0; i < estado_inicial->numero_caminos; ++i)
	{
		__liberarCamino(estado_inicial->camino[i]);
		estado_inicial->camino[i] = NULL;
	}

	free(estado_inicial->camino);
	estado_inicial->camino = NULL;
	estado_inicial->numero_caminos = 0;
	free(estado_inicial);
	estado_inicial = NULL;
	
	iniciado = FALSE;
	
}