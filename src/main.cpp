#include <iostream>
#include <fstream>
#include <random>
#include <time.h>
#include <sstream>
#include "id.h"

using namespace std;

const char FILE_NAME[] = "bookstore.txt";

struct Book
{
    string id;
    string name;
    string author;
    float price;
    int stock;
    string toString()
    {
        return id + "," + name + "," + author + "," + to_string(price) + "," + to_string(stock);
    }
};

// prototypes
void add(Book book);
void remove(string id);
void printAll();
void find(string id);
void sell(string id, int quantity);
void restock(string id, int quantity);
void changePrice(string id, float price);
int getChoice();

int getInt(string prompt);
float getFloat(string prompt);
string getID(string prompt);

int main()
{
    srand(time(NULL));

    {
        ifstream f(FILE_NAME);
        try
        {
            id::init(&f);
        }
        catch (const runtime_error &e)
        {
            cout << e.what();
        }
        f.close();
    }

    cout << "Welcome to our book store! :)" << endl;
    int choice = 0;
    do
    {
        choice = getChoice();

        // add
        if (choice == 1)
        {
            Book book;

            cout << "Book's name:\n";
            getline(cin, book.name);

            cout << "Name of the author:\n";
            getline(cin, book.author);

            book.price = getFloat("Book's price: ");
            while (book.price < 0)
            {
                cout << "Price must be a positive floating point number.\n";
                book.price = getFloat("Book's price: ");
            }
            book.stock = getInt("Book's stock: ");
            while (book.stock < 0)
            {
                cout << "Stock must be a positive integer.\n";
                book.stock = getInt("Book's stock: ");
            }

            try
            {
                id id; // create new id
                book.id = id.getValue();
                add(book);
            }
            catch (runtime_error &e)
            {
                cout << e.what() << endl;
                cout << "Nothing was saved due to the error." << endl;
            }
        }

        // remove
        if (choice == 2)
        {
            string id = getID("Please enter the ID of the book to remove:");
            remove(id);
        }

        // print all
        if (choice == 3)
            printAll();

        // find
        if (choice == 4)
        {
            cout << "What do you want to search for? (We can search by ID, Name, Author, Price, Stock)\n";
            string query;
            cin.ignore();
            cin.clear();
            getline(cin, query);
            find(query);
        }

        // sell
        if (choice == 5)
        {
            string id = getID("ID of the book to sell: ");
            int quantity = getInt("Quantity to sell: ");
            sell(id, quantity);
        }

        // restock
        if (choice == 6)
        {
            string id = getID("ID of the book to restock: ");
            int quantity = getInt("Quantity to restock: ");
            restock(id, quantity);
        }

        // change price
        if (choice == 7)
        {
            string id = getID("ID of the book to restock: ");
            float price = getFloat("New price: ");
            changePrice(id, price);
        }
    } while (choice != 0);

    cout << "Thanks for visiting our book store. :)\n";
    return 0;
}

int getChoice()
{
    string list = "";
    list += "0) Exit.\n";
    list += "1) Add a new book.\n";
    list += "2) Remove a book.\n";
    list += "3) List all books.\n";
    list += "4) Search.\n";
    list += "5) Sell a book (decrease stock).\n";
    list += "6) Restock a book (increase stock).\n";
    list += "7) Change the price of a book.\n";
    int choice = -1;
    while (choice < 0 || choice > 7)
    {
        choice = getInt("What do you want to do?\n" + list + "Please enter a valid number (0 - 7): ");
    }

    return choice;
}

void add(Book book)
{
    ofstream f(FILE_NAME, ios::app);
    f << book.toString() << endl;
    f.close();
}
void remove(string id)
{
    string line;
    ofstream temp("temp.txt", ios::app);
    ifstream f(FILE_NAME);
    while (getline(f, line))
    {
        string currentID = line.substr(0, 3);
        if (currentID != id)
            temp << line << endl;
    }
    f.close();
    temp.close();
    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);
    id::remove(id);
}
void find(string query)
{
    string line;
    ifstream f(FILE_NAME);
    cout << "==========Search Result===============\n";
    while (getline(f, line))
    {
        string currentID = line.substr(0, 3);
        if (line.find(query) != string::npos)
            cout << line << endl;
    }
    cout << "==========Search Result End===============\n";
    f.close();
}

void printAll()
{
    string line;
    ifstream f(FILE_NAME);
    cout << "===============ALL BOOKS===============\n";
    while (getline(f, line))
    {
        cout << line << endl;
    }
    cout << "===============ALL BOOKS END===========\n";
    f.close();
}

void sell(string id, int quantity)
{
    string line;
    ofstream temp("temp.txt", ios::app);
    ifstream f(FILE_NAME);
    bool changed = false;
    bool found = false;
    while (getline(f, line))
    {
        string currentID = line.substr(0, 3);
        if (currentID != id)
            temp << line << endl;
        else
        {
            found = true;
            stringstream ss(line);
            string t;
            string newline = "";
            for (int i = 0; i < 5; i++)
            {
                getline(ss, t, ',');
                if (i != 4)
                    newline += t + ",";
            }
            int stock = stoi(t) - quantity;
            if (stock < 0)
            {
                cout << "Stock cannot be less than 0\n";
                continue;
            }
            changed = true;
            newline += to_string(stock) + "\n";
            temp << newline;
        }
    }
    f.close();
    if (changed)
    {
        remove(FILE_NAME);
        rename("temp.txt", FILE_NAME);
    }
    else
    {
        remove("temp.txt");
        if (!found)
            cout << "Error: ID not found\n";
        cout << "Nothing updated\n";
    }
    temp.close();
}
void restock(string id, int quantity)
{
    string line;
    ofstream temp("temp.txt", ios::app);
    ifstream f(FILE_NAME);
    bool found = false;
    while (getline(f, line))
    {
        string currentID = line.substr(0, 3);
        if (currentID != id)
            temp << line << endl;
        else
        {
            found = true;
            stringstream ss(line);
            string t;
            string newline = "";
            for (int i = 0; i < 5; i++)
            {
                getline(ss, t, ',');
                if (i != 4)
                    newline += t + ",";
            }
            int stock = stoi(t) + quantity;
            newline += to_string(stock) + "\n";
            temp << newline;
        }
    }
    f.close();
    if (found)
    {
        remove(FILE_NAME);
        rename("temp.txt", FILE_NAME);
    }
    else
    {
        remove("temp.txt");
        if (!found)
            cout << "Error: ID not found\n";
        cout << "Nothing updated\n";
    }
    temp.close();
}
void changePrice(string id, float price)
{
    if (price < 0)
    {
        cout << "Price cannot be less than 0.\nNothing changed.\n";
        return;
    }

    string line;
    ofstream temp("temp.txt", ios::app);
    ifstream f(FILE_NAME);
    bool found = false;
    while (getline(f, line))
    {
        string currentID = line.substr(0, 3);
        if (currentID != id)
            temp << line << endl;
        else
        {
            found = true;
            stringstream ss(line);
            string t;
            string newline = "";
            for (int i = 0; i < 4; i++)
            {
                getline(ss, t, ',');
                if (i != 3)
                    newline += t + ",";
            }
            newline += to_string(price) + ",";
            getline(ss, t);
            newline += t + "\n";
            temp << newline;
        }
    }
    f.close();
    if (found)
    {
        remove(FILE_NAME);
        rename("temp.txt", FILE_NAME);
    }
    else
    {
        remove("temp.txt");
        if (!found)
            cout << "Error: ID not found\n";
        cout << "Nothing updated\n";
    }
    temp.close();
}

int getInt(string prompt)
{
    bool ok = false;
    string temp;
    int res;
    while (!ok)
    {
        cout << prompt;
        cin >> temp;
        try
        {
            res = stoi(temp);
            ok = true;
        }
        catch (const invalid_argument &e)
        {
            cout << "Please enter an integer.\n";
        }
    }
    cin.clear();
    cin.ignore();
    return res;
}

float getFloat(string prompt)
{
    bool ok = false;
    string temp;
    float res;
    while (!ok)
    {
        cout << prompt;
        cin >> temp;
        try
        {
            res = stof(temp);
            ok = true;
        }
        catch (const invalid_argument &e)
        {
            cout << "Please enter a floating point number.\n";
        }
    }
    cin.clear();
    cin.ignore();
    return res;
}

string getID(string prompt)
{
    bool ok = false;
    string id;
    while (!ok)
    {
        cout << prompt;
        cin >> id;
        if (id::validate(id))
        {
            ok = true;
            break;
        }
        cout << "Please enter a valid id.\n";
    }
    cin.clear();
    cin.ignore();
    return id;
}