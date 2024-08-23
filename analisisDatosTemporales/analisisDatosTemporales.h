#ifndef ANALISIS_DATOS_TEMPORALES_H // Guardas de inclusión para evitar múltiples inclusiones
#define ANALISIS_DATOS_TEMPORALES_H
#include <time.h>
// DECLARACIONES
void calcularTasaCrecimiento(int trimestre, int anio);
void parseFecha(const char *fecha, struct tm *tmFecha);
void mesMayorVentaDiaActivo(const char *fechaInicio, const char *fechaFinal);
#endif // ANALISIS_DATOS_TEMPORALES_H