#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm> // For remove_if

using namespace std;

class Book {
public:
    string title;
    string author;
    string isbn;
    bool available;

    Book(string t, string a, string i, bool avail = true)
        : title(t), author(a), isbn(i), available(avail) {}
};

class Borrower {
public:
    string name;
    string borrowedBookIsbn;
    time_t dueDate;

    Borrower(string n, string isbn, time_t due)
        : name(n), borrowedBookIsbn(isbn), dueDate(due) {}
};

class Library {
private:
    vector<Book> books;
    vector<Borrower> borrowers;

public:
    // Add a new book to the library
    void addBook() {
        string title, author, isbn;
        cout << "Enter Book Title: ";
        getline(cin, title);
        cout << "Enter Author Name: ";
        getline(cin, author);
        cout << "Enter ISBN: ";
        getline(cin, isbn);

        books.push_back(Book(title, author, isbn));
        cout << "Book added successfully!" << endl;
    }

    // Search for a book by title, author, or ISBN
    void searchBook() {
        string searchQuery;
        cout << "Enter search query (Title, Author, or ISBN): ";
        getline(cin, searchQuery);

        bool found = false;
        for (const auto& book : books) {
            if (book.title == searchQuery || book.author == searchQuery || book.isbn == searchQuery) {
                cout << "Book found!" << endl;
                cout << "Title: " << book.title << ", Author: " << book.author << ", ISBN: " << book.isbn << endl;
                found = true;
            }
        }

        if (!found) {
            cout << "No books found matching your search query." << endl;
        }
    }

    // Check out a book to a borrower
    void checkoutBook() {
        string isbn, name;
        cout << "Enter ISBN of the book to checkout: ";
        cin >> isbn;
        cin.ignore();  // Ignore leftover newline

        cout << "Enter borrower's name: ";
        getline(cin, name);

        // Check if the book exists and is available
        for (auto& book : books) {
            if (book.isbn == isbn && book.available) {
                book.available = false;

                // Set the due date for 14 days from today
                time_t now = time(0);
                time_t dueDate = now + (14 * 24 * 60 * 60); // 14 days in the future

                // Add borrower to the list
                borrowers.push_back(Borrower(name, isbn, dueDate));
                cout << "Book checked out successfully! Due date: " << ctime(&dueDate) << endl;
                return;
            }
        }

        cout << "This book is either not available or doesn't exist." << endl;
    }

    // Function to return a book
    void returnBook() {
        string isbn;
        cout << "Enter ISBN of the book to return: ";
        cin >> isbn;
        cin.ignore();  // Ignore leftover newline

        for (auto& borrower : borrowers) {
            if (borrower.borrowedBookIsbn == isbn) {
                // Find the corresponding book and mark it as available
                for (auto& book : books) {
                    if (book.isbn == isbn) {
                        book.available = true;
                        break;
                    }
                }

                // Calculate fine if the book is overdue
                time_t now = time(0);
                double seconds = difftime(now, borrower.dueDate);
                int daysLate = seconds / (60 * 60 * 24);

                if (daysLate > 0) {
                    double fine = daysLate * 0.5;  // Fine of 0.5 per day
                    cout << "The book is overdue by " << daysLate << " days. Fine: $" << fine << endl;
                } else {
                    cout << "Book returned on time. No fine." << endl;
                }

                // Manually erase the borrower from the vector using remove_if
                borrowers.erase(remove_if(borrowers.begin(), borrowers.end(),
                    [&isbn](const Borrower& b) { return b.borrowedBookIsbn == isbn; }), borrowers.end());

                cout << "Book returned successfully!" << endl;
                return;
            }
        }

        cout << "This book was not checked out!" << endl;
    }

    // View all books in the library
    void viewBooks() {
        cout << "Books in the Library:" << endl;
        for (const auto& book : books) {
            cout << "Title: " << book.title << ", Author: " << book.author << ", ISBN: " << book.isbn
                 << ", Available: " << (book.available ? "Yes" : "No") << endl;
        }
    }

    // View all borrowers
    void viewBorrowers() {
        cout << "Current Borrowers:" << endl;
        for (const auto& borrower : borrowers) {
            cout << "Name: " << borrower.name << ", Borrowed Book ISBN: " << borrower.borrowedBookIsbn
                 << ", Due Date: " << ctime(&borrower.dueDate);
        }
    }
};

int main() {
    Library library;
    int choice;

    do {
        cout << "\nLibrary Management System\n";
        cout << "1. Add Book\n";
        cout << "2. Search Book\n";
        cout << "3. Checkout Book\n";
        cout << "4. Return Book\n";
        cout << "5. View All Books\n";
        cout << "6. View Borrowers\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();  // Ignore leftover newline

        switch (choice) {
        case 1:
            library.addBook();
            break;
        case 2:
            library.searchBook();
            break;
        case 3:
            library.checkoutBook();
            break;
        case 4:
            library.returnBook();
            break;
        case 5:
            library.viewBooks();
            break;
        case 6:
            library.viewBorrowers();
            break;
        case 7:
            cout << "Exiting the system..." << endl;
            break;
        default:
            cout << "Invalid choice, please try again." << endl;
        }
    } while (choice != 7);

    return 0;
}
