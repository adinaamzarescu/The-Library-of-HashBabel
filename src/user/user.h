// Copyright 2022 Adina Amzarescu
#ifndef USER_H_
#define USER_H_

struct user{
  char *username;
  unsigned int score;
  const struct book *borrowed_book;
  unsigned int days_available;
};

struct user * user_create(const char *book_name);

void user_free(struct user *b);

void free_f_user(void *arg);

int user_is_banned(const struct user *u);

int user_has_book(const struct user *u);

void user_borrows(struct user *u, const struct book *b, const int days);

int user_returns(struct user *u, const struct book *b, const int days);

int user_loses(struct user *u, const struct book *b);

void user_details(const struct user *u);

int cmp_f_user(const void *arg1, const void *arg2);

#endif  // USER_H_
