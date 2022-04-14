#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "db.h"
#include "book.h"
#include "user.h"

/* Helper function to split a line into tokens */
static int splits_line(char * line, ssize_t line_len, char * args[], const size_t args_size){
  int i = 0, j = 0;
  char head = ' ';

  //split line into tokens
  while((i < args_size) && (j < line_len)){

    args[i++] = &line[j];

    //find end of token
    while(j < line_len){
      //if end is space or "
      if(line[j] == head){
        line[j++] = '\0';
        break;
      }
      ++j;
    }

    //skip until beginning of next token
    while(j < line_len){
      //if end is space or "
      if((line[j] == ' ') || (line[j] == '"')){
        head = line[j];
        line[j++] = '\0';
        break;
      }else if(isascii(line[j])){
        break;
      }
      ++j;
    }

    if(line[j+1] == '"'){
      head = '"';
      j+= 2;  //skip leading "
    }
  }

  return i; //return number of arguments
}

static void cmd_add_book(struct database * db, const char * book_name, const size_t num_def){
  //create the book
  struct book * b = book_create(book_name, num_def);
  if(b){
    //add to library
    char * key = strdup(b->book_name);
    ht_put(&db->library, (void*) key, (void*) b);
  }
}

static void cmd_get_book(struct database * db, const char * book_name){
  const struct book * b = ht_get(&db->library, book_name);
  if(b){
    book_details(b);
  }else{
    printf("The book is not in the library.\n");
  }
}

static void cmd_rmv_book(struct database * db, const char * book_name){
  if(ht_remove(&db->library, book_name) < 0){
    printf("The book is not in the library.\n");
  }
}

static void cmd_add_def(struct database * db,
  const char * book_name, const char * key, const char * val){

  struct book * b = ht_get(&db->library, book_name);
  if(b){
    book_add(b, key, val);
  }else{
    printf("The book is not in the library.\n");
  }
}

static void cmd_get_def(struct database * db, const char * book_name, const char * def_key){
  const struct book * b = ht_get(&db->library, book_name);
  if(b){
    char * def_val = ht_get(&b->content, def_key);
    if(def_val == NULL){
      printf("The definition is not in the book.\n");
    }else{
      printf("%s\n", def_val);
    }
  }else{
    printf("The book is not in the library.\n");
  }
}

static void cmd_rmv_def(struct database * db, const char * book_name, const char * def_key){
  struct book * b = ht_get(&db->library, book_name);
  if(b){
    if(ht_remove(&b->content, def_key) < 0){
      printf("The definition is not in the book.\n");
    }
  }else{
    printf("The book is not in the library.\n");
  }
}

static void cmd_add_user(struct database * db, const char * user_name){
  //create the user
  struct user * u = user_create(user_name);
  if(u){
    char * key = strdup(user_name);
    if(ht_put(&db->users, (void*) key, (void*) u) == 0){
      printf("User is already registered.\n");
    }
  }
}

static void cmd_borrow(struct database * db,
  const char * user_name, const char * book_name, const int days_available){

  struct user * u = ht_get(&db->users, user_name);
  if(u){
    if(user_is_banned(u)){
      printf("You are banned from this library\n");
    }else if(user_has_book(u)){
      printf("You have already borrowed a book.\n");
    }else{
      struct book * b = ht_get(&db->library, book_name);
      if(b){
        if(book_borow(b) == 0){ //if book can't be taken
          printf("The book is borrowed.\n");
        }else{
          user_borrows(u, b, days_available);
        }
      }else{
        printf("The book is not in the library.\n");
      }
    }
  }else{
    printf("You are not registered yet.\n");
  }
}

static void cmd_return(struct database * db,
  const char * user_name, const char * book_name, const int days_since_borrow, const int rating){

  struct user * u = ht_get(&db->users, user_name);
  if(u){
    if(user_is_banned(u)){
      printf("You are banned from this library\n");
    }else{
      struct book * b = ht_get(&db->library, book_name);
      if(b){
        if(u->borrowed_book != b){
          printf("You didn't borrow this book.\n");
        }else{
          if(user_returns(u, b, days_since_borrow) < 0){
            printf("The user %s has been banned.\n", user_name);
          }
          book_rate(b, rating);
        }
      }else{
        printf("The book is not in the library.\n");
      }
    }
  }else{
    printf("You are not registered yet.\n");
  }
}

static void cmd_lost(struct database * db, const char * user_name, const char * book_name){
  struct user * u = ht_get(&db->users, user_name);
  if(u){
    if(user_is_banned(u)){
      printf("You are banned from this library\n");
    }else{
      struct book * b = ht_get(&db->library, book_name);
      if(b){
        if(u->borrowed_book != b){
          printf("You didn't borrow this book.\n");
        }else{
          if(user_loses(u, b) < 0){
            printf("The user %s has been banned.\n", user_name);
          }
        }
      }else{
        printf("The book is not in the library.\n");
      }
    }
  }else{
    printf("You are not registered yet.\n");
  }
}

static void cmd_top_books(struct database * db){
  const struct book ** books = (const struct book **) ht_array(&db->library);
  printf("Books ranking:\n");
  if(books){
    int i;
    const int num_books = ht_size(&db->library);;

    qsort(books, num_books, sizeof(struct book*), cmp_f_book_rating);

    for(i=0; i < num_books; i++){
      printf("%d. ", i+1);
      book_details(books[i]);
    }
    free(books);
  }
}

static void cmd_top_users(struct database * db){
  const struct user ** users = (const struct user **) ht_array(&db->users);
  printf("Users ranking:\n");
  if(users){
    int i;
    const int num_users = ht_size(&db->users);

    qsort(users, num_users, sizeof(struct user*), cmp_f_user);

    for(i=0; i < num_users; i++){
      if(user_is_banned(users[i]) == 0){
        printf("%d. ", i+1);
        user_details(users[i]);
      }
    }
    free(users);
  }
}

/* Routing function to run a command from line */
static int run_line(char * const args[], const size_t nargs, struct database * db){
  const char * cmd = args[0];

  /* book commands */
  if(strcmp(cmd, "ADD_BOOK") == 0){
    cmd_add_book(db, args[1], atoi(args[2]));

  }else if(strcmp(cmd, "GET_BOOK") == 0){
    cmd_get_book(db, args[1]);

  }else if(strcmp(cmd, "RMV_BOOK") == 0){
    cmd_rmv_book(db, args[1]);

  }else if(strcmp(cmd, "ADD_DEF") == 0){
    cmd_add_def(db, args[1], args[2], args[3]);

  }else if(strcmp(cmd, "GET_DEF") == 0){
    cmd_get_def(db, args[1], args[2]);

  }else if(strcmp(cmd, "RMV_DEF") == 0){
    cmd_rmv_def(db, args[1], args[2]);

  /* user commands */
  }else if(strcmp(cmd, "ADD_USER") == 0){
    cmd_add_user(db, args[1]);

  }else if(strcmp(cmd, "BORROW") == 0){
    cmd_borrow(db, args[1], args[2], atoi(args[3]));

  }else if(strcmp(cmd, "RETURN") == 0){
    cmd_return(db, args[1], args[2], atoi(args[3]), atoi(args[4]));

  }else if(strcmp(cmd, "LOST") == 0){
    cmd_lost(db, args[1], args[2]);

  /* ranking commands */
  }else if(strcmp(cmd, "TOP_BOOKS") == 0){
    cmd_top_books(db);

  }else if(strcmp(cmd, "TOP_USERS") == 0){
    cmd_top_users(db);

  }else if(strcmp(cmd, "EXIT") == 0){
    return -1;
  }

  return 0;
}

int main(const int argc, char * argv[]){
  int nargs;
  char * line = NULL, *args[10];
  ssize_t line_len = 0;
  size_t line_size = 0;
  struct database db;

  if(db_init(&db) < 0){
    return -1;
  }

  //read command from input
  while((line_len = getline(&line, &line_size, stdin)) > 0){

    if(line[line_len-1] == '\n'){
      line[--line_len] = '\0';
    }
    if(line[line_len-1] == '\r'){
      line[--line_len] = '\0';
    }

    nargs = splits_line(line, line_len, args, sizeof(args) / sizeof(char*));
    if(nargs == 0){
      continue;
    }

    if(run_line(args, nargs, &db) < 0){
      break;
    }
  }

  if(line){
    free(line);
  }

  db_deinit(&db);

  return 0;
}
