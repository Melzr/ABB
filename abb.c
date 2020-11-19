#include "abb.h"
#include <stdio.h>

static const int EXITO = 0;
static const int ERROR = -1;
const int PRIMER_ELEMENTO_MAYOR = 1;
const int PRIMER_ELEMENTO_MENOR = -1;
const int IGUALES = 0;


abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor) {

	if ( !comparador || !destructor )
		return NULL;

	abb_t* arbol = calloc(1, sizeof(abb_t));
	if (arbol != NULL) {
		arbol->comparador = comparador;
		arbol->destructor = destructor;
	}

	return arbol;
}

/*
 * Pre: recibe el elemento con el que se inicializara el nodo
 * Post: devuelve un nodo con el elemento y NULL en los nodos izquiero y derecho
 *		 devuelve NULL en caso de error
 */
nodo_abb_t* crear_nodo_inicializado(void* elemento) {

	nodo_abb_t* nodo = calloc(1, sizeof(nodo_abb_t));

	if (nodo != NULL)
		nodo->elemento = elemento;

	return nodo;
}

/*
 *
 *
 */
nodo_abb_t* insertar_aux(nodo_abb_t* raiz, void* elemento, abb_comparador comparador) {

	if (!raiz) {
		raiz = crear_nodo_inicializado(elemento);
		return raiz;
	}

	int comparacion = comparador(raiz->elemento, elemento);
	if (comparacion == PRIMER_ELEMENTO_MENOR)
		raiz->derecha = insertar_aux(raiz->derecha, elemento, comparador);
	else if ( (comparacion == PRIMER_ELEMENTO_MAYOR) || (comparacion == IGUALES) )
		raiz->izquierda = insertar_aux(raiz->izquierda, elemento, comparador);

	return raiz; 
}


int arbol_insertar(abb_t* arbol, void* elemento) {

	if ( !arbol || !elemento || !(arbol->comparador) )
		return ERROR;

	arbol->nodo_raiz = insertar_aux(arbol->nodo_raiz, elemento, arbol->comparador);

	return EXITO;
}

/*
 *
 *
 */
void* buscar_aux(abb_comparador comparador, void* elemento, nodo_abb_t* nodo_actual) {

	if (!nodo_actual)
		return NULL;

	int comparacion = comparador(nodo_actual->elemento, elemento);

	if ( comparacion == PRIMER_ELEMENTO_MAYOR ) 
		return (buscar_aux(comparador, elemento, nodo_actual->izquierda));
	
	if ( comparacion == PRIMER_ELEMENTO_MENOR )
		return (buscar_aux(comparador, elemento, nodo_actual->derecha));

	if (comparacion == IGUALES) {
		return nodo_actual->elemento;
	}

	return NULL;
}

void* arbol_buscar(abb_t* arbol, void* elemento) {

	if (!arbol || !elemento || !(arbol->comparador) )
		return NULL;

	return buscar_aux(arbol->comparador, elemento, arbol->nodo_raiz);
}

/*
 * Pre: la raiz existe y no es NULL
 * 
 */
nodo_abb_t* mayor_elemento_rama(nodo_abb_t* raiz, nodo_abb_t** nodo_mayor) {

	if (!raiz) {
		*nodo_mayor = NULL;
		return NULL;
	}

	if (!(raiz->derecha)) {
		*nodo_mayor = raiz;
		return raiz->izquierda;
	}

	raiz->derecha = mayor_elemento_rama(raiz->derecha, nodo_mayor); 
	return raiz;
}

/*
 *
 *
 */
nodo_abb_t* borrar_raiz(nodo_abb_t* raiz, abb_liberar_elemento destructor) {

	if (!raiz || !destructor)
		return NULL;

	if (!raiz->izquierda && !raiz->derecha) {
		destructor(raiz->elemento);
		free(raiz);
		return NULL;
	}

	if ( (raiz->derecha != NULL) && (raiz->izquierda != NULL) ) {
		nodo_abb_t* nodo_mayor = NULL;
		mayor_elemento_rama(raiz->izquierda, &nodo_mayor);
		if (nodo_mayor == NULL)
			return raiz;
		
		if (nodo_mayor != raiz->izquierda)
			nodo_mayor->izquierda = raiz->izquierda;
		nodo_mayor->derecha = raiz->derecha;
		destructor(raiz->elemento);
		free(raiz);
		raiz = nodo_mayor;
		return raiz;
	}

	nodo_abb_t* nodo_aux = raiz->izquierda;
	if (raiz->derecha != NULL)
		nodo_aux = raiz->derecha;

	destructor(raiz->elemento);
	free(raiz);
	raiz = nodo_aux;
	return raiz;
}

/*
 * falta el caso en que elimino la raiz
 *OJO CON LO QUE DEVUELVE
 */
nodo_abb_t* borrar_aux(nodo_abb_t* raiz, void* elemento, abb_comparador comparador, abb_liberar_elemento destructor, bool* borrado) {

	if (!raiz) 
		return NULL;

	int comparacion = comparador(raiz->elemento, elemento);

	if (comparacion == PRIMER_ELEMENTO_MAYOR)
		raiz->izquierda = borrar_aux(raiz->izquierda, elemento, comparador, destructor, borrado);
	else if (comparacion == PRIMER_ELEMENTO_MENOR)
		raiz->derecha = borrar_aux(raiz->derecha, elemento, comparador, destructor, borrado);
	else {

		if ( !(raiz->derecha) && !(raiz->izquierda) ) {
			destructor(raiz->elemento);
			free(raiz);
			raiz = NULL;
			*borrado = true;
			return NULL;
		}

		if ( (raiz->derecha != NULL) && (raiz->izquierda != NULL) ) {
			nodo_abb_t* nodo_mayor = NULL;
			mayor_elemento_rama(raiz->izquierda, &nodo_mayor);
			if (nodo_mayor == NULL)
				return raiz;

			if (nodo_mayor != raiz->izquierda)
				nodo_mayor->izquierda = raiz->izquierda;

			nodo_mayor->derecha = raiz->derecha;
			destructor(raiz->elemento);
			free(raiz);
			raiz = nodo_mayor;
			*borrado = true;
			return raiz;
		}

		nodo_abb_t* nodo_aux = raiz->izquierda;
		if (raiz->derecha != NULL)
			nodo_aux = raiz->derecha;

		destructor(raiz->elemento);
		free(raiz);
		raiz = nodo_aux;
		*borrado = true;
		return raiz;
	}
	return raiz;
}

int arbol_borrar(abb_t* arbol, void* elemento) {

	if ( !arbol || !elemento || !(arbol->comparador) || !(arbol->destructor) )
		return ERROR;

	if (arbol->nodo_raiz == NULL)
		return ERROR;

	if (arbol->comparador(arbol->nodo_raiz->elemento, elemento) == IGUALES) {
		nodo_abb_t* nueva_raiz = borrar_raiz(arbol->nodo_raiz, arbol->destructor);
		if (nueva_raiz != arbol->nodo_raiz) {
			arbol->nodo_raiz = nueva_raiz;
			return EXITO;
		}
		return ERROR;
	}


	bool borrado = false;
	borrar_aux(arbol->nodo_raiz, elemento, arbol->comparador, arbol->destructor, &borrado);

	if (borrado)
		return EXITO;

	return ERROR;
}

void arbol_destruir(abb_t* arbol) {

	if (!arbol)
		return;
	if (!(arbol->nodo_raiz))
		return;

	void* elemento;

	while (!arbol_vacio(arbol)){
		elemento = arbol->nodo_raiz->elemento;
		arbol_borrar(arbol, elemento);
	}

	free(arbol);
}

void* arbol_raiz(abb_t* arbol) {

	if (!arbol) 
		return NULL;

	if (!(arbol->nodo_raiz))
		return NULL;

	return arbol->nodo_raiz->elemento;
}

bool arbol_vacio(abb_t* arbol) {
	return ( !arbol || !(arbol->nodo_raiz) );
}


size_t recorrido_indorden_aux(nodo_abb_t* raiz, void** array, size_t tamanio_array, size_t *contador) {
	if (!raiz)
		return 0;

	if ( (raiz->izquierda != NULL) && (*contador < tamanio_array) )
		recorrido_indorden_aux(raiz->izquierda, array, tamanio_array, contador);
	
	if (*contador < tamanio_array) {
		array[*contador] = raiz->elemento;
		(*contador)++;
	}

	if ((raiz->derecha != NULL) && (*contador < tamanio_array) )
		recorrido_indorden_aux(raiz->derecha, array, tamanio_array, contador);

	return *contador;
}

size_t arbol_recorrido_inorden(abb_t* arbol, void** array, size_t tamanio_array) {

	if (!arbol || !array)
		return 0;

	size_t contador = 0;
	return recorrido_indorden_aux(arbol->nodo_raiz, array, tamanio_array, &contador);
}

/*
 *
 *
 */
size_t recorrido_preorden_aux(nodo_abb_t* raiz, void** array, size_t tamanio_array, size_t *contador) {
	if (!raiz)
		return 0;

	if (*contador < tamanio_array) {
		array[*contador] = raiz->elemento;
		(*contador)++;
	}

	if ( (raiz->izquierda != NULL) && (*contador < tamanio_array) )
		recorrido_preorden_aux(raiz->izquierda, array, tamanio_array, contador);

	if ((raiz->derecha != NULL) && (*contador < tamanio_array) )
		recorrido_preorden_aux(raiz->derecha, array, tamanio_array, contador);

	return *contador;
}

size_t arbol_recorrido_preorden(abb_t* arbol, void** array, size_t tamanio_array) {
	if (!arbol || !array)
		return 0;

	size_t contador = 0;
	return recorrido_preorden_aux(arbol->nodo_raiz, array, tamanio_array, &contador);
}

/*
 *
 *
 */
size_t recorrido_postorden_aux(nodo_abb_t* raiz, void** array, size_t tamanio_array, size_t *contador) {
	if (!raiz)
		return 0;

	if ( (raiz->izquierda != NULL) && (*contador < tamanio_array) )
		recorrido_postorden_aux(raiz->izquierda, array, tamanio_array, contador);

	if ((raiz->derecha != NULL) && (*contador < tamanio_array) )
		recorrido_postorden_aux(raiz->derecha, array, tamanio_array, contador);

	if (*contador < tamanio_array) {
		array[*contador] = raiz->elemento;
		(*contador)++;
	}

	return *contador;
}

size_t arbol_recorrido_postorden(abb_t* arbol, void** array, size_t tamanio_array) {
	if (!arbol || !array)
		return 0;

	size_t contador = 0;
	return recorrido_postorden_aux(arbol->nodo_raiz, array, tamanio_array, &contador);
}

/*
 *
 *
 */
size_t iterar_inorden(nodo_abb_t* raiz, size_t *recorridos, bool (*funcion)(void*, void*), void* extra) {

	if (!raiz || !funcion)
		return 0;

	if ( (raiz->izquierda != NULL) && (funcion(raiz->izquierda->elemento, extra) == false) )
		iterar_inorden(raiz->izquierda, recorridos, funcion, extra);

	if (funcion(raiz->elemento, extra) == false)
		(*recorridos)++;

	if ( (raiz->derecha != NULL) && (funcion(raiz->derecha->elemento, extra) == false) ) 
		iterar_inorden(raiz->derecha, recorridos, funcion, extra);
	
	return *recorridos;
}

/*
size_t recorrido_indorden_aux(nodo_abb_t* raiz, void** array, size_t tamanio_array, size_t *contador) {
	if (!raiz)
		return 0;

	if ( (raiz->izquierda != NULL) && (*contador < tamanio_array) )
		recorrido_indorden_aux(raiz->izquierda, array, tamanio_array, contador);
	

	if (*contador < tamanio_array) {
		array[*contador] = raiz->elemento;
		(*contador)++;
	}

	if ((raiz->derecha != NULL) && (*contador < tamanio_array) )
		recorrido_indorden_aux(raiz->derecha, array, tamanio_array, contador);

	return *contador;
}*/

/*
 * Iterador interno. Recorre el arbol e invoca la funcion con cada
 * elemento del mismo. El puntero 'extra' se pasa como segundo
 * parámetro a la función. Si la función devuelve true, se finaliza el
 * recorrido aun si quedan elementos por recorrer. Si devuelve false
 * se sigue recorriendo mientras queden elementos.
 * El recorrido se realiza de acuerdo al recorrido solicitado.  Los
 * recorridos válidos son: ABB_RECORRER_INORDEN, ABB_RECORRER_PREORDEN
 * y ABB_RECORRER_POSTORDEN.
 * Devuelve la cantidad de elementos que fueron recorridos.
*/
size_t abb_con_cada_elemento(abb_t* arbol, int recorrido, bool (*funcion)(void*, void*), void* extra) {

	if (!arbol || !funcion)
		return 0;

	if ( (recorrido != ABB_RECORRER_INORDEN) && (recorrido != ABB_RECORRER_POSTORDEN) && (recorrido != ABB_RECORRER_PREORDEN) )
		return 0;

	size_t recorridos = 0;

	if (recorrido == ABB_RECORRER_INORDEN) {
		return iterar_inorden(arbol->nodo_raiz, &recorridos, funcion, extra);
	}

	return 0;
}