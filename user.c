#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "user.h"

struct user * user_create(const char * username){
  struct user * u = (struct user *) malloc(sizeof(struct user));
  if(u == NULL){
    perror("malloc");
    return NULL;
  }

  u->username = strdup(username);
  u->score = 100; //100 score at start

  //no book borrowed
  u->borrowed_book = NULL;
  u->days_available = 0;

  return u;
}

void user_free(struct user * u){
  free(u->username);
  free(u);
}

void free_f_user(void * arg){
  user_free((struct user *) arg);
}

int user_is_banned(const struct user * u){
  return (u->score < 0) ? 1 : 0;
}

int user_has_book(const struct user * u){
  return (u->borrowed_book == NULL) ? 0 : 1;
}

void user_borrows(struct user * u, const struct book * b, const int days){
  u->borrowed_book = b;
  u->days_available = days;
}


int user_returns(struct user * u, const struct book * b, const int days_borrowed){
  const int days_hold = u->days_available - days_borrowed;
  if(days_hold < 0){  //if hold more than requested
    u->score += days_hold;  //reduce score with days hold
  }else{  //faster return
    u->score += days_hold * 2;
  }

  u->borrowed_book = NULL;
  u->days_available = 0;

  return u->score;
}

int user_loses(struct user * u, const struct book * b){
  u->borrowed_book = NULL;
  u->days_available = 0;
  u->score -= 20;

  return u->score;
}

int cmp_f_user(const void * arg1, const void * arg2){
  const struct user * u1 = (const struct user *) arg1;
  const struct user * u2 = (const struct user *) arg2;

  if(u1->score < u2->score){
    return 1;
  }else if(u1->score > u2->score){
    return -1;
  }else{
    return strcmp(u1->username, u2->username);
  }
}

void user_details(const struct user * u){
  printf("Name:%s Points:%u\n", u->username, u->score);
}
