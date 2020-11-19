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
 *
 *
 *
int hijos_nodo(nodo_abb_t* nodo) {
	int hijos = 0;

	if (!nodo)
		return 0;

	if (nodo->derecha != NULL)
		hijos++;

	if (nodo->izquierda != NULL)
		hijos++;

	return hijos;
}*/

/*
 *
 *
 */
nodo_abb_t* menor_elemento_rama(nodo_abb_t* raiz) {
	if (!raiz)
		return NULL;

	if (!(raiz->izquierda))
		return raiz;

	return(menor_elemento_rama(raiz->izquierda));
}

/*
 * Pre: la raiz existe y no es NULL
 * 
 */
nodo_abb_t* padre_mayor_elemento_rama(nodo_abb_t* raiz) {

	if (!raiz)
		return NULL;

	if (!(raiz->derecha))
		return NULL;

	if (raiz->derecha->derecha == NULL)
		return raiz;

	return padre_mayor_elemento_rama(raiz->derecha); 
}

nodo_abb_t* mayor_elemento_aux(nodo_abb_t* raiz) {

	if (!raiz)
		return NULL;

	if ( !(raiz->derecha) )
		return raiz;

	nodo_abb_t* padre_del_mayor = padre_mayor_elemento_rama(raiz);
	nodo_abb_t* nodo_aux = padre_del_mayor->derecha;
	padre_del_mayor->derecha = NULL;

	if ( (nodo_aux->izquierda != NULL) && (padre_del_mayor != raiz) ) {
		nodo_abb_t* nodo_izquierda = menor_elemento_rama(nodo_aux);
		nodo_izquierda->izquierda = padre_del_mayor;
	} else if (padre_del_mayor != raiz) {
		nodo_aux->izquierda = padre_del_mayor;
	}

	if (raiz->izquierda != NULL) {
		nodo_abb_t* nodo_menor = menor_elemento_rama(nodo_aux);
		nodo_menor->izquierda = raiz->izquierda;
	}

	return nodo_aux;
}


/*
 *
 *
 */
nodo_abb_t* borrar_aux(nodo_abb_t* raiz, void* elemento, abb_comparador comparador, abb_liberar_elemento destructor) {

	if (!raiz) 
		return NULL;

	int comparacion = comparador(raiz->elemento, elemento);

	if (comparacion == PRIMER_ELEMENTO_MAYOR)
		raiz->izquierda = borrar_aux(raiz->izquierda, elemento, comparador, destructor);
	else if (comparacion == PRIMER_ELEMENTO_MENOR)
		raiz->derecha = borrar_aux(raiz->derecha, elemento, comparador, destructor);
	else {

		if ( !(raiz->derecha) && !(raiz->izquierda) ) {
			destructor(raiz->elemento);
			free(raiz);
			return NULL;
		}

		if ( (raiz->derecha != NULL) && (raiz->izquierda != NULL) ) {
			nodo_abb_t* nodo_aux = mayor_elemento_aux(raiz->izquierda);
			nodo_aux->derecha = raiz->derecha;
			destructor(raiz->elemento);
			free(raiz);
			raiz = nodo_aux;
			return NULL;
		}

		nodo_abb_t* nodo_aux = raiz->izquierda;
		if (raiz->derecha != NULL)
			nodo_aux = raiz->derecha;

		destructor(raiz->elemento);
		free(raiz);
		raiz = nodo_aux;
		return NULL;
	}

	return raiz;
}

/*
 * Busca en el arbol un elemento igual al provisto (utilizando la
 * funcion de comparación) y si lo encuentra lo quita del arbol.
 * Adicionalmente, si encuentra el elemento, invoca el destructor con
 * dicho elemento.
 * Devuelve 0 si pudo eliminar el elemento o -1 en caso contrario.
 */
int arbol_borrar(abb_t* arbol, void* elemento) {

	if ( !arbol || !elemento || !(arbol->comparador) || !(arbol->destructor) )
		return ERROR;

	if (borrar_aux(arbol->nodo_raiz, elemento, arbol->comparador, arbol->destructor) == NULL )
		return EXITO;

	return ERROR;
	/*
	nodo_abb_t* nodo = arbol->nodo_raiz;
	nodo_abb_t* nodo_padre = NULL;
	if (nodo == NULL)
		return ERROR;
	int comparacion = arbol->comparador(nodo->elemento, elemento);

	if (comparacion == 0){

		if ( (nodo->izquierda == NULL) && (nodo->derecha == NULL) ) {
			arbol->destructor(nodo->elemento);
			free(nodo);
			arbol->nodo_raiz = NULL;
			return EXITO;
		}

		if (nodo->izquierda == NULL) {
			arbol->nodo_raiz = nodo->derecha;
			arbol->destructor(nodo->elemento);
			free(nodo);
			return EXITO;
		}

		if (nodo->derecha == NULL) {
			arbol->nodo_raiz = nodo->izquierda;
			arbol->destructor(nodo->elemento);
			free(nodo);
			return EXITO;
		}

		if (hijos_nodo(arbol->nodo_raiz->izquierda) == 0) {
			arbol->nodo_raiz->izquierda->derecha = arbol->nodo_raiz->derecha;
			arbol->nodo_raiz = arbol->nodo_raiz->izquierda;
			arbol->destructor(nodo->elemento);
			free(nodo);
			return EXITO;
		}

		if (hijos_nodo(arbol->nodo_raiz->derecha) == 0) {
			arbol->nodo_raiz->derecha->izquierda = arbol->nodo_raiz->izquierda;
			arbol->nodo_raiz = arbol->nodo_raiz->derecha;
			arbol->destructor(nodo->elemento);
			free(nodo);
			return EXITO;
		}

		nodo_abb_t* nodo_padre = nodo;
		nodo_abb_t* nodo_aux = nodo->izquierda;

		while (nodo_aux->derecha != NULL) {
			nodo_padre = nodo_aux;
			nodo_aux = nodo_aux->derecha;
		}

		if (nodo_padre != arbol->nodo_raiz)
			nodo_padre->derecha = NULL;
		nodo_aux->derecha = nodo->derecha;
		nodo_aux->izquierda = nodo->izquierda;

		arbol->destructor(nodo->elemento);
		free(nodo);
		arbol->nodo_raiz = nodo_aux;

		return EXITO;

	} else if ( comparacion == PRIMER_ELEMENTO_MAYOR ){
		nodo_padre = nodo;
		nodo = nodo->izquierda; 
	} else if ( comparacion == PRIMER_ELEMENTO_MENOR ){
		nodo_padre = nodo;
		nodo = nodo->derecha;
	}

	comparacion = arbol->comparador(nodo->elemento, elemento);
	while ( (nodo != NULL) && (comparacion != IGUALES) ) {

		nodo_padre = nodo;
		if ( comparacion == PRIMER_ELEMENTO_MAYOR )
			nodo = nodo->izquierda; 
		else if ( comparacion == PRIMER_ELEMENTO_MENOR )
		 	nodo = nodo->derecha;
		comparacion = arbol->comparador(nodo->elemento, elemento);
	}

	if (nodo == NULL)
		return ERROR;

	if (hijos_nodo(nodo) == 0) {
		bool derecha = (nodo_padre->derecha == nodo);
		arbol->destructor(nodo->elemento);
		free(nodo);
		if (derecha)
			nodo_padre->derecha = NULL;
		else
			nodo_padre->izquierda = NULL;

		return EXITO;
	}
	else if (hijos_nodo(nodo) == 1) {
		bool derecha = (nodo_padre->derecha == nodo);
		nodo_abb_t* nodo_hijo = nodo->derecha;
		if (nodo_hijo == NULL)
			nodo_hijo = nodo->izquierda;
		arbol->destructor(nodo->elemento);
		free(nodo);

		if (derecha)
			nodo_padre->derecha = nodo_hijo;
		else
			nodo_padre->izquierda = nodo_hijo;

		return EXITO;
	}
	else if (hijos_nodo(nodo) == 2) {

		nodo_abb_t* nodo_padre_aux = nodo->izquierda;
		if (nodo_padre_aux->derecha == NULL){
			nodo_abb_t* nodo_aux = nodo->izquierda;
			nodo_aux->derecha = nodo->derecha ;

			if (nodo_padre->derecha == nodo)
				nodo_padre->derecha = nodo_aux;
			else
				nodo_padre->izquierda = nodo_aux;

			arbol->destructor(nodo->elemento);
			free(nodo);

			return EXITO;
		}


		nodo_abb_t* nodo_aux = nodo_padre_aux->derecha;

		while (nodo_aux->derecha != NULL) {
			nodo_padre_aux = nodo_aux;
			nodo_aux = nodo_aux->derecha;
		}

		nodo_padre_aux->derecha = NULL;
		nodo_aux->izquierda = nodo->izquierda;
		nodo_aux->derecha = nodo->derecha;

		if (nodo_padre->derecha == nodo)
			nodo_padre->derecha = nodo_aux;
		else
			nodo_padre->izquierda = nodo_aux;

		arbol->destructor(nodo->elemento);
		free(nodo);

		return EXITO;
	}

	return ERROR;*/
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


size_t recorrido_indorden_aux(nodo_abb_t* raiz, void** array, size_t tamanio_array, size_t contador) {

	if (!raiz)
		return 0;

	if ((raiz->izquierda != NULL) && (contador < tamanio_array) ) {
		array[contador] = raiz->izquierda->elemento;
		contador++;
		contador += recorrido_indorden_aux(raiz->izquierda, array, tamanio_array, contador);
	}

	if (contador < tamanio_array) {
		array[contador] = raiz->elemento;
		contador++;
	}

	if ((raiz->derecha != NULL) && (contador < tamanio_array) ) {
		array[contador] = raiz->derecha->elemento;
		contador++;
		contador += recorrido_indorden_aux(raiz->derecha, array, tamanio_array, contador);
	}

	return contador;
}

/*
 * Llena el array del tamaño dado con los elementos de arbol
 * en secuencia inorden.
 * Devuelve la cantidad de elementos del array que pudo llenar (si el
 * espacio en el array no alcanza para almacenar todos los elementos,
 * llena hasta donde puede y devuelve la cantidad de elementos que
 * pudo poner).
 */
size_t arbol_recorrido_inorden(abb_t* arbol, void** array, size_t tamanio_array) {

	if (!arbol || !array)
		return 0;

	return recorrido_indorden_aux(arbol->nodo_raiz, array, tamanio_array, 0);
}

/*
 * Llena el array del tamaño dado con los elementos de arbol
 * en secuencia preorden.
 * Devuelve la cantidad de elementos del array que pudo llenar (si el
 * espacio en el array no alcanza para almacenar todos los elementos,
 * llena hasta donde puede y devuelve la cantidad de elementos que
 * pudo poner).
 */
size_t arbol_recorrido_preorden(abb_t* arbol, void** array, size_t tamanio_array);

/*
 * Llena el array del tamaño dado con los elementos de arbol
 * en secuencia postorden.
 * Devuelve la cantidad de elementos del array que pudo llenar (si el
 * espacio en el array no alcanza para almacenar todos los elementos,
 * llena hasta donde puede y devuelve la cantidad de elementos que
 * pudo poner).
 */
size_t arbol_recorrido_postorden(abb_t* arbol, void** array, size_t tamanio_array);
