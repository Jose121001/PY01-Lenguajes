#include "importacionDatos/importacionDatos.h"
#include "cJSON/cJSON.h"
#include <stdio.h>
#include <stdlib.h>

// Funcion para importar datos desde un JSON
void importacionDatosDesdeArchivo(importacionDatos *datos, const char *rutaArchivo)
{
    FILE *archivo = fopen(rutaArchivo, "r"); // Abre el archivo de ruta para read
    if (archivo == NULL)
    {
        printf("No se pudo abrir el archivo en la ruta: %s\n", rutaArchivo);
        return;
    }
    // Leer contenido del archivo
    fseek(archivo, 0, SEEK_END);                    // Mueve el puntero de archivo al final para determinar su su size de archivo
    long fileSize = ftell(archivo);                 // Optiene la posicion actual del puntero
    fseek(archivo, 0, SEEK_SET);                    // Mueve el puntero de archivo de vuelta al inicio.
    char *contenido = (char *)malloc(fileSize + 1); // Reservamos memoria dinamica para almacenar archivo
    fread(contenido, 1, fileSize, archivo);         // Lee el contenido y lo almacena
    contenido[fileSize] = '\0';                     // Agrega simbolo nulo al final de la cadena
    fclose(archivo);                                // cIERRA EL ARCHIVO

    // Parseamos JSON
    cJSON *ventasArray = cJSON_Parse(contenido); // La convertimos a un array
    if (ventasArray == NULL)
    {
        printf("Error al parsear el archivo JSON.\n");
        free(contenido);
        return;
    }
    // Vamos obteniendo cada venta
    int totalVentas = cJSON_GetArraySize(ventasArray);
    for (int i = 0; i < totalVentas; i++)
    {
        cJSON *venta = cJSON_GetArrayItem(ventasArray, i);
        // Aquí procesarías cada venta y la almacenarías en la estructura correspondiente
        int venta_id = cJSON_GetObjectItem(venta, "venta_id")->valueint;
        const char *fecha = cJSON_GetObjectItem(venta, "fecha")->valuestring;
        int producto_id = cJSON_GetObjectItem(venta, "producto_id")->valueint;
        const char *producto_nombre = cJSON_GetObjectItem(venta, "producto_nombre")->valuestring;
        const char *categoria = cJSON_GetObjectItem(venta, "categoria")->valuestring;
        int cantidad = cJSON_GetObjectItem(venta, "cantidad")->valueint;
        double precio_unitario = cJSON_GetObjectItem(venta, "precio_unitario")->valuedouble;
        double total = cJSON_GetObjectItem(venta, "total")->valuedouble;

        // Muestra los datos de la venta para verificar
        printf("Venta ID: %d, Fecha: %s, Producto: %s, Categoría: %s, Cantidad: %d, Precio Unitario: %.2f, Total: %.2f\n",
               venta_id, fecha, producto_nombre, categoria, cantidad, precio_unitario, total);
    }

    // Liberar memoria
    cJSON_Delete(ventasArray);
    free(contenido);
}
