#include "importacionDatos/importacionDatos.h"
#include "procesamientoDatos/procesamientoDatos.h"
#include "analisisDatos/analisisDatos.h"
#include "estadisticas/estadisticas.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>

// Permite limpiar consola.
#include <stdlib.h>
// Funcion para limpiar consola.
void limpiarConsola()
{
    system("cls"); // Comando para limpiar la consola en Windows
}

// COMANDO PARA EJECUTAR: cd C:\Users\joses\Desktop\PY01-Lenguajes\PY01-Lenguajes\output
// mingw32-make al correr sin problemas podemos ejecutar:
//./PY01-Lenguajes.exe

// Declaración de las funciones
void opcionAgregarVenta();                                                        // Función para insertar datos manualmente
void importarDatosDesdeArchivo(importacionDatos *datos, const char *rutaArchivo); // Función para importar desde un archivo JSON

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
    printf("\nMenu Principal\n");
    printf("1. Importar Datos\n");
    printf("2. Procesar Datos\n");
    printf("3. Analizar Datos\n");
    printf("4. Analizar Temporal\n");
    printf("5. Ver Estadisticas\n");
    printf("0. Salir\n");
    printf("Seleccione una opcion: ");
    scanf("%d", &opcion);
    return opcion;
}

// Función para mostrar el submenú de importación de datos
void subMenuImportacionDatos()
{
    int opcion;
    bool continuar = true;

    // Declaración de la variable importacion
    importacionDatos importacion;

    while (continuar)
    {

        printf("\n--- Submenu de Importacion de Datos ---\n");
        printf("1. Insertar datos manualmente\n");
        printf("2. Importar datos desde un archivo JSON\n");
        printf("3. Borrar los datos del JSON\n");
        printf("0. Volver al menu principal\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        switch (opcion)
        {
        case 1:
            printf("Llego a agregar datos");
            opcionAgregarVenta(); // Función para insertar datos manualmente

            break;
        case 2:
            printf("Llego a importardatos");
            importarDatosDesdeArchivo(&importacion, "data/ventas.json"); // Función para importar desde un archivo JSON

            break;
        case 3:
            printf("Datos borrados");
            borrarContenidoVentasJson();
            importarDatosDesdeArchivo(&importacion, "data/ventas.json"); // Función para importar desde un archivo JSON
            break;
        case 0:
            continuar = false;
            break;
        default:
            printf("Opcion no válida. Intente de nuevo.\n");
            break;
        }
    }
}
// Función principal

// Función principal para manejar el flujo del programa
int main()
{
    setlocale(LC_ALL, "Spanish_Spain.1252"); // Para sistemas Windows en español
    mostrarBanner();

    bool continuar = true;
    while (continuar)
    {

        int opcion = menuPrincipal();
        switch (opcion)
        {
        case 1:
            subMenuImportacionDatos();
            break;
        case 2:
            // subMenuProcesamientoDatos();
            break;
        case 3:
            // subMenuAnalisisDatos();
            break;
        case 4:
            // Submenú para el análisis temporal (puedes agregarlo según necesites)
            break;
        case 5:
            // subMenuEstadisticas();
            break;
        case 0:
            continuar = false;
            break;
        default:
            printf("Opción no válida. Intente de nuevo.\n");
            break;
        }
    }

    printf("Gracias por usar el programa. ¡Hasta luego!\n");

    return 0;
}
