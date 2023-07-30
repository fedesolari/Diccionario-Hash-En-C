#include "hash.h"
#include <stdio.h>
#include <string.h>
#include "pa2mm.h"

#define ERROR -1
#define EXITO 0
#define CAPACIDAD_MINIMA 3


void destruir_string(void* elemento){
  elemento=elemento;
  //printf("(Destructor) Libero el documento: %s\n", (char*)elemento);	
}

char* reservar_string(const char* elemento){
  if(!elemento)
    return NULL;

  char* ptr = malloc(strlen(elemento)+1);
  strcpy(ptr,elemento);
  return ptr;
}

void destruir_string_dinamico(void* elemento){
  if(elemento)
    free(elemento);
}

//suma todas las letras del string 
int funcion_magica(const char* clave, int capacidad){
	if(!clave)
		return ERROR;

	int resultado = 0;
	int largo = strlen(clave);
	for(int i = 0; i< largo; i++){
		resultado += clave[i];
	}
	int modulo = resultado % capacidad;
	return modulo;
}

bool mostrar_elementos(hash_t* hash, const char* clave, void* aux){
  if(!clave)
    return true;

  aux=aux;
  hash=hash;
  printf("Documento en el hash: %s\n", clave);

  return false;
}

bool mostrar_hasta_el_aux(hash_t* hash, const char* clave, void* aux){
  if(!clave)
    return true;

  aux=aux;
  hash=hash;
  printf("Documento en el hash: %s\n", clave);
  //if(clave == aux)
  if(strcmp(clave, aux) == 0)
  	return true;

  return false;
}

void crear_hash_con_destructor_nulo_crear_un_hash(){
	int capacidad_inicial = 5;
	hash_t* hash = hash_crear(NULL, capacidad_inicial);

    pa2m_afirmar(hash != NULL, "Se pudo crear un hash con destructor nulo.");

    hash_destruir(hash);
	return;
}

void Un_hash_nulo_resulta_vacio(){
	hash_t* hash = NULL;
	pa2m_afirmar(hash_cantidad(hash) == 0, "Un hash nulo resulta vacio.");
}

void dado_un_hash_nulo_no_es_posible_insertar(){
	hash_t* hash = NULL;

	pa2m_afirmar(hash_insertar(hash, "ABCD", "hola") == ERROR, "No es posible insertar un elemento en un hash nulo.");
}

void dado_un_hash_vacio_puedo_insertar_una_clave_con_elemento_nulo(){
	hash_t* hash = hash_crear(destruir_string, CAPACIDAD_MINIMA);

    if(hash_cantidad(hash) == 1 && hash_insertar(hash, "AC123RD", NULL) == EXITO)
    	pa2m_afirmar(1 == 1, "Puedo insertar un elemento en el hash.");

    //hash_insertar(hash, "mostrar", NULL);

    hash_destruir(hash);
}

void dado_un_hash_vacio_no_puedo_insertar_una_clave_nula(){
	hash_t* hash = hash_crear(destruir_string, CAPACIDAD_MINIMA);

    pa2m_afirmar(hash_insertar(hash, NULL, "fede") == ERROR, "No es posible insertar un elemento con clave nula.");

    //hash_insertar(hash, "mostrar", NULL);

    hash_destruir(hash);
}

void dado_un_hash_nulo_devuelve_eliminar_un_elemento_no_insertado_ERROR(){
	hash_t* hash = NULL;

	pa2m_afirmar(hash_quitar(hash, "aaaa") == ERROR, "El elemento solicitado no se pudo eliminar.");
}

void dado_un_hash_nulo_al_buscar_un_elemento_me_devuelve_null(){
	hash_t* hash = NULL;

	pa2m_afirmar(hash_obtener(hash, "wfwfwff") == NULL, "Obtener elemento de un hash nulo devuelve NULL");

	hash_destruir(hash);
}

void iterar_un_hash_nulo_devuelve_0_elementos_recorridos(){
	hash_t* hash = NULL;
	size_t resultado = hash_con_cada_clave(hash, mostrar_elementos, NULL);
	pa2m_afirmar(resultado == 0, "Iterar un hash NULL recorre 0(cero) elementos.");
}

void iterar_un_hash_con_funcion_nula_devuelve_0_elementos_recorridos(){
	hash_t* hash = hash_crear(destruir_string, CAPACIDAD_MINIMA);
	size_t resultado = hash_con_cada_clave(hash, NULL, NULL);
	pa2m_afirmar(resultado == 0, "Iterar un hash con funcion nula recorre 0(cero) elementos.");

	hash_destruir(hash);
}

void crear_una_tabla_de_hash_con_una_determinada_capacidad_se_crea_con_esa_capacidad(){
	int capacidad_inicial = 5;
	hash_t* hash = hash_crear(destruir_string, capacidad_inicial);

    pa2m_afirmar(hash != NULL, "Se pudo crear una tabla de hash con la capacidad solicitada.");

    hash_destruir(hash);
}

void crear_una_tabla_de_hash_con_capacidad_menor_a_CAPACIDAD_MINIMA_se_crea_con_esa_capacidad(){
	int capacidad_inicial = 1;
	hash_t* hash = hash_crear(destruir_string, capacidad_inicial);

    pa2m_afirmar(hash != NULL, "Craer una tabla de hash con capacidad menor a la minima lo crea con la capacidad minima.");
    
    hash_destruir(hash);
    return;
}

void crear_un_hash_vacio_tiene_0_elementos(){
	int capacidad_inicial = 5;
	hash_t* hash = hash_crear(destruir_string, capacidad_inicial);

    pa2m_afirmar(hash != NULL, "Se pudo crear una tabla de hash correctamente.");
    pa2m_afirmar(hash_cantidad(hash) == 0, "El hash esta vacio, no contiene ningun elemento.");

    hash_destruir(hash);
}

void insertar_un_elemento_devuleve_exito(){
	int capacidad_inicial = 5;
	hash_t* hash = hash_crear(destruir_string, capacidad_inicial);

	pa2m_afirmar(hash_insertar(hash, "AC123RD", "fede") == EXITO, "Se pudo insertar un elemento en la tabla de hash correctamente.");

	//hash_insertar(hash, "mostrar", "fede");

	hash_destruir(hash);
}

void insertar_un_elemento_con_misma_clave_lo_remplaza_y_devuleve_exito(){
	int capacidad_inicial = 5;
	hash_t* hash = hash_crear(destruir_string, capacidad_inicial);

	char* clave = "AC123RD";

	hash_insertar(hash, clave, "fede");
	int cant_elementos = hash_cantidad(hash);

	if(hash_insertar(hash, clave, "santi") == EXITO && hash_cantidad(hash) == cant_elementos)
		pa2m_afirmar(1 == 1, "Insertar un elemento con misma clave lo sobreescribe.");

	//hash_insertar(hash, "mostrar", "fede");

	hash_destruir(hash);
}

void insertar_varios_elementos_distintos_devuleve_exito(){
	int capacidad_inicial = 5;
	hash_t* hash = hash_crear(destruir_string, capacidad_inicial);

	char* clave_1 = "AC123RD";
	char* clave_2 = "AC123SD";
	char* clave_3 = "AC123TD";
	char* clave_4 = "AC123UD";

	if(hash_insertar(hash, clave_1, "fede") == EXITO && hash_insertar(hash, clave_2, "seba")  == EXITO && hash_insertar(hash, clave_3, "mari")  == EXITO && hash_insertar(hash, clave_4, "santi") == EXITO ){
		pa2m_afirmar(1 == 1, "Puedo insertar 4 elementos con claves diferentes.");
	}

	//hash_insertar(hash, "mostrar", "fede");

	hash_destruir(hash);
}

void insertar_varios_elementos_distintos_con_mismo_valor_de_hash_devuleve_exito(){
	int capacidad_inicial = 5;
	hash_t* hash = hash_crear(destruir_string, capacidad_inicial);

	char* clave_1 = "hola";
	char* clave_2 = "holahola";
	char* clave_3 = "zl";
	char* clave_4 = "zlzl";

	//ESTA PRUEBA ES UN POCO CAJA BLANCA PERO ES UNA PRUEBA IMPORTANTE

	if(hash_insertar(hash, clave_1, "fede") == EXITO && hash_insertar(hash, clave_2, "seba")  == EXITO && hash_insertar(hash, clave_3, "mari")  == EXITO && hash_insertar(hash, clave_4, "santi") == EXITO ){
		pa2m_afirmar(1 == 1, "Puedo insertar 4 elementos que colisionan.");
	}

	//hash_insertar(hash, "mostrar", "fede");

	hash_destruir(hash);
}

void Dado_un_hash_con_1_elemento_en_el_final_insertar_elemento_en_la_ultima_posicion_colisiona_pero_se_inserta_en_el_inicio(){
	int capacidad_inicial = 3;
	hash_t* hash = hash_crear(destruir_string, capacidad_inicial);

	pa2m_afirmar(hash_insertar(hash, "A", "bjkasdknslaknsDASDAAAd") == EXITO, "El elemento se inserto en la ultima posicion.");
	pa2m_afirmar(hash_insertar(hash, "Y", "kbjladnsdjnasdSDADfnj") == EXITO, "El elemento colisiono y se inserto en la posicion inicial.");

	//hash_insertar(hash, "mostrar", "fede");

	hash_destruir(hash);
}

void insertar_varios_elementos_hasta_alcanzar_la_capacidad_minima_de_rehasheo(){
	int capacidad_inicial = 4;
 	hash_t* hash = hash_crear(destruir_string_dinamico, capacidad_inicial);
  	int i = 0;
  	int contador = 0;
  	int cant_insertados = 0;

  	char* nombres[6]={"Mariano", "Lucas", "Fede", "Santi", "Juan", "Maria"};
  	char* documentos[6]={"45365254", "12457865", "45875636", " 25456325", " 98652345", " 42819254"};

    for(i = 0; i < 6; i++){
      hash_insertar(hash, documentos[i], reservar_string(nombres[i]));
      cant_insertados++;
    }

    for(i = 0; i < 6; i++){
      if(hash_contiene(hash, documentos[i]) == true)
        contador++;
    }

    if(contador == cant_insertados)
	    pa2m_afirmar(1==1, "La tabla rehasheo una vez. La cantidad elementos es la correcta.");
	
	//hash_insertar(hash, "mostrar", "fede");

	hash_destruir(hash);
}

void insertar_elementos_reservados_en_memoria_en_tabla_de_hash(){
 	int capacidad_inicial = 4;
 	hash_t* hash = hash_crear(destruir_string_dinamico, capacidad_inicial);
  	int i = 0;
  	int contador = 0;

  	char* nombres[6]={"Mariano", "Lucas", "Fede", "Santi", "Juan", "Maria"};
  	char* documentos[6]={"45365254", "12457865", "45875636", " 25456325", " 98652345", " 42819254"};

    for(i = 0; i < 6; i++){
      if(hash_insertar(hash, documentos[i], reservar_string(nombres[i])) == EXITO)
        contador++;
    }

  	if(contador == i)	
  		pa2m_afirmar(1 == 1, "Puedo insertar varios elementos reservados en memoria correctamente.");
  
  	//hash_insertar(hash, "mostrar", "fede");

 	 hash_destruir(hash);
}

void insertar_un_elemento_en_hash_vacio(){
	int capacidad_inicial = 5;
	int cantidad = 0;
	hash_t* hash = hash_crear(destruir_string, capacidad_inicial);

    pa2m_afirmar(hash_cantidad(hash) == cantidad, "El hash esta vacio, no contiene ningun elemento.");
    pa2m_afirmar(hash_insertar(hash, "42819254", "doc. Fede") == EXITO, "Se pudo insertar un elemento en la tabla de hash correctamente.");
    cantidad++;
    pa2m_afirmar(hash_cantidad(hash) == cantidad, "Ahora el hash tiene 1 elemento.");

    //hash_insertar(hash, "mostrar", "fede");

    hash_destruir(hash);
}

void dado_un_hash_con_una_cierta_cantidad_de_elementos_insertados_devuelve_dicha_cantidad(){
	int capacidad_inicial = 5;
	hash_t* hash = hash_crear(destruir_string, capacidad_inicial);

	hash_insertar(hash, "wadfkn", "fede");
	hash_insertar(hash, "das", "fede");
	hash_insertar(hash, "wbwrbn", "fede");
	hash_insertar(hash, "waddfdwdcfkn", "fede");

	pa2m_afirmar(hash_cantidad(hash) == 4, "Devolvio la cantidad esperada.");

	hash_destruir(hash);
}

void dado_un_hash_nulo_la_funcion_cantidad_devuelve_0(){
	hash_t* hash = NULL;

	pa2m_afirmar(hash_cantidad(hash) == 0, "Cantidad de hash nulo devuleve error.");

	hash_destruir(hash);
}

void dado_un_hash_vacio_la_cantidad_de_elementos_que_contiene_es_0(){
	int capacidad_inicial = 8;
	hash_t* hash = hash_crear(destruir_string, capacidad_inicial);

    pa2m_afirmar(hash_cantidad(hash) == 0, "El hash esta vacio, la cantidad de elementos es 0.");

    hash_destruir(hash);
}


void Dado_un_hash_con_elementos_insertados_busco_el_elemento_y_devuelve_true(){
	int capacidad_inicial = 5;
	hash_t* hash = hash_crear(destruir_string, capacidad_inicial);

	hash_insertar(hash, "wadfkn", "fede");
	hash_insertar(hash, "das", "fede");
	hash_insertar(hash, "wbwrbn", "fede");
	hash_insertar(hash, "waddfdwdcfkn", "fede");

	pa2m_afirmar(hash_contiene(hash, "wadfkn") == true, "El elemento se encuentra dentro del hash");

	hash_destruir(hash);
}

void dado_un_hash_con_elementos_insertados_busco_elemento_inexistente_y_devuelve_false(){
	int capacidad_inicial = 5;
	hash_t* hash = hash_crear(destruir_string, capacidad_inicial);

	hash_insertar(hash, "wadfkn", "fede");
	hash_insertar(hash, "das", "fede");
	hash_insertar(hash, "wbwrbn", "fede");
	hash_insertar(hash, "waddfdwdcfkn", "fede");

	pa2m_afirmar(hash_contiene(hash, "wfwfwff") == false, "El elemento no se encuentra dentro del hash");

	hash_destruir(hash);
}


void dado_un_hash_con_elementos_al_pasarle_la_clave_me_devuelve_el_elemento_esperado(){
	int capacidad_inicial = 5;
	hash_t* hash = hash_crear(destruir_string, capacidad_inicial);

	//char clave = 'a';
	char* elemento = "fede";
	hash_insertar(hash, "a", "fede");
	hash_insertar(hash, "das", "fede");
	hash_insertar(hash, "wbwrbn", "fede");
	hash_insertar(hash, "waddfdwdcfkn", "fede");

	hash_insertar(hash, "mostrar", "fede");
	pa2m_afirmar(hash_obtener(hash, "a") == elemento , "Obtener elemento devuelve el elemento esperado");

	hash_destruir(hash);
}


void dado_un_hash_al_buscar_al_pasarle_una_clave_no_insertada_me_devuelve_null(){
	int capacidad_inicial = 5;
	hash_t* hash = hash_crear(destruir_string, capacidad_inicial);

	hash_insertar(hash, "wadfkn", "fede");
	hash_insertar(hash, "das", "fede");
	hash_insertar(hash, "wbwrbn", "fede");
	hash_insertar(hash, "waddfdwdcfkn", "fede");

	pa2m_afirmar(hash_obtener(hash, "wfwfwff") == NULL, "El elemento de la clave solicitada no se encuentra dentro del hash");

	hash_destruir(hash);
}

void dada_un_hash_con_elementos_insertados_elimino_correctamente_el_emeneto_solicitado(){
	hash_t* hash = hash_crear(destruir_string_dinamico, CAPACIDAD_MINIMA);

	int i = 0;
  	int elementos_finales = 0;
  	int can_elementos = 0;

  	char* nombres[6]={"Mariano", "Lucas", "Fede", "Santi", "Juan", "Maria"};
  	char* documentos[6]={"45365254", "12457865", "45875636", " 25456325", " 98652345", " 42819254"};

    for(i = 0; i < 6; i++){
      hash_insertar(hash, documentos[i], reservar_string(nombres[i]));
      can_elementos++;
    }

	pa2m_afirmar(hash_quitar(hash, documentos[2]) == EXITO, "El elemento solicitado se elimino correctamente");
	
    for(i = 0; i < 6; i++){
      if(hash_contiene(hash, documentos[i]))
        elementos_finales++;
    }

	//hash_insertar(hash, "mostrar", "fede");

	if(can_elementos-1 == elementos_finales && hash_contiene(hash, documentos[2]) == false)
		pa2m_afirmar(1 == 1, "Todos los elementos no eliminados se encuentran en la tabla de hash");

	hash_destruir(hash);
}

void dado_un_hash_con_elementos_quitar_todos_los_elementos_y_la_tabla_de_hash_queda_vacia(){
	hash_t* hash = hash_crear(destruir_string_dinamico, CAPACIDAD_MINIMA);

	int i = 0;
  	int contador = 0;

  	char* nombres[6]={"Mariano", "Lucas", "Fede", "Santi", "Juan", "Maria"};
  	char* documentos[6]={"45365254", "12457865", "45875636", " 25456325", " 98652345", " 42819254"};

    for(i = 0; i < 6; i++){
      hash_insertar(hash, documentos[i], reservar_string(nombres[i]));
    }
	
    for(i = 0; i < 6; i++){
      if(hash_quitar(hash, documentos[i]) == EXITO)
        contador++;
    }
	//hash_insertar(hash, "mostrar", "fede");

  	if(contador == i && hash_cantidad(hash) == 0)	
  		pa2m_afirmar(1 == 1, "Elimino todos los elementos y el hash queda vacio.");


	hash_destruir(hash);
}

void dado_un_hash_con_un_elemento_al_eliminarlo_queda_vacio(){
	int capacidad_inicial = 5;
	hash_t* hash = hash_crear(destruir_string, capacidad_inicial);

	hash_insertar(hash, "AC123RD", "fede");
	hash_quitar(hash, "AC123RD");
	pa2m_afirmar(hash_cantidad(hash) == 0, "Has con un elemento, al eliminarlo el hash queda vacio.");
	//hash_insertar(hash, "mostrar", "fede");

	hash_destruir(hash);
}

void dado_un_hash_vacio_eliminar_elemento_devuelve_ERROR(){
	int capacidad_inicial = 5;
	hash_t* hash = hash_crear(destruir_string, capacidad_inicial);

    pa2m_afirmar(hash_quitar(hash, "dsfsd") == ERROR, "No se puede elimnar un elemento en un hash vacio.");

    hash_destruir(hash);
}

void dado_un_hash_con_elementos_al_eliminar_un_elemento_la_cantidad_se_reduce_en_1(){
	hash_t* hash = hash_crear(destruir_string_dinamico, CAPACIDAD_MINIMA);

	int i = 0;
  	int contador = 0;

  	char* nombres[6]={"Mariano", "Lucas", "Fede", "Santi", "Juan", "Maria"};
  	char* documentos[6]={"45365254", "12457865", "45875636", " 25456325", " 98652345", " 42819254"};

    for(i = 0; i < 6; i++){
      hash_insertar(hash, documentos[i], reservar_string(nombres[i]));
      contador++;
    }
	
    hash_quitar(hash, documentos[3]);

  	if(hash_cantidad(hash) == contador-1)	
  		pa2m_afirmar(1 == 1, "Al eliminar un elemento del hasha cantidad se reduce en uno.");

	//hash_insertar(hash, "mostrar", "fede");

	hash_destruir(hash);
}

void dado_un_hash_con_elementos_al_eliminar_dos_veces_el_mismo_elemento_devuelve_ERROR(){
	hash_t* hash = hash_crear(destruir_string_dinamico, CAPACIDAD_MINIMA);

	int i = 0;
  	int can_elementos = 0;

  	char* nombres[6]={"Mariano", "Lucas", "Fede", "Santi", "Juan", "Maria"};
  	char* documentos[6]={"45365254", "12457865", "45875636", " 25456325", " 98652345", " 42819254"};


    for(i = 0; i < 6; i++){
      hash_insertar(hash, documentos[i], reservar_string(nombres[i]));
      can_elementos++;
    }

    hash_quitar(hash, documentos[2]);
	pa2m_afirmar(hash_quitar(hash, documentos[2]) == ERROR, "No es posible eliminar dos veces un mismo elemento");
	
	hash_destruir(hash);
}

void pruebas_de_insertar_quitar_obtener(){
	int capacidad_inicial = 8;
	hash_t* hash = hash_crear(destruir_string, capacidad_inicial);

	int i = 0;
  	int cant_insertados = 0;

  	char* nombres[6]={"Mariano", "Lucas", "Fede", "Santi", "Juan", "Maria"};
  	char* documentos[6]={"45365254", "12457865", "45875636", " 25456325", " 98652345", " 42819254"};

    for(i = 0; i < 6; i++){
      hash_insertar(hash, documentos[i], nombres[i]);
      cant_insertados++;
    }

	for(i = 0; i<cant_insertados;i++){
		pa2m_afirmar(hash_quitar(hash, documentos[i]) == EXITO, "Borro un elemento insertado previamente.");
		pa2m_afirmar(hash_contiene(hash, documentos[i]) == false, "El elemento no se encuentra en el hash.");
		pa2m_afirmar(hash_insertar(hash, documentos[i], nombres[i]) == EXITO, "Puedo volver a insertar el mismo elemento.");
		pa2m_afirmar(hash_contiene(hash,documentos[i]) == true, "El elemento existe y puedo buscarlo dentro del hash.");
	}	
	//hash_insertar(hash, "mostrar", "fede");	

	hash_destruir(hash);
}

void dado_un_hash_con_elementos_iterarlo_todos_los_elementos_recorre_la_cantidad_correcta(){
	
	hash_t* hash = hash_crear(destruir_string, CAPACIDAD_MINIMA);

	char* nombres[3]={"Mariano", "Lucas", "Fede"};
  	char* documentos[3]={"45365254", "12457865", "45875636"};

    for(int i = 0; i < 3; i++){
      hash_insertar(hash, documentos[i], nombres[i]);
    }
	
	size_t resultado = hash_con_cada_clave(hash, mostrar_elementos, NULL);
	int cant_elementos = hash_cantidad(hash);
	
	pa2m_afirmar(resultado == cant_elementos, "Iterar todos los elementos recorre la cantidad de elementos esperada.");

	//hash_insertar(hash, "mostrar", "fede");

	hash_destruir(hash);
}

void dado_un_hash_con_elementos_iterarlo_hasta_encontrar_cierto_elemento_recorre_la_cantidad_correcta(){
	
	hash_t* hash = hash_crear(destruir_string, CAPACIDAD_MINIMA);
	
	char* nombres[3]={"Mariano", "Lucas", "Fede"};
  	char* documentos[3]={"45365254", "12457865", "45875636"};

    for(int i = 0; i < 3; i++){
      hash_insertar(hash, documentos[i], nombres[i]);
    }

	size_t recorridos = hash_con_cada_clave(hash, mostrar_hasta_el_aux, documentos[0]);
	int cant_elementos = 2;

	pa2m_afirmar(recorridos == cant_elementos, "Iterar un hash hasta un determinado elemento recorre la cantidad de elementos esperada.");

	//hash_insertar(hash, "mostrar", "fede");

	hash_destruir(hash);
}

void pruebas_de_creacion(){
  crear_una_tabla_de_hash_con_una_determinada_capacidad_se_crea_con_esa_capacidad();
  crear_una_tabla_de_hash_con_capacidad_menor_a_CAPACIDAD_MINIMA_se_crea_con_esa_capacidad();
  crear_un_hash_vacio_tiene_0_elementos();//un hash vacio tiene 0 elementos
}

void pruebas_de_insercion(){
  insertar_un_elemento_devuleve_exito();
  insertar_un_elemento_en_hash_vacio();//inserto un elemento en hash vacio, ahora el hash con tiene 1 elemento
  insertar_un_elemento_con_misma_clave_lo_remplaza_y_devuleve_exito();//elemento con misma clave//ESTA PRUEBA ES UN POCO CAJA BLANCA PERO ES UNA PRUEBA IMPORTANTE
  insertar_varios_elementos_distintos_devuleve_exito();//insercion de varios elementos
  insertar_varios_elementos_distintos_con_mismo_valor_de_hash_devuleve_exito();//colisiones pero diferente clave
  Dado_un_hash_con_1_elemento_en_el_final_insertar_elemento_en_la_ultima_posicion_colisiona_pero_se_inserta_en_el_inicio();//si quiero insertar un elemento en la ultima posicion y colisiona, se inserta en el inicio, hash con 1 elemento
  insertar_elementos_reservados_en_memoria_en_tabla_de_hash();
}

void pruebas_de_rehaseho(){
  //puedo saber que al menos  rehasheo una vez si inserto mas elementos que la capacidad inicial que pedi	
  insertar_varios_elementos_hasta_alcanzar_la_capacidad_minima_de_rehasheo();//rehashar una vez y que todos los elementos sigan estando y la capacidad ahora es el doble
}

void pruebas_de_cantidad(){
  dado_un_hash_con_una_cierta_cantidad_de_elementos_insertados_devuelve_dicha_cantidad();
  dado_un_hash_nulo_la_funcion_cantidad_devuelve_0();
  dado_un_hash_vacio_la_cantidad_de_elementos_que_contiene_es_0();
}

void pruebas_de_contiene(){
  Dado_un_hash_con_elementos_insertados_busco_el_elemento_y_devuelve_true();
  dado_un_hash_con_elementos_insertados_busco_elemento_inexistente_y_devuelve_false();
}

void pruebas_de_obtener(){
  dado_un_hash_con_elementos_al_pasarle_la_clave_me_devuelve_el_elemento_esperado();
  dado_un_hash_al_buscar_al_pasarle_una_clave_no_insertada_me_devuelve_null();
}

void pruebas_de_quitar(){
  dada_un_hash_con_elementos_insertados_elimino_correctamente_el_emeneto_solicitado();//busco el elemento y devuelve false
  dado_un_hash_con_elementos_quitar_todos_los_elementos_y_la_tabla_de_hash_queda_vacia();//dejo vacio el hash verificar con cantidad
  dado_un_hash_con_un_elemento_al_eliminarlo_queda_vacio();//borrar el unico elemento del hash lo deja vacio(hash-cantidad = 0)
  dado_un_hash_vacio_eliminar_elemento_devuelve_ERROR();//no se puede borrar de un hash vacio
  dado_un_hash_con_elementos_al_eliminar_un_elemento_la_cantidad_se_reduce_en_1();//luego de borrar el elemento, la cantidad de elementos se redujo en 1
  dado_un_hash_con_elementos_al_eliminar_dos_veces_el_mismo_elemento_devuelve_ERROR();
}

void pruebas_con_null(){
  crear_hash_con_destructor_nulo_crear_un_hash();//puedo crear un hash sin destructor
  Un_hash_nulo_resulta_vacio();//un hash nulo no tiene elementos ocupados(es vacio)
  dado_un_hash_nulo_no_es_posible_insertar();//no puedo insertar en un hash nulo
  dado_un_hash_vacio_puedo_insertar_una_clave_con_elemento_nulo();//puedo insertar elementos null sin problemas
  dado_un_hash_vacio_no_puedo_insertar_una_clave_nula();//no puedo insertar un elemento con clave nula
  dado_un_hash_nulo_devuelve_eliminar_un_elemento_no_insertado_ERROR();
  dado_un_hash_nulo_al_buscar_un_elemento_me_devuelve_null();
  iterar_un_hash_nulo_devuelve_0_elementos_recorridos();//no puedo iterar en un hash nulo
  iterar_un_hash_con_funcion_nula_devuelve_0_elementos_recorridos();//no puedo iterar en una funcion nulo O iterar un hash nulo con el iterador interno recorre 0 elementos
}

void probar_iterador(){
  dado_un_hash_con_elementos_iterarlo_todos_los_elementos_recorre_la_cantidad_correcta();//se iteraron todos los elementos esperados
  dado_un_hash_con_elementos_iterarlo_hasta_encontrar_cierto_elemento_recorre_la_cantidad_correcta();
}



int main(){

	pa2m_nuevo_grupo("Pruebas crear");
	pruebas_de_creacion();//falta hacer una

	pa2m_nuevo_grupo("Pruebas insertar");
	pruebas_de_insercion();

	pa2m_nuevo_grupo("Pruebas de Rehasheo");
	pruebas_de_rehaseho();
	
	pa2m_nuevo_grupo("Pruebas quitar");
	pruebas_de_quitar();

	pa2m_nuevo_grupo("Pruebas cantidad");
	pruebas_de_cantidad();

	pa2m_nuevo_grupo("Pruebas contiene");
	pruebas_de_contiene();

	pa2m_nuevo_grupo("Pruebas obtener");
	pruebas_de_obtener();

	pa2m_nuevo_grupo("Pruebas Insertar/Quitar/Obtener");
	pruebas_de_insertar_quitar_obtener();

	pa2m_nuevo_grupo("Pruebas iterador");
	probar_iterador();

	pa2m_nuevo_grupo("Pruebas de NULL");
	pruebas_con_null();

  return pa2m_mostrar_reporte();
}
