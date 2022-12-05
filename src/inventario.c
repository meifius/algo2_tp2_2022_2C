#include <stdlib.h>
// #include <stdio.h>
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

// ------------------ DEFINICION DE FUNCIONES PRIVADAS ------------------ //

// ------------------ DECLARACION DE FUNCIONES PRIVADAS ------------------ //

// ------------------ DECLARACION DE FUNCIONES PUBLICAS ------------------ //

inventario_t *inventario_crear(size_t capacidad)
{
    inventario_t *inventario = malloc(sizeof(inventario_t));
    if (!inventario)
        return NULL;

    inventario->inventario_hash = hash_crear(capacidad);

    return inventario;
}

inventario_t *inventario_cargar_caja(inventario_t *inventario, const char *caja_nombre, caja_t *caja)
{
    if (!caja_nombre || !caja)
        return NULL;

    if (hash_contiene(inventario->inventario_hash, caja_nombre))
        return NULL;

    hash_insertar(inventario->inventario_hash, caja_nombre, caja, NULL);

    return inventario;
}

caja_t *inventario_combinar_cajas(inventario_t *inventario, const char *caja1_nombre, const char *caja2_nombre)
{
    if (!inventario || caja1_nombre || caja2_nombre)
        return NULL;

    if (strcmp(caja1_nombre, caja2_nombre) == 0)
        return NULL;

    if (!hash_contiene(inventario->inventario_hash, caja1_nombre) || !hash_contiene(inventario->inventario_hash, caja2_nombre))
        return NULL;

    caja_t *caja_combinada = caja_combinar(hash_obtener(inventario->inventario_hash, caja1_nombre), hash_obtener(inventario->inventario_hash, caja2_nombre));

    if (!caja_combinada)
        return NULL;

    return caja_combinada;
}

void inventario_con_cada_caja(inventario_t *inventario, bool (*f)(const char *nombre_caja, void *caja, void *aux), void *aux)
{
    if (!inventario)
        return;

    hash_con_cada_clave(inventario->inventario_hash, f, aux);
}

bool inventario_contiene_caja(inventario_t *inventario, const char *caja_nombre)
{
    return hash_contiene(inventario->inventario_hash, caja_nombre);
}

void inventario_destruir(inventario_t *inventario)
{
    if (!inventario)
        return;

    // TODO : Revisar y continuar
    // destruir cada caja con la funcion de parametro destructor
    hash_destruir_todo(inventario->inventario_hash, NULL);
    free(inventario);
}