// Copyright 2022 Adina Amzarescu
#include <string.h>
#include "./ht_func.h"

/* Compare two items of type int */
int cmp_int(const void *x, const void *y) {
  const int x1 = *(int*) x;
  const int y1 = *(int*) y;
  if (x1 < y1) {
    return 1;
  } else if (x1 > y1) {
    return -1;
  } else {
    return 0;
  }
}

/* Hash an item of type int */
unsigned int hash_int(const void* x){
  return *(unsigned int*) x;
}

/* Compare two items of type int */
int cmp_str(const void *x, const void *y) {
  const char *x1 = (const char*) x;
  const char *y1 = (const char*) y;
  return strcmp(x1, y1);
}

/* Hash an item of type int */
unsigned int hash_str(const void *x) {
  char *str = (char*) x;
  int c;
  unsigned long hash = 5381;
  while ((c = *str++) != 0) {
    hash = ((hash << 5u) + hash) + c;
  }

  return hash;
}
