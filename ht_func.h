typedef void (*FREE_F)(void*);
typedef int (*CMP_F)(const void*, const void*);
typedef unsigned int (*HASH_F)(const void*);

int cmp_int(const void* x, const void * y);
unsigned int hash_int(const void* x);
int cmp_str(const void* x, const void * y);
unsigned int hash_str(const void* x);
