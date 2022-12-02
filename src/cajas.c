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
        abb_t *pokemones;
};

struct _pokemon_archivo_t
{
        FILE *archivo;
        int pokemones_guardados;
};

struct _pokemon_enesimo_t
{
        pokemon_t *pokemon_n;
        int n_actual;
        int n_destino;
};

struct _pokemon_recorrer
{
        void (*funcion)(pokemon_t *);
};

// ------------------ DEFINICIONES DE FUNCIONES ------------------ //

// Funcion  que indica si el pokemon_actual es menor alfabeticamente que el
// pokemon_siguiente.
// Recibe pokemones validos
// Devuelve un entero. Sera igual a 0 si son iguales. Menor a 0 si es menor,
// mayor a 0 si es mayor
int pokemon_actual_menor_alfabeticamente(void *pokemon_actual,
                                         void *pokemon_otro);

// Carga pokemones en la caja. Se requiere archivo y vaja valido.
void caja_cargar_pokemones_desde_archivo(caja_t *caja, FILE *archivo);

// Carga/guarda en la caja un pokemon. Deben recibir tanto una caja como un pokemon valido.
// Devuelve la caja. En caso de que la caja no sea valdia, devuelve NULL.
struct _caja_t *caja_cargar_pokemon(struct _caja_t *caja, pokemon_t *pokemon);

// Funcion auxiliar para guardar pokemon. Recibe cada pokemon y una estructura que contiene
// una variable al archivo y otra para contabilizar lo que se pudieron guardar satisfactoriamente.
bool pokemon_guardar_archivo(void *pokemon, void *pokemones_guardados);

// Funcion  que indica si el pokemon_actual es mayor alfabeticamente que el pokemon_siguiente.
// Recibe pokemones validos
bool es_pokemon_actual_mayor_alfabeticamente(pokemon_t *pokemon_actual, pokemon_t *pokemon_siguiente);

//
bool enesimo_elemento(void *pokemon, void *pokemon_enesimo);

//
void destruir_pokemon_abb(void *pokemon);

//
bool insertar_en_abb(void *pokemon, void *pokemones);

// Debe de recibir un string y un pokemon valido.
// Convierte la estructura pokemon a formato csv: nombre;nivel;ataque;defensa
void pokemon_convertir_a_string(char *string, pokemon_t *pokemon);

// // Destruye todos lo pokemons de la caja. Debe de recibir una caja valida.
// // Devuelve 1 en caso de poder cargar/guardar al pokemon. En caso fallido -1.
// void caja_destruir_pokemones(caja_t *caja);

// // Recibe una caja y una funcion criterio valida. La caja se ordenara segun la funcion criterio.
// // Se ordena usando el algoritmo de burbujeo.
// void pokemones_ordenar(caja_t *caja, bool (*criterio)(pokemon_t *, pokemon_t *));

// // Funcion  que indica si el pokemon_actual es mayor alfabeticamente que el pokemon_siguiente.
// // Recibe pokemones validos
// bool es_pokemon_actual_mayor_alfabeticamente(pokemon_t *pokemon_actual, pokemon_t *pokemon_siguiente);

// // Debe de recibir un string y un pokemon valido.
// // Convierte la estructura pokemon a formato csv: nombre;nivel;ataque;defensa
// void pokemon_convertir_a_string(char *string, pokemon_t *pokemon);

// ------------------ DECLARACIONES DE FUNCIONES ------------------ //

int pokemon_actual_menor_alfabeticamente(void *pokemon_actual, void *pokemon_otro)
{
        pokemon_t *pokemon_actual_t = (pokemon_t *)pokemon_actual;
        pokemon_t *pokemon_otro_t = (pokemon_t *)pokemon_otro;
        return strcmp(pokemon_nombre(pokemon_actual_t), pokemon_nombre(pokemon_otro_t));
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

struct _caja_t *caja_cargar_pokemon(struct _caja_t *caja, pokemon_t *pokemon)
{
        if (!caja)
                return NULL;

        caja->pokemones = abb_insertar(caja->pokemones, pokemon);

        return caja;
}

bool pokemon_guardar_archivo(void *pokemon, void *pokemon_archivo)
{
        pokemon_t *pokemon_item = (pokemon_t *)pokemon;
        struct _pokemon_archivo_t *pokemon_archivo_item = (struct _pokemon_archivo_t *)pokemon_archivo;

        fprintf(pokemon_archivo_item->archivo, "%s;%i;%i;%i\n",
                pokemon_nombre(pokemon_item),
                pokemon_nivel(pokemon_item),
                pokemon_ataque(pokemon_item),
                pokemon_defensa(pokemon_item));

        (pokemon_archivo_item->pokemones_guardados)++;

        return true;
}

bool es_pokemon_actual_mayor_alfabeticamente(pokemon_t *pokemon_actual, pokemon_t *pokemon_siguiente)
{
        return strcmp(pokemon_nombre(pokemon_actual), pokemon_nombre(pokemon_siguiente)) > 0 ? true : false;
}

bool enesimo_elemento(void *pokemon, void *pokemon_enesimo)
{
        pokemon_t *pokemon_item = (pokemon_t *)pokemon;
        struct _pokemon_enesimo_t *pokemon_enesimo_t = (struct _pokemon_enesimo_t *)pokemon_enesimo;

        if (pokemon_enesimo_t->n_actual == pokemon_enesimo_t->n_destino)
        {
                pokemon_enesimo_t->pokemon_n = pokemon_item;
                return false;
        }
        (pokemon_enesimo_t->n_actual)++;
        return true;
}

void destruir_pokemon_abb(void *pokemon)
{
        pokemon_destruir((pokemon_t *)pokemon);
}

void pokemon_convertir_a_string(char *string, pokemon_t *pokemon)
{
        sprintf(string, "%s;%i;%i;%i",
                pokemon_nombre(pokemon),
                pokemon_nivel(pokemon),
                pokemon_ataque(pokemon),
                pokemon_defensa(pokemon));
}

bool insertar_en_abb(void *pokemon, void *pokemones)
{
        pokemon_t *pokemon_item = (pokemon_t *)pokemon;
        abb_t *pokemones_abb = (abb_t *)pokemones;

        char pokemon_string[MAX_LINEA];
        pokemon_convertir_a_string(pokemon_string, pokemon_item);

        abb_insertar(pokemones_abb, pokemon_crear_desde_string(pokemon_string));

        return true;
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

        abb_t *pokemones = abb_crear(pokemon_actual_menor_alfabeticamente);
        if (!pokemones)
        {
                free(caja_pokemon);
                fclose(archivo);
                return NULL;
        }

        caja_pokemon->pokemones = pokemones;

        caja_cargar_pokemones_desde_archivo(caja_pokemon, archivo);

        fclose(archivo);
        return caja_pokemon;
}

int caja_guardar_archivo(caja_t *caja, const char *nombre_archivo)
{
        if (!caja)
                return POKEMONES_GUARDADOS_INICIALMENTE;

        if (!nombre_archivo)
                return POKEMONES_GUARDADOS_INICIALMENTE;

        FILE *archivo = fopen(nombre_archivo, "w+");
        if (!archivo)
                return POKEMONES_GUARDADOS_INICIALMENTE;

        struct _pokemon_archivo_t pokemon_archivo;
        pokemon_archivo.archivo = archivo;
        pokemon_archivo.pokemones_guardados = POKEMONES_GUARDADOS_INICIALMENTE;

        abb_con_cada_elemento(caja->pokemones, INORDEN, pokemon_guardar_archivo, &pokemon_archivo);

        fclose(archivo);
        return pokemon_archivo.pokemones_guardados;
}

caja_t *caja_combinar(caja_t *caja1, caja_t *caja2)
{
        if (!caja1 || !caja2)
                return NULL;

        caja_t *caja_combinada = malloc(sizeof(caja_t));
        if (!caja_combinada)
                return NULL;

        abb_t *pokemones_combinados = abb_crear(pokemon_actual_menor_alfabeticamente);
        if (!pokemones_combinados)
                return NULL;

        caja_combinada->pokemones = pokemones_combinados;

        abb_con_cada_elemento(caja1->pokemones, POSTORDEN, insertar_en_abb, caja_combinada->pokemones);
        abb_con_cada_elemento(caja2->pokemones, POSTORDEN, insertar_en_abb, caja_combinada->pokemones);

        return caja_combinada;
}

int caja_cantidad(caja_t *caja)
{
        if (!caja)
                return POKEMONES_GUARDADOS_INICIALMENTE;

        return (int)abb_tamanio(caja->pokemones);
}

pokemon_t *caja_obtener_pokemon(caja_t *caja, int n)
{
        if (!caja)
                return NULL;

        if ((n >= abb_tamanio(caja->pokemones)) || (n < 0))
                return NULL;

        struct _pokemon_enesimo_t pokemon_enesimo;
        pokemon_enesimo.n_actual = 0;
        pokemon_enesimo.n_destino = n;
        pokemon_enesimo.pokemon_n = NULL;
        abb_con_cada_elemento(caja->pokemones, INORDEN, enesimo_elemento, &pokemon_enesimo);

        return pokemon_enesimo.pokemon_n;
}

bool auxiliar(void *pokemon, void *pokemon_recorrer)
{
        pokemon_t *pokemon_item = (pokemon_t *)pokemon;
        struct _pokemon_recorrer *aux = (struct _pokemon_recorrer *)pokemon_recorrer;

        aux->funcion(pokemon_item);

        return true;
}

int caja_recorrer(caja_t *caja, void (*funcion)(pokemon_t *))
{
        int recorridos = 0;

        if (!caja)
                return recorridos;

        if (!funcion)
                return recorridos;

        struct _pokemon_recorrer aux;
        aux.funcion = funcion;
        recorridos = (int)abb_con_cada_elemento(caja->pokemones, INORDEN, auxiliar, &aux);

        return recorridos;
}

void caja_destruir(caja_t *caja)
{
        if (!caja)
                return;

        abb_destruir_todo(caja->pokemones, destruir_pokemon_abb);
        free(caja);
}