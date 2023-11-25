#include <iostream>
#include <stdexcept>
#include <exception>
#include <fstream>
#include "Database.h"

using namespace std;
using namespace Records;

// Global employee number, 
// which provides employees with the ability to query their own information when logging in.
string employeeNumber;

int displayMenu();

/**
 * @brief displayEmployeeMenu
*/
int displayEmployeeMenu();
void doHire(Database& db);
void doFire(Database& db);
void doPromote(Database& db);
void doDemote(Database& db);

/**
 * @brief save the database to a text file. Ask user for the file name.
 * @param db
*/
void saveDatabase(Database& db);

/**
 * @brief edit an employee. Ask user for the employee number.
 * @param db

*/
void editEmployee(Database& db);

/**
 * @brief search an employee. Ask user for the search criteria.
 * @param db
*/
void searchEmployee(Database& db);

/**
 * @brief save the database to a text file. Ask user for the file name.
 * @param db
*/
void loadDatabase(Database& db);

/**
 * @brief Manager or Employee login menu
*/
int loginMenu(Database& db);
/**
 * @brief Manager or Employee login
*/
bool login(Database& db, bool isManager);


int main()
{

	Database employeeDB;
    int option;
    do {
        // When running the program, first log in
        option = loginMenu(employeeDB);
        // If the user is a manager, display the manager menu
        if (option == 1) {
            bool done = false;
            while (!done) {
                int selection = displayMenu();
                switch (selection) {
                    case 0:
                        done = true;
                        break;
                    case 1:
                        doHire(employeeDB);
                        break;
                    case 2:
                        doFire(employeeDB);
                        break;
                    case 3:
                        doPromote(employeeDB);
                        break;
                    case 4:
                        employeeDB.displayAll();
                        break;
                    case 5:
                        employeeDB.displayCurrent();
                        break;
                    case 6:
                        employeeDB.displayFormer();
                        break;
                    case 7:
                        employeeDB.generateNewDatabase();
                        break;
                    case 8:
                        saveDatabase(employeeDB);
                        break;
                    case 9:
                        loadDatabase(employeeDB);
                        break;
                    case 10:
                        editEmployee(employeeDB);
                        break;
                    case 11:
                        searchEmployee(employeeDB);
                        break;
                    default:
                        cerr << "Unknown command." << endl;
                        break;
                }
            }
            
        } else if (option == 2) {  // If the user is an employee, display the employee menu
            bool done = false;
            Employee& emp = employeeDB.getEmployee(stoi(employeeNumber));
            while (!done) {
                int selection = displayEmployeeMenu();

                switch (selection) {
                    case 0:
                        done = true;
                        break;
                    case 1:
                        emp.display();
                        break;
                    default:
                        cerr << "Unknown command." << endl;
                        break;
                }
            }
        }
    } while (option != 0);
	return 0;
}

bool login(Database& db, bool isManager) {

    string password;
    cout << endl;
    cout << "Employee Number: ";
    cin >> employeeNumber;
    cout << "Password: ";
    cin >> password;

    // If the user is a manager, the password is "admin"
    if (isManager) {
        return employeeNumber == "admin" || password == "admin";
    } else {
        // If the user is an employee, the password is "123456"
        if (password != "123456") {
            return false;
        }
        try {
            db.getEmployee(stoi(employeeNumber));
        } catch (const std::logic_error& exception) {
            return false;
        }
        return true;
    }
}

int loginMenu(Database& db) {
    bool done = false;
    while (!done) {
        cout << endl;
        cout << "Login Menu" << endl;
        cout << "-----------------" << endl;
        cout << "1) Manager Login" << endl;
        cout << "2) Employee Login" << endl;
        cout << "0) Quit" << endl;
        cout << endl;
        cout << "---> ";

        int selection;
        cin >> selection;

        switch (selection) {
            case 0:
                return 0;
            case 1:
                if (login(db, true)) {
                    return 1;
                } else {
                    cerr << "Login failed." << endl;
                }
                break;
            case 2:
                if (login(db, false)) {
                    return 2;
                } else {
                    cerr << "Login failed." << endl;
                }
                break;
            default:
                cerr << "Unknown command." << endl;
                break;
        }
    }
}

int displayEmployeeMenu() {
    int selection;
    cout << endl;
    cout << "Employee Database" << endl;
    cout << "-----------------" << endl;
    cout << "1) Display my information" << endl;
    cout << "0) Quit" << endl;
    cout << endl;
    cout << "---> ";
    cin >> selection;

    return selection;
}

int displayMenu()
{
	// Note:
	//		One important note is that this code assumes that the user will
	//		"play nice" and type a number when a number is requested.
	//		When you read about I/O in Chapter 13, you will learn how to
	//		protect against bad input.

    int selection;

    cout << endl;
    cout << "Employee Database" << endl;
    cout << "-----------------" << endl;
    cout << "1) Hire a new employee" << endl;
    cout << "2) Fire an employee" << endl;
    cout << "3) Promote an employee" << endl;
    cout << "4) List all employees" << endl;
    cout << "5) List all current employees" << endl;
    cout << "6) List all former employees" << endl;
    cout << "7) Generate new database" << endl;
    cout << "8) Save database to file" << endl;
    cout << "9) Load database from file" << endl;
    cout << "10) Edit employee" << endl;
    cout << "11) Search employee" << endl;
    cout << "0) Quit" << endl;
    cout << endl;
    cout << "---> ";
    
	cin >> selection;
    
	return selection;
}

void doHire(Database& db)
{
    string firstName;
    string lastName;

    cout << "First name? ";
    cin >> firstName;
    cout << "Last name? ";
    cin >> lastName;
    logic_error("Unable to add new employee.");
    db.addEmployee(firstName, lastName);
}

void doFire(Database& db)
{
    int employeeNumber;

    cout << "Employee number? ";
    cin >> employeeNumber;

    try {
        Employee& emp = db.getEmployee(employeeNumber);
        emp.fire();
        cout << "Employee " << employeeNumber << " terminated." << endl;
    } catch (const std::logic_error& exception) {
        cerr << "Unable to terminate employee: " << exception.what() << endl;
    }
}

void doPromote(Database& db)
{
    int employeeNumber;
    int raiseAmount;

    cout << "Employee number? ";
    cin >> employeeNumber;
    cout << "How much of a raise? ";
    cin >> raiseAmount;

    try {
        Employee& emp = db.getEmployee(employeeNumber);
        emp.promote(raiseAmount);
    } catch (const std::logic_error& exception) {
        cerr << "Unable to promote employee: " << exception.what() << endl;
    }
}

/**
 * @brief save the database to a text file. Ask user for the file name.
 * @param db
*/
void saveDatabase(Database& db) {
    cout << "Saving database..." << endl;

    cout << "Enter file name: ";
    string fileName;
    cin >> fileName;

    ifstream file(fileName);

    // If the file already exists, ask the user if they want to overwrite it
    if (file.good()) {
        cout << "File already exists. Overwrite? (y/n): ";
        char overwrite;
        cin >> overwrite;

        // If the user does not want to overwrite the file, cancel the save
        if (overwrite == 'n') {
            cout << "Save cancelled." << endl;
            return;
        }
    }
    file.close();

    // Save the database to the file
    db.saveDatabase(fileName);
}

/**
 * @brief load the database from a text file. Ask user for the file name.
 * @param db
*/
void loadDatabase(Database& db) {
    cout << "Loading database..." << endl;

    cout << "Enter file name: ";
    string fileName;
    cin >> fileName;

    ifstream file(fileName);

    //    If the file does not exist, cancel the load
    if (!file.good()) {
        cout << "File does not exist." << endl;
        return;
    }
    file.close();

    // Load the database from the file
    db.loadDatabase(fileName);
}


/**
 * @brief edit an employee. Ask user for the employee number.
 * @param db
*/
void editEmployee(Database& db) {

    int employeeNumber;
    cout << "Employee number: ";
    cin >> employeeNumber;

    try {
        Employee &emp = db.getEmployee(employeeNumber);
        bool done = false;
        while (!done)
        {
            emp.display();
            cout << "1) Change address" << endl;
            cout << "2) Change salary" << endl;
            cout << "3) Change hire/fire status" << endl;
            cout << "0) Cancel" << endl;
            cout << endl;
            cout << "---> ";

            int selection;
            cin >> selection;

            switch (selection) {
                case 0:
                    cout << "Edit cancelled." << endl;
                    done = true;
                    break;
                case 1: {
                    string address;
                    cout << "New address? ";
                    cin >> address;
                    emp.setAddress(address);
                    cout << "Address changed." << endl;
                    break;
                }
                case 2: {
                    int salary;
                    cout << "New salary? ";
                    cin >> salary;
                    emp.setSalary(salary);
                    cout << "Salary changed." << endl;
                    break;
                }
                case 3: {
                    bool hired;
                    cout << "Hired? (y/n): ";
                    char hiredChar;
                    cin >> hiredChar;
                    hired = hiredChar == 'y';
                    if (hired) {
                        emp.hire();
                    } else {
                        emp.fire();
                    }
                    cout << "Hire/fire status changed." << endl;
                    break;
                }
            }
        }
    } catch (const std::logic_error& exception) {
        cerr << "Unable to edit employee: " << exception.what() << endl;
    }
}

/**
 * @brief search an employee. Ask user for the search criteria.
 * @param db
*/
void searchEmployee(Database& db) {
    cout << "Search by: " << endl;
    cout << "1) First name" << endl;
    cout << "2) Middle name" << endl;
    cout << "3) Last name" << endl;
    cout << "4) Address" << endl;
    cout << "0) Cancel" << endl;
    cout << endl;
    cout << "---> ";

    int selection;
    cin >> selection;

    switch (selection) {
        case 0:
            cout << "Search cancelled." << endl;
            break;
        case 1: {
            string firstName;
            cout << "First name: ";
            cin >> firstName;
            vector<Employee> employees = db.searchByFirstName(firstName);
            cout << "Found " << employees.size() << " employees." << endl;
            for (const auto& employee : employees) {
                employee.display();
            }
            break;
        }
        case 2: {
            string middleName;
            cout << "Middle name: ";
            cin >> middleName;
            vector<Employee> employees = db.searchByMiddleName(middleName);
            cout << "Found " << employees.size() << " employees." << endl;
            for (const auto& employee : employees) {
                employee.display();
            }
            break;
        }
        case 3: {
            string lastName;
            cout << "Last name: ";
            cin >> lastName;
            vector<Employee> employees = db.searchByLastName(lastName);
            cout << "Found " << employees.size() << " employees." << endl;
            for (const auto& employee : employees) {
                employee.display();
            }
            break;
        }
        case 4: {
            string address;
            cout << "address: ";
            cin >> address;
            vector<Employee> employees = db.searchByAddress(address);
            cout << "Found " << employees.size() << " employees." << endl;
            for (const auto& employee : employees) {
                employee.display();
            }
            break;
        }
    }
}
