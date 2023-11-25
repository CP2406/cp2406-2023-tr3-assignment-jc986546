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

	private:
		std::vector<Employee> mEmployees;
		int mNextEmployeeNumber = kFirstEmployeeNumber;
	};
}
