#include "abb.h"
#include <stdio.h>

typedef struct cosa{
    int clave;
    char contenido[10];
}cosa;

cosa* crear_cosa(int clave){
    cosa* c = (cosa*)malloc(sizeof(cosa));
    if(c)
        c->clave = clave;
    return c;
}

void destruir_cosa(cosa* c){
    free(c);
}

int comparar_cosas(void* elemento1, void* elemento2){
    if(!elemento1 || !elemento2)
        return 0;

    if(((cosa*)elemento1)->clave>((cosa*)elemento2)->clave)
        return 1;
    if(((cosa*)elemento1)->clave<((cosa*)elemento2)->clave)
        return -1;
    return 0;
}

void destructor_de_cosas(void* elemento){
    if(!elemento)
        return;
    destruir_cosa((cosa*)elemento);
}

bool mostrar_elemento(void* elemento, void* extra){
    extra=extra; //para que no se queje el compilador, gracias -Werror -Wall
    if(elemento)
        printf("%i ", ((cosa*)elemento)->clave);
    return false;
}

bool mostrar_hasta_5(void* elemento, void* extra){
    extra=extra; //para que no se queje el compilador, gracias -Werror -Wall
    if(elemento){
        printf("%i ", ((cosa*)elemento)->clave);
        if(((cosa*)elemento)->clave == 5)
            return true;
    }
    return false;
}

bool mostrar_acumulado(void* elemento, void* extra){
    if(elemento && extra){
        *(int*)extra += ((cosa*)elemento)->clave;
        printf("%i ", *(int*)extra);
    }
    return false;
}


int main(){
    abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

    cosa* c1= crear_cosa(1);
    cosa* c2= crear_cosa(2);
    cosa* c3= crear_cosa(3);
    cosa* c4= crear_cosa(4);
    cosa* c5= crear_cosa(5);
    cosa* c6= crear_cosa(6);
    cosa* c7= crear_cosa(7);
    cosa* auxiliar = crear_cosa(0);
    
    arbol_insertar(arbol, c4);
    arbol_insertar(arbol, c2);
    arbol_insertar(arbol, c6);
    arbol_insertar(arbol, c1);
    arbol_insertar(arbol, c3);
    arbol_insertar(arbol, c5);
    arbol_insertar(arbol, c7);

    printf("El nodo raiz deberia ser 4: %s\n", ((cosa*)arbol_raiz(arbol))->clave==4?"SI":"NO");
    
    auxiliar->clave = 5;
    printf("Busco el elemento 5: %s\n", ((cosa*)arbol_buscar(arbol, auxiliar))->clave==5?"SI":"NO");

    auxiliar->clave = 7;
    printf("Borro nodo hoja (7): %s\n", (arbol_borrar(arbol, auxiliar))==0?"SI":"NO");

    auxiliar->clave = 6;
    printf("Borro nodo con un hijo (6): %s\n", (arbol_borrar(arbol, auxiliar))==0?"SI":"NO");

    auxiliar->clave = 2;
    printf("Borro nodo con dos hijos (2): %s\n", (arbol_borrar(arbol, auxiliar))==0?"SI":"NO");

    auxiliar->clave = 4;
    printf("Borro la raiz (4): %s\n", (arbol_borrar(arbol, auxiliar))==0?"SI":"NO");

    auxiliar->clave = 3;
    printf("Busco el elemento (3): %s\n", ((cosa*)arbol_buscar(arbol, auxiliar))->clave==3?"SI":"NO");

    if (arbol->nodo_raiz->elemento == c3) printf("raiz 3\n");
    if (arbol->nodo_raiz->izquierda->elemento == c1) printf("izq 1\n");
    if (arbol->nodo_raiz->derecha->elemento == c5) printf("der 5 \n");

    cosa* elementos[10];
    printf("Recorrido inorden (deberian salir en orden 1 3 5): ");
    size_t cantidad = arbol_recorrido_inorden(arbol, (void**)elementos, 10);
    for(size_t i=0;i<cantidad;i++)
        printf("%i ", elementos[i]->clave);
    printf("\n");

    cosa* c15 = crear_cosa(15);
    cosa* c0 = crear_cosa(0);
    cosa* c9 = crear_cosa(9);
    c4 = crear_cosa(4);
    c7 = crear_cosa(7);

    printf("\n\nInserto mas valores y pruebo el iterador interno\n\n");

    arbol_insertar(arbol, c0);
    arbol_insertar(arbol, c4);
    arbol_insertar(arbol, c15);
    arbol_insertar(arbol, c9);
    arbol_insertar(arbol, c7);

    if (arbol->nodo_raiz->elemento == c3) printf("raiz 3\n");
    if (arbol->nodo_raiz->izquierda->elemento == c1) printf("izq 1\n");
    if (arbol->nodo_raiz->izquierda->izquierda->elemento == c0) printf("izq izq 0\n");
    if (arbol->nodo_raiz->derecha->elemento == c5) printf("der 5 \n");
    if (arbol->nodo_raiz->derecha->izquierda->elemento == c4) printf("der izq 4\n");
    if (arbol->nodo_raiz->derecha->derecha->elemento == c15) printf("der der 15\n");
    if (arbol->nodo_raiz->derecha->derecha->izquierda->elemento == c9) printf("der der izq 9\n");
    if (arbol->nodo_raiz->derecha->derecha->izquierda->izquierda->elemento == c7) printf("der der izq izq 7\n");
    
    printf("Recorrido inorden iterador interno: ");    
    abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, mostrar_elemento, NULL);
    printf("\n");


    cosa* elementoss[10];
    printf("Recorrido inorden (deberian salir en orden 0 1 3 4 5 7 9 15): ");
    size_t cantidadd = arbol_recorrido_inorden(arbol, (void**)elementoss, 10);
    for(size_t i=0;i<cantidadd;i++)
        printf("%i ", elementoss[i]->clave);
    printf("\n");

    cosa* elementosss[10];
    printf("Recorrido preorden (deberian salir en orden 3 1 0 5 4 15 9): ");
    size_t cantidaddd = arbol_recorrido_preorden(arbol, (void**)elementosss, 7);
    for(size_t i=0;i<cantidaddd;i++)
        printf("%i ", elementosss[i]->clave);
    printf("\n");

    cosa* elementossss[10];
    printf("Recorrido postorden (deberian salir en orden 0 1 4 7 9 15): ");
    size_t cantidadddd = arbol_recorrido_postorden(arbol, (void**)elementossss, 6);
    for(size_t i=0;i<cantidadddd;i++)
        printf("%i ", elementossss[i]->clave);
    printf("\n");



    printf("Borro la raiz (3): %s\n", (arbol_borrar(arbol, c3))==0?"SI":"NO");

    if (arbol->nodo_raiz->elemento == c1) printf("raiz 1\n");
    if (arbol->nodo_raiz->izquierda->elemento == c0) printf("izq 0\n");
    if (arbol->nodo_raiz->derecha->elemento == c5) printf("der 5 \n");
    if (arbol->nodo_raiz->derecha->izquierda->elemento == c4) printf("der izq 4\n");
    if (arbol->nodo_raiz->derecha->derecha->elemento == c15) printf("der der 15\n");
    if (arbol->nodo_raiz->derecha->derecha->izquierda->elemento == c9) printf("der der izq 9\n");
    if (arbol->nodo_raiz->derecha->derecha->izquierda->izquierda->elemento == c7) printf("der der izq izq 7\n");

    printf("Borro la raiz (1): %s\n", (arbol_borrar(arbol, c1))==0?"SI":"NO");


    if (arbol->nodo_raiz->elemento == c0) printf("raiz 0\n");
    if (arbol->nodo_raiz->derecha->elemento == c5) printf("der 5 \n");
    if (arbol->nodo_raiz->derecha->izquierda->elemento == c4) printf("der izq 4\n");
    if (arbol->nodo_raiz->derecha->derecha->elemento == c15) printf("der der 15\n");
    if (arbol->nodo_raiz->derecha->derecha->izquierda->elemento == c9) printf("der der izq 9\n");
    if (arbol->nodo_raiz->derecha->derecha->izquierda->izquierda->elemento == c7) printf("der der izq izq 7\n");


    printf("Borro la raiz (0): %s\n", (arbol_borrar(arbol, c0))==0?"SI":"NO");

    if (arbol->nodo_raiz->elemento == c5) printf("raiz 5\n");
    if (arbol->nodo_raiz->izquierda == NULL) printf("izq NULL\n");
    if (arbol->nodo_raiz->derecha->elemento == c15) printf("der 15\n");
    if (arbol->nodo_raiz->izquierda->elemento == c4) printf("izq 4\n");
    if (arbol->nodo_raiz->derecha->izquierda->elemento == c9) printf("der izq 9\n");
    if (arbol->nodo_raiz->derecha->izquierda->izquierda->elemento == c7) printf("der izq izq 7\n");

    /*printf("Recorrido preorden iterador interno: ");    
    abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, mostrar_elemento, NULL);
    printf("\n");
    
    printf("Recorrido postorden iterador interno: ");    
    abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, mostrar_elemento, NULL);
    printf("\n");

    printf("\nRecorrido inorden hasta encontrar el 5: ");    
    abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, mostrar_hasta_5, NULL);
    printf("\n");

    printf("Recorrido preorden hasta encontrar el 5: ");    
    abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, mostrar_hasta_5, NULL);
    printf("\n");

    printf("Recorrido postorden hasta encontrar el 5: ");    
    abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, mostrar_hasta_5, NULL);
    printf("\n");

    int acumulador=0;
    printf("\nRecorrido inorden acumulando los valores: ");    
    abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, mostrar_acumulado, &acumulador);
    printf("\n");

    acumulador=0;
    printf("Recorrido preorden acumulando los valores: ");    
    abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, mostrar_acumulado, &acumulador);
    printf("\n");

    acumulador=0;
    printf("Recorrido postorden acumulando los valores: ");    
    abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, mostrar_acumulado, &acumulador);
    printf("\n");*/
    
    free(auxiliar);
    arbol_destruir(arbol);

    return 0;
}
