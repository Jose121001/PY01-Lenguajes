
#include "../analisisDatosTemporales/analisisDatosTemporales.h"

#include "../cJSON/cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // Esto habilita el tipo bool en C
#include <string.h>  // Para strlen, strtok, strdup
#include <ctype.h>   // Para isdigit
#include <time.h>    //Permite trabajar con las fechas

// Funcion que recibe una fehc ainicio y una fecha final para el calculo de mayor venta.
void mesMayorVentaDiaActivo(const char *fechaInicio, const char *fechaFinal)
{

    const char *nombreArchivo = "../data/ventas.json";
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL)
    {
        printf("No se pudo abrir el archivo en la ruta: %s\n", nombreArchivo);
        return;
    }
    // Leemos el contenido del archivo
    // Estructura brindada por chat
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

    // Convertimos las fechas de inicio y fin a estructuras de tiempo
    // Codigo brindado por chat
    struct tm tmInicio = {0}, tmFin = {0};
    parseFecha(fechaInicio, &tmInicio);
    parseFecha(fechaFinal, &tmFin);
    time_t tiempoInicio = mktime(&tmInicio);
    time_t tiempoFin = mktime(&tmFin);

    // Inicializamos un struct para almacenar los totales por mes
    typedef struct
    {
        int anio;
        int mes;
        double total;
    } TotalPorMes;
    TotalPorMes totales[12 * 100] = {0}; // Suponemos un máximo de 100 años diferentes en los datos
    int cantidadMeses = 0;
    // Inicializamos un contador para los días de la semana
    int diasSemana[7] = {0}; // 0=Domingo, 1=Lunes, ..., 6=Sábado

    int totalItems = cJSON_GetArraySize(ventasArray);
    for (int i = 0; i < totalItems; i++)
    {
        cJSON *venta = cJSON_GetArrayItem(ventasArray, i);
        cJSON *fechaItem = cJSON_GetObjectItem(venta, "fecha");
        cJSON *totalItem = cJSON_GetObjectItem(venta, "total");

        if (fechaItem != NULL && totalItem != NULL && totalItem->valuedouble > 0)
        {
            // Parsear la fecha de la venta
            struct tm tmFecha = {0};
            parseFecha(fechaItem->valuestring, &tmFecha);
            time_t tiempoVenta = mktime(&tmFecha);

            // Revisa si la fecha de la venta cae dentro del rango especificado
            if (tiempoVenta >= tiempoInicio && tiempoVenta <= tiempoFin)
            {
                int anio = tmFecha.tm_year + 1900;
                int mes = tmFecha.tm_mon + 1;
                int diaSemana = tmFecha.tm_wday; // 0=Domingo, 1=Lunes, ..., 6=Sábado

                // Contamos el día de la semana
                diasSemana[diaSemana]++;

                // Buscamos si ya tenemos ese mes registrado
                bool mesEncontrado = false;
                for (int j = 0; j < cantidadMeses; j++)
                {
                    if (totales[j].anio == anio && totales[j].mes == mes)
                    {
                        totales[j].total += totalItem->valuedouble;
                        mesEncontrado = true;
                        break;
                    }
                }

                // Si el mes no está registrado, lo agregamos
                if (!mesEncontrado)
                {
                    totales[cantidadMeses].anio = anio;
                    totales[cantidadMeses].mes = mes;
                    totales[cantidadMeses].total = totalItem->valuedouble;
                    cantidadMeses++;
                }
            }
        }
    }

    // Identifica el mes con mayor venta
    double maxVenta = 0;
    int mesMaximo = 0, anioMaximo = 0;
    for (int i = 0; i < cantidadMeses; i++)
    {
        if (totales[i].total > maxVenta)
        {
            maxVenta = totales[i].total;
            mesMaximo = totales[i].mes;
            anioMaximo = totales[i].anio;
        }
    }

    // Nos permite averiguar el día de la semana más activo
    int maxTransacciones = 0;
    int diaSemanaActivo = 0;
    for (int i = 0; i < 7; i++)
    {
        if (diasSemana[i] > maxTransacciones)
        {
            maxTransacciones = diasSemana[i];
            diaSemanaActivo = i;
        }
    }

    // OUTPUT
    printf("\nEl mes con mayor venta dentro del rango es: %d-%d con un total de %.2f\n", mesMaximo, anioMaximo, maxVenta);
    const char *dias[] = {"Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado"};
    printf("\nEl dia de la semana mas activo dentro del rango es: %s con %d transacciones\n", dias[diaSemanaActivo], maxTransacciones);

    cJSON_Delete(ventasArray);
}
// funcion para parsear la fecha
void parseFecha(const char *fecha, struct tm *tmFecha)
{
    sscanf(fecha, "%d-%d-%d", &tmFecha->tm_year, &tmFecha->tm_mon, &tmFecha->tm_mday);
    tmFecha->tm_year -= 1900; // Ajusta el año para struct tm
    tmFecha->tm_mon -= 1;     // Ajusta el mes (0-11 en struct tm)
}
// Me permite calcular el total por trismestres recibiendo un entero y el anual
void calcularTasaCrecimiento(int trimestre, int anio)
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

    // Determina los rangos de meses para el trimestre seleccionado
    int mesInicio, mesFin;
    if (trimestre == 1)
    {
        mesInicio = 1;
        mesFin = 3;
    }
    else if (trimestre == 2)
    {
        mesInicio = 4;
        mesFin = 6;
    }
    else if (trimestre == 3)
    {
        mesInicio = 7;
        mesFin = 9;
    }
    else if (trimestre == 4)
    {
        mesInicio = 10;
        mesFin = 12;
    }
    else
    {
        printf("Trimestre inválido.\n");
        cJSON_Delete(ventasArray);
        return;
    }

    // Calcula los rangos del trimestre anterior
    int anioAnterior = anio;
    int trimestreAnterior = trimestre - 1;
    if (trimestreAnterior == 0)
    {
        trimestreAnterior = 4;
        anioAnterior--;
    }

    int mesInicioAnterior, mesFinAnterior;
    if (trimestreAnterior == 1)
    {
        mesInicioAnterior = 1;
        mesFinAnterior = 3;
    }
    else if (trimestreAnterior == 2)
    {
        mesInicioAnterior = 4;
        mesFinAnterior = 6;
    }
    else if (trimestreAnterior == 3)
    {
        mesInicioAnterior = 7;
        mesFinAnterior = 9;
    }
    else if (trimestreAnterior == 4)
    {
        mesInicioAnterior = 10;
        mesFinAnterior = 12;
    }

    double ventasTrimestreActual = 0.0;
    double ventasTrimestreAnterior = 0.0;

    int totalItems = cJSON_GetArraySize(ventasArray);
    for (int i = 0; i < totalItems; i++)
    {
        cJSON *venta = cJSON_GetArrayItem(ventasArray, i);
        cJSON *fechaItem = cJSON_GetObjectItem(venta, "fecha");
        cJSON *totalItem = cJSON_GetObjectItem(venta, "total");

        if (fechaItem != NULL && totalItem != NULL)
        {
            int anioVenta, mesVenta;
            sscanf(fechaItem->valuestring, "%d-%d", &anioVenta, &mesVenta);

            //   venta pertenece al trimestre actual
            if (anioVenta == anio && mesVenta >= mesInicio && mesVenta <= mesFin)
            {
                ventasTrimestreActual += totalItem->valuedouble;
            }

            // Verificamos si la venta pertenece al trimestre anterior
            if (anioVenta == anioAnterior && mesVenta >= mesInicioAnterior && mesVenta <= mesFinAnterior)
            {
                ventasTrimestreAnterior += totalItem->valuedouble;
            }
        }
    }

    // Calcula la tasa de crecimiento o decrecimiento
    if (ventasTrimestreAnterior == 0)
    {
        printf("No hay suficientes datos del trimestre anterior para calcular la tasa de crecimiento.\n");
    }
    else
    {
        double tasaCrecimiento = ((ventasTrimestreActual - ventasTrimestreAnterior) / ventasTrimestreAnterior) * 100;
        printf("\nLa tasa de crecimiento o decrecimiento es: %.2f%%\n", tasaCrecimiento);
    }

    cJSON_Delete(ventasArray);
}
