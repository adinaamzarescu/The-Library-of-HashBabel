// Copyright 2022 Adina Amzarescu
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./book.h"

// Load book content
static int book_load(struct book * b,
  const size_t def_number) {
  int i, rv = 0;
  char * line = NULL;
  char *saveptr;
  size_t line_len = 0;
  size_t line_size = 0;

  for (i = 0; i < def_number; i++) {
    if ((line_len = getline(&line, & line_size, stdin)) < 0) {
      perror("getline");
      rv = -1;
      break;
    }

    char * key = strtok_r(line, " \r\n", &saveptr);
    char * value = strtok_r(NULL, " \r\n", &saveptr);

    if ((key == NULL) || (value == NULL)) {
      fprintf(stderr, "Error: Invalid book line\n");
      rv = -1;
      break;
    }
    // Add content to book hashtable
    book_add(b, key, value);
  }

  if (line) {
    free(line);
  }

  return rv;
}

struct book * book_create(const char * book_name,
  const size_t def_number) {
  struct book * b = (struct book * ) malloc(sizeof(struct book));
  if (b == NULL) {
    perror("malloc");
    return NULL;
  }

  b -> book_name = strdup(book_name);
  b -> ratings = 0;
  b -> num_ratings = 0;
  b -> purchases = 0;
  b -> copies = 1;  // Only one book

  if (ht_init( & b -> content, def_number,
      free, free, cmp_str, hash_str) == -1) {
    free(b);
    return NULL;
  }

  if (book_load(b, def_number) < 0) {
    book_free(b);
    return NULL;
  }

  return b;
}

void book_free(struct book *b) {
  free(b -> book_name);
  ht_deinit(&b -> content);
  free(b);
}

void free_f_book(void * arg) {
  book_free((struct book*) arg);
}

void book_details(const struct book * b) {
  printf("Name:%s Rating:%.3f Purchases:%u\n",
          b -> book_name, book_rating(b), b -> purchases);
}

int book_add(struct book * b,
  const char * key,
    const char * val) {
  int rv = 0;

  char * v = strdup(val);

  if (ht_has_key(&b -> content, key)) {
    rv = ht_update(&b -> content, key, v);
    if (rv == -1) {
      free(v);
    }
  } else {
    char *k = strdup(key);
    rv = ht_put(&b -> content, k, v);
    if (rv != 1) {
      free(k);
      free(v);
    }
  }

  return rv;
}

int book_borow(struct book *b) {
  // If the book can be borrowed
  if ((b -> copies == 1)) {
    b -> copies = 0;
    return 1;
  }
  return 0;
}

void book_rate(struct book *b, int rating) {
  b -> purchases++;
  b -> ratings += rating;
  b -> num_ratings++;
}

float book_rating(const struct book *b) {
  if (!b -> num_ratings) {
    return 0.0f;
  }

  return (float) b -> ratings / b -> num_ratings;
}

int cmp_f_book_rating(const void * arg1,
  const void * arg2) {
  const struct book *b1 = *(const struct book **) arg1;
  const struct book *b2 = *(const struct book **) arg2;

  float b1_score = book_rating(b1);
  float b2_score = book_rating(b2);

  if (b1_score > b2_score) {
    return -1;
  } else if (b1_score < b2_score) {
    return 1;
  } else {
    if (b1 -> num_ratings < b2 -> num_ratings)
      return 1;
    else if (b1 -> num_ratings > b2 -> num_ratings)
      return -1;
    else
      return strcmp(b1 -> book_name, b2 -> book_name);
  }
}
