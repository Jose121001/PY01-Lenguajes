#include "../importacionDatos/importacionDatos.h"

#include "../cJSON/cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // Esto habilita el tipo bool en C
#include <string.h>  // Para strlen, strtok, strdup
#include <ctype.h>   // Para isdigit

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
        printf("\nVenta ID: %d, Fecha: %s, Producto ID: %d, Producto: %s, Categoria: %s, Cantidad: %d, Precio Unitario: %.2f, Total: %.2f\n\n",
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
#include <string.h>

// Funcion que me permite agregar los datos solicitados

// Función que me permite agregar los datos solicitados
void opcionAgregarVenta()
{
    importacionDatos venta;
    char bufferTemporal[1000]; // Buffer temporal para capturar la entrada del usuario

    // Solicita el identificador de la venta
    printf("Ingrese el identificador de la venta: ");
    scanf("%d", &venta.idVenta);

    // Solicita la fecha con validación
    solicitarFecha(&venta);

    // Solicita el identificador del producto
    printf("Ingrese el identificador del producto: ");
    scanf("%d", &venta.idProducto);

    // Solicita la descripción del producto con validación
    do
    {
        printf("Ingrese la descripcion del producto (solo letras y espacios): ");
        scanf(" %[^\n]", bufferTemporal); // Leer la entrada en el buffer temporal
        if (!validarNombreYCategoria(bufferTemporal))
        {
            printf("Error: La descripcion solo puede contener letras y espacios.\n");
        }
    } while (!validarNombreYCategoria(bufferTemporal));
    venta.descripcionProducto = (char *)malloc((strlen(bufferTemporal) + 1) * sizeof(char)); // Asigna memoria dinamica según la longitud de la entrda
    strcpy(venta.descripcionProducto, bufferTemporal);                                       // Copia el contenido del buffer temporal a la memoria asignada

    // Solicita la categoría del producto con validación
    do
    {
        printf("Ingrese la categoria del producto (solo letras y espacios): ");
        scanf(" %[^\n]", bufferTemporal); // Lee la entrada en el buffer temporal
        if (!validarNombreYCategoria(bufferTemporal))
        {
            printf("Error: La categoria solo puede contener letras y espacios.\n");
        }
    } while (!validarNombreYCategoria(bufferTemporal));
    venta.categoria = (char *)malloc((strlen(bufferTemporal) + 1) * sizeof(char)); // Asigna memoria dinámica según la longitud de la entrada
    strcpy(venta.categoria, bufferTemporal);                                       // Copia el contenido del buffer temporal a la memoria asignada

    // Solicita la cantidad (puede ser opcional) con validación
    do
    {
        printf("Ingrese la cantidad (ingrese 0 si no desea proporcionar una cantidad): ");
        if (scanf("%d", &venta.cantidad) != 1 || !validarNumeroPositivo(venta.cantidad))
        {
            printf("Error: La cantidad debe ser un numero positivo.\n");
            venta.cantidad = -1; // Establece un valor inválido temporalmente para continuar el bucle
        }
    } while (venta.cantidad < 0);

    // Solicita el precio unitario (puede ser opcional) con validación
    do
    {
        printf("Ingrese el precio unitario (ingrese 0 si no desea proporcionar una cantidad): ");
        if (scanf("%lf", &venta.precioUnitario) != 1 || !validarPrecioUnitario(venta.precioUnitario))
        {
            printf("Error: El precio unitario debe ser un numero positivo.\n");
            venta.precioUnitario = -1.0; // Establecer un valor inválido temporalmente para continuar el bucle
        }
    } while (venta.precioUnitario < 0.0);

    // Calcular el total de línea
    venta.totalLinea = venta.cantidad * venta.precioUnitario;

    // Llama a la función para agregar la venta
    agregarVenta(venta);

    // Liberacion de memoria
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
/////////////////////////////////////////Validaciones de entrada////////////////////////////////////////

// Función para validar la fecha en formato yyyy-mm-dd
bool validarFecha(const char *fecha)
{
    // Verifica la longitud exacta de la cadena
    if (strlen(fecha) != 10)
    {
        return false;
    }

    // Verifica que los caracteres estén en el formato correcto
    for (int i = 0; i < 10; i++)
    {
        if (i == 4 || i == 7)
        {
            if (fecha[i] != '-') // Las posiciones 4 y 7 deben ser guiones
            {
                return false;
            }
        }
        else
        {
            if (!isdigit(fecha[i])) // El resto deben ser números
            {
                return false;
            }
        }
    }

    // Validación adicional para asegurar que los valores no sean exagerados
    int anio = atoi(strtok(strdup(fecha), "-"));
    int mes = atoi(strtok(NULL, "-"));
    int dia = atoi(strtok(NULL, "-"));

    if (anio < 1900 || anio > 2100)
        return false; // Rango de años válido
    if (mes < 1 || mes > 12)
        return false; // Mes válido
    if (dia < 1 || dia > 31)
        return false; // Día válido

    return true;
}

// Función para solicitar la fecha y validarla
void solicitarFecha(importacionDatos *venta)
{
    bool fechaValida = false;

    while (!fechaValida)
    {
        printf("Ingrese la fecha (yyyy-mm-dd): ");
        venta->fecha = (char *)malloc(11 * sizeof(char)); // Asignar memoria para la fecha
        scanf("%s", venta->fecha);

        // Validar la fecha ingresada
        if (validarFecha(venta->fecha))
        {
            fechaValida = true;
        }
        else
        {
            printf("Fecha inválida. Asegurese de que sigue el formato yyyy-mm-dd y que los valores son validos.\n");
            free(venta->fecha); // Libera la memoria si la fecha no es válida
        }
    }
}
// Valida que la entrada de los string sean correctas
bool validarNombreYCategoria(const char *entrada)
{
    for (int i = 0; entrada[i] != '\0'; i++)
    {
        if (!isalpha(entrada[i]) && entrada[i] != ' ')
        {
            // La entrada contiene algo que no es letra ni espacio
            return false;
        }
    }
    return true;
}
// Valida que la entrada no sea negativos
bool validarNumeroPositivo(int numero)
{
    return numero >= 0; // Solo números no negativos
}
// Valida que la entrada no sea negativos
bool validarPrecioUnitario(double precio)
{
    return precio >= 0.0; // Solo números no negativos
}
