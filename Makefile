CC=gcc
CFLAGS=-Wall -g
OBJECTS=main.o hashtbl.o ht_func.o db.o book.o user.o

build: main 

main: $(OBJECTS)
	$(CC) $(CFLAGS) -o main $(OBJECTS)

main.o: ./src/main/main.c
	$(CC) $(CFLAGS) -c ./src/main/main.c

ht_func.o: ./src/hashtable/ht_func.c ./src/hashtable/ht_func.h
	$(CC) $(CFLAGS) -c ./src/hashtable/ht_func.c

hashtbl.o: ./src/hashtable/hashtbl.c ./src/hashtable/hashtbl.h
	$(CC) $(CFLAGS) -c ./src/hashtable/hashtbl.c

db.o: ./src/database/db.c ./src/database/db.h
	$(CC) $(CFLAGS) -c ./src/database/db.c

book.o: ./src/book/book.c ./src/book/book.h
	$(CC) $(CFLAGS) -c ./src/book/book.c

user.o: ./src/user/user.c ./src/user/user.h
	$(CC) $(CFLAGS) -c ./src/user/user.c

check: build
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
         ./src/main < ./src/in/test1.in

clean:
	rm main $(OBJECTS)
