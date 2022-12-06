#include <string.h>
#include "src/inventario.h"
#include "pa2m.h"

#define CAPACIDAD_INICIAL_MINIMA 3
#define OK 0
#define ERROR -1

// FUNCIONES AUXILIARES

// FUNCIONES PRUEBAS

void prueba_crear_y_cargar_inventario()
{
    inventario_t *inventario = inventario_crear(3);
    pa2m_afirmar(inventario != NULL, "El inventario se pudo crear correctamente.");
    pa2m_afirmar(inventario_cantidad_cajas(inventario) == 0, "La creacion de un inventario posee 0 cajas.");

    const char *caja_nombre = "pokemones_caja1.csv";
    caja_t *caja1 = caja_cargar_archivo((caja_nombre));

    inventario = inventario_cargar_caja(inventario, caja_nombre, caja1);
    pa2m_afirmar(inventario != NULL, "El inventario pudo cargar una caja correctamente.");
    pa2m_afirmar(inventario_contiene_caja(inventario, caja_nombre), "Contiene la caja recien cargada.");
    pa2m_afirmar(inventario_cantidad_cajas(inventario) == 1, "Agregar la primera caja hace que el inventario posea 1 caja.");
    pa2m_afirmar(inventario_contiene_caja(inventario, "pokemones_caja1.csv"), "Contiene la caja agregada.");

    const char *caja_nombre_2 = "pokemones_caja2.csv";
    caja_t *caja2 = caja_cargar_archivo((caja_nombre_2));

    inventario = inventario_cargar_caja(inventario, caja_nombre_2, caja2);
    pa2m_afirmar(inventario != NULL, "El inventario pudo cargar una caja correctamente.");
    pa2m_afirmar(inventario_contiene_caja(inventario, caja_nombre_2), "Contiene la caja recien cargada.");
    pa2m_afirmar(inventario_cantidad_cajas(inventario) == 2, "Agregar la primera caja hace que el inventario posea 2 caja.");
    pa2m_afirmar(inventario_contiene_caja(inventario, caja_nombre), "Contiene la caja que previamente aun tenia cargada.");
    pa2m_afirmar(inventario_contiene_caja(inventario, "pokemones_caja2.csv"), "Contiene la caja agregada.");

    const char *caja_nombre_3 = "pokemones_caja3.csv";
    pa2m_afirmar(inventario_cantidad_cajas(NULL) == 0, "La cantidad de un inventario nulo es 0.");
    inventario_t *inventario_aux = inventario_cargar_caja(inventario, caja_nombre_3, NULL);
    pa2m_afirmar(inventario_aux == NULL, "El inventario no pudo cargar una caja nula.");
    pa2m_afirmar(inventario_cantidad_cajas(inventario) == 2, "Intentar agregar una caja nula hace que el inventario siga teniendo 2 caja.");
    pa2m_afirmar(!inventario_contiene_caja(inventario, "pokemones_caja3.csv"), "No contiene la caja nula que se intento agregar.");

    inventario_destruir(inventario);
}

void prueba_combinar_cajas_en_el_inventario()
{
    inventario_t *inventario = inventario_crear(3);

    const char *caja_nombre = "pokemones_caja1.csv";
    caja_t *caja1 = caja_cargar_archivo((caja_nombre));
    inventario = inventario_cargar_caja(inventario, caja_nombre, caja1);
    const char *caja_nombre_2 = "pokemones_caja2.csv";
    caja_t *caja2 = caja_cargar_archivo((caja_nombre_2));
    inventario = inventario_cargar_caja(inventario, caja_nombre_2, caja2);
    const char *caja_nombre_3 = "pokemones_caja3.csv";
    caja_t *caja3 = caja_cargar_archivo((caja_nombre_3));
    inventario = inventario_cargar_caja(inventario, caja_nombre_3, caja3);

    const char *caja_nombre_combinada = "pokemones_caja_combinada.csv";

    pa2m_afirmar(inventario_combinar_cajas(NULL, caja_nombre, caja_nombre_2, caja_nombre_combinada) == NULL, "No se puede combinar en un inventario nulo.");
    pa2m_afirmar(inventario_combinar_cajas(inventario, NULL, caja_nombre_2, caja_nombre_combinada) == NULL, "No se puede combinar si una caja fuente es nula.");
    pa2m_afirmar(inventario_combinar_cajas(inventario, caja_nombre, NULL, caja_nombre_combinada) == NULL, "No se puede combinar si una caja fuente es nula.");
    pa2m_afirmar(inventario_combinar_cajas(inventario, caja_nombre, caja_nombre_2, NULL) == NULL, "No se puede combinar si el nombre de la caja combinada es nulo.");
    pa2m_afirmar(inventario_combinar_cajas(inventario, caja_nombre, caja_nombre_2, caja_nombre_2) == NULL, "No se puede combinar cajas con uno de los nombre de las cajas fuente.");
    pa2m_afirmar(inventario_combinar_cajas(inventario, caja_nombre, caja_nombre_2, caja_nombre_3) == NULL, "No se puede combinar cajas si el nombre de la caja combinada ya existe en el inventario.");

    pa2m_afirmar(inventario_combinar_cajas(inventario, caja_nombre, caja_nombre_2, caja_nombre_combinada) != NULL, "Se puede combinar cajas.");
    pa2m_afirmar(inventario_contiene_caja(inventario, "pokemones_caja_combinada.csv"), "El inventario contiene la caja combinada.");
    pa2m_afirmar(inventario_cantidad_cajas(inventario) == 4, "La cantidad de cajas es la esperada luego de combinar.");

    inventario_destruir(inventario);
}

void prueba_busqueda_pokemon_en_el_inventario()
{
    inventario_t *inventario = inventario_crear(3);

    const char *caja_nombre = "pokemones_caja1.csv";
    caja_t *caja1 = caja_cargar_archivo((caja_nombre));
    inventario = inventario_cargar_caja(inventario, caja_nombre, caja1);
    const char *caja_nombre_2 = "pokemones_caja2.csv";
    caja_t *caja2 = caja_cargar_archivo((caja_nombre_2));
    inventario = inventario_cargar_caja(inventario, caja_nombre_2, caja2);
    const char *caja_nombre_3 = "pokemones_caja3.csv";
    caja_t *caja3 = caja_cargar_archivo((caja_nombre_3));
    inventario = inventario_cargar_caja(inventario, caja_nombre_3, caja3);

    pa2m_afirmar(inventario_buscar_pokemon(inventario, "Pikachu") == 1, "Pikachu se encuentra en la cantidad de cajas esperada.");
    pa2m_afirmar(inventario_buscar_pokemon(inventario, "Ivysaur") == 2, "Ivysaur se encuentra en la cantidad de cajas esperada.");
    pa2m_afirmar(inventario_buscar_pokemon(NULL, "Ivysaur") == 0, "Un inventario nulo devuelve 0 resultados en la busqueda.");
    pa2m_afirmar(inventario_buscar_pokemon(inventario, NULL) == 0, "Un nombre de pokemon nulo devuelve 0 resultados en la busqueda.");

    inventario_destruir(inventario);
}

int main()
{
    pa2m_nuevo_grupo("Pruebas de creacion y carga del inventario.");
    prueba_crear_y_cargar_inventario();

    pa2m_nuevo_grupo("Pruebas de cajas en el inventario.");
    prueba_combinar_cajas_en_el_inventario();

    pa2m_nuevo_grupo("Pruebas de busqueda de pokemon en el inventario.");
    prueba_busqueda_pokemon_en_el_inventario();

    return pa2m_mostrar_reporte();

    return OK;
}