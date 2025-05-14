#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include <vector>
#include "Employee.h"
#include "ENUM.h"
#include "AccountRepository.h"

using namespace std;

// GLOBALS
// отображение
extern bool input_active ;
extern bool main_exit ;
extern bool screen_needs_update ; // Flag for screen update optimization
// авторизация + регистрация
extern std::string login_input, password_input;
extern std::string login_input_reg, password_input_reg, password_confirmation;
extern bool show_password;
extern bool login_error;
// для сотрудников
extern std::string name_input, surname_input, date_of_start_input;
extern std::string salary_ratio_input;

extern bool isBanned;
extern bool isIncorrectInput;
extern bool password_error;
extern bool confirm_error;

extern std::string banReason;

extern double salary_ratio;
extern double bid;                                 // базовая ставка З/П

extern double month_expanses;
extern double quarter_expanses;
extern double year_expanses;
extern double alltime_expanses;

extern bool is_admin_sublist_open;

extern EmployeeParameter input_parameter;
extern AccountManager current_account;

extern std::vector<bool> valid_positions;
extern std::vector<bool> valid_sublist;  // Какие элементы могут иметь подсписок
extern std::vector<bool> sublist_state;  // Текущее состояние подсписка (открыт/закрыт)
extern bool is_sorting_active;

extern size_t index_for_change;

extern MenuHierarchy current_menu;

extern bool current_ban_reason_input;
extern string ban_reason_input;

#endif // !GLOBALS_H
