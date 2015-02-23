# Archivo : Makefile
# Descripción : Este es el archivo de construcción del programa principal
# Modificado :10/11/2013

# Uso : Desde la consola de comandos o terminal colocarse en la carpeta /Interprete,
# llamar al programa de construcción tecleando "make" terminado el proceso
# el archivo resultante estará alojado en /bin, para limpiar el espacio de trabajo teclee
# "make clean"

# Carpetas principales del proyecto
# SRC : código de las librerías, aquí se debe colocar los .cpp
# INC : archivos de cabecera .h correctamente enlazados.
# OBJ : aquí se colocan todos los objetos .o de los archivos
# BIN : se encuentra aquí el ejecutable resultante de la compilación
SRC= source
INC= include
OBJ= object
BIN= bin
# Llamada al compilador
GPP=g++
# Opciones para el compilador
FLAGS= -c -g -I $(INC)
# Archivos principales del proyecto, actualmente solo es el analizador léxico,
# pero se plantea que sea el programa principal del interprete.
MAIN=$(SRC)/Interprete.cpp
MAIN.O=$(OBJ)/Interprete.o
# Listado de todos los archivos con sus correspondientes prefijos de 
# ubicación en las Carpetas
ENTRADA-H=$(INC)/entrada.h
ENTRADA-C=$(SRC)/entrada.cpp
ENTRADA-O=$(OBJ)/entrada.o

TOKEN-H=$(INC)/token.h
TOKEN-C=$(SRC)/token.cpp
TOKEN-O=$(OBJ)/token.o

LEXICO-H=$(INC)/lexico.h
LEXICO-C=$(SRC)/lexico.cpp
LEXICO-O=$(OBJ)/lexico.o

ERROR-H=$(INC)/error.h
ERROR-C=$(SRC)/error.cpp
ERROR-O=$(OBJ)/error.o

SINTACTICO-H= $(INC)/sintactico.h
SINTACTICO-C= $(SRC)/sintactico.cpp
SINTACTICO-O= $(OBJ)/sintactico.o

TABLA_SIMBOLOS-H = $(INC)/tabla_simbolos.h
TABLA_SIMBOLOS-C = $(SRC)/tabla_simbolos.cpp
TABLA_SIMBOLOS-O = $(OBJ)/tabla_simbolos.o

# Cabeceras usadas por el programa principal
MAINHEADERS=$(ENTRADA-H) $(TOKEN-H) $(LEXICO-H) $(ERROR-H) $(SINTACTICO-H) \
	$(TABLA_SIMBOLOS-H)

# Listado de los objetos usados para el enlazador
OBJETOS=$(ENTRADA-O) $(TOKEN-O) $(LEXICO-O) $(ERROR-O) $(SINTACTICO-O) \
	$(TABLA_SIMBOLOS-O)

# Define la ubicación y el nombre del ejecutable resultante
PROGRAMA=$(BIN)/Interprete

# Genera el programa resultante a partir de los programas objetos y cabeceras
# generados por las compilaciones a continuación declaradas
$(PROGRAMA): $(MAIN.O) $(OBJETOS)
	$(GPP) -o $(PROGRAMA) $(MAIN.O) $(OBJETOS)

# Compila el programa principal
$(MAIN.O): $(MAINHEADERS)
	$(GPP) $(FLAGS) $(MAIN) -o $(MAIN.O)

# Definición de la creación de los archivos objeto .o

$(ENTRADA-O): $(ENTRADA-C) $(ENTRADA-H)
	$(GPP) $(FLAGS) $(ENTRADA-C) -o $(ENTRADA-O)

$(TOKEN-O): $(TOKEN-C) $(TOKEN-H)
	$(GPP) $(FLAGS) $(TOKEN-C) -o $(TOKEN-O)

$(LEXICO-O): $(LEXICO-C) $(LEXICO-H) $(ERROR-H)
	$(GPP) $(FLAGS) $(LEXICO-C) -o $(LEXICO-O)

$(ERROR-O): $(ERROR-C) $(ENTRADA-H) $(LEXICO-H)
	$(GPP) $(FLAGS) $(ERROR-C) -o $(ERROR-O)

$(TABLA_SIMBOLOS-O):$(TABLA_SIMBOLOS-C) $(TABLA_SIMBOLOS-H) $(ERROR-H)
	$(GPP) $(FLAGS) $(TABLA_SIMBOLOS-C) -o $(TABLA_SIMBOLOS-O)

$(SINTACTICO-O): $(SINTACTICO-C) $(SINTACTICO-H) $(ENTRADA-H) $(LEXICO-H) $(TOKEN-H) $(TABLA_SIMBOLOS-H)
	$(GPP) $(FLAGS) $(SINTACTICO-C) -o $(SINTACTICO-O)

# Instrucción para limpiar los archivos generados por el compilador
clean:
	rm -f $(PROGRAMA) $(MAIN.O) $(OBJETOS)