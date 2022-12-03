// BIBLIOTECAS

#include "hash.h"
#include "stdlib.h"
#include "string.h"

// CONSTANTES

#define CAPACIDAD_INICIAL_MINIMA 3
#define CANTIDAD_INICIAL 0
#define FACTOR_CARGA_MAXIMO 0.85
#define FACTOR_CAPACIDAD_NUEVA 2
#define FACTOR_CARGA_ERROR -1

// DEFINICION DE ESTRUCTURAS

typedef struct par
{
        char *clave;
        void *valor;
} par_t;

typedef struct nodo
{
        par_t par;
        struct nodo *siguiente;
} nodo_t;

struct hash
{
        size_t cantidad;
        size_t capacidad;
        nodo_t **pares;
        size_t (*funcionHash)(const char *clave);
};

// DEFINICION DE FUNCIONES PRIVADAS

/*
Reserva memoria para un string.
*/
char *__strdup(const char *s);

/*
Dado cualquier string, devolvera un numero.
*/
size_t funcion_hash(const char *clave);

/*
Debera poseer una clave valida.
Si existe el elemento, se guardara la informacion en anterior.
Si no se pudo insertar correctamente, devolvera NULL.
*/
nodo_t *nodo_insertar(nodo_t *nodo, const char *clave, void *elemento,
                      void **anterior, hash_t *hash);

/*
Clave debe ser un string valido valida
Devuelve un booleano en caso si el algun nodo contiene a esa clave
*/
bool nodo_contiene(nodo_t *nodo, const char *clave);

/*
Clave debe ser un string valido valida
Obtiene el valor de una clave, si el nodo lo contiene
*/
void *nodo_obtener(nodo_t *nodo, const char *clave);

/*
Se elimina el nodo si la clave es coincidente.
En caso de eliminacion se modifica la variable elemento_quitado con dicho
valor. Tambien se libera la memoria del nodo como de la clave.
*/
nodo_t *nodo_quitar(nodo_t *nodo, const char *clave, void **elemento_quitado,
                    hash_t *hash);

/*
Destruye de forma recursiva todos los nodos y clave que tenga.
*/
void nodo_destruir(nodo_t *nodo);

/*
Destruye de forma recursiva todos los nodos y claves.
Tambien destruye los valores mediante la funcion destructor.
*/
void nodo_destruir_todo(nodo_t *nodo, void (*destructor)(void *));

/*
Recorre de forma recursiva cada nodo mientras la funcion f devuelva true.
A la funcion fe le pasa como parámetros la clave, el valor asociado a la clave
y el puntero auxiliar.
*/
bool seguir_nodos_con_cada_clave(size_t *contador, nodo_t *nodo,
                                 bool (*f)(const char *clave,
                                           void *valor, void *aux),
                                 void *aux);

/*
Debera recibir un hash valido o en caso contrario devolvera -1.
Calcula el factor de carga del hash. Es un numero entre 0 y 1.
*/
float factor_carga(hash_t *hash);

/*
Se realiza un rehash del hash enviado.
Se crean nuevos pares y se liberan lo anteriores.
En caso de error devuelve NULL.
*/
hash_t *hash_rehash(hash_t *hash);

/*
Se inserta el par clave valor(elemento) en pares_destino.
*/
void par_insertar(nodo_t **pares_destino, const char *clave, void *elemento,
                  size_t (*funcionHash)(const char *clave), size_t capacidad);

/*
La clave debe ser valida.
Inserta el nodo con su par en el ultimo nodo enlazado.
*/
nodo_t *nodo_insertar_rehash(nodo_t *nodo, const char *clave, void *elemento);

/*
Recorre nodo por nodo e inserta el par clave-valor en pares_destino.
clave y pares_destino deben ser validos.
*/
void recorrer_nodos_rehash(nodo_t *nodo,
                           size_t (*funcionHash)(const char *clave),
                           nodo_t **pares_destino, size_t capacidad);

/*
Destruye todos los pares del hash
*/
void pares_destruir(hash_t *hash);

// DECLARACION DE FUNCIONES PRIVADAS
char *__strdup(const char *s)
{
        size_t len = strlen(s) + 1;
        void *new = malloc(len);
        if (new == NULL)
                return NULL;
        return (char *)memcpy(new, s, len);
}

size_t funcion_hash(const char *str)
{
        size_t hash = 5381;
        size_t c;

        while ((c = (size_t)(*(str++))))
                hash = ((hash << 5) + hash) + c;

        return hash;
}

nodo_t *nodo_insertar(nodo_t *nodo, const char *clave, void *elemento,
                      void **anterior, hash_t *hash)
{
        if (!nodo)
        {
                nodo_t *nodo_nuevo = malloc(sizeof(nodo_t));
                if (!nodo_nuevo)
                        return NULL;

                if (anterior)
                        *anterior = NULL;

                nodo_nuevo->par.clave = __strdup(clave);
                nodo_nuevo->par.valor = elemento;
                nodo_nuevo->siguiente = NULL;
                hash->cantidad++;

                return nodo_nuevo;
        }

        if (strcmp(clave, nodo->par.clave) == 0)
        {
                if (anterior)
                        *anterior = nodo->par.valor;
                nodo->par.valor = elemento;

                return nodo;
        }

        nodo->siguiente = nodo_insertar(nodo->siguiente, clave, elemento,
                                        anterior, hash);
        return nodo;
}

bool nodo_contiene(nodo_t *nodo, const char *clave)
{
        if (!nodo)
                return false;

        if (strcmp(nodo->par.clave, clave) == 0)
                return true;

        return nodo_contiene(nodo->siguiente, clave);
}

void *nodo_obtener(nodo_t *nodo, const char *clave)
{
        if (!nodo || !clave)
                return NULL;

        if (strcmp(nodo->par.clave, clave) == 0)
                return nodo->par.valor;

        return nodo_obtener(nodo->siguiente, clave);
}

nodo_t *nodo_quitar(nodo_t *nodo, const char *clave, void **elemento_quitado,
                    hash_t *hash)
{
        if (!nodo || !clave)
                return NULL;

        if (strcmp(nodo->par.clave, clave) == 0)
        {
                *elemento_quitado = nodo->par.valor;
                nodo_t *nodo_siguiente = nodo->siguiente;
                free(nodo->par.clave);
                free(nodo);
                hash->cantidad--;

                return nodo_siguiente;
        }

        nodo->siguiente = nodo_quitar(nodo->siguiente, clave, elemento_quitado,
                                      hash);
        return nodo;
}

void nodo_destruir(nodo_t *nodo)
{
        if (!nodo)
                return;

        nodo_t *nodo_siguiente = nodo->siguiente;
        free(nodo->par.clave);
        free(nodo);

        nodo_destruir(nodo_siguiente);
}

void nodo_destruir_todo(nodo_t *nodo, void (*destructor)(void *))
{
        if (!nodo)
                return;

        if (destructor)
                destructor(nodo->par.valor);

        nodo_destruir_todo(nodo->siguiente, destructor);
}

bool seguir_nodos_con_cada_clave(size_t *contador, nodo_t *nodo,
                                 bool (*f)(const char *clave,
                                           void *valor, void *aux),
                                 void *aux)
{
        if (!nodo)
                return true;

        if (!f(nodo->par.clave, nodo->par.valor, aux))
        {
                (*contador)++;
                return false;
        }
        (*contador)++;

        return seguir_nodos_con_cada_clave(contador, nodo->siguiente, f, aux);
}

float factor_carga(hash_t *hash)
{
        if (!hash)
                return FACTOR_CARGA_ERROR;

        return (float)hash->cantidad / (float)hash->capacidad;
}

nodo_t *nodo_insertar_rehash(nodo_t *nodo, const char *clave, void *elemento)
{
        if (!nodo)
        {
                nodo_t *nodo_nuevo = malloc(sizeof(nodo_t));
                if (!nodo_nuevo)
                        return NULL;

                nodo_nuevo->par.clave = __strdup(clave);
                nodo_nuevo->par.valor = elemento;
                nodo_nuevo->siguiente = NULL;

                return nodo_nuevo;
        }

        nodo->siguiente = nodo_insertar_rehash(nodo->siguiente, clave,
                                               elemento);
        return nodo;
}

void par_insertar(nodo_t **pares_destino, const char *clave, void *elemento,
                  size_t (*funcionHash)(const char *clave), size_t capacidad)
{
        size_t hash_posicion = (funcionHash(clave)) % capacidad;
        nodo_t *nodo = nodo_insertar_rehash(pares_destino[hash_posicion],
                                            clave, elemento);
        if (!nodo)
                return;

        pares_destino[hash_posicion] = nodo;
        return;
}

void recorrer_nodos_rehash(nodo_t *nodo,
                           size_t (*funcionHash)(const char *clave),
                           nodo_t **pares_destino, size_t capacidad)
{
        if (!nodo)
                return;

        par_insertar(pares_destino, nodo->par.clave, nodo->par.valor,
                     funcion_hash, capacidad);

        recorrer_nodos_rehash(nodo->siguiente, funcionHash, pares_destino,
                              capacidad);
}

void pares_destruir(hash_t *hash)
{
        if (!hash)
                return;

        for (int i = 0; i < hash->capacidad; i++)
                nodo_destruir(hash->pares[i]);

        free(hash->pares);
}

hash_t *hash_rehash(hash_t *hash)
{
        if (!hash)
                return NULL;

        size_t nueva_capacidad = (hash->capacidad) * FACTOR_CAPACIDAD_NUEVA;
        nodo_t **pares_nuevos = calloc(nueva_capacidad, sizeof(nodo_t *));
        if (!pares_nuevos)
                return NULL;

        for (int i = 0; i < hash->capacidad; i++)
                recorrer_nodos_rehash(hash->pares[i], hash->funcionHash,
                                      pares_nuevos, nueva_capacidad);

        pares_destruir(hash);

        hash->capacidad = nueva_capacidad;
        hash->pares = pares_nuevos;

        return hash;
}

// DECLARACION DE FUNCIONES PUBLICAS

hash_t *hash_crear(size_t capacidad)
{
        if (capacidad < CAPACIDAD_INICIAL_MINIMA)
                capacidad = CAPACIDAD_INICIAL_MINIMA;

        hash_t *hash = malloc(sizeof(hash_t));
        if (!hash)
                return NULL;

        nodo_t **pares = calloc(capacidad, sizeof(nodo_t *));
        if (!pares)
                return NULL;

        hash->cantidad = CANTIDAD_INICIAL;
        hash->capacidad = capacidad;
        hash->pares = pares;
        hash->funcionHash = funcion_hash;

        return hash;
}

hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento,
                      void **anterior)
{
        if (!hash || !clave)
                return NULL;

        if (factor_carga(hash) > FACTOR_CARGA_MAXIMO)
        {
                if (!hash_rehash(hash))
                        return NULL;
        }

        if (!hash)
                return NULL;

        size_t hash_posicion = (hash->funcionHash(clave)) % hash->capacidad;
        nodo_t *nodo = nodo_insertar(hash->pares[hash_posicion], clave,
                                     elemento, anterior, hash);
        if (!nodo)
                return NULL;

        hash->pares[hash_posicion] = nodo;
        return hash;
}

void *hash_quitar(hash_t *hash, const char *clave)
{
        if (!hash || !clave)
                return NULL;

        size_t hash_posicion = (hash->funcionHash(clave)) % hash->capacidad;

        void *elemento_quitado = NULL;
        nodo_t *nodo = nodo_quitar(hash->pares[hash_posicion], clave,
                                   &elemento_quitado, hash);

        hash->pares[hash_posicion] = nodo;

        return elemento_quitado;
}

void *hash_obtener(hash_t *hash, const char *clave)
{
        if (!hash || !clave)
                return NULL;

        size_t hash_posicion = (hash->funcionHash(clave)) % hash->capacidad;

        return nodo_obtener(hash->pares[hash_posicion], clave);
}

bool hash_contiene(hash_t *hash, const char *clave)
{
        if (!hash || !clave)
                return false;

        size_t hash_posicion = (hash->funcionHash(clave)) % hash->capacidad;
        return nodo_contiene(hash->pares[hash_posicion], clave);
}

size_t hash_cantidad(hash_t *hash)
{
        if (!hash)
                return 0;
        return hash->cantidad;
}

void hash_destruir(hash_t *hash)
{
        if (!hash)
                return;
        hash_destruir_todo(hash, NULL);
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
        if (!hash)
                return;

        if (destructor)
        {
                for (int i = 0; i < hash->capacidad; i++)
                        nodo_destruir_todo(hash->pares[i], destructor);
        }

        for (int i = 0; i < hash->capacidad; i++)
                nodo_destruir(hash->pares[i]);

        free(hash->pares);

        free(hash);
}

size_t hash_con_cada_clave(hash_t *hash,
                           bool (*f)(const char *clave, void *valor,
                                     void *aux),
                           void *aux)
{
        if (!hash || !f)
                return 0;

        bool seguir_recorriendo = true;
        int i = 0;
        size_t contador = 0;

        while (seguir_recorriendo && (i < hash->capacidad))
        {
                seguir_recorriendo =
                    seguir_nodos_con_cada_clave(&contador, hash->pares[i], f,
                                                aux);
                i++;
        }

        return contador;
}