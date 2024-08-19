#ifndef IMPORTACION_DATOS_H
#define IMPORTACION_DATOS_H

#include <stdbool.h>

// Estructura para manejar la importación de datos
typedef struct
{
    int idVenta;
    char *fecha;
    int idProducto;
    char *descripcionProducto;
    char *categoria;
    int cantidad;
    double precioUnitario;
    double totalLinea;
} importacionDatos;

// Declaración de funciones
void importarDatosDesdeArchivo(importacionDatos *datos, const char *rutaArchivo);
void opcionAgregarVenta(); // Función para insertar datos manualmente
bool archivoExisteYNoVacio(const char *nombreArchivo);
void agregarVenta(importacionDatos venta);
void borrarContenidoVentasJson();

#endif // IMPORTACION_DATOS_H
