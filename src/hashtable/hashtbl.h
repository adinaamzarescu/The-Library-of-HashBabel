// Copyright 2022 Adina Amzarescu
#ifndef HASHTBL_H_
#define HASHTBL_H_

#include "./ht_func.h"

struct hash_item {
  void *key;
  void *value;

  struct hash_item *next, *prev;
};

struct hashtbl {
  int capacity, size;
  struct hash_item **table;

  FREE_F free_k;    // Free function for the value of a key
  FREE_F free_v;    // Free function for the value of a key
  CMP_F cmp_f;      // Key compare function
  HASH_F hash_f;    // Key hash function
};

int ht_init(struct hashtbl *ht, const int capacity,
            FREE_F free_k, FREE_F free_v, CMP_F cmp_f, HASH_F hash_f);

void ht_deinit(struct hashtbl *ht);

int ht_put(struct hashtbl *ht, void *key, void *value);

int ht_update(struct hashtbl *ht, const void *key, void *value);

int ht_remove(struct hashtbl *ht, const void *key);

void *ht_get(const struct hashtbl *ht, const void *key);

int ht_has_key(struct hashtbl *ht, const void *key);

void **ht_array(struct hashtbl *ht);

int ht_size(struct hashtbl * ht);

#endif  // HASHTBL_H_
