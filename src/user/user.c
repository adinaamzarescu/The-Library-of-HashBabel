// Copyright 2022 Adina Amzarescu
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./user.h"

struct user * user_create(const char *username) {
  struct user *u = (struct user *) malloc(sizeof(struct user));
  if (!u) {
    perror("malloc");
    return NULL;
  }

  u -> username = strdup(username);
  // 100 score at start
  u -> score = 100;

  // No book borrowed
  u -> borrowed_book = NULL;
  u -> days_available = 0;

  return u;
}

void user_free(struct user *u) {
  free(u -> username);
  free(u);
}

void free_f_user(void *arg) {
  user_free((struct user *) arg);
}

int user_is_banned(const struct user *u) {
  if (u -> score != -1)
    return 0;
  else
    return 1;
}

int user_has_book(const struct user *u) {
  return (u -> borrowed_book == NULL) ? 0 : 1;
}

void user_borrows(struct user *u, const struct book *b, const int days) {
  u -> borrowed_book = b;
  u -> days_available = days;
}

int user_returns(struct user *u, const struct book *b,
                 const int days_borrowed) {
  int days_hold = u -> days_available - days_borrowed;
  // If hold more than requested
  if (days_hold < 0) {
    // Reduce score with days hold
    u -> score += days_hold * 2;
  } else {
    // Returned before
    u -> score += days_hold;
  }

  u -> borrowed_book = NULL;
  u -> days_available = 0;
  return u -> score;
}

int user_loses(struct user *u, const struct book *b) {
  u -> borrowed_book = NULL;
  u -> days_available = 0;
  u -> score -= 50;
  return u -> score;
}

int cmp_f_user(const void *arg1, const void *arg2) {
  const struct user *u1 = *(const struct user **) arg1;
  const struct user *u2 = *(const struct user **) arg2;

  if (u1 -> score < u2 -> score) {
    return 1;
  } else if (u1 -> score > u2 -> score) {
    return -1;
  } else {
    return strcmp(u1 -> username, u2 -> username);
  }
}

void user_details(const struct user *u) {
  printf("Name:%s Points:%u\n", u -> username, u -> score);
}
