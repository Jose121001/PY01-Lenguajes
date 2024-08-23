# Nombre del ejecutable
TARGET = PY01-Lenguajes

# Archivos fuente
SRC = ../main.c ../importacionDatos/importacionDatos.c ../procesamientoDatos/procesamientoDatos.c ../analisisDatos/analisisDatos.c ../analisisDatosTemporales/analisisDatosTemporales.c ../estadisticas/estadisticas.c ../cJSON/cJSON.c

# Directorios de los archivos .h
INCLUDES = -I../importacionDatos -I../procesamientoDatos -I../analisisDatos -I../analisisDatosTemporales -I../estadisticas -I../cJSON

# Compilador
CC = gcc

# Flags de compilación
CFLAGS = -Wall

# Comando por defecto (se ejecuta al usar "make")
all:
	$(CC) $(SRC) $(INCLUDES) $(CFLAGS) -o $(TARGET)

# Comando para limpiar archivos generados
clean:
	rm -f $(TARGET)
