#include "importacionDatos/importacionDatos.h"
#include "procesamientoDatos/procesamientoDatos.h"
#include "analisisDatos/analisisDatos.h"
#include "estadisticas/estadisticas.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>

// Función para mostrar el banner de bienvenida
void mostrarBanner()
{
    printf("\n");
    printf("                   ########  #### ######## ##    ## ##     ## ######## ##    ## #### ########   #######    \n");
    printf("                   ##     ##  ##  ##       ###   ## ##     ## ##       ###   ##  ##  ##     ## ##     ##   \n");
    printf("                   ##     ##  ##  ##       ####  ## ##     ## ##       ####  ##  ##  ##     ## ##     ##   \n");
    printf("                   ########   ##  ######   ## ## ## ##     ## ######   ## ## ##  ##  ##     ## ##     ##   \n");
    printf("                   ##     ##  ##  ##       ##  ####  ##   ##  ##       ##  ####  ##  ##     ## ##     ##   \n");
    printf("                   ##     ##  ##  ##       ##   ###   ## ##   ##       ##   ###  ##  ##     ## ##     ##   \n");
    printf("                   ########  #### ######## ##    ##    ###    ######## ##    ## #### ########   #######    \n");
}

// Función para mostrar el menú principal y capturar la opción seleccionada
int menuPrincipal()
{
    int opcion;
    printf("\nMenú Principal\n");
    printf("1. Importar Datos\n");
    printf("2. Procesar Datos\n");
    printf("3. Analizar Datos\n");
    printf("4. Ver Estadísticas\n");
    printf("0. Salir\n");
    printf("Seleccione una opción: ");
    scanf("%d", &opcion);
    return opcion;
}

// // Función para manejar las opciones seleccionadas en el menú
// bool subMenu(int opcion, importacionDatos *importacion, procesamientoDatos *procesamiento, analisisDatos *analisis, estadisticas *stats)
// {
//     switch (opcion)
//     {
//     case 1:
//         importarDatos(importacion);
//         break;
//     case 2:
//         procesarDatos(procesamiento);
//         break;
//     case 3:
//         analizarDatos(analisis);
//         break;
//     case 4:
//         mostrarEstadisticas(stats);
//         break;
//     case 0:
//         return false; // Terminar el programa
//     default:
//         printf("Opción inválida\n");
//         break;
//     }
//     return true;
// }

// Función principal
int main()
{
    setlocale(LC_ALL, "spanish"); // Para manejar correctamente los caracteres especiales
    mostrarBanner();

    // // Instancias de las estructuras para cada funcionalidad
    // importacionDatos importacion;
    // procesamientoDatos procesamiento;
    // analisisDatos analisis;
    // estadisticas stats;

    // bool continuar = true;
    // while (continuar)
    // {
    //     int opcion = menuPrincipal();
    //     continuar = subMenu(opcion, &importacion, &procesamiento, &analisis, &stats);
    // }

    printf("Gracias por usar el programa. ¡Hasta luego!\n");

    return 0;
}
