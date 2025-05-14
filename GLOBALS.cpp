#include "GLOBALS.h"

bool input_active = 0;
bool main_exit = false;
bool screen_needs_update = true;

std::string login_input, password_input;
std::string login_input_reg, password_input_reg, password_confirmation;
bool show_password;
bool login_error;

std::string name_input, surname_input, date_of_start_input;
std::string salary_ratio_input = "";

bool isBanned = false;
bool isIncorrectInput = false;
bool password_error = false;
bool confirm_error = false;

string banReason;
double salary_ratio = 1;

extern double month_expanses = 0;
extern double quarter_expanses = 0;
extern double year_expanses = 0;
extern double alltime_expanses = 0;

bool is_admin_sublist_open = false;

std::vector<bool> valid_positions = { 0,1,1,1,1 };
std::vector<bool> valid_sublist;
std::vector<bool> sublist_state;
bool is_sorting_active = false;

size_t index_for_change;

MenuHierarchy current_menu = MenuTitle;
EmployeeParameter input_parameter;

AccountManager current_account;

bool current_ban_reason_input = false;
string ban_reason_input;

double bid = 1111.00;                                 // базовая ставка З/П
