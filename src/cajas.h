#ifndef CAJAS_H_
#define CAJAS_H_

#include "pokemon.h"

typedef struct _caja_t caja_t;

/*
 * Carga el contenido de una caja desde un archivo csv (ver el formato en el
 * enunciado de TP).
 *
 * Al finaliar la carga, la caja queda inicializada con todos los pokemon del
 * archivo csv ORDENADOS ALFABETICAMENTE por nombre.
 *
 * Si el archivo no existe, es inválido o no se puede reservar memoria, se debe
 * devolver NULL.
 */
caja_t *caja_cargar_archivo(const char *nombre_archivo);

/*
 * Genera un archivo .csv con el contenido de la caja.
 *
 * Devuelve la cantidad de pokemon que se pudieron escribir correctamente o 0 en
 * caso de error.
 *
 */
int caja_guardar_archivo(caja_t *caja, const char *nombre_archivo);

/*
 * Combina el contenido de 2 cajas en una caja nueva. El contenido de caja1 y
 * caja2 no se modifica.
 *
 * La caja combinada debe contener los pokemon de ambas cajas ORDENADOS ALFABETICAMENTE por nombre.
 *
 * Devuelve la caja nueva creada o NULL en caso de error.
 */
caja_t *caja_combinar(caja_t *caja1, caja_t *caja2);

/*
 * Devuelve la cantidad de pokemon almacenados en la caja o 0 en caso de no
 * existir.
 */
int caja_cantidad(caja_t *caja);

/*
 * Obtiene el n-esimo pokemon dentro de la caja o NULL en caso de error.
 *
 */
pokemon_t *caja_obtener_pokemon(caja_t *caja, int n);

/*
 * Aplica una función a cada uno de los pokemon de la caja (en orden
 * alfabético).
 *
 * Devuelve la cantidad de pokemon a los que se les aplicó la función
 * correctamente.
 */
int caja_recorrer(caja_t *caja, void (*funcion)(pokemon_t *));

/*
 * Destruye una caja liberando toda la memoria asociada a la misma.
 */
void caja_destruir(caja_t *caja);

#endif // CAJAS_H_