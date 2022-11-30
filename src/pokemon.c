// ------------------ BIBLIOTECAS ------------------ //

#include "pokemon.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ------------------ CONSTANTES ------------------ //

#define MAX_NOMBRE_POKEMON 30
#define POKEMON_CAMPOS_LEIDOS_POR_LINEA 4
#define FORMATO "%[^;];%i;%i;%i\n"

// ------------------ ESTRUCTURAS ------------------ //

struct _pokemon_t
{
        char nombre[MAX_NOMBRE_POKEMON];
        int nivel;
        int ataque;
        int defensa;
};

// ------------------ DECLARACIONES DE FUNCIONES ------------------ //

pokemon_t *pokemon_crear_desde_string(char *string)
{
        if (!string)
                return NULL;

        char nombre[MAX_NOMBRE_POKEMON];
        int nivel;
        int ataque;
        int defensa;

        int leidos = sscanf(string, FORMATO, nombre, &nivel, &ataque, &defensa);

        if (leidos != POKEMON_CAMPOS_LEIDOS_POR_LINEA)
                return NULL;

        struct _pokemon_t *pokemon = malloc(sizeof(struct _pokemon_t));
        if (!pokemon)
                return NULL;

        strcpy(pokemon->nombre, nombre);
        pokemon->nivel = nivel;
        pokemon->ataque = ataque;
        pokemon->defensa = defensa;

        return pokemon;
}

int pokemon_nivel(pokemon_t *pokemon)
{
        if (!pokemon)
                return 0;

        return pokemon->nivel;
}

int pokemon_ataque(pokemon_t *pokemon)
{
        if (!pokemon)
                return 0;

        return pokemon->ataque;
}

int pokemon_defensa(pokemon_t *pokemon)
{
        if (!pokemon)
                return 0;

        return pokemon->defensa;
}

const char *pokemon_nombre(pokemon_t *pokemon)
{
        if (!pokemon)
                return NULL;

        return pokemon->nombre;
}

void pokemon_destruir(pokemon_t *pokemon)
{
        free(pokemon);
}