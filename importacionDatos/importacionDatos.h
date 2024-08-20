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

// Declaración de las funciones de validacion
bool archivoExisteYNoVacio(const char *nombreArchivo);
bool validarNombreYCategoria(const char *entrada);
bool validarNumeroPositivo(int numero);
bool validarPrecioUnitario(double precio);

// Declaración de las demás funciones
void importarDatosDesdeArchivo(importacionDatos *datos, const char *rutaArchivo);
void opcionAgregarVenta(); // Función para insertar datos manualmente
void agregarVenta(importacionDatos venta);
void borrarContenidoVentasJson();
void solicitarFecha(importacionDatos *venta);

#endif // IMPORTACION_DATOS_H
