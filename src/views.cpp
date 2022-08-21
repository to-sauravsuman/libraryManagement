// contains the function assossiated with the models
#include "bits/stdc++.h"
#include "fstream"
#include <ctime>
using namespace std;

void pr() {
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << endl;
}

vector<long long> today() {
    time_t now = time(0);
    tm* ltm = localtime(&now);

    long long yr = 1900 + ltm->tm_year;
    long long mn = 1 + ltm->tm_mon;
    long long dy = ltm->tm_mday;

    return { dy, mn, yr };
}

long long total_days(long long dd, long long mm, long long yyyy) {
    long long days = dd;
    days += (yyyy * 12 * 30);
    days += (mm * 30);
    return days;
}

// book database
void book_database::Display() {
    cout << "------------------------Books----------------------------" << endl;
    if ((int)books.size()) {
        cout << "Title" << " " << "Author" << " " << "ISBN" << " " << "Publication" << endl << endl;
        for (auto& book : books) {
            Book* x = book.second;
            cout << x->Title << " " << x->Author << " " << x->ISBN << " " << x->Publication << endl;
        }
    }
    cout << "---------------------------------------------------------" << endl;
    cout << endl;
}

Book* book_database::get_book(string sno) {
    ifstream db_books;
    db_books.open(DB_BOOK);
    Book* book = NULL;

    string book_sno, book_title, book_author, book_ISBN, book_publication;
    while (db_books >> book_sno >> book_title >> book_author >> book_ISBN >> book_publication) {
        if (book_sno == sno) {
            book = new Book(book_title, book_author, book_ISBN, book_publication);
        }
    }
    db_books.close();
    return book;
}

// user database
void user_database::Display() {
    cout << "------------------------Users----------------------------" << endl;
    if ((int)users.size()) {
        cout << "[Name" << " " << "ID" << " " << "Password" << " " << "Role]" << endl << endl;
        for (auto& user : users) {
            User* x = user.second;
            cout << x->Name << " " << x->ID << " " << x->password << " " << x->role << endl;
        }
    }
    cout << "---------------------------------------------------------" << endl;
    cout << endl;
}

User* user_database::get_user(string user_sno) {
    ifstream dbuser;
    dbuser.open(DB_USER);
    User* user = NULL;

    string sno, name, pwd, id, role;
    while (dbuser >> sno >> name >> id >> pwd >> role) {
        if (user_sno == sno) {
            user = new User(name, id, pwd, role);
        }
    }
    dbuser.close();
    return user;
}


// user
string User::get_sno() {
    string sno;

    ifstream db_users;
    db_users.open(DB_USER);
    string user_sno, user_name, user_id, user_password, user_role;
    while (db_users >> user_sno >> user_name >> user_id >> user_password >> user_role) {
        if (user_name == this->Name && user_password == this->password && user_id == this->ID) {
            sno = user_sno;
            break;
        }
    }
    db_users.close();
    return sno;
}

void User::view_issued_books() {
    string user_sno = get_sno();

    string sno_user, sno_book, day, month, year, num;

    ifstream db_user_book;
    db_user_book.open(DB_USER_BOOK);
    book_database* bookdb = new book_database();
    user_database* userdb = new user_database();

    string x = "30";
    string user_role = userdb->get_user(user_sno)->role;
    if (user_role != "Student") x = "60";

    cout << "------------------------Books Issued----------------------------" << endl;
    while (db_user_book >> sno_user >> sno_book >> day >> month >> year >> num) {
        if (sno_user == user_sno) {
            Book* this_book = bookdb->get_book(sno_book);
            cout << this_book->Title << " " << this_book->Author << " " << this_book->ISBN << " " << this_book->Publication << " ";
            cout << " [Due date : " << x << " days from " << day << "-" << month << "-" << year << " ]" << endl;

        }
    }
    db_user_book.close();

    cout << "--------------------------------------------------------------" << endl;
    return;
}

// student
void Student::issue_book(Book* book, string num) {
    ifstream db_book;
    db_book.open(DB_BOOK);

    long long numBooks = stoll(num);
    if (numBooks > 5) {
        cout << "You can't issue more than 5 books" << endl;
        cout << endl;
        return;
    }

    string sno, title, author, isbn, publication;
    string current_sno;
    while (db_book >> sno >> title >> author >> isbn >> publication) {
        if (title == book->Title && author == book->Author && isbn == book->ISBN && publication == book->Publication) {
            current_sno = sno;
        }
    }
    db_book.close();

    if (current_sno == "") {
        cout << "There is no such Book" << endl;
        cout << endl;
        return;
    }

    book_database* bookdb = new book_database();

    ifstream db_user_book;
    db_user_book.open(DB_USER_BOOK);
    string suser, sbook, day, month, year, numm;

    string cuser = get_sno();
    string cbook = current_sno;

    vector<vector<string>> store;
    while (db_user_book >> suser >> sbook >> day >> month >> year >> numm) {
        if (suser == cuser && sbook == cbook) {
            cout << "Book already issued! Kindly issue another book." << endl;
            return;
        }
        vector<string> cur;
        cur.push_back(suser);
        cur.push_back(sbook);
        cur.push_back(day);
        cur.push_back(month);
        cur.push_back(year);
        cur.push_back(numm);

        store.push_back(cur);
    }
    db_user_book.close();

    cout << "Press `y` if you want to issue this book !" << endl;
    string inp; cin >> inp;
    if (inp != "y") {
        cout << "Ok, not issuing this book !" << endl;
        return;
    }

    suser = cuser;
    sbook = current_sno;

    vector<long long> temp = today();
    string yyyy = to_string(temp[2]);
    string mm = to_string(temp[1]);
    string dd = to_string(temp[0]);
    numm = num;

    vector<string> cur;
    cur.push_back(suser);
    cur.push_back(sbook);
    cur.push_back(dd);
    cur.push_back(mm);
    cur.push_back(yyyy);
    cur.push_back(numm);
    store.push_back(cur);

    ofstream fout;
    fout.open(DB_USER_BOOK);
    for (auto& x : store) {
        for (auto& y : x) fout << y << " ";
        fout << endl;
    }
    fout.close();

    cout << "The due date is 30 days from today" << endl;
    cout << "The book has been successfully issued" << endl;
}
void Student::calculate_fine() {
    string user_sno = get_sno();

    ifstream db_user_book;
    db_user_book.open(DB_USER_BOOK);
    string suser, sbook, dd, mm, yyyy, num;

    long double fine = 0.0;
    vector<pair<string, long long >> due_books_sno;
    while (db_user_book >> suser >> sbook >> dd >> mm >> yyyy >> num) {
        if (suser == user_sno) {
            long long given_day = stoll(dd);
            long long given_month = stoll(mm);
            long long given_year = stoll(yyyy);

            vector<long long> date_today = today();
            long long cur_day = date_today[0];
            long long cur_month = date_today[1];
            long long cur_year = date_today[2];

            // at most 30 days
            long long year_diff = cur_year - given_year;
            long long month_diff = cur_month - given_month;
            long long day_diff = cur_day - given_day;

            long long totday = total_days(cur_day, cur_month, cur_year) - total_days(given_day, given_month, given_year);
            if (totday > 30) {
                due_books_sno.push_back({ sbook, totday });
                fine += (2LL) * (totday - 30);
            }
        }
    }

    book_database* bookdb = new book_database();

    if ((int)due_books_sno.size()) {
        cout << "List of books on which fine is imposed : " << endl;
        cout << "[Title" << " " << "Author" << " " << "ISBN" << " " << "Publication" << " " << "Total-days-due" << "]";
        cout << endl;

        for (auto& p : due_books_sno) {
            Book* cur_book = bookdb->get_book(p.first);
            cout << cur_book->Title << " " << cur_book->Author << " " << cur_book->ISBN << " " << cur_book->Publication << "  ";
            cout << to_string(p.second) << endl;
        }
    }
    Fine_amount = fine;
    cout << "Total fine is :: " << fine << endl;
}


// professor
void Professor::issue_book(Book* book, string num) {
    ifstream db_book;
    db_book.open(DB_BOOK);

    string sno, title, author, isbn, publication;
    string current_sno;
    while (db_book >> sno >> title >> author >> isbn >> publication) {
        if (title == book->Title && author == book->Author && isbn == book->ISBN && publication == book->Publication) {
            current_sno = sno;
        }
    }
    db_book.close();

    if (current_sno == "") {
        cout << "There is no such Book" << endl;
        cout << endl;
        return;
    }

    book_database* bookdb = new book_database();

    ifstream db_user_book;
    db_user_book.open(DB_USER_BOOK);
    string suser, sbook, day, month, year, numm;

    string cuser = get_sno();
    string cbook = current_sno;

    vector<vector<string>> store;
    while (db_user_book >> suser >> sbook >> day >> month >> year >> numm) {
        if (suser == cuser && sbook == cbook) {
            cout << " func " << cuser << " " << cbook << endl;
            cout << " pres " << suser << " " << sbook << endl;

            cout << "Book already issued! Kindly issue another book." << endl;
            return;
        }
        vector<string> cur;
        cur.push_back(suser);
        cur.push_back(sbook);
        cur.push_back(day);
        cur.push_back(month);
        cur.push_back(year);
        cur.push_back(numm);

        store.push_back(cur);
    }
    db_user_book.close();

    cout << "Press `y` if you want to issue this book !" << endl;
    string inp; cin >> inp;
    if (inp != "y") {
        cout << "Ok, not issuing this book !" << endl;
        return;
    }

    suser = cuser;
    sbook = current_sno;

    vector<long long> temp = today();
    string yyyy = to_string(temp[2]);
    string mm = to_string(temp[1]);
    string dd = to_string(temp[0]);
    numm = num;

    vector<string> cur;
    cur.push_back(suser);
    cur.push_back(sbook);
    cur.push_back(dd);
    cur.push_back(mm);
    cur.push_back(yyyy);
    cur.push_back(numm);
    store.push_back(cur);

    ofstream fout;
    fout.open(DB_USER_BOOK);
    for (auto& x : store) {
        for (auto& y : x) fout << y << " ";
        fout << endl;
    }
    fout.close();
    cout << "The due date is 60 days from today" << endl;
    cout << "The book has been successfully issued" << endl;
}
void Professor::calculate_fine() {
    string user_sno = get_sno();

    ifstream db_user_book;
    db_user_book.open(DB_USER_BOOK);
    string suser, sbook, dd, mm, yyyy, num;

    long double fine = 0.0;
    vector<pair<string, long long >> due_books_sno;
    while (db_user_book >> suser >> sbook >> dd >> mm >> yyyy >> num) {
        if (suser == user_sno) {
            long long given_day = stoll(dd);
            long long given_month = stoll(mm);
            long long given_year = stoll(yyyy);

            vector<long long> date_today = today();
            long long cur_day = date_today[0];
            long long cur_month = date_today[1];
            long long cur_year = date_today[2];

            // at most 60 days
            long long year_diff = cur_year - given_year;
            long long month_diff = cur_month - given_month;
            long long day_diff = cur_day - given_day;

            long long totday = total_days(cur_day, cur_month, cur_year) - total_days(given_day, given_month, given_year);
            if (totday > 60) {
                due_books_sno.push_back({ sbook, totday });
                fine += (5LL) * (totday - 60);
            }
        }
    }

    book_database* bookdb = new book_database();
    if ((int)due_books_sno.size()) {
        cout << "List of books on which fine is imposed : " << endl;
        cout << "[Title" << " " << "Author" << " " << "ISBN" << " " << "Publication" << " " << "Total-days-due" << "]";
        cout << endl;

        for (auto& p : due_books_sno) {
            Book* cur_book = bookdb->get_book(p.first);
            cout << cur_book->Title << " " << cur_book->Author << " " << cur_book->ISBN << " " << cur_book->Publication << "  ";
            cout << to_string(p.second) << endl;
        }
    }
    Fine_amount = fine;
    cout << "Total fine is :: " << fine << endl;
}

// librarian
void Librarian::Add(User* new_user) {
    ifstream db_user;
    db_user.open(DB_USER);

    string sno, name, pwd, id, role;
    while (db_user >> sno >> name >> id >> pwd >> role) {
        if (name == new_user->Name || id == new_user->ID) {
            cout << "User already exists !" << endl;
            cout << endl;
            return;
        }
    }

    user_database* userdb = new user_database();
    long long num = stoll(sno) + 1;
    string new_sno = to_string(num);
    userdb->users[new_sno] = new_user;

    ofstream fout;
    fout.open(DB_USER);
    for (auto& user : userdb->users) {
        fout << user.first << " ";
        User* cur_user = user.second;
        fout << cur_user->Name << " " << cur_user->ID << " " << cur_user->password << " " << cur_user->role << endl;
    }
    fout.close();

    cout << "User successfully registered" << endl;
    cout << endl;
}

void Librarian::Update(string user_name, string user_id) {
    ifstream db_user;
    db_user.open(DB_USER);

    string sno, name, pwd, id, role;
    string current_sno;
    while (db_user >> sno >> name >> id >> pwd >> role) {
        if (name == user_name && id == user_id) {
            current_sno = sno;
            break;
        }
    }

    if (current_sno == "") {
        cout << "There is no such user" << endl;
        cout << endl;
        return;
    }
    db_user.close();

    user_database* userdb = new user_database();

    cout << "------------------------------------" << endl;
    cout << "The current details of the user is as : " << endl;
    cout << "User Name : " << userdb->users[current_sno]->Name << endl;
    cout << "User ID : " << userdb->users[current_sno]->ID << endl;
    cout << "User Password : " << userdb->users[current_sno]->password << endl;
    cout << "User Role : " << userdb->users[current_sno]->role << endl;
    cout << "------------------------------------" << endl;
    cout << endl;

    cout << "Enter the new details of the user : " << endl;
    cout << "Enter the new user name" << endl;
    string new_username; cin >> new_username;

    cout << "Enter the new user id " << endl;
    string new_userid; cin >> new_userid;

    cout << "Enter the new user password" << endl;
    string new_userpassword; cin >> new_userpassword;

    cout << "Enter the new user role" << endl;
    cout << "Enter `a` for Student" << endl;
    cout << "Enter `b` for Professor" << endl;
    cout << "Enter `c` for Librarian" << endl;
    string c, user_role;
    string new_userrole; ;
    cin >> c;
    if (c == "a") new_userrole = "Student";
    else if (c == "b") new_userrole = "Professor";
    else if (c == "c") new_userrole = "Librarian";
    else {
        cout << "Please select from the given options" << endl;
        cout << endl;
        return;
    }

    userdb->users[current_sno] = new User(new_username, new_userid, new_userpassword, new_userrole);

    // write to files
    ofstream fout;
    fout.open(DB_USER);
    for (auto& user : userdb->users) {
        fout << user.first << " ";
        User* cur_user = user.second;
        fout << cur_user->Name << " " << cur_user->ID << " " << cur_user->password << " " << cur_user->role << endl;
    }
    fout.close();

    cout << "User successfully updated" << endl;
    cout << endl;
}

void Librarian::Delete(string user_name, string user_id) {
    ifstream db_user;
    db_user.open(DB_USER);

    string sno, name, pwd, id, role;
    string current_sno;
    while (db_user >> sno >> name >> id >> pwd >> role) {
        if (name == user_name && id == user_id) {
            current_sno = sno;
            break;
        }
    }
    db_user.close();

    if (current_sno == "") {
        cout << "There is no such user" << endl;
        cout << endl;
        return;
    }

    user_database* userdb = new user_database();

    cout << "------------------------------------" << endl;
    cout << "The current details of the user is as : " << endl;
    cout << "User Name : " << userdb->users[current_sno]->Name << endl;
    cout << "User ID : " << userdb->users[current_sno]->ID << endl;
    cout << "User Password : " << userdb->users[current_sno]->password << endl;
    cout << "User Role : " << userdb->users[current_sno]->role << endl;
    cout << endl;

    auto it = (userdb->users).find(current_sno);
    if (it == (userdb->users).end()) {
        cout << "There is no such user" << endl;
        cout << endl;
        return;
    }
    else {
        (userdb->users).erase(it);
    }


    // write to files
    ofstream fout;
    fout.open(DB_USER);
    for (auto& user : userdb->users) {
        fout << user.first << " ";
        User* cur_user = user.second;
        fout << cur_user->Name << " " << cur_user->ID << " " << cur_user->password << " " << cur_user->role << endl;
    }
    fout.close();

    cout << "User successfully deleted!" << endl;
    cout << "------------------------------------" << endl;
    cout << endl;
}

void Librarian::Search(string user_name, string user_id) {
    ifstream db_user;
    db_user.open(DB_USER);

    string sno, name, pwd, id, role;
    string current_sno;
    while (db_user >> sno >> name >> id >> pwd >> role) {
        if (name == user_name && id == user_id) {
            current_sno = sno;
            break;
        }
    }
    db_user.close();

    if (current_sno == "") {
        cout << "There is no such user" << endl;
        cout << endl;
        return;
    }

    book_database* bookdb = new book_database();
    ifstream db_user_book;
    db_user_book.open(DB_USER_BOOK);
    string suser, sbook, day, month, year, num;

    cout << endl;
    cout << " List of Books: " << endl;
    cout << "[Title" << " " << "Author" << " " << "ISBN" << " " << "Publication]" << endl << endl;
    while (db_user_book >> suser >> sbook >> day >> month >> year >> num) {
        if (suser == current_sno) {
            Book* curbook = bookdb->get_book(sbook);
            cout << curbook->Title << " " << curbook->Author << " " << curbook->ISBN << " " << curbook->Publication << endl;
        }
    }
    db_user_book.close();

    cout << "------------------------------------" << endl;
    cout << endl;
}

void Librarian::Add(Book* book) {
    ifstream db_book;
    db_book.open(DB_BOOK);

    string title, author, isbn, publication;
    string sno;
    while (db_book >> sno >> title >> author >> isbn >> publication) {
        if (title == book->Title && author == book->Author && isbn == book->ISBN && publication == book->Publication) {
            cout << "Book already exists !" << endl;
            cout << endl;
            return;
        }
    }
    db_book.close();

    book_database* bookdb = new book_database();
    long long num = stoll(sno) + 1;
    string new_sno = to_string(num);
    bookdb->books[new_sno] = book;

    ofstream fout;
    fout.open(DB_BOOK);
    for (auto& bk : bookdb->books) {
        fout << bk.first << " ";
        Book* cur_book = bk.second;
        fout << cur_book->Title << " " << cur_book->Author << " " << cur_book->ISBN << " " << cur_book->Publication << endl;
    }
    fout.close();

    cout << "Book successfully Added" << endl;
    cout << endl;
}

void Librarian::Update(Book* book) {
    ifstream db_book;
    db_book.open(DB_BOOK);

    string sno, title, author, isbn, publication;
    string current_sno;
    while (db_book >> sno >> title >> author >> isbn >> publication) {
        if (title == book->Title && author == book->Author && isbn == book->ISBN && publication == book->Publication) {
            current_sno = sno;
        }
    }
    db_book.close();

    if (current_sno == "") {
        cout << "There is no such Book" << endl;
        cout << endl;
        return;
    }

    book_database* bookdb = new book_database();

    cout << "------------------------------------" << endl;
    cout << "The current details of the Book is as : " << endl;
    cout << "Book Name : " << bookdb->books[current_sno]->Title << endl;
    cout << "Book Author : " << bookdb->books[current_sno]->Author << endl;
    cout << "Book ISBN : " << bookdb->books[current_sno]->ISBN << endl;
    cout << "Book Publication : " << bookdb->books[current_sno]->Publication << endl;
    cout << "------------------------------------" << endl;
    cout << endl;

    cout << "Enter the new details of the Book : " << endl;
    cout << "Enter the new name of the book" << endl;
    string newtitle; cin >> newtitle;

    cout << "Enter the new name of the Author" << endl;
    string newauthor; cin >> newauthor;

    cout << "Enter the new ISBN" << endl;
    string newisbn; cin >> newisbn;

    cout << "Enter the new name of the Publication" << endl;
    string newpublication; cin >> newpublication;

    bookdb->books[current_sno] = new Book(newtitle, newauthor, newisbn, newpublication);

    // write to files
    ofstream fout;
    fout.open(DB_BOOK);
    for (auto& bk : bookdb->books) {
        fout << bk.first << " ";
        Book* cur_book = bk.second;
        fout << cur_book->Title << " " << cur_book->Author << " " << cur_book->ISBN << " " << cur_book->Publication << endl;
    }
    fout.close();

    cout << "Book successfully Updated" << endl;
    cout << endl;
}

void Librarian::Delete(Book* book) {
    ifstream db_book;
    db_book.open(DB_BOOK);

    string sno, title, author, isbn, publication;
    string current_sno;
    while (db_book >> sno >> title >> author >> isbn >> publication) {
        if (title == book->Title && author == book->Author && isbn == book->ISBN && publication == book->Publication) {
            current_sno = sno;
        }
    }
    db_book.close();

    if (current_sno == "") {
        cout << "There is no such Book" << endl;
        cout << endl;
        return;
    }
    book_database* bookdb = new book_database();

    cout << "------------------------------------" << endl;
    cout << "The current details of the Book is as : " << endl;
    cout << "Book Name : " << bookdb->books[current_sno]->Title << endl;
    cout << "Book Author : " << bookdb->books[current_sno]->Author << endl;
    cout << "Book ISBN : " << bookdb->books[current_sno]->ISBN << endl;
    cout << "Book Publication : " << bookdb->books[current_sno]->Publication << endl;
    cout << endl;

    auto it = (bookdb->books).find(current_sno);
    if (it == (bookdb->books).end()) {
        cout << "There is no such book" << endl;
        cout << endl;
        return;
    }
    else {
        (bookdb->books).erase(it);
    }

    // write to files
    ofstream fout;
    fout.open(DB_BOOK);
    for (auto& bk : bookdb->books) {
        fout << bk.first << " ";
        Book* cur_book = bk.second;
        fout << cur_book->Title << " " << cur_book->Author << " " << cur_book->ISBN << " " << cur_book->Publication << endl;
    }
    fout.close();

    cout << "Book successfully deleted!" << endl;
    cout << "------------------------------------" << endl;
    cout << endl;
}

void Librarian::Search(Book* book) {
    ifstream db_book;
    db_book.open(DB_BOOK);

    string sno, title, author, isbn, publication;
    string current_sno;
    while (db_book >> sno >> title >> author >> isbn >> publication) {
        if (title == book->Title && author == book->Author && isbn == book->ISBN && publication == book->Publication) {
            current_sno = sno;
        }
    }
    db_book.close();

    if (current_sno == "") {
        cout << "There is no such Book" << endl;
        cout << endl;
        return;
    }

    user_database* userdb = new user_database();
    ifstream db_user_book;
    db_user_book.open(DB_USER_BOOK);
    string suser, sbook, day, month, year, num;

    cout << endl;
    cout << " List of Users: " << endl;
    cout << "Name" << " " << "ID" << " " << "Password" << " " << "Role" << endl << endl;
    while (db_user_book >> suser >> sbook >> day >> month >> year >> num) {
        if (sbook == current_sno) {
            User* curuser = userdb->get_user(suser);
            cout << curuser->Name << " " << curuser->ID << " " << curuser->password << " " << curuser->role << endl;
        }
    }
    db_user_book.close();

    cout << "------------------------------------" << endl;
    cout << endl;
}

void Librarian::view_all_books() {
    book_database* bookdb = new book_database();
    bookdb->Display();
}

void Librarian::view_all_users() {
    user_database* userdb = new user_database();
    userdb->Display();
}