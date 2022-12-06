#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "inventario.h"
#include "cajas.h"
#include "pokemon.h"
#include "hash.h"

// ------------------ CONSTANTES ------------------ //

// ------------------ ESTRUCTURAS ------------------ //

struct _inventario_t
{
        hash_t *inventario_hash;
};

struct _pokemon_en_caja
{
        int cajas_contedoras;
        char pokemon_nombre[50];
};

// ------------------ DEFINICION DE FUNCIONES PRIVADAS ------------------ //

// ------------------ DECLARACION DE FUNCIONES PRIVADAS ------------------ //

bool por_cada_caja(const char *clave, void *valor, void *aux)
{
        caja_t *caja = (caja_t *)valor;

        int i = 0;
        bool pokemon_esta_caja = false;

        struct _pokemon_en_caja *pokemon_en_caja =
            (struct _pokemon_en_caja *)aux;
        while (i < caja_cantidad(caja) && !pokemon_esta_caja) {
                pokemon_t *pokemon = caja_obtener_pokemon(caja, i);
                if (strcmp(pokemon_nombre(pokemon),
                           pokemon_en_caja->pokemon_nombre) == 0) {
                        pokemon_esta_caja = true;
                        (pokemon_en_caja->cajas_contedoras)++;
                        printf("Caja %s, contiene al pokemon %s.\n", clave,
                               pokemon_en_caja->pokemon_nombre);
                }
                i++;
        }

        return true;
}

bool destruir(const char *clave, void *valor, void *aux)
{
        caja_destruir((caja_t *)valor);
        return true;
}

// ------------------ DECLARACION DE FUNCIONES PUBLICAS ------------------ //

inventario_t *inventario_crear(size_t capacidad)
{
        inventario_t *inventario = malloc(sizeof(inventario_t));
        if (!inventario)
                return NULL;

        inventario->inventario_hash = hash_crear(capacidad);

        return inventario;
}

inventario_t *inventario_cargar_caja(inventario_t *inventario,
                                     const char *caja_nombre, caja_t *caja)
{
        if (!caja_nombre || !caja)
                return NULL;

        if (hash_contiene(inventario->inventario_hash, caja_nombre))
                return NULL;

        hash_insertar(inventario->inventario_hash, caja_nombre, caja, NULL);

        return inventario;
}

size_t inventario_cantidad_cajas(inventario_t *inventario)
{
        if (!inventario)
                return 0;

        return hash_cantidad(inventario->inventario_hash);
}

caja_t *inventario_combinar_cajas(inventario_t *inventario,
                                  const char *caja1_nombre,
                                  const char *caja2_nombre,
                                  const char *caja_nombre_combinada)
{
        if (!inventario)
                return NULL;

        if (!caja1_nombre || !caja2_nombre || !caja_nombre_combinada)
                return NULL;

        if ((strcmp(caja1_nombre, caja2_nombre) == 0) ||
            (strcmp(caja1_nombre, caja_nombre_combinada) == 0) ||
            (strcmp(caja_nombre_combinada, caja2_nombre) == 0))
                return NULL;

        if (!hash_contiene(inventario->inventario_hash, caja1_nombre) ||
            !hash_contiene(inventario->inventario_hash, caja2_nombre) ||
            hash_contiene(inventario->inventario_hash, caja_nombre_combinada))
                return NULL;

        caja_t *caja_combinada = caja_combinar(
            hash_obtener(inventario->inventario_hash, caja1_nombre),
            hash_obtener(inventario->inventario_hash, caja2_nombre));

        if (!caja_combinada)
                return NULL;

        hash_insertar(inventario->inventario_hash, caja_nombre_combinada,
                      caja_combinada, NULL);

        return caja_combinada;
}

void inventario_con_cada_caja(inventario_t *inventario,
                              bool (*f)(const char *nombre_caja,
                                        void *caja, void *aux),
                              void *aux)
{
        if (!inventario)
                return;

        hash_con_cada_clave(inventario->inventario_hash, f, aux);
}

bool inventario_contiene_caja(inventario_t *inventario,
                              const char *caja_nombre)
{
        return hash_contiene(inventario->inventario_hash, caja_nombre);
}

void inventario_recorrer_caja(inventario_t *inventario,
                              const char *caja_nombre,
                              void (*funcion)(pokemon_t *))
{
        if (!inventario || !caja_nombre || !funcion)
                return;

        caja_t *caja = hash_obtener(inventario->inventario_hash, caja_nombre);
        if (!caja)
                return;

        caja_recorrer(caja, funcion);
}

int inventario_buscar_pokemon(inventario_t *inventario,
                              const char *pokemon_nombre)
{
        if (!inventario || !pokemon_nombre)
                return 0;

        struct _pokemon_en_caja pokemon_en_caja;
        strcpy(pokemon_en_caja.pokemon_nombre, pokemon_nombre);
        pokemon_en_caja.cajas_contedoras = 0;

        hash_con_cada_clave(inventario->inventario_hash, por_cada_caja, &pokemon_en_caja);

        return pokemon_en_caja.cajas_contedoras;
}

void inventario_destruir(inventario_t *inventario)
{
        if (!inventario)
                return;

        hash_con_cada_clave(inventario->inventario_hash, destruir, NULL);
        hash_destruir(inventario->inventario_hash);
        free(inventario);
}