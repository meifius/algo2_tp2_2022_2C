#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "cajas.h"
#include "pokemon.h"
#include "abb.h"

// ------------------ CONSTANTES ------------------ //

#define MAX_LINEA 60
#define POKEMONES_CAPACIDAD_INICIAL 10
#define FACTOR_CAPACIDAD_POKEMONES 2
#define POKEMONES_CANTIDAD_INICIAL 0
#define POKEMONES_GUARDADOS_INICIALMENTE 0

// ------------------ ESTRUCTURAS ------------------ //

struct _caja_t
{
        pokemon_t *abb_t;
        int capacidad_pokemones;
        int cantidad_pokemones;
};

// ------------------ DEFINICIONES DE FUNCIONES ------------------ //

// Carga/guarda en la caja un pokemon. Deben recibir tanto una caja como un pokemon valido.
int caja_cargar_pokemon(struct _caja_t *caja, pokemon_t *pokemon);

// Destruye todos lo pokemons de la caja. Debe de recibir una caja valida.
// Devuelve 1 en caso de poder cargar/guardar al pokemon. En caso fallido -1.
void caja_destruir_pokemones(caja_t *caja);

// Recibe una caja y una funcion criterio valida. La caja se ordenara segun la funcion criterio.
// Se ordena usando el algoritmo de burbujeo.
void pokemones_ordenar(caja_t *caja, bool (*criterio)(pokemon_t *, pokemon_t *));

// Funcion  que indica si el pokemon_actual es mayor alfabeticamente que el pokemon_siguiente.
// Recibe pokemones validos
bool es_pokemon_actual_mayor_alfabeticamente(pokemon_t *pokemon_actual, pokemon_t *pokemon_siguiente);

// Carga pokemones en la caja. Se requiere archivo y vaja valido.
void caja_cargar_pokemones_desde_archivo(caja_t *caja, FILE *archivo);

// Debe de recibir un string y un pokemon valido.
// Convierte la estructura pokemon a formato csv: nombre;nivel;ataque;defensa
void pokemon_convertir_a_string(char *string, pokemon_t *pokemon);

// ------------------ DECLARACIONES DE FUNCIONES ------------------ //

int caja_cargar_pokemon(struct _caja_t *caja, pokemon_t *pokemon)
{
        int indicador_carga = 0;
        if (!caja)
        {
                return indicador_carga;
        }

        if (caja->capacidad_pokemones == caja->cantidad_pokemones)
        {
                pokemon_t **pokemones = realloc(
                    caja->pokemones,
                    sizeof(pokemon_t *) * FACTOR_CAPACIDAD_POKEMONES * (unsigned long)(caja->capacidad_pokemones));

                if (!pokemones)
                        return indicador_carga;

                caja->pokemones = pokemones;
                caja->capacidad_pokemones = FACTOR_CAPACIDAD_POKEMONES * caja->capacidad_pokemones;
        }

        caja->pokemones[caja->cantidad_pokemones] = pokemon;
        caja->cantidad_pokemones++;

        return indicador_carga;
}

void caja_destruir_pokemones(caja_t *caja)
{
        for (int i = 0; i < caja->cantidad_pokemones; i++)
        {
                pokemon_destruir(caja->pokemones[i]);
        }
}

void pokemones_ordenar(caja_t *caja_pokemon, bool (*criterio)(pokemon_t *, pokemon_t *))
{
        for (int i = 0; i < caja_pokemon->cantidad_pokemones; i++)
        {
                for (int j = 0; j < caja_pokemon->cantidad_pokemones - 1; j++)
                {
                        if (criterio(caja_pokemon->pokemones[j], caja_pokemon->pokemones[j + 1]))
                        {
                                pokemon_t *pokemon_auxiliar = caja_pokemon->pokemones[j];
                                caja_pokemon->pokemones[j] = caja_pokemon->pokemones[j + 1];
                                caja_pokemon->pokemones[j + 1] = pokemon_auxiliar;
                        }
                }
        }
}

bool es_pokemon_actual_mayor_alfabeticamente(pokemon_t *pokemon_actual, pokemon_t *pokemon_siguiente)
{
        return strcmp(pokemon_nombre(pokemon_actual), pokemon_nombre(pokemon_siguiente)) > 0 ? true : false;
}

void caja_cargar_pokemones_desde_archivo(caja_t *caja_pokemon, FILE *archivo)
{
        char linea_pokemon_archivo[MAX_LINEA];
        while (fgets(linea_pokemon_archivo, MAX_LINEA, archivo) != NULL)
        {
                pokemon_t *pokemon = pokemon_crear_desde_string(linea_pokemon_archivo);
                caja_cargar_pokemon(caja_pokemon, pokemon);
        }
}

void pokemon_convertir_a_string(char *string, pokemon_t *pokemon)
{
        sprintf(string, "%s;%i;%i;%i",
                pokemon_nombre(pokemon),
                pokemon_nivel(pokemon),
                pokemon_ataque(pokemon),
                pokemon_defensa(pokemon));
}

// FUNCIONES PUBLICAS

caja_t *caja_cargar_archivo(const char *nombre_archivo)
{
        if (!nombre_archivo)
                return NULL;

        FILE *archivo = fopen(nombre_archivo, "r");
        if (!archivo)
                return NULL;

        caja_t *caja_pokemon = malloc(sizeof(caja_t));
        if (!caja_pokemon)
        {
                fclose(archivo);
                return NULL;
        }

        pokemon_t **pokemones = malloc(POKEMONES_CAPACIDAD_INICIAL * sizeof(pokemon_t *));
        if (!pokemones)
        {
                free(caja_pokemon);
                fclose(archivo);
                return NULL;
        }

        caja_pokemon->pokemones = pokemones;
        caja_pokemon->cantidad_pokemones = POKEMONES_CANTIDAD_INICIAL;
        caja_pokemon->capacidad_pokemones = POKEMONES_CAPACIDAD_INICIAL;

        caja_cargar_pokemones_desde_archivo(caja_pokemon, archivo);
        pokemones_ordenar(caja_pokemon, es_pokemon_actual_mayor_alfabeticamente);

        fclose(archivo);
        return caja_pokemon;
}

int caja_guardar_archivo(caja_t *caja, const char *nombre_archivo)
{
        if (!caja)
                return POKEMONES_GUARDADOS_INICIALMENTE;

        if (!nombre_archivo)
                return POKEMONES_GUARDADOS_INICIALMENTE;

        int pokemones_guardados = POKEMONES_GUARDADOS_INICIALMENTE;

        FILE *archivo = fopen(nombre_archivo, "w+");
        if (!archivo)
                return POKEMONES_GUARDADOS_INICIALMENTE;

        for (int i = 0; i < caja->cantidad_pokemones; i++)
        {
                fprintf(archivo, "%s;%i;%i;%i\n",
                        pokemon_nombre(caja->pokemones[i]),
                        pokemon_nivel(caja->pokemones[i]),
                        pokemon_ataque(caja->pokemones[i]),
                        pokemon_defensa(caja->pokemones[i]));

                pokemones_guardados++;
        }

        fclose(archivo);
        return pokemones_guardados;
}

caja_t *caja_combinar(caja_t *caja1, caja_t *caja2)
{
        if (!caja1 || !caja2)
                return NULL;

        caja_t *caja_combinada = malloc(sizeof(caja_t));
        if (!caja_combinada)
                return NULL;

        pokemon_t **pokemones = malloc(POKEMONES_CAPACIDAD_INICIAL * sizeof(pokemon_t *));
        if (!pokemones)
                return NULL;

        caja_combinada->capacidad_pokemones = POKEMONES_CAPACIDAD_INICIAL;
        caja_combinada->cantidad_pokemones = 0;
        caja_combinada->pokemones = pokemones;

        int i = 0;
        int j = 0;
        char pokemon_string[MAX_LINEA];

        while ((i < caja1->cantidad_pokemones) && (j < caja2->cantidad_pokemones))
        {
                if (es_pokemon_actual_mayor_alfabeticamente(caja2->pokemones[j], caja1->pokemones[i]))
                {
                        pokemon_convertir_a_string(pokemon_string, caja1->pokemones[i]);
                        caja_cargar_pokemon(caja_combinada, pokemon_crear_desde_string(pokemon_string));
                        i++;
                }
                else
                {
                        pokemon_convertir_a_string(pokemon_string, caja2->pokemones[j]);
                        caja_cargar_pokemon(caja_combinada, pokemon_crear_desde_string(pokemon_string));
                        j++;
                }
        }

        while (i < caja1->cantidad_pokemones)
        {
                pokemon_convertir_a_string(pokemon_string, caja1->pokemones[i]);
                caja_cargar_pokemon(caja_combinada, pokemon_crear_desde_string(pokemon_string));
                i++;
        }

        while (j < caja2->cantidad_pokemones)
        {
                pokemon_convertir_a_string(pokemon_string, caja2->pokemones[j]);
                caja_cargar_pokemon(caja_combinada, pokemon_crear_desde_string(pokemon_string));
                j++;
        }

        return caja_combinada;
}

int caja_cantidad(caja_t *caja)
{
        if (!caja)
                return POKEMONES_GUARDADOS_INICIALMENTE;

        return caja->cantidad_pokemones;
}

pokemon_t *caja_obtener_pokemon(caja_t *caja, int n)
{
        if (!caja)
                return NULL;

        if ((n >= caja->cantidad_pokemones) || (n < 0))
                return NULL;

        return caja->pokemones[n];
}

int caja_recorrer(caja_t *caja, void (*funcion)(pokemon_t *))
{
        int recorridos = 0;

        if (!caja)
                return recorridos;

        if (!funcion)
                return recorridos;

        for (int i = 0; i < caja->cantidad_pokemones; i++)
        {
                funcion(caja->pokemones[recorridos]);
                recorridos++;
        }

        return recorridos;
}

void caja_destruir(caja_t *caja)
{
        if (!caja)
                return;

        caja_destruir_pokemones(caja);
        free(caja->pokemones);
        free(caja);
}