#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "src/cajas.h"
#include "src/pokemon.h"
#include "src/inventario.h"

#define OK 0
#define ERROR -1
#define CAJAS_MINIMAS 1

void imprimir_pokemon(pokemon_t *pokemon)
{
        printf("Nombre: %s\n", pokemon_nombre(pokemon));
        printf("Nivel: %i\n", pokemon_nivel(pokemon));
        printf("Ataque: %i\n", pokemon_ataque(pokemon));
        printf("Defensa: %i\n", pokemon_defensa(pokemon));
        printf("\n");
}

void menu_imprimir()
{
        printf("------------\n");
        printf("1. Mostrar inventario (i/I)\n");
        printf("2. Cargar otra caja (c/C)\n");
        printf("3. Combinar cajas (m/M)\n");
        printf("4. Mostrar caja (d/D)\n");
        printf("5. Buscar caja (b/B)\n");
        printf("6. Salir (q/Q)\n");
        printf("------------\n");
}

void menu_cargar_caja(inventario_t *inventario, char *caja_nombre, int *cajas_cantidad_cargados)
{
        printf("Cargando caja: %s\n", caja_nombre);

        caja_t *caja_i = caja_cargar_archivo((caja_nombre));

        if (!caja_i)
                printf("Error al cargar la caja %s\n", caja_nombre);
        else
        {
                inventario_t *aux = inventario_cargar_caja(inventario, caja_nombre, caja_i);
                if (!aux)
                        printf("Error al cargar la caja al inventario %s\n", caja_nombre);
                else
                        (*cajas_cantidad_cargados)++;
        }
}

int inventario_cargar_inicialmente(inventario_t *inventario, int tope_nombres, char *nombres_cajas[])
{
        int cajas_cantidad_cargados = 0;
        for (int i = 1; i < tope_nombres; i++)
        {
                menu_cargar_caja(inventario, nombres_cajas[i], &cajas_cantidad_cargados);
        }

        return cajas_cantidad_cargados;
}

bool imprimir_caja(const char *nombre_caja, void *caja, void *aux)
{
        printf("Caja: %s, cantidad pokemones: %i\n", nombre_caja, caja_cantidad(caja));
        return true;
}

void mostrar_inventario(inventario_t *inventario)
{
        inventario_con_cada_caja(inventario, imprimir_caja, NULL);
        return;
}

void menu_opciones(inventario_t *inventario, char opcion, bool *hay_que_salir)
{
        if (opcion == 'q' || opcion == 'Q')
        {
                *hay_que_salir = true;
                return;
        }

        if (opcion == 'i' || opcion == 'I')
        {
                mostrar_inventario(inventario);
                return;
        }

        if (opcion == 'c' || opcion == 'C')
        {
                printf("Ingresar nombre del archivo a cargar: \n");
                char caja_nombre[50];
                int result = scanf("%s", caja_nombre);
                printf("Ingreso caja con nombre: %s\n", caja_nombre);
                if (result != 1 || inventario_contiene_caja(inventario, caja_nombre))
                {
                        printf("Ya existe una caja con el mismo nombre: %s\n", caja_nombre);
                        return;
                }
        }

        if (opcion == 'm' || opcion == 'M')
        {
                return;
        }

        if (opcion == 'd' || opcion == 'D')
        {
                return;
        }

        if (opcion == 'b' || opcion == 'B')
        {
                return;
        }
}

int main(int argc, char *argv[])
{
        inventario_t *inventario = inventario_crear(5);
        int cajas_cargadas = inventario_cargar_inicialmente(inventario, argc, argv);
        printf("Cajas cargadas: %i\n", cajas_cargadas);

        if (cajas_cargadas < CAJAS_MINIMAS)
                return ERROR;

        bool hay_que_salir = false;
        char opcion;
        int result = 0;

        while (!hay_que_salir && result >= 0)
        {
                menu_imprimir();
                result = scanf(" %c", &opcion);
                menu_opciones(inventario, opcion, &hay_que_salir);
        }

        inventario_destruir(inventario);

        // caja_t *caja = caja_cargar_archivo("./ejemplo/pokemones.csv");
        // if (!caja)
        //         return ERROR;

        // printf("-----------------------\n");
        // printf("Pokemones en la caja:\n\n");
        // int pokemones_recorridos = caja_recorrer(caja, imprimir_pokemon);
        // printf("\nPokemones recorridos: %i\n", pokemones_recorridos);
        // printf("-----------------------\n");

        // printf("\n");

        // printf("-----------------------\n");
        // int pokemones_guardados = caja_guardar_archivo(caja, "./ejemplo/archivo_prueba.csv");
        // printf("Cantidad de pokemones guardados con exito en el archivo: %i\n", pokemones_guardados);
        // printf("-----------------------\n");

        // printf("\n");

        // caja_t *caja1 = caja_cargar_archivo("./ejemplo/pokemones_caja1.csv");
        // if (!caja1)
        // {
        //         caja_destruir(caja);
        //         return ERROR;
        // }

        // printf("-----------------------\n");
        // printf("Pokemones en la caja 1:\n\n");
        // int pokemones_recorridos_caja1 = caja_recorrer(caja1, imprimir_pokemon);
        // printf("\nPokemones recorridos: %i\n", pokemones_recorridos_caja1);
        // printf("-----------------------\n");

        // caja_t *caja2 = caja_cargar_archivo("./ejemplo/pokemones_caja2.csv");
        // if (!caja2)
        // {
        //         caja_destruir(caja);
        //         caja_destruir(caja1);
        //         return ERROR;
        // }

        // printf("-----------------------\n");
        // printf("Pokemones en la caja 2:\n\n");
        // int pokemones_recorridos_caja2 = caja_recorrer(caja2, imprimir_pokemon);
        // printf("\nPokemones recorridos: %i\n", pokemones_recorridos_caja2);
        // printf("-----------------------\n");

        // caja_t *caja_combinada = caja_combinar(caja1, caja2);
        // if (!caja_combinada)
        // {
        //         caja_destruir(caja);
        //         caja_destruir(caja1);
        //         caja_destruir(caja2);
        //         return ERROR;
        // }

        // printf("-----------------------\n");
        // printf("Pokemones en la caja combinada:\n\n");
        // int pokemones_recorridos_caja_combinada = caja_recorrer(caja_combinada, imprimir_pokemon);
        // printf("\nPokemones recorridos: %i\n", pokemones_recorridos_caja_combinada);
        // printf("-----------------------\n");

        // caja_destruir(caja);
        // caja_destruir(caja1);
        // caja_destruir(caja2);
        // caja_destruir(caja_combinada);

        return OK;
}