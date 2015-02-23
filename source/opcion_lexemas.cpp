/*
* opcion_lexemas.cpp
* Descripción:este programa hace posible la 
* impresión de todos los lexemas
*/
void tokenToString(const Tipo_Token tipo_token)
{
	switch(tipo_token)
	{
		case IDENTIFICADOR : printf("\n\t%s","IDENTIFICADOR");break;
		case CADENA : printf("\n\t%s","CADENA");break;
		case NUMERO_ENTERO : printf("\n\t%s","NUMERO_ENTERO");break;
		case NUMERO_DECIMAL : printf("\n\t%s","NUMERO_DECIMAL");break;
		case IGUALDAD : printf("\n\t%s","IGUALDAD");break;
		case ASIGNACION : printf("\n\t%s","ASIGNACION");break;
		case OPERADOR : printf("\n\t%s","OPERADOR");break;
		case CONSTANTE : printf("\n\t%s","CONSTANTE");break;
		case FUNCION : printf("\n\t%s","FUNCION");break;
		case PALABRA_RESERVADA : printf("\n\t%s","PALABRA_RESERVADA");break;
		case COMENTARIO : printf("\n\t%s","COMENTARIO");break;
		case DOSPUNTOS : printf("\n\t%s","DOSPUNTOS");break;
		case PARIZQ : printf("\n\t%s","PARIZQ");break;
		case PARDER : printf("\n\t%s","PARDER");break;
		default : printf("\n\t%s","NONE");break;
	}
}
void imprimirToken(const Token *token)
{
	tokenToString(token->tipo_token);
	printf("\t %s\n",token->cuerpo);
}
void impimirListaTokens(const Token *lista_token)
{
	if(lista_token == NULL)return;
	imprimirToken(lista_token);
	impimirListaTokens(lista_token->sig);
}
void printLexema(Token *tokens)
{
	impimirListaTokens(tokens);
	return;
}
