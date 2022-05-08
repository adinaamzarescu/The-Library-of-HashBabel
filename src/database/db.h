// Copyright 2022 Adina Amzarescu
#ifndef DB_H_
#define DB_H_

#include "../hashtable/hashtbl.h"

struct database {
  // Each book in the library is a hashtable
  struct hashtbl library;   // hashtable {key=title, value=book}
  // Each user is a hashtable
  struct hashtbl users;     // hashtable {key=username, value=user}
};

int db_init(struct database *db);

void db_deinit(struct database *db);

#endif  // DB_H_
