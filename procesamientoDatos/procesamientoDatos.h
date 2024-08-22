#ifndef PROCESAMIENTO_DATOS_H // Guardas de inclusión para evitar múltiples inclusiones
#define PROCESAMIENTO_DATOS_H

#include "../cJSON/cJSON.h"

// Aquí van tus declaraciones y definiciones.
void borrarDuplicados();
void completarCeros();

// AUXILIARES
double calcularMedia(cJSON *ventasArray, const char *campo);
double calcularMediana(cJSON *ventasArray, const char *campo);

#endif //  PROCESAMIENTO_DATOS_H