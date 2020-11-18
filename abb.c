#include "abb.h"
#include <stdio.h>

static const int EXITO = 0;
static const int ERROR = -1;
const int PRIMER_ELEMENTO_MAYOR = 1;
const int PRIMER_ELEMENTO_MENOR = -1;
const int IGUALES = 0;


abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor) {

	if ( (comparador == NULL) || (destructor == NULL) )
		return NULL;

	abb_t* arbol = malloc( sizeof(abb_t) );
	if (arbol == NULL)
		return NULL;
	arbol->nodo_raiz = NULL;
	arbol->comparador = comparador;
	arbol->destructor = destructor;

	return arbol;
}

/*
 * Pre: recibe el elemento con el que se inicializara el nodo
 * Post: devuelve un nodo con el elemento y NULL en los nodos izquiero y derecho
 *		 devuelve NULL en caso de error
 */
nodo_abb_t* crear_nodo_inicializado(void* elemento) {

	nodo_abb_t* nodo = malloc( sizeof(nodo_abb_t) );

	if (nodo != NULL) {
		nodo->elemento = elemento;
		nodo->derecha = NULL;
		nodo->izquierda = NULL;
	}

	return nodo;
}

/* 
 * Pre: comparador y elemento existen y nodo_actual es el nodo de un arbol valido
 * Post: devuelve 0 si pudo insertar o -1 en caso contrario
 */
int insertar_aux(abb_comparador comparador, void* elemento, nodo_abb_t* nodo_actual) {

	if (nodo_actual == NULL) {
		nodo_actual = crear_nodo_inicializado(elemento);
		if (nodo_actual == NULL)
			return ERROR;

		return EXITO;
	}

	int comparacion = comparador(nodo_actual->elemento, elemento);

	if ( (comparacion == PRIMER_ELEMENTO_MAYOR) || (comparacion == IGUALES) ) 
		return (insertar_aux(comparador, elemento, nodo_actual->izquierda));
	
	if ( comparacion == PRIMER_ELEMENTO_MENOR )
		return (insertar_aux(comparador, elemento, nodo_actual->derecha));

	return ERROR;
}


/*
 *
 *
 */
nodo_abb_t* recorrer_arbol(abb_comparador comparador, void* elemento, nodo_abb_t* nodo_actual) {

	if (nodo_actual == NULL)
		return nodo_actual;

	int comparacion = comparador(nodo_actual->elemento, elemento);

	if ( comparacion == PRIMER_ELEMENTO_MAYOR ) 
		return (recorrer_arbol(comparador, elemento, nodo_actual->izquierda));
	
	if ( comparacion == PRIMER_ELEMENTO_MENOR )
		return (recorrer_arbol(comparador, elemento, nodo_actual->derecha));

	if (comparacion == IGUALES)
		return nodo_actual;

	return NULL;
}

int arbol_insertar(abb_t* arbol, void* elemento) {

	if ( (arbol == NULL) || (elemento == NULL) || (arbol->comparador == NULL) )
		return ERROR;

	if (arbol->nodo_raiz == NULL) {
		arbol->nodo_raiz = crear_nodo_inicializado(elemento);
		if (arbol->nodo_raiz != NULL)
			return EXITO;
		return ERROR;
	}

	nodo_abb_t* nodo_padre = arbol->nodo_raiz;
	nodo_abb_t* nodo_actual = arbol->nodo_raiz;

	int comparacion = arbol->comparador(arbol->nodo_raiz->elemento, elemento);
	if ( (comparacion == PRIMER_ELEMENTO_MAYOR) ) 
		nodo_actual = nodo_padre->izquierda;
	else if ( comparacion == PRIMER_ELEMENTO_MENOR )
		nodo_actual = nodo_padre->derecha;

	while (nodo_actual != NULL) {

		comparacion = arbol->comparador(nodo_actual->elemento, elemento);
		if ( (comparacion == PRIMER_ELEMENTO_MAYOR) || (comparacion == IGUALES) ) {
			nodo_padre = nodo_actual;
			nodo_actual = nodo_actual->izquierda;
		}
		else if ( comparacion == PRIMER_ELEMENTO_MENOR ) {
			nodo_padre = nodo_actual;
			nodo_actual = nodo_actual->derecha;
		}
	}

	if ( (nodo_padre != NULL) && (nodo_actual == NULL) ) {
		if (arbol->comparador(nodo_padre->elemento, elemento) == PRIMER_ELEMENTO_MAYOR ) {
			nodo_padre->izquierda = crear_nodo_inicializado(elemento);
			if (nodo_padre->izquierda != NULL)
				return EXITO;
		} else {
			nodo_padre->derecha = crear_nodo_inicializado(elemento);
			if (nodo_padre->derecha != NULL)
				return EXITO;
		}
	}

	return ERROR;



	/*nodo_abb_t* nodo = recorrer_arbol(arbol->comparador, elemento, arbol->raiz);

	if (nodo == NULL) {
		nodo = crear_nodo_inicializado(elemento);
		if (nodo_actual == NULL)
			return ERROR;

		return EXITO;
	} else {
		nodo_abb_t* nuevo_nodo = crear_nodo_inicializado(elemento);
		if (nuevo_nodo == NULL)
			return ERROR;

		nuevo_nodo->derecha = nodo->derecha;
		nodo->derecha = nuevo_nodo;
		return EXITO;
	}*/

	//return (insertar_aux(arbol->comparador, elemento, arbol->nodo_raiz));
}

/*
 *
 *
 */
void* buscar_aux(abb_comparador comparador, void* elemento, nodo_abb_t* nodo_actual) {

	if (nodo_actual == NULL)
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

	if ( (arbol == NULL) || (elemento == NULL) || (arbol->comparador == NULL ) )
		return NULL;

	return buscar_aux(arbol->comparador, elemento, arbol->nodo_raiz);
}

/*
 *
 *
 */
int hijos_nodo(nodo_abb_t* nodo) {
	int hijos = 0;

	if (nodo == NULL)
		return 0;

	if (nodo->derecha != NULL)
		hijos++;

	if (nodo->izquierda != NULL)
		hijos++;

	return hijos;
}

/*
 * Busca en el arbol un elemento igual al provisto (utilizando la
 * funcion de comparación) y si lo encuentra lo quita del arbol.
 * Adicionalmente, si encuentra el elemento, invoca el destructor con
 * dicho elemento.
 * Devuelve 0 si pudo eliminar el elemento o -1 en caso contrario.
 */
int arbol_borrar(abb_t* arbol, void* elemento) {

	if ( (arbol == NULL) || (elemento == NULL) || (arbol->comparador == NULL) || (arbol->destructor == NULL) )
		return ERROR;

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


		/*
		nodo_abb_t* nodo_aux = nodo->izquierda;

		while (nodo_aux->izquierda != NULL) {
			nodo_aux = nodo_aux->izquierda;
		}

		nodo_abb_t
		while ()

		bool derecha = (nodo_padre->derecha == nodo);
		if (derecha) {
			if (nodo_aux->derecha == NULL)
				nodo_aux->derecha = nodo->derecha;
			else {
				nodo_abb_t* nodo_aux2 = nodo_aux->derecha;
				while (nodo_aux2->derecha != NULL) {
					nodo_aux2 = nodo_aux2->derecha;
				}
				if (nodo_aux2 == NULL)
					nodo_aux2->derecha = nodo->derecha;

			}
			nodo_padre->derecha = nodo_aux;
		}
		else
			nodo_padre->izquierda = nodo_aux;
		arbol->destructor(nodo->elemento);
		free(nodo);

		return EXITO;
	*/
	}

	return ERROR;

}

/*
 * Destruye el arbol liberando la memoria reservada por el mismo.
 * Adicionalmente invoca el destructor con cada elemento presente en
 * el arbol.
 */
void arbol_destruir(abb_t* arbol) {

	if (arbol == NULL)
		return;
	if (arbol->nodo_raiz == NULL)
		return;

	void* elemento;

	while (!arbol_vacio(arbol)){
		elemento = arbol->nodo_raiz->elemento;
		arbol_borrar(arbol, elemento);
	}

	free(arbol);
}


void* arbol_raiz(abb_t* arbol) {

	if ( (arbol == NULL) ) 
		return NULL;

	if (arbol->nodo_raiz == NULL)
		return NULL;

	return arbol->nodo_raiz->elemento;
}

/*
 * Determina si el árbol está vacío.
 * Devuelve true si está vacío o el arbol es NULL, false si el árbol tiene elementos.
 */
bool arbol_vacio(abb_t* arbol) {

	return ( (arbol == NULL) || (arbol->nodo_raiz == NULL) );
}