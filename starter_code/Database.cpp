#include <iostream>
#include <stdexcept>
#include <random>
#include <fstream>
#include <chrono>
#include <unistd.h>
#include <sstream>
#include <random>
#include "Database.h"

using namespace std;

namespace Records {

	Employee& Database::addEmployee(const string& firstName,
		const string& lastName)
	{
		Employee theEmployee(firstName, lastName);
		theEmployee.setEmployeeNumber(mNextEmployeeNumber++);
		theEmployee.hire();
		mEmployees.push_back(theEmployee);

		return mEmployees[mEmployees.size() - 1];
	}

	Employee& Database::getEmployee(int employeeNumber)
	{
		for (auto& employee : mEmployees) {
			if (employee.getEmployeeNumber() == employeeNumber) {
				return employee;
			}
		}
		throw logic_error("No employee found.");
	}

	Employee& Database::getEmployee(const string& firstName, const string& lastName)
	{
		for (auto& employee : mEmployees) {
			if (employee.getFirstName() == firstName &&
				employee.getLastName() == lastName) {
					return employee;
			}
		}
		throw logic_error("No employee found.");
	}

	void Database::displayAll() const
	{
		for (const auto& employee : mEmployees) {
			employee.display();
		}
	}

	void Database::displayCurrent() const
	{
		for (const auto& employee : mEmployees) {
			if (employee.isHired())
				employee.display();
		}
	}

	void Database::displayFormer() const
	{
		for (const auto& employee : mEmployees) {
			if (!employee.isHired())
				employee.display();
		}
	}

   /**
     * @brief Generate a new database with 8000 employees
    */
    void Database::generateNewDatabase()
    {
        cout << "Generating new database..." << endl;
        // Generate 20 first names, middle names, and last names.
        string firstNames[20] = {"Jacob", "Michael", "Joshua", "Matthew", "Ethan", "Andrew", "Daniel", "Anthony", "Christopher", "Joseph", "William", "Alexander", "Ryan", "David", "Nicholas", "Tyler", "James", "John", "Jonathan", "Nathan"};
        string middleNames[20] = {"Aaron", "Adam", "Alan", "Albert", "Alexander", "Andrew", "Anthony", "Arthur", "Benjamin", "Bruce", "Carl", "Charles", "Christopher", "Daniel", "David", "Donald", "Douglas", "Edward", "Eric", "Ethan"};
        string lastNames[20] = {"Smith", "Johnson", "Williams", "Jones", "Brown", "Davis", "Miller", "Wilson", "Moore", "Taylor", "Anderson", "Thomas", "Jackson", "White", "Harris", "Martin", "Thompson", "Garcia", "Martinez", "Robinson"};
        const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

        random_device seed;
        ranlux48 engine(seed());
        // Generate a random number between 8000 and 10000000.
        uniform_int_distribution<> distrib1(8000, 10000000);
        uniform_int_distribution<> distrib(0, characters.length() - 1);

        // Generate 8000 employees.
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 20; ++j) {
                for (int k = 0; k < 20; ++k) {
                    Employee& employee = addEmployee(firstNames[i], lastNames[j]);
                    employee.setMiddleName(middleNames[k]);
                    auto now = std::chrono::steady_clock::now();
                    auto duration = now.time_since_epoch();
                    auto timestamp = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();

                    std::string randomString;
                    randomString.reserve(20);
                    int random_number = distrib1(engine);
                    // Generate a random string of 20 characters.
                    for (int r = 0; r < 20; ++r) {
                        randomString += characters[distrib(engine)];
                    }
                    // Generate a unique address for each employee.
                    std::string unique_address = "Addr" + std::to_string(timestamp) + std::to_string(random_number) + randomString;
                    employee.setAddress(unique_address);
                }
            }
        }

        cout << "Done!" << endl;
    }
    /**
     * @brief Save the database to a file
     * @param fileName The name of the file to save to
    */
    void Database::saveDatabase(const std::string& fileName) const {
        ofstream databaseFile(fileName);
        if (databaseFile.is_open()) {
            // Write the employee data to the file
            for (const auto& employee : mEmployees) {
                databaseFile << employee.getEmployeeNumber();
                databaseFile << " " << employee.getFirstName();
                databaseFile << " " << employee.getMiddleName();
                databaseFile << " " << employee.getLastName();
                databaseFile << " " << employee.getAddress();
                databaseFile << " " << employee.getSalary();
                databaseFile << " " << employee.isHired() << endl;
            }
            databaseFile.close();
        } else {
            throw logic_error("Unable to open file.");
        }
    }
    /**
     * @brief Load the database from a file
     * @param fileName The name of the file to load from
    */
    void Database::loadDatabase(const std::string& fileName) {
        ifstream databaseFile(fileName);
        if (databaseFile.is_open()) {
            string line;
            while (getline(databaseFile, line)) {
                istringstream iss(line);
                int employeeNumber;
                string firstName;
                string middleName;
                string lastName;
                string address;
                int salary;
                bool hired;
                // Read the employee data from the file
                if (!(iss >> employeeNumber >> firstName >> middleName >> lastName >> address >> salary >> hired)) {
                    throw logic_error("Unable to read file.");
                }
                Employee& employee = addEmployee(firstName, lastName);
                employee.setMiddleName(middleName);
                employee.setAddress(address);
                employee.setSalary(salary);
                employee.setEmployeeNumber(employeeNumber);

                // Hire or fire the employee
                if (hired) {
                    employee.hire();
                } else {
                    employee.fire();
                }
            }
            databaseFile.close();
        } else {
            throw logic_error("Unable to open file.");
        }
    }
    /**
     * @brief Load the database from a file
     * @param fileName The name of the file to load from
    */
    vector<Employee> Database::searchByFirstName(const string& basicString) {
        vector<Employee> result;
        for (const auto &item: mEmployees) {
            if (item.getFirstName().find(basicString) != string::npos) {
                result.push_back(item);
            }
        }
        return result;

    }

    /**
     * @brief Fuzzy search for employees by MiddleName
     * @param basicString The name to search for
     * @return A vector of employees that match the search
    */
    vector<Employee> Database::searchByMiddleName(const std::string& basicString) {
        vector<Employee> result;
        for (const auto &item: mEmployees) {
            if (item.getMiddleName().find(basicString) != string::npos) {
                result.push_back(item);
            }
        }
        return result;
    }

    /**
     * @brief Fuzzy search for employees by LastName
     * @param basicString The name to search for
     * @return A vector of employees that match the search
    */
    vector<Employee> Database::searchByLastName(const std::string& basicString) {
        vector<Employee> result;
        for (const auto &item: mEmployees) {
            if (item.getLastName().find(basicString) != string::npos) {
                result.push_back(item);
            }
        }
        return result;
    }

    /**
     * @brief Fuzzy search for employees by Address
     * @param basicString The name to search for
     * @return A vector of employees that match the search
    */
    std::vector<Employee> Database::searchByAddress(const std::string& basicString) {
        vector<Employee> result;
        for (const auto &item: mEmployees) {
            if (item.getAddress().find(basicString) != string::npos) {
                result.push_back(item);
            }
        }
        return result;
    }

    
}
