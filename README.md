# The-Library-of-HashBabel - Homework2
#### Copyright Adina-Maria Amzarescu
__________________________________________________________

Project that simulates a library.
It mainly uses hashtables.

The source code is in the src directory.

There are 5 directories for:
* book
* database
* hashtables
* user
* main

__________________________________________________________

HASHTABLE
__________________________________________________________

The hashtable directory contains the main functions
for a hashtable as well as the structure itself.
Most of the functions were taken from the laboratory
and edited in order to match this project.

The genericity of the structure is given by the void type
of data.

__________________________________________________________

DATABASE
__________________________________________________________

The database contains 2 hashtables.
    * for the users
    * for the library (also each book is a hastable)

__________________________________________________________

BOOK
__________________________________________________________

The book structure contains the name, the ratings,
the number of ratings, number of purchases as well
as the content of the book (a hashtable of definitions).
You will also find a variable, copies. This is only used
to see if a book can be borrowed or not. In this
implementation the only 2 possiblities are 1 and 0.

__________________________________________________________

USER
__________________________________________________________

Here you will find the functions for the users.
The main ones are:
    * add
    * borrow
    * return
    * lost

The cmp_f_user function is used for the TOP_USERS
ranking.

__________________________________________________________

Feedback:

I really liked this project, I enjoy reading and I had
a great time working on this. Unfortunatelly I started
too late and I wasn't able to fix the errors.

I understand that the problem is with 
`b -> content.capacity`
This becomes 0 after a while and I get the
"Integer divide by zero" error. I thought the problem
is with capacity so I transformed it in long long,
but it still didn't work.

I am sure the problem is something small but frustrating that
I missed. Overall I really liked working on this.

__________________________________________________________

Resources:

   1. [OCW](https://ocw.cs.pub.ro/courses/sd-ca/laboratoare/lab-04)

   2. [Journaldev](https://www.journaldev.com/35238/hash-table-in-c-plus-plus#:~:text=A%20Hash%20Table%20in%20C,value%20at%20the%20appropriate%20location.)

   3. [Hackerearth](https://www.hackerearth.com/practice/data-structures/hash-tables/basics-of-hash-tables/tutorial)

__________________________________________________________

