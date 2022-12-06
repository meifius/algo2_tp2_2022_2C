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

void comando_mostrar_inventario(inventario_t *inventario)
{
        inventario_con_cada_caja(inventario, imprimir_caja, NULL);
        return;
}

void comando_carga_otra_caja(inventario_t *inventario)
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
        int caja_cantidad_cargados = 0;
        menu_cargar_caja(inventario, caja_nombre, &caja_cantidad_cargados);
        if (caja_cantidad_cargados == 1)
                printf("Se cargo exitosamente la caja\n");
}

void comando_combinar_cajas(inventario_t *inventario)
{
        printf("Ingresar nombre de la primera caja a combinar: \n");
        char caja1_nombre[50];
        int result = scanf("%s", caja1_nombre);
        if (result != 1 || !inventario_contiene_caja(inventario, caja1_nombre))
        {
                printf("No existe tal caja.\n");
                return;
        }

        printf("Ingresar nombre de la primera caja a combinar: \n");
        char caja2_nombre[50];
        result = scanf("%s", caja2_nombre);
        if (result != 1 || !inventario_contiene_caja(inventario, caja2_nombre))
        {
                printf("No existe tal caja.\n");
                return;
        }

        printf("Ingresar nombre que tendra el resultado de combinar las cajas.\n");
        char caja_nombre_combinada[50];
        result = scanf("%s", caja_nombre_combinada);
        if (result != 1 || inventario_contiene_caja(inventario, caja_nombre_combinada))
                printf("El nombre de la caja combinada ya existe. Intente de nuevo.\n");

        caja_t *caja_combinada =
            inventario_combinar_cajas(inventario, caja1_nombre,
                                      caja2_nombre, caja_nombre_combinada);
        if (!caja_combinada)
                printf("No se pudo cargar exitosamente al sistema la caja combinada.\n");

        int pokemones_guardados = caja_guardar_archivo(caja_combinada, caja_nombre_combinada);
        if (pokemones_guardados <= 0)
                printf("No se pudo guardar exitosamente en un archivo la caja combinada.\n");
}

void comando_mostrar_caja(inventario_t *inventario)
{
        printf("Ingresar nombre de la caja a listar: \n");
        char caja_nombre[50];
        int result = scanf("%s", caja_nombre);
        if (result != 1 || !inventario_contiene_caja(inventario, caja_nombre))
        {
                printf("No existe tal caja.\n");
                return;
        }
        inventario_recorrer_caja(inventario, caja_nombre, imprimir_pokemon);
}

void comando_buscar_pokemon_en_cajas(inventario_t *inventario)
{
        printf("Ingresar nombre del pokemon a encontrar en el inventario: \n");
        char pokemon_nombre[50];
        int result = scanf("%s", pokemon_nombre);
        if (result != 1)
        {
                return;
        }
        int cajas_contenedoras = inventario_buscar_pokemon(inventario,
                                                           pokemon_nombre);
        printf("Se ha encontrado el pokemon %s en %i cajas\n",
               pokemon_nombre, cajas_contenedoras);
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
                comando_mostrar_inventario(inventario);
                return;
        }

        if (opcion == 'c' || opcion == 'C')
        {
                comando_carga_otra_caja(inventario);
                return;
        }

        if (opcion == 'm' || opcion == 'M')
        {
                comando_combinar_cajas(inventario);
                return;
        }

        if (opcion == 'd' || opcion == 'D')
        {
                comando_mostrar_caja(inventario);
                return;
        }

        if (opcion == 'b' || opcion == 'B')
        {
                comando_buscar_pokemon_en_cajas(inventario);
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

        return OK;
}