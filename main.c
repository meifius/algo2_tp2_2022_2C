#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "src/cajas.h"
#include "src/pokemon.h"

#define OK 0
#define ERROR -1

void imprimir_pokemon(pokemon_t *pokemon)
{
        printf("Nombre: %s\n", pokemon_nombre(pokemon));
        printf("Nivel: %i\n", pokemon_nivel(pokemon));
        printf("Ataque: %i\n", pokemon_ataque(pokemon));
        printf("Defensa: %i\n", pokemon_defensa(pokemon));
        printf("\n");
}

int main(int argc, char *argv[])
{
        caja_t *caja = caja_cargar_archivo("./ejemplo/pokemones.csv");
        if (!caja)
                return ERROR;

        printf("-----------------------\n");
        printf("Pokemones en la caja:\n\n");
        int pokemones_recorridos = caja_recorrer(caja, imprimir_pokemon);
        printf("\nPokemones recorridos: %i\n", pokemones_recorridos);
        printf("-----------------------\n");

        printf("\n");

        printf("-----------------------\n");
        int pokemones_guardados = caja_guardar_archivo(caja, "./ejemplo/archivo_prueba.csv");
        printf("Cantidad de pokemones guardados con exito en el archivo: %i\n", pokemones_guardados);
        printf("-----------------------\n");

        printf("\n");

        caja_t *caja1 = caja_cargar_archivo("./ejemplo/pokemones_caja1.csv");
        if (!caja1)
        {
                caja_destruir(caja);
                return ERROR;
        }

        printf("-----------------------\n");
        printf("Pokemones en la caja 1:\n\n");
        int pokemones_recorridos_caja1 = caja_recorrer(caja1, imprimir_pokemon);
        printf("\nPokemones recorridos: %i\n", pokemones_recorridos_caja1);
        printf("-----------------------\n");

        caja_t *caja2 = caja_cargar_archivo("./ejemplo/pokemones_caja2.csv");
        if (!caja2)
        {
                caja_destruir(caja);
                caja_destruir(caja1);
                return ERROR;
        }

        printf("-----------------------\n");
        printf("Pokemones en la caja 2:\n\n");
        int pokemones_recorridos_caja2 = caja_recorrer(caja2, imprimir_pokemon);
        printf("\nPokemones recorridos: %i\n", pokemones_recorridos_caja2);
        printf("-----------------------\n");

        caja_t *caja_combinada = caja_combinar(caja1, caja2);
        if (!caja_combinada)
        {
                caja_destruir(caja);
                caja_destruir(caja1);
                caja_destruir(caja2);
                return ERROR;
        }

        printf("-----------------------\n");
        printf("Pokemones en la caja combinada:\n\n");
        int pokemones_recorridos_caja_combinada = caja_recorrer(caja_combinada, imprimir_pokemon);
        printf("\nPokemones recorridos: %i\n", pokemones_recorridos_caja_combinada);
        printf("-----------------------\n");

        caja_destruir(caja);
        caja_destruir(caja1);
        caja_destruir(caja2);
        caja_destruir(caja_combinada);

        return OK;
}