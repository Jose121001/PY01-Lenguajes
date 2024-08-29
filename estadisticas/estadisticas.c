#include "estadisticas.h"
#include "../cJSON/cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estructura para almacenar el total por categoría
typedef struct
{
    char categoria[64];
    double totalVentas;
} CategoriaTotal;

// Función para comparar categorías por total de ventas (para ordenar)
int compararCategorias(const void *a, const void *b)
{
    CategoriaTotal *categoriaA = (CategoriaTotal *)a;
    CategoriaTotal *categoriaB = (CategoriaTotal *)b;
    if (categoriaB->totalVentas > categoriaA->totalVentas)
        return 1;
    else if (categoriaB->totalVentas < categoriaA->totalVentas)
        return -1;
    return 0;
}

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
    fseek(archivo, 0, SEEK_END);
    long fileSize = ftell(archivo);
    fseek(archivo, 0, SEEK_SET);
    char *contenido = (char *)malloc(fileSize + 1);
    fread(contenido, 1, fileSize, archivo);
    contenido[fileSize] = '\0';
    fclose(archivo);

    // Parsear el JSON
    cJSON *ventasArray = cJSON_Parse(contenido);
    free(contenido);
    if (ventasArray == NULL)
    {
        printf("Error al parsear el archivo JSON.\n");
        return;
    }

    int totalItems = cJSON_GetArraySize(ventasArray);
    CategoriaTotal categorias[totalItems];
    int totalCategorias = 0;

    // Calculamos el total de ventas por categoría
    for (int i = 0; i < totalItems; i++)
    {
        cJSON *venta = cJSON_GetArrayItem(ventasArray, i);
        cJSON *categoriaItem = cJSON_GetObjectItem(venta, "categoria");
        cJSON *totalItem = cJSON_GetObjectItem(venta, "total");

        if (categoriaItem != NULL && totalItem != NULL)
        {
            // Buscamos si la categoría ya está registrada
            int categoriaEncontrada = 0;
            for (int j = 0; j < totalCategorias; j++)
            {
                if (strcmp(categorias[j].categoria, categoriaItem->valuestring) == 0)
                {
                    categorias[j].totalVentas += totalItem->valuedouble;
                    categoriaEncontrada = 1;
                    break;
                }
            }

            // Si no se encontró, la agregamos
            if (!categoriaEncontrada)
            {
                strcpy(categorias[totalCategorias].categoria, categoriaItem->valuestring);
                categorias[totalCategorias].totalVentas = totalItem->valuedouble;
                totalCategorias++;
            }
        }
    }

    // Ordenamos las categorías por total de ventas
    qsort(categorias, totalCategorias, sizeof(CategoriaTotal), compararCategorias);

    // Mostramos el top 5
    printf("\nTop 5 categorias con mayores ventas:\n");
    for (int i = 0; i < 5 && i < totalCategorias; i++)
    {
        printf("%d. Categoria: %s - Total Ventas: %.2f\n", i + 1, categorias[i].categoria, categorias[i].totalVentas);
    }

    cJSON_Delete(ventasArray);
}
