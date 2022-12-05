#ifndef __INVENTARIO_H__
#define __INVENTARIO_H__

#include <stdbool.h>
#include <stdlib.h>
#include "cajas.h"

typedef struct _inventario_t inventario_t;

/*
Crea inventario con una capacidad inicial determinada por parametro.
En caso que no se pueda crear devolvera NULL.
*/
inventario_t *inventario_crear(size_t capacidad);

/*
Se carga en el inventario una caja con su respectivo nombre.
No se permite cargar una caja con el mismo nombre.
En caso de error, devolvera NULL.
*/
inventario_t *inventario_cargar_caja(inventario_t *inventario, const char *caja_nombre, caja_t *caja);

/*
Se combinan dos cajas distintas del inventario.
Recibe un inventario y el nombre de las 2 cajas existentes del inventario.
Devuelve NULL en caso de error al combinar las cajas.
*/
caja_t *inventario_combinar_cajas(inventario_t *inventario, const char *caja1_nombre, const char *caja2_nombre);

/*
 */
void inventario_con_cada_caja(inventario_t *inventario, bool (*f)(const char *nombre_caja, void *caja, void *aux), void *aux);

bool inventario_contiene_caja(inventario_t *inventario, const char *caja_nombre);

// inventario_buscar_caja

/*
Libera toda la memoria del inventario.
*/
void inventario_destruir(inventario_t *inventario);

#endif /* __INVENTARIO_H__ */