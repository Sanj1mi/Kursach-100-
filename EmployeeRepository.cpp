// EmployeeRepository.cpp
#include "EmployeeRepository.h"
#include <algorithm>
#include <stdexcept>
#include <sstream>

std::vector<Employee> EmployeeRepository::employees;




int EmployeeRepository::GetVectorSize() {
    return employees.size();
}
void EmployeeRepository::Initialize() {
    LoadFromFile();
}

void EmployeeRepository::AddEmployee(Employee& employee) {
    employee.ID_employee = GenerateUniqueID();
    for (size_t i = 0; i < employees.size(); i++) {
        if (employees[i].GetID() == employee.GetID()) {
            throw std::runtime_error("Employee with this ID already exists!");
        }
    }
    employees.push_back(employee);
    SaveToFile();
}

void EmployeeRepository::RemoveEmployee(int id) {
    size_t index = employees.size();
    for (size_t i = 0; i < employees.size(); i++) {
        if (employees[i].GetID() == id) {
            index = i;
            break;
        }
    }

    if (index == employees.size()) {
        throw std::runtime_error("Employee not found!");
    }

    employees.erase(employees.begin() + index);
    SaveToFile();
}

void EmployeeRepository::UpdateEmployee(const Employee& employee) {
    bool found = false;
    for (size_t i = 0; i < employees.size(); i++) {
        if (employees[i].GetID() == employee.GetID()) {
            employees[i] = employee;
            found = true;
            break;
        }
    }

    if (!found) {
        throw std::runtime_error("Employee not found!");
    }

    SaveToFile();
}

const std::vector<Employee>& EmployeeRepository::GetAllEmployees() {
    return employees;
}

Employee* EmployeeRepository::GetEmployeeByID(int id) {
    for (size_t i = 0; i < employees.size(); i++) {
        if (employees[i].GetID() == id) {
            return &employees[i];
        }
    }
    return nullptr;
}

std::vector<Employee> EmployeeRepository::FindEmployeesByName(const std::string& name) {
    std::vector<Employee> result;
    for (size_t i = 0; i < employees.size(); i++) {
        if (employees[i].GetName() == name) {
            result.push_back(employees[i]);
        }
    }
    return result;
}

std::vector<Employee> EmployeeRepository::FindEmployeesBySurname(const std::string& surname) {
    std::vector<Employee> result;
    for (size_t i = 0; i < employees.size(); i++) {
        if (employees[i].GetSurname() == surname) {
            result.push_back(employees[i]);
        }
    }
    return result;
}

std::vector<Employee> EmployeeRepository::FindEmployeesByDate(const std::string& date) {
    std::vector<Employee> result;
    for (size_t i = 0; i < employees.size(); i++) {
        if (employees[i].GetDateOfStart() == date) {
            result.push_back(employees[i]);
        }
    }
    return result;
}

std::vector<Employee> EmployeeRepository::FindEmployeesBySalary(const double& salary_ratio) {
    std::vector<Employee> result;
    for (size_t i = 0; i < employees.size(); i++) {
        if (employees[i].GetSalary() == salary_ratio) {
            result.push_back(employees[i]);
        }
    }
    return result;
}

void EmployeeRepository::ChangeEmployee(int id, const Employee& newData) {
    bool found = false;
    for (auto& emp : employees) {
        if (emp.GetID() == id) {
            // Проверяем, что ID не изменяется (если это не нужно)
            emp = newData;
            emp.ID_employee = id; // Сохраняем оригинальный ID
            found = true;
            break;
        }
    }

    if (!found) {
        throw std::runtime_error("Employee not found!");
    }

    SaveToFile();
}

void EmployeeRepository::RemoveSelectedEmployees(const std::vector<int> selected_index) {
    for (size_t i = 0; i < employees.size() ; i++) {
        for (size_t j = 0; j < selected_index.size(); j++) {
            if (i == selected_index[j]) {
                employees.erase(employees.begin() + i);
            }
        }
    }
    SaveToFile();
}

vector<Employee> EmployeeRepository::SortEmployees(SortParameter parameter, bool ascending) {
    vector<Employee> sorted = employees;

    if (sorted.empty()) return sorted;

    // Создаем массив указателей для сортировки
    vector<Employee*> ptrs;
    ptrs.reserve(sorted.size());
    for (auto& emp : sorted) {
        ptrs.push_back(&emp);
    }

    // Определяем компаратор в зависимости от параметра сортировки
    auto comparator = [parameter, ascending](const Employee* a, const Employee* b) {
        switch (parameter) {
        case SortParameter::Name:
            return ascending ? a->GetName() < b->GetName() : b->GetName() < a->GetName();
        case SortParameter::Surname:
            return ascending ? a->GetSurname() < b->GetSurname() : b->GetSurname() < a->GetSurname();
        case SortParameter::Date: {
            // Сравниваем даты как строки в формате дд.мм.гггг
            string dateA = a->GetDateOfStart();
            string day = dateA.substr(0,1);
            string month = dateA.substr(3,4);
            string year = dateA.substr(6,9);
            dateA = year + month + day;
            
            string dateB = b->GetDateOfStart();
            
            day = dateB.substr(0, 1);
            month = dateB.substr(3, 4);
            year = dateB.substr(6, 9);
            dateB = year + month + day;
            return ascending ? dateA < dateB : dateB < dateA;
        }
        case SortParameter::Salary:
            return ascending ? a->GetSalary() < b->GetSalary() : b->GetSalary() < a->GetSalary();
        default:
            return false;
        }
        };

    // Сортируем с помощью quick_sort
    quick_sort(ptrs.data(), 0, ptrs.size() - 1, comparator);

    // Собираем отсортированный вектор
    vector<Employee> result;
    result.reserve(sorted.size());
    for (const auto* p : ptrs) {
        result.push_back(*p);
    }

    return result;
}

// EmployeeRepository.h




int EmployeeRepository::GenerateUniqueID() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(100000, 999999);

    while (true) {
        int candidate = distrib(gen);
        bool unique = true;

        for (size_t i = 0; i < employees.size(); i++) {
            if (employees[i].GetID() == candidate) {
                unique = false;
                break;
            }
        }

        if (unique) {
            return candidate;
        }
    }
}

void EmployeeRepository::SaveToFile() {
    std::ofstream out("employees.txt");
    if (!out.is_open()) {
        throw std::runtime_error("Failed to open employees file for writing!");
    }

    for (size_t i = 0; i < employees.size(); i++) {
        const Employee& emp = employees[i];
        out << emp.GetID() << "|"
            << emp.GetName() << "|"
            << emp.GetSurname() << "|"
            << emp.GetDateOfStart() << "|"
            << emp.GetSalaryRatio() << "\n";
    }
}

void EmployeeRepository::LoadFromFile() {
    std::ifstream in("employees.txt");
    if (!in.is_open()) {
        return; // Файл не существует - это нормально при первом запуске
    }

    employees.clear();
    std::string line;

    while (std::getline(in, line)) {
        std::istringstream iss(line);
        std::string token;
        Employee emp;

        // ID
        std::getline(iss, token, '|');
        emp.ID_employee = std::stoi(token);

        // Name
        std::getline(iss, token, '|');
        emp.name_employee = token;

        // Surname
        std::getline(iss, token, '|');
        emp.surname_employee = token;

        // Date
        std::getline(iss, token, '|');
        emp.date_of_start = token;

        // Salary ratio
        std::getline(iss, token, '|');
        emp.salary_ratio = std::stod(token);

        employees.push_back(emp);
    }
}

// EmployeeRepository.cpp
double EmployeeRepository::CalculateSalaryExpenses(SalaryExpenses period) {
    double total = 0.0;
    time_t now = time(nullptr);
    tm currentTime;
    localtime_s(&currentTime, &now);

    for (const auto& emp : employees) {
        // Парсим дату начала работы
        int day, month, year;
        sscanf_s(emp.GetDateOfStart().c_str(), "%d.%d.%d", &day, &month, &year);

        // Проверяем попадает ли сотрудник в указанный период
        bool include = false;
        switch (period) {
        case Month:
            include = (year == currentTime.tm_year + 1900) &&
                (month == currentTime.tm_mon + 1);
            break;
        case Quarter: {
            int currentQuarter = (currentTime.tm_mon + 1) / 3 + 1;
            int empQuarter = month / 3 + 1;
            include = (year == currentTime.tm_year + 1900) &&
                (empQuarter == currentQuarter);
            break;
        }
        case Year:
            include = (year == currentTime.tm_year + 1900);
            break;
        case AllTime:
            include = true;
            break;
        }

        if (include) {
            total += emp.GetSalary();
        }
    }
    return total;
}