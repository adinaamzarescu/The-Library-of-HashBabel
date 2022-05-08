// Copyright 2022 Adina Amzarescu
#include <stdlib.h>
#include "./hashtbl.h"
#include "./db.h"
#include "./book.h"
#include "./user.h"

int db_init(struct database *db) {
  // Initialize the library hashtable
  if (ht_init(&db -> library, 10,
      free, free_f_book, cmp_str, hash_str) == -1) {
    return -1;
  }

  if (ht_init( & db -> users, 10,
      free, free_f_user, cmp_str, hash_str) == -1) {
    return -1;
  }

  return 0;
}

void db_deinit(struct database *db) {
  ht_deinit(&db -> library);
  ht_deinit(&db -> users);
}
