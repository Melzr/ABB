#include "abb.h"

static const int EXITO = 0;
static const int ERROR = -1;
static const int PRIMER_ELEMENTO_MAYOR = 1;
static const int PRIMER_ELEMENTO_MENOR = -1;
static const int IGUALES = 0;


abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor) {

	if (!comparador)
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
 * Pre: recibe la raiz y comparador de un arbol validos, y el elemento a insertar
 * Post: inserta el elemento manteniendo el orden y devuelve el puntero a la raiz del arbol con el elemento
 */
nodo_abb_t* insertar_aux(nodo_abb_t* raiz, void* elemento, abb_comparador comparador) {

	if (!raiz) {
		raiz = crear_nodo_inicializado(elemento);
		return raiz;
	}

	int comparacion = comparador(raiz->elemento, elemento);
	if (comparacion <= PRIMER_ELEMENTO_MENOR)
		raiz->derecha = insertar_aux(raiz->derecha, elemento, comparador);
	else if (comparacion >= IGUALES)
		raiz->izquierda = insertar_aux(raiz->izquierda, elemento, comparador);

	return raiz; 
}

int arbol_insertar(abb_t* arbol, void* elemento) {

	if ( !arbol || !(arbol->comparador) )
		return ERROR;

	arbol->nodo_raiz = insertar_aux(arbol->nodo_raiz, elemento, arbol->comparador);

	return EXITO;
}

/*
 * Pre: recibe la raiz y el comparador de un arbol valido, y el elemento que se busca
 * Post: devuelve un puntero al elemento buscado en el arbol o NULL en caso de no encontrarse
 */
void* buscar_aux(abb_comparador comparador, void* elemento, nodo_abb_t* raiz) {

	if (!raiz)
		return NULL;

	int comparacion = comparador(raiz->elemento, elemento);

	if ( comparacion >= PRIMER_ELEMENTO_MAYOR ) 
		return (buscar_aux(comparador, elemento, raiz->izquierda));
	
	if ( comparacion <= PRIMER_ELEMENTO_MENOR )
		return (buscar_aux(comparador, elemento, raiz->derecha));

	if (comparacion == IGUALES) {
		return raiz->elemento;
	}

	return NULL;
}

void* arbol_buscar(abb_t* arbol, void* elemento) {

	if (!arbol || !elemento || !(arbol->comparador) )
		return NULL;

	return buscar_aux(arbol->comparador, elemento, arbol->nodo_raiz);
}

/*
 * Pre: recibe una raiz valida a partir de la cual se quiere conocer el elemento mayor de su rama (su predecesor inorden)
 *		recibe un puntero a nodo que se llenara con el nodo que tenga el mayor elemento de la rama (el que este mas a la derecha)
 * Post: llena el puntero a nodo_mayor y su padre queda apuntando a NULL o a su hijo menor en casod e que tenga
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
 * Pre: recibe la raiz y el destructor de un arbol
 * Post: borra la raiz del arbol y devuelve un puntero a la nueva raiz
 */
nodo_abb_t* borrar_raiz(nodo_abb_t* raiz, abb_liberar_elemento destructor) {

	if (!raiz)
		return NULL;

	if (!raiz->izquierda && !raiz->derecha) {
		if(destructor != NULL) destructor(raiz->elemento);
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
		if(destructor != NULL) destructor(raiz->elemento);
		free(raiz);
		raiz = nodo_mayor;
		return raiz;
	}

	nodo_abb_t* nodo_aux = raiz->izquierda;
	if (raiz->derecha != NULL)
		nodo_aux = raiz->derecha;

	if(destructor != NULL) destructor(raiz->elemento);
	free(raiz);
	raiz = nodo_aux;
	return raiz;
}

/*
 * Pre: recibe la raiz, comparador y destructor de un arbol, el elemento a eliminar y un puntero a un booleano false
 *		el elemento a borrar NO PUEDE ESTAR EN LA RAIZ DEL ARBOL
 * Post: borra el elemento del arbol, se mantiene el puntero a la raiz del mismo (sigue valiendo arbol->nodo_raiz)
 */
nodo_abb_t* borrar_aux(nodo_abb_t* raiz, void* elemento, abb_comparador comparador, abb_liberar_elemento destructor, bool* borrado) {

	if (!raiz || !comparador ) 
		return NULL;

	int comparacion = comparador(raiz->elemento, elemento);

	if (comparacion >= PRIMER_ELEMENTO_MAYOR)
		raiz->izquierda = borrar_aux(raiz->izquierda, elemento, comparador, destructor, borrado);
	else if (comparacion <= PRIMER_ELEMENTO_MENOR)
		raiz->derecha = borrar_aux(raiz->derecha, elemento, comparador, destructor, borrado);
	else {

		if ( !(raiz->derecha) && !(raiz->izquierda) ) {
			if(destructor != NULL) destructor(raiz->elemento);
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
			if(destructor != NULL) destructor(raiz->elemento);
			free(raiz);
			raiz = nodo_mayor;
			*borrado = true;
			return raiz;
		}

		nodo_abb_t* nodo_aux = raiz->izquierda;
		if (raiz->derecha != NULL)
			nodo_aux = raiz->derecha;

		if(destructor != NULL) destructor(raiz->elemento);
		free(raiz);
		raiz = nodo_aux;
		*borrado = true;
		return raiz;
	}
	return raiz;
}

int arbol_borrar(abb_t* arbol, void* elemento) {

	if ( !arbol || !(arbol->comparador) )
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

/*
 * Pre: recibe el nodo raiz y el destructor del arbol
 * Post: destruye todos los elementos y nodos del arbol
 */
void destruir_aux(nodo_abb_t* nodo, abb_liberar_elemento destructor) {

	if (!nodo) return;

	if (nodo->izquierda != NULL)
		destruir_aux(nodo->izquierda, destructor);

	if (nodo->derecha != NULL)
		destruir_aux(nodo->derecha, destructor);

	if(destructor != NULL) destructor(nodo->elemento);
	free(nodo);
}

void arbol_destruir(abb_t* arbol) {

	if (!arbol) return;

	destruir_aux(arbol->nodo_raiz, arbol->destructor);
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



	/**************************************** RECORRER ****************************************/



/*
 * Pre: recibe la raiz de un arbol, un array con su tamanio validos, y un puntero a contador inicialmente en 0
 * Post: recorre inorden los elementos del arbol y devuelve la cantidad de nodos recorridos
 */
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
 * Pre: recibe la raiz de un arbol, un array con su tamanio validos, y un puntero a contador inicialmente en 0
 * Post: recorre preorden los elementos del arbol y devuelve la cantidad de nodos recorridos
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
 * Pre: recibe la raiz de un arbol, un array con su tamanio validos, y un puntero a contador inicialmente en 0
 * Post: recorre postorden los elementos del arbol y devuelve la cantidad de nodos recorridos
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



	/**************************************** ITERADOR ****************************************/



/*
 * Pre: recibe la raiz de un arbol, un puntero a recorridos inicialmente en 0, una funcion no nula que determine hasta donde iterar,
 *		su parametro extra, y un puntero *parar inicialmente false
 * Post: devuelve la cantidad de veces que se pudo iterar inorden
 */
size_t iterar_inorden(nodo_abb_t* raiz, size_t *recorridos, bool (*funcion)(void*, void*), void* extra, bool* parar) {

	if (!raiz || !funcion)
		return 0;

	if (raiz->izquierda != NULL)
		iterar_inorden(raiz->izquierda, recorridos, funcion, extra, parar);

	if ((*parar) == false)
		(*recorridos)++;

	if ( (*parar == false) && funcion(raiz->elemento, extra) == true) {
		*parar = true;
		return *recorridos;
	}

	if (raiz->derecha != NULL) 
		iterar_inorden(raiz->derecha, recorridos, funcion, extra, parar);
	
	return *recorridos;
}

/*
 * Pre: recibe la raiz de un arbol, un puntero a recorridos inicialmente en 0, una funcion no nula que determine hasta donde iterar,
 *		su parametro extra, y un puntero *parar inicialmente false
 * Post: devuelve la cantidad de veces que se pudo iterar preorden
 */
size_t iterar_preorden(nodo_abb_t* raiz, size_t *recorridos, bool (*funcion)(void*, void*), void* extra, bool* parar) {
	if (!raiz || !funcion)
		return 0;

	if ((*parar) == false)
		(*recorridos)++;

	if ( (*parar == false) && funcion(raiz->elemento, extra) == true) {
		*parar = true;
		return *recorridos;
	}

	if (raiz->izquierda != NULL)
		iterar_preorden(raiz->izquierda, recorridos, funcion, extra, parar);

	if (raiz->derecha != NULL) 
		iterar_preorden(raiz->derecha, recorridos, funcion, extra, parar);

	return *recorridos;
}

/*
 * Pre: recibe la raiz de un arbol, un puntero a recorridos inicialmente en 0, una funcion no nula que determine hasta donde iterar,
 *		su parametro extra, y un puntero *parar inicialmente false
 * Post: devuelve la cantidad de veces que se pudo iterar postorden
 */
size_t iterar_postorden(nodo_abb_t* raiz, size_t *recorridos, bool (*funcion)(void*, void*), void* extra, bool* parar) {

	if (!raiz || !funcion)
		return 0;

	if (raiz->izquierda != NULL)
		iterar_postorden(raiz->izquierda, recorridos, funcion, extra, parar);

	if (raiz->derecha != NULL) 
		iterar_postorden(raiz->derecha, recorridos, funcion, extra, parar);

	if ((*parar) == false)
		(*recorridos)++;

	if ( (*parar == false) && funcion(raiz->elemento, extra) == true) {
		*parar = true;
		return *recorridos;
	}

	return (*recorridos);
}

size_t abb_con_cada_elemento(abb_t* arbol, int recorrido, bool (*funcion)(void*, void*), void* extra) {

	if (!arbol || !funcion)
		return 0;

	if ( (recorrido != ABB_RECORRER_INORDEN) && (recorrido != ABB_RECORRER_POSTORDEN) && (recorrido != ABB_RECORRER_PREORDEN) )
		return 0;

	size_t recorridos = 0;
	bool parar = false;

	if (recorrido == ABB_RECORRER_INORDEN)
		return iterar_inorden(arbol->nodo_raiz, &recorridos, funcion, extra, &parar);

	if (recorrido == ABB_RECORRER_PREORDEN)
		return iterar_preorden(arbol->nodo_raiz, &recorridos, funcion, extra, &parar);

	if (recorrido == ABB_RECORRER_POSTORDEN)
		return iterar_postorden(arbol->nodo_raiz, &recorridos, funcion, extra, &parar);
	
	return 0;
}