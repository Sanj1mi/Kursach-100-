// EmployeeRepository.h
#ifndef EMPLOYEE_REPOSITORY_H
#define EMPLOYEE_REPOSITORY_H

#include "Employee.h"
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

class EmployeeRepository {
private:
    static std::vector<Employee> employees;

public:


    static int GetVectorSize();
    static int GenerateUniqueID();
    static void SaveToFile();
    static void LoadFromFile();
    static void AddEmployee(Employee& employee);
    static void RemoveEmployee(int id);
    static void UpdateEmployee(const Employee& employee);

    static const std::vector<Employee>& GetAllEmployees();
    static Employee* GetEmployeeByID(int id);
    static std::vector<Employee> FindEmployeesByName(const std::string& name);
    static std::vector<Employee> FindEmployeesBySurname(const std::string& surname);
    static std::vector<Employee> FindEmployeesByDate(const std::string& date);
    static std::vector<Employee> FindEmployeesBySalary(const double& salary_ratio);
    
    static void ChangeEmployee(int id, const Employee& newData);
    static void RemoveSelectedEmployees(const std::vector<int> selected_index);
    
    template <typename T, typename Compare>
    static void quick_sort(T* array, int start_idx, int end_idx, Compare compare) {
        if (start_idx >= end_idx) return;
        if (start_idx < 0 || end_idx < 0 || start_idx >= end_idx) return;

        const T& pivot = array[(start_idx + end_idx) / 2];
        int left = start_idx;
        int right = end_idx;

        while (left <= right) {
            while (compare(array[left], pivot)) left++;
            while (compare(pivot, array[right])) right--;

            if (left <= right) {
                std::swap(array[left], array[right]);
                left++;
                right--;
            }
        }

        quick_sort(array, start_idx, right, compare);
        quick_sort(array, left, end_idx, compare);
    }

    //static void SortEmployees(EmployeeParameter parameter);
    static vector<Employee> SortEmployees(SortParameter parameter, bool ascending);

    static void Initialize();
    double CalculateSalaryExpenses(SalaryExpenses period);

};

#endif // !EMPLOYEE_REPOSITORY_H