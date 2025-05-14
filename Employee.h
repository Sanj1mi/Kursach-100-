// Employee.h
#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>
#include <vector>
#include <ctime>
#include "ENUM.h"
#include "GLOBALS.h"

extern double new_bid;

class Employee {
private:
    int ID_employee;
    std::string name_employee;
    std::string surname_employee;
    std::string date_of_start;
    double salary_ratio;

public:
    Employee();
    Employee(const Employee& other);
    Employee(Employee&& other) noexcept;
    Employee& operator=(const Employee& other);
    Employee& operator=(Employee&& other) noexcept;
    bool operator<(const Employee& other) const;


    // Геттеры
    int GetID() const { return ID_employee; }
    std::string GetName() const { return name_employee; }
    std::string GetSurname() const { return surname_employee; }
    std::string GetDateOfStart() const { return date_of_start; }
    double GetSalaryRatio() const { return salary_ratio; }
    double GetSalary() const { return salary_ratio * new_bid; }

    // Сеттеры с валидацией
    void SetName(const std::string& name);
    void SetSurname(const std::string& surname);
    void SetDateOfStart(const std::string& date);
    void SetSalaryRatio(double ratio);

    // Валидация данных
    static bool IsNameValid(const std::string& name);
    static bool IsSurnameValid(const std::string& surname);
    static bool IsDateValid(const std::string& date);
    static bool IsSalaryRatioValid(double ratio);

    friend class EmployeeRepository;
};

#endif // !EMPLOYEE_H