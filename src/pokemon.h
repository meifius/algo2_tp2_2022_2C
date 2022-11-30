#ifndef POKEMON_H_
#define POKEMON_H_

typedef struct _pokemon_t pokemon_t;

/*
 * Crea un pokemon a partir de una representación csv. Ver el enunciado para el
 * formato.
 *
 * Devuelve el pokemon creado o NULL en caso de error.
 */
pokemon_t *pokemon_crear_desde_string(char *string);

/*
 * Devuelve el nivel de un pokemon o 0 en caso de error.
 */
int pokemon_nivel(pokemon_t *pokemon);

/*
 * Devuelve el ataque de un pokemon o 0 en caso de error.
 */
int pokemon_ataque(pokemon_t *pokemon);

/*
 * Devuelve la defensa de un pokemon o 0 en caso de error.
 */
int pokemon_defensa(pokemon_t *pokemon);

/*
 * Devuelve el nombre de un pokemon o NULL en caso de error.
 */
const char *pokemon_nombre(pokemon_t *pokemon);

/*
 * Libera la memoria asociada al pokemon;
 */
void pokemon_destruir(pokemon_t *pokemon);

#endif // POKEMON_H_