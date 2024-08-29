
#include "../analisisDatos/analisisDatos.h"

#include "../cJSON/cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // Esto habilita el tipo bool en C
#include <string.h>  // Para strlen, strtok, strdup
#include <ctype.h>   // Para isdigit

// Funcion que permite calcular el total de ventas sumando todo el total
void calculoTotalVentas()
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

    // Inicializamos la variable de su suma total
    double totalVentas = 0.0;
    // Recorremos todas las ventas y sumamos solo los totales
    int totalItems = cJSON_GetArraySize(ventasArray);
    for (int i = 0; i < totalItems; i++)
    {
        // Obtiene la venta actual en el array de ventas
        cJSON *venta = cJSON_GetArrayItem(ventasArray, i);
        // Ahora obtiene el objeto "total" dentro de la venta
        cJSON *totalItem = cJSON_GetObjectItem(venta, "total"); // Aquí debe ser 'venta', no 'ventasArray'
        if (totalItem != NULL && totalItem->valuedouble > 0)
        {
            totalVentas += totalItem->valuedouble; // Suma el valor total
        }
    }

    printf("\nEl total de ventas es: %.2f\n", totalVentas);
    // Liberamos la memoria
    cJSON_Delete(ventasArray);
}
// Funcion que me mostrara el total por año
void totalAnual()
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

    // Creamos una estructura para alamcenar los totales por año
    typedef struct
    {
        int anio;
        double total;
    } totalPorAnio;
    // Asignamos memoria estatica
    totalPorAnio totales[20];
    int cantidadAnios = 0;

    int totalItems = cJSON_GetArraySize(ventasArray);
    for (int i = 0; i < totalItems; i++)
    {

        // Obtiene la venta actual en el array de ventas
        cJSON *venta = cJSON_GetArrayItem(ventasArray, i);
        // Ahora obtiene el objeto "fecha" dentro de la venta
        cJSON *fechaItem = cJSON_GetObjectItem(venta, "fecha");
        cJSON *totalItem = cJSON_GetObjectItem(venta, "total");

        if (fechaItem != NULL && totalItem != NULL && totalItem->valuedouble > 0)
        {
            // Parseamos la fecha (sabiendo el formato yyyy-mm-dd)
            int anio, mes;
            sscanf(fechaItem->valuestring, "%d-%d", &anio, &mes);

            // Vemos si ya tenemos ese año registrado.
            bool anioEncontrado = false;
            for (int j = 0; j < cantidadAnios; j++)
            {
                // Asignamos la memoria
                //  totalPorAnio totales[20];
                if (totales[j].anio == anio)
                {
                    totales[j].total += totalItem->valuedouble;
                    anioEncontrado = true;
                    break;
                }
            }

            // Si el año no está registrado, lo agregamos
            if (!anioEncontrado)
            {
                totales[cantidadAnios].anio = anio;
                totales[cantidadAnios].total = totalItem->valuedouble;
                cantidadAnios++;
            }
        }
    }
    // Mostramos los totales por año
    printf("\nTotal de ventas por anualidad:\n");
    for (int i = 0; i < cantidadAnios; i++)
    {
        printf("anualidad %d: %.2f\n", totales[i].anio, totales[i].total);
    }

    cJSON_Delete(ventasArray);
}
// Funcion que me mostrara el total por mes de x año
void totalMensual()
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

    // Creamos una estructura para alamcenar los totales por mes
    typedef struct
    {
        int anio;
        int mes;
        double total;
    } totalPorMes;
    // Asignamos memoria estatica
    totalPorMes totales[20];
    int cantidadMes = 0;
    int totalItems = cJSON_GetArraySize(ventasArray);
    for (int i = 0; i < totalItems; i++)
    {
        cJSON *venta = cJSON_GetArrayItem(ventasArray, i);
        cJSON *fechaItem = cJSON_GetObjectItem(venta, "fecha");
        cJSON *totalItem = cJSON_GetObjectItem(venta, "total");
        if (fechaItem != NULL && totalItem != NULL && totalItem->valuedouble > 0)
        {
            // Parseamos la fecha
            int anio, mes;
            sscanf(fechaItem->valuestring, "%d-%d", &anio, &mes);

            // Buscamos si ya tenemos esa combinación de año y mes registrada
            bool mesEncontrado = false;
            for (int j = 0; j < cantidadMes; j++)
            {
                if (totales[j].anio == anio && totales[j].mes == mes)
                {
                    totales[j].total += totalItem->valuedouble;
                    mesEncontrado = true;
                    break;
                }
            }

            // Si la combinación de mes y año no está registrada, la agregamos
            if (!mesEncontrado)
            {
                totales[cantidadMes].anio = anio;
                totales[cantidadMes].mes = mes;
                totales[cantidadMes].total = totalItem->valuedouble;
                cantidadMes++;
            }
        }
    }

    // Mostrar los totales por mes y año
    printf("\nTotal de ventas por mes y anualidad:\n");
    for (int i = 0; i < cantidadMes; i++)
    {
        printf("El total del mes %d de la anualidad %d es: %.2f\n", totales[i].mes, totales[i].anio, totales[i].total);
    }

    cJSON_Delete(ventasArray);
}
