// Employee.cpp
#include "Employee.h"
#include <regex>
#include <stdexcept>

double new_bid = 1111;

// Конструктор по умолчанию
Employee::Employee() = default;

// Конструктор копирования
Employee::Employee(const Employee& other)
    : ID_employee(other.ID_employee),
    name_employee(other.name_employee),
    surname_employee(other.surname_employee),
    date_of_start(other.date_of_start),
    salary_ratio(other.salary_ratio) {
}

// Конструктор перемещения
Employee::Employee(Employee&& other) noexcept
    : ID_employee(std::move(other.ID_employee)),
    name_employee(std::move(other.name_employee)),
    surname_employee(std::move(other.surname_employee)),
    date_of_start(std::move(other.date_of_start)),
    salary_ratio(std::move(other.salary_ratio)) {
    // Инициализировать other, чтобы избежать двойного использования ресурсов
    other.ID_employee = 0;
    other.name_employee = "";
    other.surname_employee = "";
    other.date_of_start = "";
    other.salary_ratio = 0.0;
}

// Оператор присваивания копии
Employee& Employee::operator=(const Employee& other) {
    if (this != &other) {
        ID_employee = other.ID_employee;
        name_employee = other.name_employee;
        surname_employee = other.surname_employee;
        date_of_start = other.date_of_start;
        salary_ratio = other.salary_ratio;
    }
    return *this;
}

// Оператор присваивания перемещения
Employee& Employee::operator=(Employee&& other) noexcept {
    if (this != &other) {
        ID_employee = std::move(other.ID_employee);
        name_employee = std::move(other.name_employee);
        surname_employee = std::move(other.surname_employee);
        date_of_start = std::move(other.date_of_start);
        salary_ratio = std::move(other.salary_ratio);

        // Инициализировать other, чтобы избежать двойного использования ресурсов
        other.ID_employee = 0;
        other.name_employee = "";
        other.surname_employee = "";
        other.date_of_start = "";
        other.salary_ratio = 0.0;
    }
    return *this;
}

bool Employee::operator<(const Employee& other) const {
    return ID_employee < other.ID_employee;
}

void Employee::SetName(const std::string& name) {
    if (!IsNameValid(name)) {
        throw std::runtime_error("Invalid name format!");
    }
    name_employee = name;
}

void Employee::SetSurname(const std::string& surname) {
    if (!IsSurnameValid(surname)) {
        throw std::runtime_error("Invalid surname format!");
    }
    surname_employee = surname;
}

void Employee::SetDateOfStart(const std::string& date) {
    if (!IsDateValid(date)) {
        throw std::runtime_error("Invalid date format!");
    }
    date_of_start = date;
}

void Employee::SetSalaryRatio(double ratio) {
    if (!IsSalaryRatioValid(ratio)) {
        throw std::runtime_error("Salary ratio must be between 0.1 and 10.0!");
    }
    salary_ratio = ratio;
}

bool Employee::IsNameValid(const std::string& name) {
    if (name.empty() || name.length() > 50) return false;
    for (char c : name) {
        if (!((c >= 'А' && c <= 'я') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))) {
            return false;
        }
    }
    return true;
}

bool Employee::IsSurnameValid(const std::string& surname) {
    if (surname.empty() || surname.length() > 50) return false;
    for (char c : surname) {
        if (!((c >= 'А' && c <= 'я') || (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') || c == '-')) {
            return false;
        }
    }
    return true;
}

bool Employee::IsDateValid(const std::string& date) {
    if (date.length() != 10) return false;
    if (date[2] != '.' || date[5] != '.') return false;

    for (int i = 0; i < 10; i++) {
        if (i != 2 && i != 5 && !isdigit(date[i])) {
            return false;
        }
    }

    // Дополнительные проверки дня, месяца и года
    int day = stoi(date.substr(0, 2));
    int month = stoi(date.substr(3, 2));
    int year = stoi(date.substr(6, 4));

    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;

    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
        return false;                                               // Проверка для месяцев с 30 днями
    }
    if (month == 2) {                                               // Проверка для февраля
        bool isLeap = (year % 400 == 0) || (year % 100 != 0 && year % 4 == 0);
        if (day > (isLeap ? 29 : 28)) return false;
    }
    if (year > 2025 || year < 1970) {                               // проверка года
        return false;
    }
    if ((year == 2025) && month > 5 || (month == 5 && day > 14)) {  // проверка текущей даты(в иделае добавить считывание в реальном времени)
        return false;
    }
    return true;
}

bool Employee::IsSalaryRatioValid(double ratio) {
    return ratio >= 0.1 && ratio <= 10.0;
}