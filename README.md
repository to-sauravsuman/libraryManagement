# library-management-system

## Task
- To Implement a Library Management System in C++ language using Object-oriented programming concepts. 
- Need to implement the system on command line interface.  

## How to run    
* Open the console
* Run 
```
cd src  
g++ src/main.cpp -o a
./a
```

## Directories   
## src
* `main.cpp` : contains the main code to run the program
* `model.h` : contains the required classes
* `urls.cpp` : contains the location of databases stored in `db`  
* `views.cpp` : contains the functions to execute function present in classes

## db
* `user.txt`  : stores the list of users and their passwords 
Format: sno name id password role 

* `book.txt`: stores the list of books in the library  
Format: sno title author isbn pblication

* `cookie.txt`: stores the information of logged in user 
Format: sno name id password role 

* `user_book.txt`: stores the user and the corresponding issued book.
Format: sno_user sno_book day month year numBooks

## Functionalities
* ``Librarian`` can add, update, delete a user or a book; can see the list of available user or books; can see which book is issued to which user and which user has which books issued; can clear the fine of a respective user corresponding to a particular book.

* ``Student`` can see all the available books, see the list of books s/he has issued along with his/her current fine, can check if he/she can issue a particular book or not, and can issue a book.

* ``Professors`` can see the list of all the books, see the list of books he/she has along with the fine amount, and can can issue a book, and can check whether a particular book is available for issue or not .

