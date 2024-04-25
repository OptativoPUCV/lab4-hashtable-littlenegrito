#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}
long resuelveColision(HashMap *map, int posicion, char *key) {
    // Verificar si la posición está dentro de los límites
    if (posicion < 0 || posicion >= map->capacity)
        return -1; // Valor de retorno para indicar error

    // Usar resolución de colisiones lineal
    int i = 1; // Contador para el desplazamiento
    long index = (posicion + i) % map->capacity; // Calcular el nuevo índice

    // Buscar el siguiente índice vacío o el índice con la misma clave
    while (map->buckets[index] != NULL && !is_equal(map->buckets[index]->key, key)) {
        i++;
        index = (posicion + i) % map->capacity;
    }

    return index; // Devolver el índice encontrado
}


void insertMap(HashMap * map, char * key, void * value) {
    if(map == NULL || key == NULL)
        return;

    long index = hash(key, map->capacity);

    // If the bucket at the index is empty, create a new pair and insert it
    if(map->buckets[index] == NULL) 
    {
        map->buckets[index] = createPair(key, value);
        map->size++;
    } 
    else 
    {
        long current = index; // current indica la posición actual en la tabla hash
        while(map->buckets[current] != NULL) 
        {
            if(is_equal(map->buckets[current]->key, key)) 
            {
                map->buckets[current]->value = value;
                return;
            }
            current = resuelveColision(map, current, key); // Actualizamos current según la resolución de colisiones
        }
        Pair *new_pair = createPair(key, value);
        map->buckets[current] = new_pair; // Insertamos el nuevo par en la posición encontrada
        map->size++;
    }
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)


}


HashMap * createMap(long capacity) {
  HashMap * map = (HashMap *)malloc(sizeof(HashMap)); // reservar memoria
  map->buckets = (Pair **) calloc(capacity, sizeof(Pair *)); // reservar memoria casilla
  map->capacity = capacity; // asignar la capacidad
  map->size = 0; // inicializar el tamaño
  map->current = -1; // inicializar el indice actual
  return map;
}

void eraseMap(HashMap * map,  char * key) {    


}

Pair * searchMap(HashMap * map,  char * key) {   


    return NULL;
}

Pair * firstMap(HashMap * map) {

    return NULL;
}

Pair * nextMap(HashMap * map) {

    return NULL;
}
