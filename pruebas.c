#include "abb.h"
#include "pa2m.h"
#include <stdio.h>
#include <string.h>

#define MAX_NOMBRE 20
const int ERROR = -1;
const int EXITO = 0;

typedef struct pokemon{
    int id;
    char nombre[MAX_NOMBRE];
}pokemon_t;


pokemon_t* crear_pokemon(int id, char nombre[MAX_NOMBRE]){

    pokemon_t* pokemon = (pokemon_t*)malloc(sizeof(pokemon_t));
    if (pokemon != NULL) {
    	pokemon->id = id;
    	strcpy(pokemon->nombre, nombre);
    }

    return pokemon;
}

int comparar_por_id(void* pokemon1, void* pokemon2){
    if(!pokemon1 || !pokemon2)
        return 0;

    if(((pokemon_t*)pokemon1)->id > ((pokemon_t*)pokemon2)->id)
        return 1;
    if(((pokemon_t*)pokemon1)->id < ((pokemon_t*)pokemon2)->id)
        return -1;
    return 0;
}

void destruir(pokemon_t* pokemon) {
	free(pokemon);
}

void pokemon_destruir(void* pokemon){
    if(!pokemon)
        return;
    destruir((pokemon_t*)pokemon);
}

void pruebas_creacion() {

	abb_comparador comparador = comparar_por_id;
	abb_liberar_elemento destructor = pokemon_destruir;
	abb_t* arbol;

	arbol = arbol_crear(comparador, NULL);
	pa2m_afirmar(arbol == NULL, "No se puede crear un arbol sin destructor");
	arbol = arbol_crear(NULL, destructor);
	pa2m_afirmar(arbol == NULL, "No se puede crear un arbol sin comparador");
	arbol = arbol_crear(comparador, destructor);
	pa2m_afirmar(arbol != NULL, "Se puede crear un arbol");
	pa2m_afirmar( arbol_vacio(arbol) == true, "El arbol esta vacio");
	pa2m_afirmar( arbol_raiz(arbol) == NULL, "El arbol no tiene raiz");
	pokemon_t* pokemon = crear_pokemon(67, "Machoke");
	pa2m_afirmar( arbol_buscar(arbol, (void*)pokemon) == NULL, "No se encuentra elemento en arbol vacio");

	pokemon_destruir(pokemon);
	arbol_destruir(arbol);
}	

void pruebas_insercion() {

	abb_comparador comparador = comparar_por_id;
	abb_liberar_elemento destructor = pokemon_destruir;

	abb_t* arbol = arbol_crear(comparador, destructor);

	pokemon_t* pokemon = crear_pokemon(67, "Machoke");
	pa2m_afirmar( arbol_insertar(arbol, pokemon) == EXITO, "Puedo insertar en un arbol vacio");
	pa2m_afirmar( arbol_raiz(arbol) == pokemon, "La raiz del arbol contiene al elemento insertado");
	pa2m_afirmar( (arbol->nodo_raiz->izquierda == NULL) && (arbol->nodo_raiz->derecha == NULL), "No tiene elementos a su izquierda ni derecha" );
	pa2m_afirmar( arbol_vacio(arbol) == false, "El arbol no esta vacio");
	pa2m_afirmar( arbol_buscar(arbol, pokemon) == pokemon, "Buscar el elemento insertado devuelve el elemento");

	pokemon_t* pokemon2 = crear_pokemon(52, "Meowth");
	pokemon_t* pokemon3 = crear_pokemon(78, "Rapidash");
	pa2m_afirmar( arbol_insertar(arbol, pokemon2) == EXITO, "Puedo insertar un elemento menor");
	pa2m_afirmar( arbol_insertar(arbol, pokemon3) == EXITO, "Puedo insertar un elemento mayor");
	pa2m_afirmar( (arbol->nodo_raiz->izquierda != NULL) && (arbol->nodo_raiz->izquierda->elemento == pokemon2), "El elemento menor esta a la izquierda");
	pa2m_afirmar( (arbol->nodo_raiz->derecha != NULL) && (arbol->nodo_raiz->derecha->elemento == pokemon3), "El elemento mayor esta a la derecha");
	pa2m_afirmar( (arbol->nodo_raiz->izquierda->izquierda == NULL) && (arbol->nodo_raiz->izquierda->derecha == NULL), "El nodo izquierdo no tiene hijos");
	pa2m_afirmar( (arbol->nodo_raiz->derecha->izquierda == NULL) && (arbol->nodo_raiz->derecha->derecha == NULL), "El nodo derecho no tiene hijos");

	arbol_destruir(arbol);
}

void pruebas_borrar() {

	abb_comparador comparador = comparar_por_id;
	abb_liberar_elemento destructor = pokemon_destruir;
	abb_t* arbol = arbol_crear(comparador, destructor);
	pokemon_t* pokemon = crear_pokemon(67, "Machoke");

	pa2m_afirmar( arbol_borrar(arbol, pokemon) == ERROR, "No puedo borrar de un arbol vacio");
	arbol_insertar(arbol, pokemon);
	pa2m_afirmar( arbol_borrar(arbol, pokemon) == EXITO , "Puedo borrar de un arbol con un elemento");
	pa2m_afirmar( arbol_vacio(arbol) == true, "El arbol quedo vacio");


	arbol_destruir(arbol);
}

int main() {

	pa2m_nuevo_grupo("PRUEBAS CREACION");
	pruebas_creacion();
	pa2m_nuevo_grupo("PRUEBAS INSERCION");
	pruebas_insercion();
	pa2m_nuevo_grupo("PRUEBAS BORRAR");
	pruebas_borrar();

	pa2m_mostrar_reporte();
}