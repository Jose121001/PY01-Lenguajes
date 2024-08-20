#include "../procesamientoDatos/procesamientoDatos.h"

#include "../cJSON/cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // Esto habilita el tipo bool en C
#include <string.h>  // Para strlen, strtok, strdup
#include <ctype.h>   // Para isdigit

// Nombre del archivo JSON (declarado globalmente) No puede declararse en mas de un archivo C
// const char *nombreArchivo = "../data/ventas.json";

// Funcion que me permite caerle encima al archvo JSON si es que hay duplicados, eliminandolos.
void borrarDuplicados()
{
    const char *nombreArchivo = "../data/ventas.json";
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL)
    {
        printf("No se pudo abrir el archivo en la ruta: %s\n", nombreArchivo);
        return;
    }
    // Leemos el contenido del archivo
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

    // Se crea un nuevo array sin duplicados.
    cJSON *ventasSinDuplicados = cJSON_CreateArray();
    int totalVentas = cJSON_GetArraySize(ventasArray);

    for (int i = 0; i < totalVentas; i++)
    {
        // Se incrementa los valores del for ya que se van cambiando.
        // i tomara los valores del 0 al 9.
        cJSON *ventaActual = cJSON_GetArrayItem(ventasArray, i);
        bool esDuplicado = false;

        // Comparar con las ventas anteriores en ventasSinDuplicados
        for (int j = 0; j < cJSON_GetArraySize(ventasSinDuplicados); j++)
        {
            cJSON *ventaComparada = cJSON_GetArrayItem(ventasSinDuplicados, j);

            // Compara si todas las propiedades son iguales
            // El método cJSON_Compare permite comparar si dos objetos JSON son iguales
            if (cJSON_Compare(ventaActual, ventaComparada, 1))
            {
                esDuplicado = true;
                break;
            }
        }

        // Si no es duplicado, agregarlo al nuevo array
        if (!esDuplicado)
        {
            cJSON_AddItemToArray(ventasSinDuplicados, cJSON_Duplicate(ventaActual, 1));
        }
    }

    // Guardar el nuevo array sin duplicados en el archivo
    archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL)
    {
        printf("No se pudo abrir el archivo para escribir.\n");
        cJSON_Delete(ventasArray);
        cJSON_Delete(ventasSinDuplicados);
        return;
    }

    char *jsonString = cJSON_Print(ventasSinDuplicados);
    fprintf(archivo, "%s", jsonString);
    fclose(archivo);

    // Liberar memoria
    cJSON_free(jsonString);
    cJSON_Delete(ventasArray);
    cJSON_Delete(ventasSinDuplicados);

    printf("Duplicados eliminados correctamente.\n");
}