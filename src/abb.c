#include "abb.h"
#include <stddef.h>
#include <stdlib.h>

#define ABB_TAMANIO_INICIAL 0

// DECLARAACION DE FUNCIONES PRIVADAS

/**
 * Inserta el elemento en el arbol de forma recursiva.
 * En caso de error devuelve NULL
 */
nodo_abb_t *nodo_insertar(abb_t *arbol, nodo_abb_t *nodo, void *elemento);

/**
 * Recibe un nodo valido.
 * Devuelve NULL si no encontro el elemento, en caso contrario el elemento mismo.
 */
void *nodo_buscar(abb_t *arbol, nodo_abb_t *nodo, void *elemento);

/**
 * Se recibe un puntero a nodo valido.
 * Se procede a borrar la memoria de todos los nodos.
 * Se realiza el borrado de acuerdo a un recorrido postorden
 */
void nodo_destruir_postorden(nodo_abb_t *nodo);

/**
 * Se recibe un puntero a nodo y un destructor valido.
 * Se procede aplicar a todo los elementos del arbol la funcion destructor y
 * a borrar la memoria de todos los nodos.
 * Se realiza el borrado de acuerdo a un recorrido postorden
 */
void nodo_destruir_todo_postorden(nodo_abb_t *nodo, void (*destructor)(void *));

/**
 * Recibe un arbol, un nodo valido. Recibe un puntero a cualquier elemento a quitar
 * Se debe recibir un parametro incializado en NULL del elemento que sea quitado.
 * Devolvera un nodo que servira para encadenar.
 * Busca el elemento a quitar y de coincidir lo quita correctamente guardando
 * su valor en el parametro elemento_quitado.
 */
nodo_abb_t *nodo_quitar(abb_t *arbol, nodo_abb_t *nodo, void *elemento, void **elemento_quitado);

/**
 * Se recibe un puntero a nodo valido.
 * Devuelve si es cierto que tiene dos hijos
 */
bool nodo_tiene_dos_hijos(nodo_abb_t *nodo);

/**
 * Se recibe un puntero a nodo valido.
 * Se recibira un parametro par guardar la referencia al predecesor inmediato.
 * Se devolvera un puntero del nodo subarbol para poder encadenar.
 */
nodo_abb_t *extraer_predecesor_inorden(nodo_abb_t *nodo, nodo_abb_t **extraido);

/**
 * Se recibe un puntero a nodo valido.
 * Se Recibe un puntero doble del elemento a extraer. Debera ser NULL y se modificara
 * con el nodo mas a la derecha.
 * Se devolvera un puntero a nodo para poder encadenar el subarbol.
 */
nodo_abb_t *nodo_mas_a_la_derecha(nodo_abb_t *nodo, nodo_abb_t **extraido);

/**
 * Recibe nodo y funcion validos. Se asegura que a la funcion se le inyecatara como primer
 * parametro el elemento actual, y en el segundo parametro el auxiliar.
 * Se recibe tambien el contador de veces que la funcion fue invocada, debe haber sido incializada en 0.
 * Tambien se recibe el parametro de si seguir recorriendo.
 * Se ejecuatara la funcion por cada elemento del arbol recorriendolo de forma inorden
 */
void nodo_elementos_inorden(nodo_abb_t *nodo, bool (*funcion)(void *, void *), void *aux, size_t *contador, bool *seguir);

/**
 * Recibe nodo y funcion validos. Se asegura que a la funcion se le inyecatara como primer
 * parametro el elemento actual, y en el segundo parametro el auxiliar.
 * Se recibe tambien el contador de veces que la funcion fue invocada, debe haber sido incializada en 0.
 * Tambien se recibe el parametro de si seguir recorriendo.
 * Se ejecuatara la funcion por cada elemento del arbol recorriendolo de forma preorden
 */
void nodo_elementos_preoden(nodo_abb_t *nodo, bool (*funcion)(void *, void *), void *aux, size_t *contador, bool *seguir);

/**
 * Recibe nodo y funcion validos. Se asegura que a la funcion se le inyecatara como primer
 * parametro el elemento actual, y en el segundo parametro el auxiliar.
 * Se recibe tambien el contador de veces que la funcion fue invocada, debe haber sido incializada en 0.
 * Tambien se recibe el parametro de si seguir recorriendo.
 * Se ejecuatara la funcion por cada elemento del arbol recorriendolo de forma postorden
 */
void nodo_elementos_postorden(nodo_abb_t *nodo, bool (*funcion)(void *, void *), void *aux, size_t *contador, bool *seguir);

/**
 * Recibe puntero a nodo y doble puntero a un arreglo y su tope validos.
 * Se recibira un contador de elementos guardados, que hara de tope del vector.
 * Tambien se recibira un parametro para sabe si se sigue o no en el recorrido.
 * Se recorrera por cada elemento del arbol recorriendolo de forma inorden
 * para ir guardando los elementos posibles en el vector con dicho recorrido.
 */
void nodo_recorrer_inorden(nodo_abb_t *nodo, void **array, size_t tamanio_array, size_t *contador, bool *seguir);

/**
 * Recibe puntero a nodo y doble puntero a un arreglo y su tope validos.
 * Se recibira un contador de elementos guardados, que hara de tope del vector.
 * Tambien se recibira un parametro para sabe si se sigue o no en el recorrido.
 * Se recorrera por cada elemento del arbol recorriendolo de forma preorden
 * para ir guardando los elementos posibles en el vector con dicho recorrido.
 */
void nodo_recorrer_preorden(nodo_abb_t *nodo, void **array, size_t tamanio_array, size_t *contador, bool *seguir);

/**
 * Recibe puntero a nodo y doble puntero a un arreglo y su tope validos.
 * Se recibira un contador de elementos guardados, que hara de tope del vector.
 * Tambien se recibira un parametro para sabe si se sigue o no en el recorrido.
 * Se recorrera por cada elemento del arbol recorriendolo de forma postorden
 * para ir guardando los elementos posibles en el vector con dicho recorrido.
 */
void nodo_recorrer_postorden(nodo_abb_t *nodo, void **array, size_t tamanio_array, size_t *contador, bool *seguir);

// DEFINICIONES DE FUNCIONES PRIVADAS

nodo_abb_t *nodo_insertar(abb_t *arbol, nodo_abb_t *nodo, void *elemento)
{
    if (!nodo)
    {
        nodo_abb_t *nodo_nuevo = malloc(sizeof(nodo_abb_t));
        if (!nodo_nuevo)
            return NULL;

        nodo_nuevo->elemento = elemento;
        nodo_nuevo->izquierda = NULL;
        nodo_nuevo->derecha = NULL;

        arbol->tamanio++;
        return nodo_nuevo;
    }

    if (arbol->comparador(elemento, nodo->elemento) <= 0)
    {
        nodo->izquierda = nodo_insertar(arbol, nodo->izquierda, elemento);
    }
    else
    {
        nodo->derecha = nodo_insertar(arbol, nodo->derecha, elemento);
    }
    return nodo;
}

void *nodo_buscar(abb_t *arbol, nodo_abb_t *nodo, void *elemento)
{
    if (nodo == NULL)
        return NULL;

    void *elemento_a_buscar = NULL;
    if (arbol->comparador(elemento, nodo->elemento) == 0)
        return nodo->elemento;

    if (arbol->comparador(elemento, nodo->elemento) <= 0)
        elemento_a_buscar = nodo_buscar(arbol, nodo->izquierda, elemento);
    else
        elemento_a_buscar = nodo_buscar(arbol, nodo->derecha, elemento);

    return elemento_a_buscar;
}

void nodo_destruir_postorden(nodo_abb_t *nodo)
{
    if (!nodo)
        return;

    nodo_destruir_postorden(nodo->izquierda);
    nodo_destruir_postorden(nodo->derecha);
    free(nodo);
    return;
}

void nodo_destruir_todo_postorden(nodo_abb_t *nodo, void (*destructor)(void *))
{
    if (!nodo)
        return;
    nodo_destruir_todo_postorden(nodo->izquierda, destructor);
    nodo_destruir_todo_postorden(nodo->derecha, destructor);
    if (destructor)
        destructor(nodo->elemento);
    free(nodo);
}

nodo_abb_t *nodo_quitar(abb_t *arbol, nodo_abb_t *nodo, void *elemento, void **elemento_quitado)
{
    if (!nodo)
        return NULL;

    int comparacion = arbol->comparador(elemento, nodo->elemento);

    if (comparacion == 0)
    {
        if (nodo_tiene_dos_hijos(nodo))
        {
            nodo_abb_t *predecesor_inorden = NULL;
            nodo->izquierda = extraer_predecesor_inorden(nodo, &predecesor_inorden);
            predecesor_inorden->derecha = nodo->derecha;
            predecesor_inorden->izquierda = nodo->izquierda;
            *elemento_quitado = nodo->elemento;
            free(nodo);
            arbol->tamanio--;
            return predecesor_inorden;
        }
        else
        {
            nodo_abb_t *hijo = nodo->izquierda ? nodo->izquierda : nodo->derecha;
            *elemento_quitado = nodo->elemento;
            free(nodo);
            arbol->tamanio--;
            return hijo;
        }
    }
    else if (comparacion < 0)
        nodo->izquierda = nodo_quitar(arbol, nodo->izquierda, elemento, elemento_quitado);
    else
        nodo->derecha = nodo_quitar(arbol, nodo->derecha, elemento, elemento_quitado);

    return nodo;
}

bool nodo_tiene_dos_hijos(nodo_abb_t *nodo)
{
    return (nodo->izquierda && nodo->derecha);
}

nodo_abb_t *nodo_mas_a_la_derecha(nodo_abb_t *nodo, nodo_abb_t **extraido)
{
    if (nodo->derecha == NULL)
    {
        *extraido = nodo;
        return nodo->izquierda;
    }

    nodo->derecha = nodo_mas_a_la_derecha(nodo->derecha, extraido);

    return nodo;
}

nodo_abb_t *extraer_predecesor_inorden(nodo_abb_t *nodo, nodo_abb_t **extraido)
{
    return nodo_mas_a_la_derecha(nodo->izquierda, extraido);
}

void nodo_elementos_inorden(nodo_abb_t *nodo, bool (*funcion)(void *, void *), void *aux, size_t *contador, bool *seguir)
{
    if (!nodo || !(*seguir))
        return;

    nodo_elementos_inorden(nodo->izquierda, funcion, aux, contador, seguir);
    if (!(*seguir))
        return;

    (*seguir) = funcion(nodo->elemento, aux);
    (*contador)++;
    if (!(*seguir))
        return;

    nodo_elementos_inorden(nodo->derecha, funcion, aux, contador, seguir);
    if (!(*seguir))
        return;
}

void nodo_elementos_preoden(nodo_abb_t *nodo, bool (*funcion)(void *, void *), void *aux, size_t *contador, bool *seguir)
{
    if (!nodo || !(*seguir))
        return;

    (*seguir) = funcion(nodo->elemento, aux);
    (*contador)++;
    if (!seguir)
        return;

    nodo_elementos_preoden(nodo->izquierda, funcion, aux, contador, seguir);
    if (!seguir)
        return;

    nodo_elementos_preoden(nodo->derecha, funcion, aux, contador, seguir);
}

void nodo_elementos_postorden(nodo_abb_t *nodo, bool (*funcion)(void *, void *), void *aux, size_t *contador, bool *seguir)
{
    if (!nodo || !(*seguir))
        return;

    nodo_elementos_postorden(nodo->izquierda, funcion, aux, contador, seguir);
    if (!(*seguir))
        return;
    nodo_elementos_postorden(nodo->derecha, funcion, aux, contador, seguir);
    if (!(*seguir))
        return;

    (*seguir) = funcion(nodo->elemento, aux);
    (*contador)++;
    if (!(*seguir))
        return;
}

void nodo_recorrer_inorden(nodo_abb_t *nodo, void **array, size_t tamanio_array, size_t *contador, bool *seguir)
{
    if (!nodo || !seguir)
        return;

    nodo_recorrer_inorden(nodo->izquierda, array, tamanio_array, contador, seguir);
    if (!(*seguir))
        return;

    array[*contador] = nodo->elemento;
    (*contador)++;
    if ((*contador) == tamanio_array)
    {
        (*seguir) = false;
        return;
    }

    nodo_recorrer_inorden(nodo->derecha, array, tamanio_array, contador, seguir);
}

void nodo_recorrer_preorden(nodo_abb_t *nodo, void **array, size_t tamanio_array, size_t *contador, bool *seguir)
{
    if (!nodo || !seguir)
        return;

    array[*contador] = nodo->elemento;
    (*contador)++;
    if ((*contador) == tamanio_array)
    {
        (*seguir) = false;
        return;
    }

    nodo_recorrer_preorden(nodo->izquierda, array, tamanio_array, contador, seguir);
    if (!(*seguir))
        return;
    nodo_recorrer_preorden(nodo->derecha, array, tamanio_array, contador, seguir);
}

void nodo_recorrer_postorden(nodo_abb_t *nodo, void **array, size_t tamanio_array, size_t *contador, bool *seguir)
{
    if (!nodo || !seguir)
        return;

    nodo_recorrer_postorden(nodo->izquierda, array, tamanio_array, contador, seguir);
    if (!(*seguir))
        return;
    nodo_recorrer_postorden(nodo->derecha, array, tamanio_array, contador, seguir);
    if (!(*seguir))
        return;

    array[*contador] = nodo->elemento;
    (*contador)++;
    if ((*contador) == tamanio_array)
    {
        (*seguir) = false;
        return;
    }
}

// DEFINICIONES DE FUNCIONES PUBLICAS

abb_t *abb_crear(abb_comparador comparador)
{
    if (comparador == NULL)
        return NULL;

    abb_t *arbol = malloc(sizeof(abb_t));
    if (!arbol)
        return NULL;

    arbol->comparador = comparador;
    arbol->nodo_raiz = NULL;
    arbol->tamanio = ABB_TAMANIO_INICIAL;

    return arbol;
}

abb_t *abb_insertar(abb_t *arbol, void *elemento)
{
    if (!arbol)
        return NULL;

    arbol->nodo_raiz = nodo_insertar(arbol, arbol->nodo_raiz, elemento);

    return arbol;
}

void *abb_quitar(abb_t *arbol, void *elemento)
{
    if (!arbol)
        return NULL;

    if (!arbol->comparador)
        return NULL;

    void *elemento_quitado = NULL;
    arbol->nodo_raiz = nodo_quitar(arbol, arbol->nodo_raiz, elemento, &elemento_quitado);

    return elemento_quitado;
}

void *abb_buscar(abb_t *arbol, void *elemento)
{
    if (!arbol || (arbol->nodo_raiz == NULL))
        return NULL;

    if (arbol->comparador(elemento, arbol->nodo_raiz->elemento) == 0)
    {
        return arbol->nodo_raiz->elemento;
    }

    return nodo_buscar(arbol, arbol->nodo_raiz, elemento);
}

bool abb_vacio(abb_t *arbol)
{
    if (!arbol)
        return true;

    return (arbol->tamanio == ABB_TAMANIO_INICIAL) || (arbol->nodo_raiz == NULL);
}

size_t abb_tamanio(abb_t *arbol)
{
    if (!arbol)
        return ABB_TAMANIO_INICIAL;

    return arbol->tamanio;
}

void abb_destruir(abb_t *arbol)
{
    if (!arbol)
        return;

    nodo_destruir_postorden(arbol->nodo_raiz);
    free(arbol);
}

void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *))
{
    if (!arbol)
        return;

    if (!destructor)
    {
        abb_destruir(arbol);
        return;
    }
    nodo_destruir_todo_postorden(arbol->nodo_raiz, destructor);
    free(arbol);
}

size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido,
                             bool (*funcion)(void *, void *), void *aux)
{
    size_t contador = 0;
    if (!arbol)
        return contador;

    if (!funcion)
        return contador;

    bool seguir = true;
    switch (recorrido)
    {
    case INORDEN:
        nodo_elementos_inorden(arbol->nodo_raiz, funcion, aux, &contador, &seguir);
        break;
    case PREORDEN:
        nodo_elementos_preoden(arbol->nodo_raiz, funcion, aux, &contador, &seguir);
        break;
    case POSTORDEN:
        nodo_elementos_postorden(arbol->nodo_raiz, funcion, aux, &contador, &seguir);
        break;
    default:
        break;
    }

    return contador;
}

size_t abb_recorrer(abb_t *arbol, abb_recorrido recorrido, void **array,
                    size_t tamanio_array)
{

    size_t elementos_almacenados = 0;
    if (!arbol)
        return elementos_almacenados;

    bool seguir = true;
    switch (recorrido)
    {
    case INORDEN:
        nodo_recorrer_inorden(arbol->nodo_raiz, array, tamanio_array, &elementos_almacenados, &seguir);
        break;
    case PREORDEN:
        nodo_recorrer_preorden(arbol->nodo_raiz, array, tamanio_array, &elementos_almacenados, &seguir);
        break;
    case POSTORDEN:
        nodo_recorrer_postorden(arbol->nodo_raiz, array, tamanio_array, &elementos_almacenados, &seguir);
        break;
    default:
        break;
    }

    return elementos_almacenados;
}