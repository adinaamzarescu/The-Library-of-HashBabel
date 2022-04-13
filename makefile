CC=gcc
CFLAGS=-Wall -g
OBJECTS=main.o hashtbl.o ht_func.o db.o book.o user.o

main: $(OBJECTS)
	$(CC) $(CFLAGS) -o main $(OBJECTS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

ht_func.o: ht_func.c ht_func.h
	$(CC) $(CFLAGS) -c ht_func.c

hashtbl.o: hashtbl.c hashtbl.h
	$(CC) $(CFLAGS) -c hashtbl.c

db.o: db.c db.h
	$(CC) $(CFLAGS) -c db.c

book.o: book.c book.h
	$(CC) $(CFLAGS) -c book.c

user.o: user.c user.h
	$(CC) $(CFLAGS) -c user.c

clean:
	rm main $(OBJECTS)
