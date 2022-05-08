// Copyright 2022 Adina Amzarescu
#include <stdio.h>
#include <stdlib.h>
#include "./hashtbl.h"

/**
 * Function to find a key in the hashtable
 * @param ht  hashtable
 * @param key the key
 */
static struct hash_item *ht_find(const struct hashtbl *ht, const void *key) {
  // Compute the index for this key
  const unsigned int h = ht -> hash_f(key) % ht -> capacity;

  // Search for the key
  struct hash_item *item = ht -> table[h];
  while (item) {
    if (!ht -> cmp_f(item -> key, key))
      // Key is found
      return item;
    item = item -> next;
  }
  // Doesn't have key
  return NULL;
}

/**
 * Function to clear a hashtable and free the memory
 * @param ht hashtable
 */

static void ht_clear(struct hashtbl *ht) {
  for (int i = 0; i < ht -> capacity; ++i) {
    struct hash_item *iter = ht -> table[i];

    while (iter) {
      struct hash_item *next = iter -> next;

      ht -> free_k(iter -> key);
      ht -> free_v(iter -> value);
      free(iter);

      iter = next;
    }
    ht -> table[i] = NULL;
  }
}

/* Check if the table is overcrowded */
static int ht_is_crowded(struct hashtbl *ht) {
  const float load_factor = (ht -> size / ht -> capacity);
  return (load_factor > 1.0) ? 1 : 0;
}

static int ht_resize(struct hashtbl *ht) {
  // Steal table from ht
  const int old_capacity = ht -> capacity;
  struct hash_item **old_table = ht -> table;

  // Re-init the table with twice the capacity
  ht_init(ht, ht -> capacity * 2, ht -> free_k, ht -> free_v,
          ht -> cmp_f, ht -> hash_f);

  // Re-hash
  for (int i = 0; i < old_capacity; i++) {
    struct hash_item * iter = old_table[i];
    while (iter) {
      struct hash_item *next = iter -> next;

      // Add to new table (re-hash)
      ht_put(ht, iter -> key, iter -> value);
      // Free old item
      free(iter);

      iter = next;
    }
  }
  free(old_table);
  return 0;
}

int ht_init(struct hashtbl *ht, const int capacity, FREE_F free_k,
            FREE_F free_v, CMP_F cmp_f, HASH_F hash_f) {
  ht -> capacity = capacity;
  ht -> size = 0;

  ht -> table = (struct hash_item **) malloc(sizeof(struct hash_item **)
                *ht -> capacity);
  if (!ht -> table) {
    perror("malloc");
    return -1;
  }

  // Set the key/value functions
  ht -> free_k = free_k;
  ht -> free_v = free_v;
  ht -> cmp_f = cmp_f;
  ht -> hash_f = hash_f;

  for (int i = 0; i < ht -> capacity; ++i) {
    ht -> table[i] = NULL;
  }

  return 0;
}

void ht_deinit(struct hashtbl *ht) {
  ht_clear(ht);
  free(ht -> table);
  ht -> size = 0;
}

/**
 * Function to add a new value in the hashtable
 * @param ht    hashtable
 * @param key   value's key
 * @param value the value
 */

int ht_put(struct hashtbl *ht, void *key, void *value) {
  // Test for resizing
  if (ht_is_crowded(ht)) {
    ht_resize(ht);
  }

  // Get hash for this key
  const unsigned int h = ht -> hash_f(key) % ht -> capacity;

  // Does it exist ?
  struct hash_item *item = ht -> table[h];

  while (item) {
    if (ht -> cmp_f(key, item -> key) == 0) {
      // Not inserted
      return 0;
    }
    item = item -> next;
  }

  item = (struct hash_item *) malloc(sizeof(struct hash_item));

  if (!item) {
    perror("Malloc for item failed");
    return -1;
  }

  // Add key value to item
  item -> key = key;
  item -> value = value;

  // Prepend to list
  item -> next = ht -> table[h];
  ht -> table[h] = item;

  // Update table size
  ht -> size++;

  // Inserted
  return 1;
}

int ht_update(struct hashtbl *ht, const void *key, void *value) {
  struct hash_item *item = ht_find(ht, key);
  if (!item) {
    perror("Malloc for item failed");
    return -1;
  }

  ht -> free_v(item -> value);
  item -> value = value;

  return 0;
}

/**
 * Function to remove a key and its value
 * @param ht    hashtable
 * @param key   the key
 */
int ht_remove(struct hashtbl *ht, const void *key) {
  // Get hash for this key
  const unsigned int h = ht -> hash_f(key) % ht -> capacity;

  struct hash_item *item = ht -> table[h];
  struct hash_item *prev = NULL;

  while (item) {
    if (!ht -> cmp_f(item -> key, key))
      break;
    prev = item;
    item = item -> next;
  }
  // If the item is not found
  if (!item) {
    return -1;
  }

  // Unlink from list
  if (prev)
    prev -> next = item -> next;
  else
    ht -> table[h] = item -> next;

  // Free the item
  ht -> free_k(item -> key);
  ht -> free_v(item -> value);
  free(item);

  // Reduce table size
  ht -> size--;

  return 0;
}

/**
 * Function to find a value based on its key
 * @param ht    hashtable
 * @param key   the key
 */

void * ht_get(const struct hashtbl *ht, const void *key) {
  struct hash_item *item = ht_find(ht, key);
  return (item) ? item -> value : NULL;
}

int ht_has_key(struct hashtbl *ht, const void *key) {
  struct hash_item *item = ht_find(ht, key);
  return (item) ? 1 : 0;
}

void ** ht_array(struct hashtbl *ht) {
  int len = 0;

  void **array = (void **) malloc(sizeof(void *) *ht -> size);
  if (!array) {
    perror("malloc");
    return NULL;
  }

  for (int i = 0; i < ht -> capacity; i++) {
    struct hash_item *iter = ht -> table[i];
    while (iter) {
      array[len++] = iter -> value;
      iter = iter -> next;
    }
  }

  return array;
}

int ht_size(struct hashtbl *ht) {
  return ht -> size;
}
