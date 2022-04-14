The main idea: Implementing a hashtable

You have to implement a library.

You have to:
* Implement a generic  Hhashtable
* Implement a data base for the library

__________________________________________________________________

HASHTABLE:

* It should support any type of data
* It should use an efficient hash function
* Treat the collusions by chaining
* Has the resize functionality

* Functions:
	* Put(key, value)
	* Remove(key)
	* Get(key)
	* Has_Key(key)

* Resizing
	In order to make it efficient, you have to
	resize the hashtable when it's too crowded.

	load_factor = number_of_elements / buckets
	
	If load_factor > 1, the structure is too crowded.
	You don't have to implement the case when load_factor
	is too low.
	
	Steps:
	* Realloc the bucket array (2 * buckets)
	* Rehashing

	!! you can't use a high value as teh initial
	value of the bucket array, that number should be 
	<= 10 !!
__________________________________________________________________

LIBRARY:

The users will interact with the library by using commands.

Dictionary data structure:
	* Pair
		* def_key
		* def_val
	* It doesn't have to by sorted by key
	* def_key and def_val are 2 strings (2 words)
		ex: mamal:cat
		    reptile:snake
		    bird:parrot
	* You have to implement a data base that simulates the library shelves.
	* The whole library should be a hashtable
	* Each book should be a hashtable

Commands for the library/dictionary:

A book can be added or removed from a library.
A definition can be added or removed.

* ADD_BOOK <book_name> <def_number>
	<def_key1> <def_val1>
	<def_key2> <def_val2>
	<def_key3> <def_val3>

	This commands adds the book in the library. 
	There are no 2 books with the same name.
	The name of the book can contain multiple words, separated by the ' " ' char.

	length(book_name) <= 40
	def_number = number of definitions of a book.
	def_key <= 20 (string)
	def_val <= 20 (string)

* GET_BOOK <book_name>

	Shows the details of the book.
	If the book isn't in the library, message: "The book is not in the library."

	print: Name:book_name Rating:book_rating Purchase:book_purchases

* RMV_BOOK <book_name>
	
	Removes a book from the library.
	If the book isn't in the library, message: "The book is not in the library."

* ADD_DEF <book_name> <def_key> <def_val>
	
	Adds a new definition for the book. 
	If there already is a definitition with the same key, the value will be changed with
	the new one.
	If the book isn't in the library, message: "The book is not in the library."

* GET_DEF <book_name> <def_key>
	
	Shows the definition from def_key.
	If the definition is not in the book, message: "The definition is not in the book."
	If the book isn't in the library, message: "The book is not in the library."

* RMV_DEF <book_name> <def_key>

	Removes the definition from def_key.
	If the definition is not in the book, message: "The definition is not in the book."
	If the book isn't in the library, message: "The book is not in the library."

__________________________________________________________________

USERS:

You will also use a hashtable for the users.
An user can borrow, return or lose a book.
When an user return a book, they also give that book a rating.

In the end there will be a competition to determine the best users and the best books.
So each user will have a score. At the beginning every user has 100p.
When they borrow a book they will have to give a number of days until the returning.
If the user returns it faster, they gain points. If they return it after the number of
days given, they lose points.

If an user has a score < 0 they will be removed from the system.
They will no longer be able to participate and will not enter the system again. (banned)

Commands:

* ADD_USER <user_name>
	
	Adds a new user in the system.
	If the user already exists, message: "User is already registered."
	user_name <= 20

* BORROW <user_name> <book_name> <days_available>
	
	The book will remain in the system but will be marked as unreachable until
	it is returned. 
	An user can borrow a book only if they have no other books borrowed.

	If the user is not in the system, message: "You are not registered yet."
	If the user has been removed, message: "You are banned from this library"
	If the book isn't in the library, message: "The book is not in the library."
	If the book is unreachable, message: "The book is borrowed."
	If they already have a book, message: "You have already borrowed a book."

* RETURN <user_name> <book_name> <days_since_borrow> <rating>
	
	An user returns a book. They also give a rating.
	
	If they returned it after they said they would, they will lose points (-1p per day).
	If they return a book faster, they will gain points (+2p per day)
	If their score < 0, they will be banned. + message: "The user <user_name> has been banned."
	If the user has been removed, message: "You are banned from this library".
	If the user returns a book that they didn't borrow, message: "You didn't borrow this book.".
	If they didn't borrow a book but they want to return, message: "You didn't borrow this book.".

* LOST <user_name> <book_name>

	That user will lose 20p.
	The book will be removed from the library.

	If their score < 0, they will be banned. + message: "The user <user_name> has been banned."
	If the user is not in the system, message: "You are not registered yet.".
	If the user has been removed, message: "You are banned from this library".

__________________________________________________________________

END:

Command : "EXIT"

There will be 2 rankings:

* TOP_BOOKS

	Prints the books in the library, sorted by rating.
	Rating: (all ratings)/(number of purchases)
	Purchases = the number of times a book has been borrowed.
	A purchase is valid if the user rated the book when they returned it.
	The reting will be printed with 3 decimals.
	If there are 2 books with the same rating, the top one will be the
	one with more purchases.
	If there are 2 books with the same rating and the same number of
	purchases, they will be sorted by name. 
	

	Books ranking:

	1. Name: Rating: Purchases:
	2. Name: Rating: Purchases:

* TOP_USERS

	Prints the users sorted by their score.
	Only the users thay weren't removed (banned) will be printed.
	If 2 users have the same score, sort them by name.
	
	Users ranking:

		1. Name: Points:
		2. Name: Points:

__________________________________________________________________

IMPORTANT

You have to use a generic hashtable.
The commands must use the functionality of the hashtable.
	ex: GET_DEF will use get()
Reading from stdin and printing to stdout.

You have to use dynamic memory allocation!
You can't use global variables.