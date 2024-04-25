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

int resolverColision(HashMap* map, int pos){
  int copiaPos = pos; // preservar la posicion original
  pos = (pos+1)%map->capacity;
  while(pos != copiaPos){ // recorrer mapa hasta llegar a la posicion inicial
    if(map->buckets[pos]== NULL || map->buckets[pos]->key == NULL) return pos;
    pos = (pos+1)%map->capacity; // avanzar posicion
  }
  return -1;
}

void insertMap(HashMap * map, char * key, void * value) {
  int pos = hash(key, map->capacity);
  if(map->buckets[pos] == NULL || is_equal(map->buckets[pos]->key, key)){ // condicion
    map->buckets[pos] = createPair(key, value);
    map->size++; // aumentar cantidad de elementos
  }
  else{
    int nuevaPos = resolverColision(map, pos); // encontrar nueva posicion
    map->buckets[nuevaPos] = createPair(key, value);
    map->size++; // aumentar cantidad de elementos
  }
}


void enlarge(HashMap * map) {
  Pair **oldBuckets = map->buckets; // preservar buckets
  long originalCapacity = map->capacity; // preservar capacidad
  map->capacity *=2; // duplicar capacidad
  map->buckets = (Pair **)calloc(map->capacity, sizeof(Pair *)); // crear nuevo arreglo con nueva capacidad
  for(int i = 0; i < originalCapacity; i++){ // recorrer para mover los buckets nuevos 
    if (oldBuckets[i] != NULL && oldBuckets[i]->key != NULL) {
      insertMap(map, oldBuckets[i]->key, oldBuckets[i]->value);
      free(oldBuckets[i]);
    }
  }
  free(oldBuckets); // limpiar el arreglo
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
  int pos = hash(key, map->capacity);
  while(map->buckets[pos]!= NULL && map->buckets[pos]->key != NULL){
    if(is_equal(map->buckets[pos]->key, key)){ // si coinciden claves borramos la pareja de datos y le asignamos NULL
      free(map->buckets[pos]->key);
      free(map->buckets[pos]->value);
      map->buckets[pos]->key = NULL;
      map->buckets[pos]->value = NULL;
      map->size--; // reducir tamaño
      return;
    }
    pos = (pos+1)%map->capacity; // avanzar posicion
  }

}

Pair * searchMap(HashMap * map,  char * key) {   
  if(map == NULL || key==NULL) return NULL;
  int pos = hash(key, map->capacity);
  while(map->buckets[pos]!= NULL && map->buckets[pos]->key != NULL){
    if(is_equal(map->buckets[pos]->key, key)){ // si la clave coincide
      map->current = pos; // actualizamos el indice actual
      return map->buckets[pos]; // retornamos el par
    }
    pos = (pos+1)%map->capacity; // avanzamos posicion
  }
  return NULL;
}


Pair * firstMap(HashMap * map) {
  for (int i = 0; i < map->capacity; i++) { // recorrer mapa
      if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
          map->current = i; // actualizar actual
          return map->buckets[i];
      }
  }
    return NULL;
}

Pair * nextMap(HashMap * map) {
  for (int i = map->current + 1; i < map->capacity; i++) { // recorrer mapa desde el indice actual
    if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
        map->current = i;
        return map->buckets[i];
    }
  }
  return NULL;
}
