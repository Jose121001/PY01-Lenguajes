#include "../estadisticas/estadisticas.h"

#include "../cJSON/cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // Esto habilita el tipo bool en C
#include <string.h>  // Para strlen, strtok, strdup
#include <ctype.h>   // Para isdigit
#include <time.h>    //Permite trabajar con las fechas
#include "pdf_generator.h"

// Funcion para visualizar top 5 categorias con mayores ventas con su monto()

void topCincoCate()
{

    const char *nombreArchivo = "../data/ventas.json";
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL)
    {
        printf("No se pudo abrir el archivo en la ruta: %s\n", nombreArchivo);
        return;
    }
    // Leemos el contenido del archivo
    // Estructura brindada por chat
    fseek(archivo, 0, SEEK_END);
    long fileSize = ftell(archivo);
    fseek(archivo, 0, SEEK_SET);
    char *contenido = (char *)malloc(fileSize + 1);
    fread(contenido, 1, fileSize, archivo);
    contenido[fileSize] = '\0';
    fclose(archivo);

    // Parsear el JSON
    cJSON *ventasArray = cJSON_Parse(contenido);
    if (ventasArray == NULL)
    {
        printf("Error al parsear el archivo JSON.\n");
        free(contenido);
        return;
    }
    free(contenido);
}