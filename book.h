#include "hashtbl.h"

struct book {
  char * book_name;
  int ratings;
  int num_ratings;
  unsigned int copies;  //number of book copies available
  unsigned int purchases;
  struct hashtbl content;
};

struct book * book_create(const char * book_name, const size_t def_number);
void book_free(struct book * b);
void free_f_book(void * arg);

void book_details(const struct book * b);
//Add a def key/value to book
int book_add(struct book * b, const char * key, const char * val);
int book_borow(struct book * b);
void book_rate(struct book * b, int rating);

float book_rating(const struct book * b);

int cmp_f_book_rating(const void * arg1, const void * arg2);
