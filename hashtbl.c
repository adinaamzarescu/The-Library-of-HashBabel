#include <stdio.h>
#include <stdlib.h>
#include "hashtbl.h"

static struct hash_item* ht_find(const struct hashtbl * ht, const void* key){
  //compute the index for this key
  const unsigned int h = ht->hash_f(key) % ht->capacity;

  //search for the key
  struct hash_item * item = ht->table[h];
  while(item){
    if(ht->cmp_f(item->key, key) == 0){
      return item; //key is found
    }
    item = item->next;
  }

  return NULL; //doens't have key
}

static void ht_clear(struct hashtbl * ht){
  int i;
  for(i=0; i < ht->capacity; i++){

    struct hash_item * iter = ht->table[i];
    while(iter){
      struct hash_item * next = iter->next;

      ht->free_k(iter->key);
      ht->free_v(iter->value);
      free(iter);

      iter = next;
    }
    ht->table[i] = NULL;
  }
}

/* Check if the table is overcrowded */
static int ht_is_crowded(struct hashtbl * ht){
  const float load_factor = (ht->size / ht->capacity);
  return (load_factor > 1.0) ? 1 : 0;
}

static int ht_resize(struct hashtbl * ht){
  int i;

  //steal table from ht
  const int old_capacity = ht->capacity;
  struct hash_item** old_table = ht->table;

  //re-init the table with twice the capacity
  ht_init(ht, ht->capacity * 2, ht->free_k, ht->free_v, ht->cmp_f, ht->hash_f);

  //re-hash
  for(i=0; i < old_capacity; i++){
    struct hash_item * iter = old_table[i];
    while(iter){
      struct hash_item * next = iter->next;

      //add to new table (re-hash)
      ht_put(ht, iter->key, iter->value);
      //free old item
      free(iter);

      iter = next;
    }
  }
  free(old_table);
  return 0;
}

int ht_init(struct hashtbl * ht, const int capacity, FREE_F free_k, FREE_F free_v, CMP_F cmp_f, HASH_F hash_f){
  int i;

  ht->capacity = capacity;
  ht->size = 0;

  ht->table = (struct hash_item**) malloc(sizeof(struct hash_item**)*ht->capacity);
  if(ht->table == NULL){
    perror("malloc");
    return -1;
  }

  //set the key/value functions
  ht->free_k = free_k;
  ht->free_v = free_v;
  ht->cmp_f  = cmp_f;
  ht->hash_f = hash_f;

  for(i=0; i < ht->capacity; ++i){
    ht->table[i] = NULL;
  }

  return 0;
}

void ht_deinit(struct hashtbl * ht){
  ht_clear(ht);
  ht->size = 0;
}

int   ht_put(struct hashtbl * ht, const void* key, void * value){

  // get hash for this key
  const unsigned int h = ht->hash_f(key) % ht->capacity;

  // does it exist ?
  struct hash_item * item = ht->table[h];
  while(item){
    if(ht->cmp_f(key, item->key) == 0){
      return 0; //not inserted
    }
    item = item->next;
  }

  item = (struct hash_item *) malloc(sizeof(struct hash_item));
  if(item == NULL){
    perror("malloc");
    return -1;  //error
  }

  //add key value to item
  item->key = key;
  item->value = value;

  //prepend to bucket list
  item->next = ht->table[h];
  ht->table[h] = item;

  //update table size
  ht->size++;

  //test for resizing
  if(ht_is_crowded(ht)){
    ht_resize(ht);
  }

  return 1; //inserted
}

int ht_update(struct hashtbl * ht, const void* key, void * value){
  struct hash_item * item = ht_find(ht, key);
  if(item == NULL){
    return -1;
  }

  ht->free_v(item->value);
  item->value = value;

  return 0;
}

int   ht_remove(struct hashtbl * ht, const void* key){

  // get hash for this key
  const unsigned int h = ht->hash_f(key) % ht->capacity;

  struct hash_item * item = ht->table[h];
  struct hash_item * prev = NULL;
  while(item){
    if(ht->cmp_f(item->key, key) == 0){
      break;
    }
    prev = item;
    item = item->next;
  }

  if(item == NULL){ //if item not found
    return -1;
  }

  //unlink from list
  if(prev){
    prev->next          = item->next;
  }else{
    ht->table[h] = item->next;
  }

  //free the item
  ht->free_k(item->key);
  ht->free_v(item->value);
  free(item);

  //reduce table size
  ht->size--;

  return 0;
}

void* ht_get(const struct hashtbl * ht, const void* key){
  struct hash_item * item = ht_find(ht, key);
  return (item) ? item->value : NULL;
}

int ht_has_key(struct hashtbl * ht, const void* key){
  struct hash_item * item = ht_find(ht, key);
  return (item) ? 1 : 0;
}

void ** ht_array(struct hashtbl * ht){
  int i, len = 0;

  void ** array = (void **) malloc(sizeof(void*) * ht->size);
  if(array == NULL){
    perror("malloc");
    return NULL;
  }

  for(i=0; i < ht->capacity; i++){

    struct hash_item * iter = ht->table[i];
    while(iter){
      array[len++] = iter->value;
      iter = iter->next;
    }
  }

  return array;
}

int ht_size(struct hashtbl * ht){
  return ht->size;
}
