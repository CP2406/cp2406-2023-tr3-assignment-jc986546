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
