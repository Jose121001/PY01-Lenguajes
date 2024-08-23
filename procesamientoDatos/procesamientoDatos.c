#include "../procesamientoDatos/procesamientoDatos.h"

#include "../cJSON/cJSON.h"
#include <math.h>
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
// Funcion que permite completar los datos en 0 en el json
void completarCeros()
{
    const char *nombreArchivo = "../data/ventas.json";
    // Abre y lee el archivo JSON
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL)
    {
        printf("No se pudo abrir el archivo en la ruta: %s\n", nombreArchivo);
        return;
    }
    fseek(archivo, 0, SEEK_END);
    long fileSize = ftell(archivo);
    fseek(archivo, 0, SEEK_SET);
    char *contenido = (char *)malloc(fileSize + 1);
    fread(contenido, 1, fileSize, archivo);
    contenido[fileSize] = '\0';
    fclose(archivo);

    // Parseamos JSON
    cJSON *ventasArray = cJSON_Parse(contenido);
    free(contenido);
    if (ventasArray == NULL)
    {
        printf("Error al parsear el archivo JSON.\n");
        return;
    }
    // Validamos que hayan 3 o mas datos para podre realizar el calculo
    int totalItems = cJSON_GetArraySize(ventasArray);
    if (totalItems < 3)
    {
        printf("No hay suficientes elementos para completar los datos (minimo 3).\n");
        cJSON_Delete(ventasArray);
        return;
    }

    // Calculamos la media y la mediana para cantidad y precio_unitario
    double mediaCantidad = calcularMedia(ventasArray, "cantidad");
    double mediaPrecio = calcularMedia(ventasArray, "precio_unitario");
    double medianaCantidad = calcularMediana(ventasArray, "cantidad");
    double medianaPrecio = calcularMediana(ventasArray, "precio_unitario");

    // Intercalamos el método de completitud
    for (int i = 0; i < totalItems; i++)
    {
        cJSON *venta = cJSON_GetArrayItem(ventasArray, i);
        // puntero al valor de cantidad y precio
        cJSON *cantidadItem = cJSON_GetObjectItem(venta, "cantidad");
        cJSON *precioItem = cJSON_GetObjectItem(venta, "precio_unitario");
        // Si los valores no son nullos->
        if (cantidadItem != NULL && precioItem != NULL)
        {
            if (cantidadItem->valuedouble == 0)
            {
                // Usamos media o mediana alternadamente para completar los ceros
                // Si la operacion da par se usa media de lo contrarui se usa mediana
                int cantidad = (i % 2 == 0) ? round(mediaCantidad) : round(medianaCantidad);
                // ReplaceItem nos permite cambiar un dato de una seccion especifica de un json.
                cJSON_ReplaceItemInObject(venta, "cantidad", cJSON_CreateNumber(cantidad));
                printf("Cantidad actualizada para venta ID %d: %d\n", cJSON_GetObjectItem(venta, "venta_id")->valueint, cantidad);
            }

            if (precioItem->valuedouble == 0)
            {
                int precio = (i % 2 == 0) ? round(mediaPrecio) : round(medianaPrecio);
                cJSON_ReplaceItemInObject(venta, "precio_unitario", cJSON_CreateNumber(precio));
                // me muestra el cambio.
                printf("Precio unitario actualizado para venta ID %d: %d\n", cJSON_GetObjectItem(venta, "venta_id")->valueint, precio);
            }

            // Recalcular el total con los valores completados
            int nuevaCantidad = round(cJSON_GetObjectItem(venta, "cantidad")->valuedouble);
            int nuevoPrecio = round(cJSON_GetObjectItem(venta, "precio_unitario")->valuedouble);
            int nuevoTotal = nuevaCantidad * nuevoPrecio;
            cJSON_ReplaceItemInObject(venta, "total", cJSON_CreateNumber(nuevoTotal));
            printf("Total actualizado para venta ID %d: %d\n", cJSON_GetObjectItem(venta, "venta_id")->valueint, nuevoTotal);
        }
    }

    // Guarda el array actualizado de nuevo en el archivo
    archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL)
    {
        printf("No se pudo abrir el archivo para escribir.\n");
        cJSON_Delete(ventasArray);
        return;
    }

    char *jsonString = cJSON_Print(ventasArray);
    fprintf(archivo, "%s", jsonString);
    fclose(archivo);
    cJSON_free(jsonString);
    cJSON_Delete(ventasArray);

    printf("Datos completados y actualizados correctamente en el archivo JSON.\n");
}

// Función para calcular la media de los valores de un campo específico en un array JSON
double calcularMedia(cJSON *ventasArray, const char *campo)
{
    int totalItems = cJSON_GetArraySize(ventasArray);
    double suma = 0.0;
    int contador = 0;

    for (int i = 0; i < totalItems; i++)
    {
        cJSON *venta = cJSON_GetArrayItem(ventasArray, i);
        cJSON *item = cJSON_GetObjectItem(venta, campo);
        if (item != NULL && item->valuedouble > 0)
        { // Ignora los valores 0
            suma += item->valuedouble;
            contador++;
        }
    }

    return (contador > 0) ? suma / contador : 0.0;
}

// Función para calcular la mediana de los valores de un campo específico en un array JSON
double calcularMediana(cJSON *ventasArray, const char *campo)
{
    int totalItems = cJSON_GetArraySize(ventasArray);
    double *valores = (double *)malloc(totalItems * sizeof(double));
    int contador = 0;

    for (int i = 0; i < totalItems; i++)
    {
        cJSON *venta = cJSON_GetArrayItem(ventasArray, i);
        cJSON *item = cJSON_GetObjectItem(venta, campo);
        if (item != NULL && item->valuedouble > 0)
        { // Ignora los valores 0
            valores[contador++] = item->valuedouble;
        }
    }

    if (contador == 0)
    {
        free(valores);
        return 0; // No hay valores válidos para calcular la mediana
    }

    // Ordenar el array de valores
    for (int i = 0; i < contador - 1; i++)
    {
        for (int j = 0; j < contador - i - 1; j++)
        {
            if (valores[j] > valores[j + 1])
            {
                double temp = valores[j];
                valores[j] = valores[j + 1];
                valores[j + 1] = temp;
            }
        }
    }

    double mediana;
    if (contador % 2 == 0)
    {
        // Si hay un número par de elementos, la mediana es el promedio de los dos del medio
        mediana = (valores[contador / 2 - 1] + valores[contador / 2]) / 2;
    }
    else
    {
        // Si hay un número impar de elementos, la mediana es el valor del medio
        mediana = valores[contador / 2];
    }

    free(valores);
    return mediana;
}