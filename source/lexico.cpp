/*
* Nombre : lexico.cpp
* Descripción : Este archivo proporciona funciones y
* convenciones para manejar la Máquina Discriminatoria Determinista
* creada por el archivo mddcore.cpp
* 
* Las funciones que comienzan con (__) son privadas
* y no deben ser llamadas directamente desde el exterior
*/
#include <stdio.h>
#include <string.h>

#include "token.h"
#include "lexico.h"
#include "error.h"
#include "entrada.h"
#include "mddcore.cpp"/*El núcleo de la maquina determinista*/

#define TRUE 1
#define FALSE 0

static int esta_iniciada = FALSE;/*Indica si la maquina esta iniciada*/
static Token *lista_token = NULL;/*Apunta al primer elemento de la lista de tokens*/
static Estado *estado_actual = NULL;
static Estado *ultimo_estado_final = NULL;
static int indice_cuerpo  = 0;/*La posicion de los cuerpos de los tokens y su
			dimension*/
static char cuerpo[MAX_SIZE_CUERPO];/*agrega cuerpos a los tokrns*/

/*Evalua si el caracter ingresado es una letra ya sea mayúscula o minúscula*/
static int isLetter(const char c){
	if(c >= 'A' && c <= 'Z')
		return TRUE;
	if(c >= 'a' && c <= 'z')
		return TRUE;
	return FALSE;
}

/*Evaluá si el carácter ingresado es un numero*/
static int isNumber(const char c){	
	if(c >= '0' && c <= '9')
		return TRUE;
	return FALSE;
}
/*
Devuelve el tipo de expresión perteneciente a la enumeración "Tipo_Expresion"
de acuerdo al carácter recibido*/
static Tipo_Expresion __charToExpresion(const char c){
	if(isLetter(c))return LETRA;
	if(isNumber(c))return NUMERO;
	if(c == '_')return GUION_BAJO;
	if(c == '.')return PUNTO;
	if(c == '+')return MAS;
	if(c == '-')return MENOS;
	if(c == '*')return ASTERISCO;
	if(c == '/')return DIAGONAL;
	if(c == '=')return IGUAL;
	if(c == '"')return COMILLAS;

	if(c == ':')return EXPDOSPUNTOS;
	if(c == '(')return EXPPARIZQ;
	if(c == ')')return EXPPARDER;

	return COMODIN;/*cualquier otro*/
}

/*Inserta un nuevo token en la lista de tokens apuntada por "lista_token"*/
static void __addToken(Token* token)
{
	if(lista_token == NULL){
		lista_token = token;
		return;
	}
	Token * aux = lista_token;
	while(aux->sig != NULL){
		aux = aux->sig;
	}
	aux->sig = token;
}

/*Agrega el carácter recibido al cuerpo en la posición indicada por indice_cuerpo*/
static void __addCuerpo(const char c)
{
	cuerpo[indice_cuerpo] = c;
	indice_cuerpo++;
}/*agrega el caracter nulo para indicar el fin de la cadena*/
static void __addNULL()
{
	cuerpo[indice_cuerpo] = '\0';
	indice_cuerpo = 0;
}

/*Genera un token y lo agrega a la lista que se devolverá*/
static void __makeToken(Tipo_Token tipo_token,const char* cuerpo)
{
	
	__addNULL();

	/*Para que ignore los comentarios*/
	if(tipo_token == COMENTARIO){
		estado_actual = NULL;
		ultimo_estado_final = NULL;
		return;
	}

	Token *token = newToken(tipo_token,cuerpo);
	
	__addToken(token);
	estado_actual = NULL;
	ultimo_estado_final = NULL;
}

/*Devuelve un apuntador al nodo del camino que coinsida con el tipo de expresión
ingresado*/
static Estado * buscarCamino(Tipo_Expresion tipo_expresion)
{
	Estado *buscado = NULL;
	Inicial *inicio = estado_inicial;
	int i = 0, j = 0;
	for (i = 0; i < inicio->numero_caminos ; ++i)
	{
		if(inicio->camino[i] != NULL)
		{
			for (j = 0; j < inicio->camino[i]->size_expresion; ++j)
			{
				if(inicio->camino[i]->tipo_expresion[j] == tipo_expresion)
				{
					buscado = inicio->camino[i];
					return buscado;
				}
			}
		}
	}
	return NULL;/*no hay coincidencia*/
}
/*Verifica si el tipo de expresión recibido es aceptado por el estado actual*/
static int __isAcepted(Estado *estado, Tipo_Expresion e)
{
	int i = 0, com = FALSE;
	if(estado == NULL)return FALSE;
	for (i = 0; i < estado->size_expresion; ++i)
	{
		if (estado->tipo_expresion[i] == e)
		{
			return TRUE;
		}
		if(estado->tipo_expresion[i] == COMODIN){
			com = TRUE;
		}
	}
	
	/*Si el estado actual tiene comodín y el siguiente estado es un
	estado valido para la expresión contenida en e, entonces retornamos
	falso indicando que no la aceptaremos ya que hay otro estado en la
	espera*/
	if(com == TRUE )
	{
		if(__isAcepted(estado->sig,e))
		{
			return FALSE;
		}else{
			return TRUE;
		}
	}else{
		return FALSE;
	}
}
/*Mueve el estado del apuntador "estado_actual" y devuelve
verdadero si se pudo mover , en caso contrario devuelve falso*/
static int __mover(const char c)
{
	if ( estado_actual == NULL)/*Esta en el estado inicial*/
	{
		estado_actual = buscarCamino( __charToExpresion(c) );
		if(estado_actual == NULL)
		{
			ERROR_MESSAGE_LINE_EXIT("Caracter no reconocido %c",c,numLine());
		}else{
			return TRUE;
		}
	}else{/*Significa que ya estamos en algun estado*/
		if(estado_actual->tipo_estado == FINAL)
		{
			ultimo_estado_final = estado_actual;
		}else{/*Para que los decimales solo acepten un punto*/
			if (estado_actual->tipo_estado == INTERMEDIO)
			{
				if ( __charToExpresion(c) == PUNTO)
				{
					estado_actual = estado_actual->sig;
					return FALSE;
				}
			}
		}
		if( __isAcepted(estado_actual, __charToExpresion(c)) )
		{
			/*no se hace nada ya que el estado sigue siendo correcto y aceptable*/
			return TRUE;
		}else{
			if( estado_actual->sig != NULL)
			{
				if( __isAcepted(estado_actual->sig, __charToExpresion(c)))
				{
					estado_actual = estado_actual->sig;
					return TRUE;
				}else{
					return FALSE;
				}
			}else{
				return FALSE;
			}
		}
	}
}
/*Retorna verdadero o falso indicando si el carácter de entrada se pudo o no
procesar*/
static int __procesar(const char c)
{
	if(c == ' '){
		if (estado_actual == NULL)return TRUE;
		if (estado_actual->tipo_estado == FINAL)
		{
			if(estado_actual->tipo_estado == INTERMEDIO)
			{
				estado_actual = ultimo_estado_final;
			}
			__makeToken(estado_actual->tipo_token,cuerpo);
			return TRUE;
		}
		/*vemos si el espacio es necesario o no*/
		if(__isAcepted(estado_actual , COMODIN)||
			__isAcepted(estado_actual , COMILLAS)){
			__addCuerpo(c);
		}
		return TRUE;
	}

	/*Vemos si pudimos cambiar el estado de la máquina*/
	if(! __mover(c))
	{
		if(estado_actual->tipo_estado == INTERMEDIO)
		{
			estado_actual = ultimo_estado_final;
		}
		__makeToken(estado_actual->tipo_token,cuerpo);
		return FALSE;
	}
	/*Verdadero por lo tanto agregamos el caracter al cuerpo del siquiente token*/
	__addCuerpo(c);
	return TRUE;
}


/*Funcion Principal de este archivo, recibe una cadena de caracteres y
la transforma en una lista enlazada en donde cada nodo es un token 
reconocido por el analizador*/
Token* getTokensFromString(const char *cadena)
{
	if(!esta_iniciada){
		startMDD();
		esta_iniciada = TRUE;
	}
	clearLexico();
	cuerpo[0] = '\0';

	int i = 0;
	int string_size = strlen(cadena);
	for (i = 0; i < string_size ; )
	{
		if(!__procesar(cadena[i])){/*Si no se pudo procesar es que creamos un
		nuevo token y por lo tanto necesitamos regresar al caracter leido*/
			continue;
		}
		++i;
	}
	if(estado_actual != NULL)
	{
		if(estado_actual->tipo_estado == FINAL)
		{
			__makeToken(estado_actual->tipo_token,cuerpo);
		}else{
			__addNULL();
			ERROR_MESSAGE_LINE_EXIT(
				"Lexema no reconocido %s",cuerpo,numLine());
		}
	}

	return lista_token;
}

/*Realiza tareas de limpieza en este archivo*/
void clearLexico()
{
	/*Limpia la lista de tokens*/
	clearTokens(lista_token);
	lista_token = NULL;
}