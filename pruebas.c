#include "abb.h"
#include "pa2m.h"
#include <stdio.h>
#include <string.h>

#define MAX_NOMBRE 20
const int ERROR = -1;
const int EXITO = 0;
const size_t CANTIDAD_POKEMONES = 10;
const int IGUALES = 0;
const int PRIMERO_MAYOR = 1;
const int SEGUNDO_MAYOR = -1;

typedef struct pokemon{
    int id;
    char nombre[MAX_NOMBRE];
}pokemon_t;


/*
 * Devuelve un pokemon_t creado en memoria dinamica
 */
pokemon_t* crear_pokemon(int id, char nombre[MAX_NOMBRE]){

    pokemon_t* pokemon = (pokemon_t*)malloc(sizeof(pokemon_t));
    if (pokemon != NULL) {
    	pokemon->id = id;
    	strcpy(pokemon->nombre, nombre);
    }

    return pokemon;
}

/*
 * Pre: recibe dos pokemon_t* en forma de void*
 * Post: devuelve 0 si el id de ambos es igual, 1 si el id del primero es mayor o -1 si el id del segundo es mayor
 */
int comparar_por_id(void* pokemon1, void* pokemon2){
    if (!pokemon1 || !pokemon2)
        return IGUALES;

    if (((pokemon_t*)pokemon1)->id > ((pokemon_t*)pokemon2)->id)
        return PRIMERO_MAYOR;
    if (((pokemon_t*)pokemon1)->id < ((pokemon_t*)pokemon2)->id)
        return SEGUNDO_MAYOR;

    return IGUALES;
}

void destruir(pokemon_t* pokemon) {
	free(pokemon);
}

void pokemon_destruir(void* pokemon){
    if (!pokemon)
        return;
    destruir((pokemon_t*)pokemon);
}

bool es_meowth(void* elemento, void* extra){
    extra=extra;
    if (elemento) {
        if ( strcmp( ((pokemon_t*)elemento)->nombre, "Meowth") == 0)
            return true;
    }
    return false;
}

bool es_psyduck(void* elemento, void* extra){
    extra=extra;
    if (elemento) {
        if (strcmp( ((pokemon_t*)elemento)->nombre, "Psyduck") == 0)
            return true;
    }
    return false;
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

	abb_t* arbol = arbol_crear(comparar_por_id, pokemon_destruir);

	pokemon_t* pokemon = crear_pokemon(67, "Machoke");
	pa2m_afirmar( arbol_insertar(NULL, pokemon) == ERROR, "No puedo insertar en un arbol nulo");
	pa2m_afirmar( arbol_insertar(arbol, pokemon) == EXITO, "Puedo insertar en un arbol vacio");
	pa2m_afirmar( arbol_raiz(arbol) == pokemon, "La raiz del arbol contiene al elemento insertado");
	pa2m_afirmar( (arbol->nodo_raiz->izquierda == NULL) && (arbol->nodo_raiz->derecha == NULL), "No tiene elementos a su izquierda ni derecha" );
	pa2m_afirmar( arbol_vacio(arbol) == false, "El arbol no esta vacio");
	pa2m_afirmar( arbol_buscar(arbol, pokemon) == pokemon, "Buscar el elemento insertado devuelve el elemento");

	pokemon_t* pokemon2 = crear_pokemon(52, "Meowth");
	pokemon_t* pokemon3 = crear_pokemon(78, "Rapidash");
	pa2m_afirmar( arbol_buscar(arbol, pokemon2) == NULL, "No se encuentra un elemento que no fue insertado" );
	pa2m_afirmar( arbol_insertar(arbol, pokemon2) == EXITO, "Puedo insertar un elemento menor");
	pa2m_afirmar( arbol_insertar(arbol, pokemon3) == EXITO, "Puedo insertar un elemento mayor");
	pa2m_afirmar( (arbol->nodo_raiz->izquierda != NULL) && (arbol->nodo_raiz->izquierda->elemento == pokemon2), "El elemento menor esta a la izquierda");
	pa2m_afirmar( (arbol->nodo_raiz->derecha != NULL) && (arbol->nodo_raiz->derecha->elemento == pokemon3), "El elemento mayor esta a la derecha");
	pa2m_afirmar( (arbol->nodo_raiz->izquierda->izquierda == NULL) && (arbol->nodo_raiz->izquierda->derecha == NULL), "El nodo izquierdo no tiene hijos");
	pa2m_afirmar( (arbol->nodo_raiz->derecha->izquierda == NULL) && (arbol->nodo_raiz->derecha->derecha == NULL), "El nodo derecho no tiene hijos");
	pokemon_t* pokemon4 = crear_pokemon(78, "Rapidash");
	pa2m_afirmar( arbol_insertar(arbol, pokemon4) == EXITO, "Puedo insertar un elemento repetido");
	pa2m_afirmar( arbol->nodo_raiz->derecha->izquierda->elemento == pokemon4, "El elemento se inserto a la izquierda de su repetido");
	pa2m_afirmar( arbol_insertar(arbol, NULL) == EXITO, "Puedo insertar un elemento nulo");

	arbol_destruir(arbol);
}

void pruebas_borrar() {

	abb_t* arbol = arbol_crear(comparar_por_id, pokemon_destruir);
	pokemon_t* pokemon = crear_pokemon(67, "Machoke");

	pa2m_afirmar( arbol_borrar(NULL, pokemon) == ERROR, "No puedo borrar de un arbol nulo");
	pa2m_afirmar( arbol_borrar(arbol, pokemon) == ERROR, "No puedo borrar de un arbol vacio");
	arbol_insertar(arbol, pokemon);
	pa2m_afirmar( arbol_borrar(arbol, pokemon) == EXITO , "Puedo borrar de un arbol con un elemento");
	pa2m_afirmar( arbol_vacio(arbol) == true, "El arbol quedo vacio");

	pokemon = crear_pokemon(67, "Machoke");
	pokemon_t* pokemon2 = crear_pokemon(52, "Meowth");
	arbol_insertar(arbol, pokemon);
	arbol_insertar(arbol, pokemon2);

	pa2m_afirmar( arbol_borrar(arbol, pokemon2) == EXITO, "Puedo borrar una hoja de un arbol con dos elementos");
	pokemon2 = crear_pokemon(52, "Meowth");
	pa2m_afirmar( arbol_buscar(arbol, pokemon2) == NULL, "No se encuentra el elemento eliminado" );
	pokemon_destruir(pokemon2);
	pa2m_afirmar( (arbol->nodo_raiz->izquierda == NULL) && (arbol->nodo_raiz->derecha == NULL), "La raiz del arbol quedo sin hijos");

	pokemon2 = crear_pokemon(52, "Meowth");
	pokemon_t* pokemon3 = crear_pokemon(25, "Pikachu");
	arbol_insertar(arbol, pokemon2);
	arbol_insertar(arbol, pokemon3);

	pa2m_afirmar( arbol_borrar(arbol, pokemon2) == EXITO, "Puedo borrar un nodo con un hijo de un abrol con 3 elementos");
	pokemon2 = crear_pokemon(52, "Meowth");
	pa2m_afirmar( arbol_buscar(arbol, pokemon2) == NULL, "No se encuentra el elemento eliminado" );
	pokemon_destruir(pokemon2);
	pa2m_afirmar( arbol->nodo_raiz->izquierda->elemento == pokemon3, "La raiz tiene de hijo al hijo del nodo borrado");

	pokemon2 = crear_pokemon(52, "Meowth");
	pokemon_t* pokemon4 = crear_pokemon(6, "Charizard");
	arbol_insertar(arbol, pokemon2);
	arbol_insertar(arbol, pokemon4);

	pa2m_afirmar( arbol_borrar(arbol, pokemon3) == EXITO, "Puedo borrar un nodo con dos hijos de un abrol con 4 elementos");
	pokemon3 = crear_pokemon(25, "Pikachu");
	pa2m_afirmar( arbol_buscar(arbol, pokemon3) == NULL, "No se encuentra el elemento eliminado" );
	pokemon_destruir(pokemon3);
	pa2m_afirmar( arbol->nodo_raiz->izquierda->elemento == pokemon4, "El hijo menor del elemento borrado ahora ocupa su lugar");
	pa2m_afirmar( arbol->nodo_raiz->izquierda->derecha->elemento == pokemon2, "El hijo mayor del nodo borrado ahora es hijo del nodo que ocupo su lugar");

	pokemon3 = crear_pokemon(78, "Rapidash");
	arbol_insertar(arbol, pokemon3);
	pa2m_afirmar ( arbol_borrar(arbol, pokemon) == EXITO, "Puedo borrar la raiz del arbol con dos hijos");
	pokemon = crear_pokemon(67, "Machoke");
	pa2m_afirmar( arbol_buscar(arbol, pokemon) == NULL, "No se encuentra el elemento eliminado" );
	pokemon_destruir(pokemon);
	pa2m_afirmar( arbol_raiz(arbol) == pokemon2, "La raiz es ahora el mayor de los hijos menores de la raiz borrada");
	pa2m_afirmar( arbol->nodo_raiz->izquierda->elemento == pokemon4, "El hijo menor de la nueva raiz es el que anteriormente era su padre");
	pa2m_afirmar( arbol->nodo_raiz->derecha->elemento == pokemon3, "El hijo mayor de la nueva raiz es el hijo mayor de la anterior raiz");
	
	pokemon = crear_pokemon(78, "Rapidash");
	arbol_insertar(arbol, pokemon);
	pa2m_afirmar ( arbol_borrar(arbol, pokemon) == EXITO, "Puedo borrar un elemento que esta repetido");
	pa2m_afirmar ( arbol_buscar(arbol, pokemon) == pokemon, "El repetido se puede encontrar en el arbol");

	arbol_destruir(arbol);
}

/*
 * Llena un vector con 10 pokemones arbitrarios
 */
void llenar_pokemones(pokemon_t* pokemones[CANTIDAD_POKEMONES]) {

	pokemones[0] = crear_pokemon(67, "Machoke");
	pokemones[1] = crear_pokemon(25, "Pikachu");
	pokemones[2] = crear_pokemon(52, "Meowth");
	pokemones[3] = crear_pokemon(6, "Charizard");
	pokemones[4] = crear_pokemon(113, "Chansey");
	pokemones[5] = crear_pokemon(78, "Rapidash");
	pokemones[6] = crear_pokemon(129, "Magikarp");
	pokemones[7] = crear_pokemon(124, "Jynx");
	pokemones[8] = crear_pokemon(10, "Caterpie");
	pokemones[9] = crear_pokemon(41, "Zubat");
}

/*
 * Devuelve true si todos los elementos de los vectores son iguales
 */
bool vectores_iguales(size_t tamanio, void* vector1[], void* vector2[]) {
	bool distintos = false;
	int i = 0;
	while ( (i < tamanio) && !distintos) {
		if (vector1[i] != vector2[i])
			distintos = true;
		i++;	
	}

	return !distintos;
}

void pruebas_recorridos() {

	abb_t* arbol = arbol_crear(comparar_por_id, pokemon_destruir);
	pokemon_t* pokemones[CANTIDAD_POKEMONES];
	llenar_pokemones(pokemones);

	pokemon_t* pokemones_recorridos[10];
	pokemon_t* pokemones_inorden[10] = {pokemones[3], pokemones[8], pokemones[1], pokemones[9], pokemones[2], pokemones[0], pokemones[5], pokemones[4], pokemones[7], pokemones[6]};
	pokemon_t* pokemones_preorden[10] = {pokemones[0], pokemones[1], pokemones[3], pokemones[8], pokemones[2], pokemones[9], pokemones[4], pokemones[5], pokemones[6], pokemones[7]};
	pokemon_t* pokemones_postorden[10] = {pokemones[8], pokemones[3], pokemones[9], pokemones[2], pokemones[1], pokemones[5], pokemones[7], pokemones[6], pokemones[4], pokemones[0]};

	for (int i = 0; i < CANTIDAD_POKEMONES; i++) {
		arbol_insertar(arbol, pokemones[i]);
	}

	pa2m_afirmar( arbol_recorrido_inorden(arbol, (void**)pokemones_recorridos, CANTIDAD_POKEMONES) == CANTIDAD_POKEMONES, 
		"Recorrido inorden llena un vector con todos los elementos del arbol");
	pa2m_afirmar( vectores_iguales(CANTIDAD_POKEMONES, (void**)pokemones_recorridos, (void**)pokemones_inorden), 
		"Vector llenado correctamente");
	pa2m_afirmar( arbol_recorrido_preorden(arbol, (void**)pokemones_recorridos, CANTIDAD_POKEMONES) == CANTIDAD_POKEMONES, 
		"Recorrido preorden llena un vector con todos los elementos del arbol");
	pa2m_afirmar( vectores_iguales(CANTIDAD_POKEMONES, (void**)pokemones_recorridos, (void**)pokemones_preorden), 
		"Vector llenado correctamente");
	pa2m_afirmar( arbol_recorrido_postorden(arbol, (void**)pokemones_recorridos, CANTIDAD_POKEMONES) == CANTIDAD_POKEMONES, 
		"Recorrido postorden llena un vector con todos los elementos del arbol");
	pa2m_afirmar( vectores_iguales(CANTIDAD_POKEMONES, (void**)pokemones_recorridos, (void**)pokemones_postorden), 
		"Vector llenado correctamente");

	pa2m_afirmar( arbol_recorrido_inorden(arbol, (void**)pokemones_recorridos, 5) == 5, 
		"Recorrido inorden recibe un vector mas chico que todos los elementos del arbol, lo llena");
	pa2m_afirmar( vectores_iguales(5, (void**)pokemones_recorridos, (void**)pokemones_inorden), "Vector llenado correctamente");
	pa2m_afirmar( arbol_recorrido_preorden(arbol, (void**)pokemones_recorridos, 5) == 5, 
		"Recorrido preorden recibe un vector mas chico que todos los elementos del arbol, lo llena");
	pa2m_afirmar( vectores_iguales(5, (void**)pokemones_recorridos, (void**)pokemones_preorden), "Vector llenado correctamente");
	pa2m_afirmar( arbol_recorrido_postorden(arbol, (void**)pokemones_recorridos, 5) == 5, 
		"Recorrido postorden recibe un vector mas chico que todos los elementos del arbol, lo llena");
	pa2m_afirmar( vectores_iguales(5, (void**)pokemones_recorridos, (void**)pokemones_postorden), "Vector llenado correctamente");	

	pa2m_afirmar( arbol_recorrido_inorden(NULL, (void**)pokemones_recorridos, CANTIDAD_POKEMONES) == 0, 
		"Recorrido inorden recibe un arbol nulo, devuelve 0");
	pa2m_afirmar( arbol_recorrido_preorden(NULL, (void**)pokemones_recorridos, CANTIDAD_POKEMONES) == 0, 
		"Recorrido preorden recibe un arbol nulo, devuelve 0");
	pa2m_afirmar( arbol_recorrido_postorden(NULL, (void**)pokemones_recorridos, CANTIDAD_POKEMONES) == 0, 
		"Recorrido postorden recibe un arbol nulo, devuelve 0");

	pa2m_afirmar( arbol_recorrido_inorden(arbol, NULL, CANTIDAD_POKEMONES) == 0, "Recorrido inorden recibe un array nulo, devuelve 0");
	pa2m_afirmar( arbol_recorrido_preorden(arbol, NULL, CANTIDAD_POKEMONES) == 0, "Recorrido preorden recibe un array nulo, devuelve 0");
	pa2m_afirmar( arbol_recorrido_postorden(arbol, NULL, CANTIDAD_POKEMONES) == 0, "Recorrido postorden recibe un array nulo, devuelve 0");

	arbol_destruir(arbol);
}

void pruebas_iterador() {

	abb_t* arbol = arbol_crear(comparar_por_id, pokemon_destruir);
	pokemon_t* pokemones[CANTIDAD_POKEMONES];
	llenar_pokemones(pokemones);

	for (int i = 0; i < CANTIDAD_POKEMONES; i++) {
		arbol_insertar(arbol, pokemones[i]);
	}

	pa2m_afirmar( abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, es_meowth, NULL) == 4, "Iterador inorden recorre hasta que la funcion sea true");
	pa2m_afirmar( abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, es_meowth, NULL) == 4, "Iterador preorden recorre hasta que la funcion sea true");
	pa2m_afirmar( abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, es_meowth, NULL) == 3, "Iterador postorden recorre hasta que la funcion sea true");

	pa2m_afirmar( abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, es_psyduck, NULL) == 10, "Iterador inorden recorre todos los elementos");
	pa2m_afirmar( abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, es_psyduck, NULL) == 10, "Iterador preorden recorre todos los elementos");
	pa2m_afirmar( abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, es_psyduck, NULL) == 10, "Iterador postorden recorre todos los elementos");

	pa2m_afirmar( abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, NULL, NULL) == 0, "Iterador inorden devuelve 0 si la funcion es nula");
	pa2m_afirmar( abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, NULL, NULL) == 0, "Iterador preorden devuelve 0 si la funcion es nula");
	pa2m_afirmar( abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, NULL, NULL) == 0, "Iterador postorden devuelve 0 si la funcion es nula");

	arbol_destruir(arbol);
}

int main() {

	pa2m_nuevo_grupo("PRUEBAS CREACION");
	pruebas_creacion();
	pa2m_nuevo_grupo("PRUEBAS INSERCION");
	pruebas_insercion();
	pa2m_nuevo_grupo("PRUEBAS BORRAR");
	pruebas_borrar();
	pa2m_nuevo_grupo("PRUEBAS RECORRIDOS");
	pruebas_recorridos();
	pa2m_nuevo_grupo("PRUEBAS ITERADOR");
	pruebas_iterador();

	pa2m_mostrar_reporte();
}