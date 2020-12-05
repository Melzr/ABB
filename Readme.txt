
INTRODUCCION

	Lo entregado es una implementacion de un TDA ABB (Arbol Binario de Busqueda), se proporciona tanto el abb.h como el abb.c ademas de 
	abb_minipruebas.c y pruebas.c donde se desarrollo el testing del mismo.
	Utilice en su mayoria funciones recursivas para la implementacion ya que simplifica mucho el entendimiento de las funciones al ser
	el tipo de dato ABB recursivo por naturaleza. 


COMPILACION:

	Con minipruebas del enunciado:
	gcc abb.c abb_minipruebas.c -o abb -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0

	Con mis pruebas:
	gcc abb.c pruebas.c -o abb -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0


EJECUCION:

	./abb

	Con Valgrind:
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./abb


DESARROLLO:

	¿Que es un ABB?

		Un ABB (Arbol Binario de Busqueda) es un tipo de dato abstracto con nodos enlazados, con un primer nodo denominado raiz que puede apuntar
		hasta a dos nodos 'hijos', uno a su izquierda y otro a su derecha que a su vez estos pueden tener hasta dos nodos hijos y asi sucesivamente.
		La diferencia con un Arbol Binario es que el ABB mantiene un orden: al moverme hacia la izquierda siempre encontrare un elemento menor al del
		padre, y al moverme a la derecha este sera mayor. Esto permite reducir considerablemente la complejidad de los algoritmos.


	Funcion de destruccion

		El destructor sirve para liberar la memoria de los elementos contenidos en el arbol de ser necesario, de esta manera me aseguro que el TDA no
		pierde memoria sin importar el tipo de dato que el usuario quiera almacenar. Para el usuario no tenerla implicaria que, para no perder memoria, 
		deberia guardar todos los elementos en una estructura auxiliar a la que si tenga acceso para poder liberarlos, ocupando asi el doble de espacio 
		en memoria y teniendo que volver a recorrer todos los elementos.


	Complejidad de las operaciones

		En el peor de los casos el arbol estara completamente desbalanceado, pareciendose a una lista, en este caso la complejidad de buscar, insertar
		y borrar sera O(n). En el caso en que el arbol este balanceado, la complejidad de estas operaciones se acercara a O(log(n)).
		Crear arbol siempre es O(1) pues aun no tiene elementos. Tambien lo seran arbol_raiz y arbol_vacio pues solo se opera con la raiz del arbol.
		Destruir el arbol es O(n) pues tengo debo pararme en todos sus elementos para eliminarlos.
		Recorrer inorden, postorden y preorden seran O(n) ya que se recorren todos los elementos a menos que el vector donde se cargan se llene. Mismo 
		caso con Iterador, que sera O(n) a menos que la funcion recibida indique que se debe parar de iterar.