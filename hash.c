#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "hash.h"

#define ERROR -1
#define EXITO 0
#define ERROR_ITERADOR 0
#define CAPACIDAD_MINIMA 3
#define POSICION_INICIAL 0
#define PORCENTAJE_DE_REHASHEO 0.75	
#define LINEAS "------------------------------|"	


typedef struct ocupante{
	char* id;//es la clave
	void* informacion;// es el elemento que acompaña a la hora de insertar 
}ocupante_t;


typedef struct celda{
	ocupante_t ocupante;
	bool esta_ocupada;
}celda_t;

struct hash{
	celda_t* tabla;//estatico no va a ser
	hash_destruir_dato_t destructor;
	int capacidad;//capacidad total de la tabla de hash, cantidad de celdas
	int ocupadas;//es la cantidad de celdas ocupadas
};	

/*
* Libera la memoria previamente reservada para almacenar la clave
*/
void destruir_duplicado(char* clave){
	free(clave);
	return;
}

/*
* reserva la memoria necesaria para almacenar el string dado
* Si fue posiblereservar dicha memoria copiara el string en dicho lugar
*/
char* duplicar_string(const char* clave){
	if(!clave)
    return NULL;

  char* ptr = malloc(strlen(clave)+1);//no debo verificar porque de ultima devuelve null, chequeo en el insertar
  strcpy(ptr,clave);
  return ptr;
}

/*
* Imprime por pantalla la tabla de hash
*/
void mostrar_tabla(hash_t* hash){
	printf("\n");
	for(int i = 0; i < hash->capacidad; i++){
			
		if(i <= 9){
		//if(i <= 9&& hash->tabla[i].esta_ocupada){
			printf("%s\n", LINEAS);
			printf("| %i| %s | %s |\n", i, hash->tabla[i].ocupante.id, (char*)hash->tabla[i].ocupante.informacion);
		}

		if(i > 9){
		//if(i > 9 && hash->tabla[i].esta_ocupada){
			printf("%s\n", LINEAS);
			printf("|%i| %s | %s |\n", i, hash->tabla[i].ocupante.id, (char*)hash->tabla[i].ocupante.informacion);
		}
	}
	printf("%s\n", LINEAS);
}

/*
* Si el hash contiene un destructor, le aplicara el destructor al elementos que recibe, sino no hara nada.
*/
void destruir_informacion(hash_t* hash, void* elemento){
	if(hash->destructor)
		hash->destructor(elemento);
	return;
}

/*
* Destruye en cada posicion ocupada el elemento, luego la clave reservada y por ultimo libera la tabla de hash reservada previamente
*/
void liberar_tabla(hash_t* hash){
	for(int i = hash->capacidad-1; i >= POSICION_INICIAL; i--){
		if(hash->tabla[i].esta_ocupada){
			destruir_informacion(hash, hash->tabla[i].ocupante.informacion);
			destruir_duplicado(hash->tabla[i].ocupante.id);
		}
	}
	free(hash->tabla);
}

hash_t* hash_crear(hash_destruir_dato_t destruir_elemento, size_t capacidad_inicial){

	hash_t* hash = calloc(1, sizeof(hash_t));
	if(!hash)
		return NULL;

	hash->destructor = destruir_elemento;

	if(capacidad_inicial <= CAPACIDAD_MINIMA){
		hash->tabla = calloc(1, CAPACIDAD_MINIMA*sizeof(celda_t));
		hash->capacidad = CAPACIDAD_MINIMA;
	}
	else{
		hash->tabla = calloc(1, capacidad_inicial*sizeof(celda_t));
		hash->capacidad = capacidad_inicial;
	}

	return hash;
}


/*
* Suma cada uno de los caracteres del string
*/
int funcion_hash(const char* clave){
	if(!clave)
		return ERROR;

	int resultado = 0;
	int largo = strlen(clave);
	for(int i = 0; i< largo; i++){
		resultado += clave[i];
	}
	return resultado;
}

/*
* Modulariza el valor del resultado obtenido por la funcion de hash, devuelve un valor entre 0 y la capacidad de la tabla de hash
*/
int modularizar_id(int capacidad, int id){
	int modulo = id % capacidad;
	return modulo;
}

/*
* Mediante la funcion de hash obtiene un numero y dicho numero lo modulariza y devuelve el resultado optenido 
*/
int calcular_posicion_de_insercion(int capacidad, const char* clave){
	int posicion = funcion_hash(clave);
    posicion = modularizar_id(capacidad, posicion);
    return posicion;
}

/*
* Inserta el nuevo ocupante en la posicion que indica, en caso de que la posicion este ocupada,
* hara probing lineal hasta encontrar un espacio vacio y lo insertara alli
*/
int insertar_en_posicion(hash_t* hash, int posicion, ocupante_t nuevo){

  if(hash->ocupadas == hash->capacidad)
    return ERROR;

  if(hash->tabla[posicion].esta_ocupada){

    if(strcmp(hash->tabla[posicion].ocupante.id, nuevo.id) == 0	){
      destruir_informacion(hash, hash->tabla[posicion].ocupante.informacion);//destruyo la info que tenia
      destruir_duplicado(nuevo.id);//borro la nueva clave porque esta repetida
      hash->tabla[posicion].ocupante.informacion = nuevo.informacion;
      return EXITO;
    }
    else if(posicion == hash->capacidad-1){//NO DEBERIA ENTRAR A MENOS QUE LA POSICION SEA LA ULTIMA
      return insertar_en_posicion(hash, POSICION_INICIAL, nuevo);
    }
    else{
      int pos_incrementada = posicion +1;
      return insertar_en_posicion(hash, pos_incrementada, nuevo);
    }
  }
  hash->tabla[posicion].ocupante = nuevo;
  hash->tabla[posicion].esta_ocupada = true;
  hash->ocupadas++;
  return EXITO;
}

/*
* Recibe la tabla con la capacidad aumentada, por lo tanto insertara el elemento sin la necesidad de verificar si hay lugar para insertarlo.
* No recibira dos claves iguales
* La cantidad de celdas ocupadas sera la misma que la que tenia la tabla anterior, por lo tanto no demo aumentarla al reinsertar los elementos
*/
void reinsertar_en_tabla(celda_t* tabla, int capacidad, int posicion, ocupante_t actual){

  if(tabla[posicion].esta_ocupada){
    if(posicion == capacidad-1){//NO DEBERIA ENTRAR A MENOS QUE LA POSICION SEA LA ULTIMA
        reinsertar_en_tabla(tabla, capacidad, POSICION_INICIAL, actual);
    	return;
    }	
    else{
        int pos_incrementada = posicion +1;
        reinsertar_en_tabla(tabla, capacidad, pos_incrementada, actual);
        return;
    }
  }
  tabla[posicion].ocupante = actual;
  tabla[posicion].esta_ocupada = true;
}


/*
* Recibe un hash, crea una tabla de hash nueva con el doble de la capacidad de la del hash recibido
* Recorre toda la tabla del hash recibido e inserta, dentro de la nueva tabla, todas las claves con sus respectivos elementos.
* Devuelve la nueva tabla en el caso de que haya podido reservar la memoria necesaria, sino devuelve NULL 
*/
celda_t* rehashear_tabla(hash_t* hash){
  int doble = (hash->capacidad*2);
	celda_t* tabla_nueva = calloc(1, sizeof(celda_t)*doble);
	if(!tabla_nueva)
		return NULL;

  int posicion = 0;
  for(int i = 0; i < hash->capacidad; i++){
    if(hash->tabla[i].esta_ocupada){
      posicion = calcular_posicion_de_insercion(doble, hash->tabla[i].ocupante.id);
      reinsertar_en_tabla(tabla_nueva, doble, posicion, hash->tabla[i].ocupante);//lo ingreso en la nueva tabla
    }
  } 

	return tabla_nueva;
}

/*
* Recibe un hash, crea una tabla de hash con el doble de capacidad que la anterior, la remplaza y libera la anterior.
* Aumenta el valor de la capacidad almacenado al doble
* Si pudo crear la nueva tabla de hash devuelve el hash con la nueva tabla y la capacidad aumentada, sino devuelve NULL 
*/
hash_t* rehashear(hash_t* hash){
	celda_t* aux = rehashear_tabla(hash);
  	if(!aux)
  		return NULL;
  	free(hash->tabla);
  	hash->tabla = aux;
  	hash->capacidad = hash->capacidad*2;
  	return hash;
}

/*
* Le agregue una opcion que si la clave es mostrar imprime la matriz por pantalla
*/
int hash_insertar(hash_t* hash, const char* clave, void* elemento){
  
  if(!hash || !clave)//asumo que el elemento puede ser nulo, pero no la clave
  	return ERROR;

  if(strcmp("mostrar", clave) == 0){
  	mostrar_tabla(hash);
  	return EXITO;
   }

  float factor_de_carga = (hash->capacidad* PORCENTAJE_DE_REHASHEO);//me da el valor que debe tener como minimo la capacidad 
  if(factor_de_carga <= hash->ocupadas){
  	hash_t* auxiliar = rehashear(hash);
  	if(!auxiliar)
  		return ERROR;
  	hash = auxiliar;
  }

  int posicion = calcular_posicion_de_insercion(hash->capacidad, clave);
  ocupante_t aux;

  char* clave_duplicada = duplicar_string(clave);
  if(!clave_duplicada)
  	return ERROR;

  aux.id = clave_duplicada;
  aux.informacion = elemento;

  return insertar_en_posicion(hash, posicion, aux);
}

/*
* Recibe una tabla de hash, junto con una clave y una posicion
* Recorre la tabal desde la posicion recibida hasta encontrar la clave recibida o hasta encontrar una celda vacia
* Si llega al final de la tabla y la ultima celda esta ocupada, continua por el principio de la tabla con las mismas premisas
* Si encuentra la clave recibida, devuelve su elemento. En caso contrario devuelve NULL
*/
void* buscar_elemento_linealmente(celda_t* tabla, const char* clave, int* posicion, int capacidad){
	if((*posicion) > capacidad-1){
		*(posicion) = 0;
		buscar_elemento_linealmente(tabla, clave, posicion, capacidad);
	}

	if(tabla[*posicion].esta_ocupada && strcmp(tabla[*posicion].ocupante.id, clave) == 0)
		return tabla[*posicion].ocupante.informacion;

	if(tabla[*posicion].esta_ocupada){
		(*posicion) +=1;
		return buscar_elemento_linealmente(tabla, clave, posicion, capacidad);
	}
	return NULL;
}

/*
* Recibe un hash y una posicion
* Reinicia los valores de la tabla del hash en esa posicion, es decir que marca que la celda esta desocupada y que la clave y el elemento son NULL 
*/
void vaciar_celda(hash_t* hash, int posicion){
	hash->tabla[posicion].ocupante.id = NULL;
  	hash->tabla[posicion].ocupante.informacion = NULL;
  	hash->tabla[posicion].esta_ocupada = false;
}

/*
* Recibe una tabla de hash, junto con una clave y una posicion
* Recorre la tabal desde la posicion recibida hasta encontrar la clave recibida o hasta encontrar una celda vacia
* Si llega al final de la tabla y la ultima celda esta ocupada, continua por el principio de la tabla con las mismas premisas
* Si encuentra la clave recibida, devuelve su posicion. En caso contrario devuelve -1
*/
int buscar_posicion_del_elemento(celda_t* tabla, const char* clave, int posicion, int capacidad){

	if(posicion > capacidad-1)
		return buscar_posicion_del_elemento(tabla, clave, 0, capacidad);

	if(tabla[posicion].esta_ocupada && strcmp(tabla[posicion].ocupante.id, clave) == 0)
		return posicion;

	if(tabla[posicion].esta_ocupada)
		return buscar_posicion_del_elemento(tabla, clave, posicion+1, capacidad);

	return ERROR;
}


/*
	* Recibe el hash y una posicion
	* Incrementa esa posicion, a menos que se encuentre en la ultima posicion y en ese caso le otorga la posicion inicial
	* Mientras que la posicion este ocupada la funcion se llamara a si misma, a su vez, calcula la posicion en donde deberia estar ubicado dentro
		de la tabla la clave del la posicion de esa tabla, si esta en una posicion que no es la suya vaciara la celda y reinsertara la clave
	* Si la posicion actual no se encuentra, se retiraq de la funcion devolviendo un valor exitoso.
*/
int buscar_sucesor(hash_t* hash, int* posicion){
	
	if(*posicion >= hash->capacidad-1)
		(*posicion) = POSICION_INICIAL;
	else
		(*posicion) +=1;// incremento la posicion

	if(hash->tabla[*posicion].esta_ocupada){
		int pos_real = calcular_posicion_de_insercion(hash->capacidad, hash->tabla[*posicion].ocupante.id);
    	if(*posicion != pos_real){
    		ocupante_t actual = hash->tabla[*posicion].ocupante;//guardo la info
  			vaciar_celda(hash, *posicion);
    		reinsertar_en_tabla(hash->tabla, hash->capacidad, pos_real, actual);
    	}
  		return buscar_sucesor(hash, posicion);
	}
	return EXITO;
}

int hash_quitar(hash_t* hash, const char* clave){
  if(!hash || !clave)
  	return ERROR;

  int posicion = calcular_posicion_de_insercion(hash->capacidad, clave);

  //busco el que quiero eliminar
  posicion = buscar_posicion_del_elemento(hash->tabla, clave, posicion, hash->capacidad);	
  if(posicion == ERROR)
  	return ERROR;


  destruir_informacion(hash, hash->tabla[posicion].ocupante.informacion);
  destruir_duplicado(hash->tabla[posicion].ocupante.id);
  vaciar_celda(hash, posicion);
  hash->ocupadas--;
  //guardo la posicion donde me encuentro
  return buscar_sucesor(hash, &posicion);
}

void* hash_obtener(hash_t* hash, const char* clave){
  	if(!hash || !clave)
		return NULL;
	int posicion = calcular_posicion_de_insercion(hash->capacidad, clave);

	//return buscar_elemento_linealmente(hash->tabla, clave, &posicion, hash->capacidad);
	posicion = buscar_posicion_del_elemento(hash->tabla, clave, posicion, hash->capacidad);
	
	if(posicion == ERROR)
		return NULL;

	return hash->tabla[posicion].ocupante.informacion;

}

size_t hash_cantidad(hash_t* hash){
	if(!hash)
		return 0;
  	return hash->ocupadas;
}

bool hash_contiene(hash_t* hash, const char* clave){
	
	if(!hash || !clave)
		return NULL;
	int posicion = calcular_posicion_de_insercion(hash->capacidad, clave);

	int buscado = buscar_posicion_del_elemento(hash->tabla, clave, posicion, hash->capacidad);
	if(buscado == ERROR)
		return false;

	return true;//puedo devolver true porque la funcion de buscar la posicion si no lo encuentra devuelve error, por lo tanto si lo encuentra devuelve la posicion del elemento
}

void hash_destruir(hash_t* hash){
	if(!hash)
		return;

	liberar_tabla(hash);
	free(hash);
}


size_t hash_con_cada_clave(hash_t* hash, bool (*funcion)(hash_t* hash, const char* clave, void* aux), void* aux){
	if(!hash || !funcion)
		return ERROR_ITERADOR;

	int contador = 0;
	int i = 0;
	bool cortar = false;

	while(hash->capacidad > i && !cortar){
		
		if(hash->tabla[i].esta_ocupada){
			if(funcion(hash, hash->tabla[i].ocupante.id, aux))//mientras que sea false debo seguir iterando
				cortar = true;
			contador++;
		}	
		i++;
	}

	return contador;
}

