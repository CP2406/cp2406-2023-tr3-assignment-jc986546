#pragma once

#include <iostream>
#include <vector>
#include <random>
#include "Employee.h"

namespace Records {
	const int kFirstEmployeeNumber = 1000;

	class Database
	{
	public:
		Employee& addEmployee(const std::string& firstName,
							  const std::string& lastName);
		Employee& getEmployee(int employeeNumber);
		Employee& getEmployee(const std::string& firstName,
							  const std::string& lastName);

		void displayAll() const;
		void displayCurrent() const;
		void displayFormer() const;
		/**
		 * @brief Generate a new database with 8000 employees
		*/
		void generateNewDatabase();
		/**
		 * @brief Save the database to a file
		 * @param fileName The name of the file to save to
		*/
		void saveDatabase(const std::string& fileName = "database.txt") const;

		/**
		 * @brief Load the database from a file
		 * @param fileName The name of the file to load from
		*/
        	void loadDatabase(const std::string& fileName = "database.txt");


	private:
		std::vector<Employee> mEmployees;
		int mNextEmployeeNumber = kFirstEmployeeNumber;
	};
}
