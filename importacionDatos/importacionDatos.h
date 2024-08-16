#ifndef IMPORTACION_DATOS_H
#define IMPORTACION_DATOS_H

#include <stdio.h>
#include <stdlib.h>

// Definición de la estructura importacionDatos dinamica.

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

// Declaración de la función para importar datos
void importarDatos(importacionDatos *datos);

#endif // IMPORTACION_DATOS_H
