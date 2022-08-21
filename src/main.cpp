#include "bits/stdc++.h"
#include "fstream"
#include <ctime>
using namespace std;

#include "model.h"
#include "views.cpp"
#include "urls.cpp"


// function protypes
void login();
void enter_library();
void student_page();
void librarian_page();
void professor_page();
void logout();

// global containers for storing databases
map<string, Book*> books;
map<string, User*> users;


void enter_library() {
    ifstream fin;
    fin.open(DB_COOKIE);
    string s_no, user_name, user_id, user_password, role; // in user.txt
    fin >> s_no >> user_name >> user_id >> user_password >> role;
    fin.close();

    if (s_no == "") {
        pr();
        cout << "Welcome to the library!" << endl;
        cout << "Kindly go through the Readme to get an idea of the application" << endl;
        cout << "More specically have a look at the 'Things to take care of` section in the readme !" << endl << endl;
        pr();
        login();
    }
    else {
        if (role == "Student") {
            cout << "Welcome " << user_name << endl;
            cout << "You are logged in as Student" << endl;
            cout << endl;
            student_page();
        }
        else if (role == "Librarian") {
            cout << "Welcome " << user_name << endl;
            cout << "You are logged in as Librarian" << endl;
            cout << endl;
            librarian_page();
        }
        else if (role == "Professor") {
            cout << "Welcome " << user_name << endl;
            cout << "You are logged in as Professor" << endl;
            cout << endl;
            professor_page();
        }
        return;
    }
    return;
}

// login in to the system
void login() {
    cout << "Welcome to the Library!. Kindly login ! " << endl;
    cout << "*****************************" << endl;

    string username, password;
    cout << "Enter your username" << endl;
    cin >> username;

    cout << "Enter your password" << endl;
    cin >> password;

    cout << endl;

    ifstream fin;
    fin.open(DB_USER);

    string s_no, user_name, user_id, user_password, role; // in user.txt
    while (fin >> s_no >> user_name >> user_id >> user_password >> role) {
        if (user_name == username && user_password == password) {
            ofstream fout;
            fout.open(DB_COOKIE);
            fout << s_no << " " << user_name << " " << user_id << " " << user_password << " " << role << endl;
            fout.close();

            enter_library();
            return;
        }
    }

    fin.close();
    cout << "Invalid Credentials! Kindly repeat the process" << endl;
    login();

    return;
}

// logout the user
void logout() {
    ofstream fout;
    fout.open(DB_COOKIE);
    fout.close();

    login();
    return;
}

// Student , Librarian, Professor
void student_page() {
    ifstream fin;
    fin.open(DB_COOKIE);
    string s_no, user_name, user_id, user_password, role; // in user.txt
    fin >> s_no >> user_name >> user_id >> user_password >> role;
    fin.close();

    assert(s_no != "");
    assert(role == "Student");

    cout << "************COMMANDS*****************" << endl;
    cout << "Press " << 1 << " to see all the books" << endl;
    cout << "Press " << 2 << " to see the list of books you have or to view due date of books" << endl;
    cout << "Press " << 3 << " to check if book is available for issue or not" << endl;
    cout << "Press " << 4 << " to issue a book" << endl;
    cout << "Press " << 5 << " to calculate your fine amount" << endl;
    cout << "Press " << 6 << " logout" << endl;
    cout << "************************************" << endl;
    cout << endl;

    Student* user = new Student(user_name, user_id, user_password, 0.0);
    string ops;
    cin >> ops;

    int op = 0;
    for (int i = 0; i < (int)ops.size(); i++) {
        if (ops[i] >= '0' && ops[i] <= '9') continue;
        else op = -1;
    }
    if (op == 0) op = stoi(ops);

    pr();
    if (op == 1) {
        book_database* bookdb = new book_database();
        bookdb->Display();
    }
    else if (op == 2) {
        user->view_issued_books();
    }
    else if (op == 3 || op == 4) {
        // to issue a book
        cout << "Enter the name of the book to issue" << endl;
        string title; cin >> title;

        cout << "Enter the name of the Author" << endl;
        string author; cin >> author;

        cout << "Enter the ISBN" << endl;
        string isbn; cin >> isbn;

        cout << "Enter the name of the Publication" << endl;
        string publication; cin >> publication;

        cout << "Enter the number of Books to be issued" << endl;
        string num; cin >> num;

        Book* newbook = new Book(title, author, isbn, publication);
        user->issue_book(newbook, num);
    }
    else if (op == 5) {
        // calculate your fine amount
        user->calculate_fine();
    }
    else if (op == 6) {
        logout();
    }
    else {
        cout << "Invalid input!" << endl;
    }
    pr();

    student_page();
    return;
}

void librarian_page() {
    ifstream fin;
    fin.open(DB_COOKIE);
    string s_no, user_name, user_id, user_password, role; // in user.txt
    fin >> s_no >> user_name >> user_id >> user_password >> role;
    fin.close();

    assert(s_no != "");
    assert(role == "Librarian");

    cout << "************COMMANDS*****************" << endl;
    cout << "Press " << 1 << " add an user" << endl;
    cout << "Press " << 2 << " update an user" << endl;
    cout << "Press " << 3 << " delete an user" << endl;
    cout << "Press " << 4 << " add a book" << endl;
    cout << "Press " << 5 << " update a book" << endl;
    cout << "Press " << 6 << " delete a book" << endl;
    cout << "Press " << 7 << " to get the list of books" << endl;
    cout << "Press " << 8 << " to get the list of users" << endl;
    cout << "Press " << 9 << " to see which book is issued to which user" << endl;
    cout << "Press " << 10 << " to check list of books issued to user" << endl;
    cout << "Press " << 11 << " to clear the fine amount" << endl;
    cout << "Press " << 12 << " logout" << endl;
    cout << "************************************" << endl;
    cout << endl;

    Librarian* user = new Librarian(user_name, user_id, user_password);
    string ops;
    cin >> ops;

    int op = 0;
    for (int i = 0; i < (int)ops.size(); i++) {
        if (ops[i] >= '0' && ops[i] <= '9') continue;
        else op = -1;
    }
    if (op == 0) op = stoi(ops);

    pr();
    if (op == 1) {
        // add an user
        cout << "--------Add user----------" << endl;
        cout << "Enter the Name of the User" << endl;
        string user_name; cin >> user_name;

        cout << "Enter the ID of the User" << endl;
        string user_id; cin >> user_id;

        cout << "Enter the password for the User" << endl;
        string user_password; cin >> user_password;

        cout << "Enter the role of the User" << endl;
        cout << "Enter `a` for Student" << endl;
        cout << "Enter `b` for Professor" << endl;
        cout << "Enter `c` for Librarian" << endl;
        string c, user_role;
        cin >> c;
        if (c == "a") user_role = "Student";
        else if (c == "b") user_role = "Professor";
        else if (c == "c") user_role = "Librarian";
        else {
            cout << "Please select from the given options" << endl;
            cout << endl;
            librarian_page();
        }

        User* new_user = new User(user_name, user_id, user_password, user_role);
        user->Add(new_user);
    }
    else if (op == 2) {
        // update an user
        cout << "--------Update User---------" << endl;
        cout << "Enter the Name of the User" << endl;
        string user_name; cin >> user_name;

        cout << "Enter the ID of the User" << endl;
        string user_id; cin >> user_id;

        user->Update(user_name, user_id);
    }
    else if (op == 3) {
        // delete an user
        cout << "--------Delete User---------" << endl;
        cout << "Enter the Name of the User" << endl;
        string user_name; cin >> user_name;

        cout << "Enter the ID of the User" << endl;
        string user_id; cin >> user_id;

        user->Delete(user_name, user_id);
    }
    else if (op == 4) {
        // add a book
        cout << "--------Add Book---------" << endl;
        cout << "Enter the name of the book" << endl;
        string title; cin >> title;

        cout << "Enter the name of the Author" << endl;
        string author; cin >> author;

        cout << "Enter the ISBN" << endl;
        string isbn; cin >> isbn;

        cout << "Enter the name of the Publication" << endl;
        string publication; cin >> publication;

        Book* newbook = new Book(title, author, isbn, publication);
        user->Add(newbook);
    }
    else if (op == 5) {
        // update a book
        cout << "--------Update Book---------" << endl;
        cout << "Enter the name of the book" << endl;
        string title; cin >> title;

        cout << "Enter the name of the Author" << endl;
        string author; cin >> author;

        cout << "Enter the ISBN" << endl;
        string isbn; cin >> isbn;

        cout << "Enter the name of the Publication" << endl;
        string publication; cin >> publication;

        Book* newbook = new Book(title, author, isbn, publication);
        user->Update(newbook);
    }
    else if (op == 6) {
        // delete a book
        cout << "--------Delete Book---------" << endl;
        cout << "Enter the name of the book" << endl;
        string title; cin >> title;

        cout << "Enter the name of the Author" << endl;
        string author; cin >> author;

        cout << "Enter the ISBN" << endl;
        string isbn; cin >> isbn;

        cout << "Enter the name of the Publication" << endl;
        string publication; cin >> publication;

        Book* newbook = new Book(title, author, isbn, publication);

        user->Delete(newbook);
    }
    else if (op == 7) {
        // list of books
        user->view_all_books();
    }
    else if (op == 8) {
        // list of users
        user->view_all_users();
    }
    else if (op == 9) {
        // list of books issued to a user
        cout << "--------get the list of Books corresponding to User---------" << endl;
        cout << "Enter the Name of the User" << endl;
        string user_name; cin >> user_name;

        cout << "Enter the ID of the User" << endl;
        string user_id; cin >> user_id;

        user->Search(user_name, user_id);
    }
    else if (op == 10) {
        // list of users corresponding to a book
        cout << "--------get the list of Users corresponding to a Book---------" << endl;
        cout << "Enter the Title of the book" << endl;
        string title; cin >> title;

        cout << "Enter the name of the Author" << endl;
        string author; cin >> author;

        cout << "Enter the ISBN" << endl;
        string isbn; cin >> isbn;

        cout << "Enter the name of the Publication" << endl;
        string publication; cin >> publication;

        Book* newbook = new Book(title, author, isbn, publication);
        user->Search(newbook);
    }
    else if (op == 11) {
        // clear the fine amount
        cout << "---Clear the fine amount---" << endl;
        cout << "Enter user details" << endl;
        cout << "Enter the user name : " << endl;
        string username; cin >> username;

        cout << "Enter the user ID : " << endl;
        string userid; cin >> userid;

        cout << "Enter the new user role" << endl;
        cout << "Enter `a` for Student" << endl;
        cout << "Enter `b` for Professor" << endl;
        cout << "Enter `c` for Librarian" << endl;
        string c, user_role;
        string userrole; ;
        cin >> c;
        if (c == "a") userrole = "Student";
        else if (c == "b") userrole = "Professor";
        else userrole = "Librarian";

        ifstream db_user;
        db_user.open(DB_USER);

        string usno, uname, upwd, uid, urole;
        string ucurrent_sno;
        while (db_user >> usno >> uname >> uid >> upwd >> urole) {
            if (uname == username && uid == userid && urole == userrole) {
                ucurrent_sno = usno;
                break;
            }
        }
        db_user.close();

        if (ucurrent_sno == "") {
            cout << "There is no such user" << endl;
            cout << endl;
        }
        else {
            cout << endl;
            cout << "Enter book details" << endl;
            cout << "Enter the name of the book" << endl;
            string title; cin >> title;

            cout << "Enter the name of the Author" << endl;
            string author; cin >> author;

            cout << "Enter the ISBN" << endl;
            string isbn; cin >> isbn;

            cout << "Enter the name of the Publication" << endl;
            string publication; cin >> publication;

            Book* book = new Book(title, author, isbn, publication);

            ifstream db_book;
            db_book.open(DB_BOOK);

            string bsno, btitle, bauthor, bisbn, bpublication;
            string bcurrent_sno;
            while (db_book >> bsno >> btitle >> bauthor >> bisbn >> bpublication) {
                if (btitle == book->Title && bauthor == book->Author && bisbn == book->ISBN && bpublication == book->Publication) {
                    bcurrent_sno = bsno;
                }
            }
            db_book.close();

            if (bcurrent_sno == "") {
                cout << "There is no such Book" << endl;
                cout << endl;
            }
            else {
                book_database* bookdb = new book_database();
                ifstream db_user_book;
                db_user_book.open(DB_USER_BOOK);
                string suser, sbook, day, month, year, numm;

                string cuser = ucurrent_sno;
                string cbook = bcurrent_sno;

                cout << cuser << " " << cbook << endl;

                string dd, mm, yyyy, num;
                vector<vector<string>> store;
                bool got = 0;
                while (db_user_book >> suser >> sbook >> day >> month >> year >> numm) {
                    if (suser == cuser && sbook == cbook) {
                        dd = day;
                        mm = month;
                        yyyy = year;
                        num = numm;
                        got = 1;
                    }
                    else {
                        cout << suser << " " << sbook << endl;
                        vector<string> cur;
                        cur.push_back(suser);
                        cur.push_back(sbook);
                        cur.push_back(day);
                        cur.push_back(month);
                        cur.push_back(year);
                        cur.push_back(numm);

                        store.push_back(cur);
                    }
                }
                db_user_book.close();

                if (!got) {
                    cout << "Book has not been issued!" << endl;
                }
                else {
                    long double fine = 0.0;
                    long long given_day = stoll(dd);
                    long long given_month = stoll(mm);
                    long long given_year = stoll(yyyy);

                    vector<long long> date_today = today();
                    long long cur_day = date_today[0];
                    long long cur_month = date_today[1];
                    long long cur_year = date_today[2];

                    long long year_diff = cur_year - given_year;
                    long long month_diff = cur_month - given_month;
                    long long day_diff = cur_day - given_day;

                    long long totday = total_days(cur_day, cur_month, cur_year) - total_days(given_day, given_month, given_year);
                    if (urole == "Student" && totday > 30) fine += (2LL) * (totday - 30);
                    if (urole == "Professor" && totday > 60) fine += (5LL) * (totday - 60);

                    cout << "To clear the fine for this book and user :: " << endl;
                    cout << "The number of books that need to be returned is " << num << endl;
                    cout << "The number of fine to be submitted is " << fine << endl;

                    cout << "Press `y` to clear the fine " << endl;
                    string op; cin >> op;
                    if (op == "y") {
                        ofstream fout;
                        fout.open(DB_USER_BOOK);
                        for (auto& x : store) {
                            for (auto& y : x) fout << y << " ";
                            fout << endl;
                            break;
                        }
                        fout.close();
                        cout << "Fine has been cleared successfully and book has been returned!" << endl;
                    }
                    else {
                        cout << "Fine not cleared ! " << endl;
                    }
                }
            }
        }

    }
    else if (op == 12) {
        // logout
        logout();
    }
    else {
        cout << "Invalid input!" << endl;
    }
    pr();

    librarian_page();
    return;
}

void professor_page() {
    ifstream fin;
    fin.open(DB_COOKIE);
    string s_no, user_name, user_id, user_password, role; // in user.txt
    fin >> s_no >> user_name >> user_id >> user_password >> role;
    fin.close();

    assert(s_no != "");
    assert(role == "Professor");

    cout << "**************COMMANDS***************" << endl;
    cout << "Press " << 1 << " to see all the books" << endl;
    cout << "Press " << 2 << " to see your list of books or to view due date of issued books" << endl;
    cout << "Press " << 3 << " to see if a book is available for issue or not" << endl;
    cout << "Press " << 4 << " to issue a book" << endl;
    cout << "Press " << 5 << " to calculate your fine amount" << endl;
    cout << "Press " << 6 << " to logout" << endl;
    cout << "*************************************" << endl;
    cout << endl;

    Professor* user = new Professor(user_name, user_id, user_password, 0.0);

    string ops;
    cin >> ops;

    int op = 0;
    for (int i = 0; i < (int)ops.size(); i++) {
        if (ops[i] >= '0' && ops[i] <= '9') continue;
        else op = -1;
    }
    if (op == 0) op = stoi(ops);

    pr();
    if (op == 1) {
        // see all the list of books
        book_database* bookdb = new book_database();
        bookdb->Display();
    }
    else if (op == 2) {
        // to see all the issued books
        user->view_issued_books();
    }
    else if (op == 3 || op == 4) {
        // to issue a book
        cout << "Enter the name of the book to issue" << endl;
        string title; cin >> title;

        cout << "Enter the name of the Author" << endl;
        string author; cin >> author;

        cout << "Enter the ISBN" << endl;
        string isbn; cin >> isbn;

        cout << "Enter the name of the Publication" << endl;
        string publication; cin >> publication;

        cout << "Enter the number of Books to be issued" << endl;
        string num; cin >> num;


        Book* newbook = new Book(title, author, isbn, publication);
        user->issue_book(newbook, num);
    }
    else if (op == 5) {
        // to calculate your fine amount
        user->calculate_fine();
    }
    else if (op == 6) {
        logout();
    }
    else {
        cout << "Invalid input!" << endl;
    }
    pr();

    professor_page();
    return;
}




int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    cout << fixed << setprecision(20);

    enter_library();
}
