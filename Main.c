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

// Función para validar que la entrada sea solo numérica

// Función para validar que la entrada sea solo numérica
int leerOpcionNumerica()
{
    int opcion;
    int resultado;

    while (true)
    {
        printf("Seleccione una opcion: ");
        resultado = scanf("%d", &opcion);

        // Si scanf no lee un número entero
        if (resultado != 1)
        {
            printf("Entrada invalida. Por favor ingrese un numero.\n");
            while (getchar() != '\n')
                ; // Limpia el buffer de entrada
        }
        else
        {
            return opcion; // Retorna la opción válida
        }
    }
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
    opcion = leerOpcionNumerica(); // Leer opción válida
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
        opcion = leerOpcionNumerica(); // Leer opción válida
        switch (opcion)
        {
        case 1:
            opcionAgregarVenta(); // Función para insertar datos manualmente

            break;
        case 2:
            // Función para importar desde un archivo JSON
            importarDatosDesdeArchivo(&importacion, "../data/ventas.json");
            break;
        case 3:
            borrarContenidoVentasJson();
            break;
        case 0:
            continuar = false;
            break;
        default:
            printf("Opcion no valida. Intente de nuevo.\n");
            break;
        }
    }
}

void subMenuProcesamientoDatos()
{
    int opcion;
    bool continuar = true;

    // Declaración de la variable importacion
    // importacionDatos importacion;

    while (continuar)
    {

        printf("\n--- Submenu de Procesamiento de Datos ---\n");
        printf("1. Completar datos faltantes\n");
        printf("2. Eliminar datos duplicados. \n");
        printf("0. Volver al menu principal\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        opcion = leerOpcionNumerica(); // Leer opción válida
        switch (opcion)
        {
        case 1:
            completarCeros();

            break;
        case 2:
            borrarDuplicados();

            break;
        case 0:
            continuar = false;
            break;
        default:
            printf("Opcion no valida. Intente de nuevo.\n");
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
            subMenuProcesamientoDatos();
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
            printf("Opcion no valida. Intente de nuevo.\n");
            break;
        }
    }

    printf("Gracias por usar el programa.\n");

    return 0;
}
