#ifndef DB_H
#define DB_H

#include "hashtbl.h"

struct database {
  //each book in the library is a hashtable
  struct hashtbl library; //hashtable {key=title, value=book}
  struct hashtbl users;  //hashtable {key=username, value=user}
};

int db_init(struct database * db);
void db_deinit(struct database * db);

#endif
