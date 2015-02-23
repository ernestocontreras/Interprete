/*
* Nombre : recerved_words.h
* Descripción : Declara parablas clave
*/
#define IF "si"
#define FOR "para"
#define WHILE "mientras"
#define MINOR "menor"
#define MAJOR "mayor"
#define NULO "nulo"
int isReservedWord(const char cuerpo[])
{
	if (strcmp(IF,cuerpo) == 0)return 1;
	if (strcmp(FOR,cuerpo) == 0)return 1;
	if (strcmp(WHILE,cuerpo) == 0)return 1;
	if (strcmp(MINOR,cuerpo) == 0)return 1;
	if (strcmp(MAJOR,cuerpo) == 0)return 1;
	if (strcmp(NULO,cuerpo) == 0)return 1;
	return 0;
}
