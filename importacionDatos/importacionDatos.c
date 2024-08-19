#include "../importacionDatos/importacionDatos.h"

#include "../cJSON/cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // Esto habilita el tipo bool en C

// Nombre del archivo JSON (declarado globalmente)
const char *nombreArchivo = "../data/ventas.json";

// Funcion para importar datos desde un JSON.
void importarDatosDesdeArchivo(importacionDatos *datos, const char *rutaArchivo)
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
        // sE PROCESA cada venta y se almacena en la estructura correspondiente
        int venta_id = cJSON_GetObjectItem(venta, "venta_id")->valueint;
        const char *fecha = cJSON_GetObjectItem(venta, "fecha")->valuestring;
        int producto_id = cJSON_GetObjectItem(venta, "producto_id")->valueint;
        const char *producto_nombre = cJSON_GetObjectItem(venta, "producto_nombre")->valuestring;
        const char *categoria = cJSON_GetObjectItem(venta, "categoria")->valuestring;
        int cantidad = cJSON_GetObjectItem(venta, "cantidad")->valueint;
        double precio_unitario = cJSON_GetObjectItem(venta, "precio_unitario")->valuedouble;
        double total = cJSON_GetObjectItem(venta, "total")->valuedouble;

        // Muestra los datos de la venta para verificar
        printf("Venta ID: %d, Fecha: %s, Producto ID: %d, Producto: %s, Categoría: %s, Cantidad: %d, Precio Unitario: %.2f, Total: %.2f\n",
               venta_id, fecha, producto_id, producto_nombre, categoria, cantidad, precio_unitario, total);
    }

    // Liberar memoria
    cJSON_Delete(ventasArray);
    free(contenido);
}

// Me permite saber si el archivo existe y si esta vacio.
bool archivoExisteYNoVacio(const char *nombreArchivo)
{
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo != NULL)
    {
        fseek(archivo, 0, SEEK_END);
        long fileSize = ftell(archivo);
        fclose(archivo);
        return fileSize > 0; // Devuelve true si el archivo no está vacío
    }
    return false;
}

// Esta funcion me permite agregar valores que le enviare desde
// el submenu del submenu y su informacion se agregara al json
void agregarVenta(importacionDatos venta)
{
    FILE *archivo;
    cJSON *ventasArray;

    // Verifica si el archivo ya existe y no está vacío
    if (archivoExisteYNoVacio(nombreArchivo))
    {
        // Lee el archivo existente
        archivo = fopen(nombreArchivo, "r");
        fseek(archivo, 0, SEEK_END);
        long fileSize = ftell(archivo);
        fseek(archivo, 0, SEEK_SET);
        char *contenido = (char *)malloc(fileSize + 1);
        fread(contenido, 1, fileSize, archivo);
        contenido[fileSize] = '\0';
        fclose(archivo);

        // Parsep el contenido existente
        ventasArray = cJSON_Parse(contenido);
        free(contenido);
    }
    else
    {
        // Si no existe o está vacío, crea un nuevo array vacío
        ventasArray = cJSON_CreateArray();
    }

    // Crea un nuevo objeto JSON para la venta
    cJSON *nuevaVenta = cJSON_CreateObject();
    cJSON_AddNumberToObject(nuevaVenta, "venta_id", venta.idVenta);
    cJSON_AddStringToObject(nuevaVenta, "fecha", venta.fecha);
    cJSON_AddNumberToObject(nuevaVenta, "producto_id", venta.idProducto);
    cJSON_AddStringToObject(nuevaVenta, "producto_nombre", venta.descripcionProducto);
    cJSON_AddStringToObject(nuevaVenta, "categoria", venta.categoria);
    cJSON_AddNumberToObject(nuevaVenta, "cantidad", venta.cantidad);
    cJSON_AddNumberToObject(nuevaVenta, "precio_unitario", venta.precioUnitario);
    cJSON_AddNumberToObject(nuevaVenta, "total", venta.totalLinea);

    // Agrega la nueva venta al array existente
    cJSON_AddItemToArray(ventasArray, nuevaVenta);

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

    printf("Venta agregada correctamente.\n");
}

// Funcion que me permite agregar los datos solicitados
void opcionAgregarVenta()
{
    importacionDatos venta;

    // Solicitar el identificador de la venta
    printf("Ingrese el identificador de la venta: ");
    scanf("%d", &venta.idVenta);

    // Solicitar la fecha (formato yyyy-mm-dd)
    printf("Ingrese la fecha (yyyy-mm-dd): ");
    venta.fecha = (char *)malloc(11 * sizeof(char)); // Asignar memoria para la fecha
    scanf("%s", venta.fecha);

    // Solicitar el identificador del producto
    printf("Ingrese el identificador del producto: ");
    scanf("%d", &venta.idProducto);

    // Solicitar la descripción del producto
    printf("Ingrese la descripcion del producto: ");
    venta.descripcionProducto = (char *)malloc(200 * sizeof(char)); // Asignar memoria para la descripción
    scanf(" %[^\n]", venta.descripcionProducto);                    // Leer hasta el salto de línea

    // Solicitar la categoría del producto
    printf("Ingrese la categoria del producto: ");
    venta.categoria = (char *)malloc(100 * sizeof(char)); // Asignar memoria para la categoría
    scanf(" %[^\n]", venta.categoria);                    // Leer hasta el salto de línea

    // Solicitar la cantidad (puede ser opcional)
    printf("Ingrese la cantidad (puede dejarlo vacio presionando Enter): ");
    if (scanf("%d", &venta.cantidad) != 1)
    {
        venta.cantidad = 0; // Valor por defecto si no se ingresa nada
    }

    // Solicitar el precio unitario (puede ser opcional)
    printf("Ingrese el precio unitario (puede dejarlo vacio presionando Enter): ");
    if (scanf("%lf", &venta.precioUnitario) != 1)
    {
        venta.precioUnitario = 0.0; // Valor por defecto si no se ingresa nada
    }

    // Calcular el total de línea
    venta.totalLinea = venta.cantidad * venta.precioUnitario;

    // Llamar a la función para agregar la venta
    agregarVenta(venta);

    // Liberar la memoria asignada
    free(venta.fecha);
    free(venta.descripcionProducto);
    free(venta.categoria);
}

// Función para borrar el contenido de ventas.json
void borrarContenidoVentasJson()
{
    // Abre el archivo en modo de escritura, lo que automáticamente borra su contenido
    FILE *archivo = fopen("../data/ventas.json", "w");

    if (archivo == NULL)
    {
        printf("No se pudo abrir el archivo ventas.json para borrar su contenido.\n");
        return;
    }

    // Simplemente cerramos el archivo para que quede vacío
    fclose(archivo);

    printf("El contenido de ventas.json ha sido borrado.\n");
}
