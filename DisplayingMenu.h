#ifndef DISPLAYINGMENU_H
#define DISPLAYINGMENU_H

// INCLUDE
	#include <iostream>
	#include <iomanip>

	#include <colors.h>
	#include "ENUM.h"
	#include "AccountRepository.h"
	#include "EmployeeRepository.h"
	#include "Employee.h"
	#include "EmployeeGlobals.h"
// PROTOTYPE
	void DisplayConfirmDialog(bool isYesSelected);
	void DisplayDividingLine();
	void DisplayConfirmOption();
	string GetMaskedPassword(const string& input);

	void DisplayExitArt();
	void DisplayNyanCat();
	void DisplayMiniManualWithHints();
	void DisplayMenuTitle();
	void DisplayCurrentAccountStatus(UserAccountRole role, const AccountManager& current_account);
	void DisplayMenuLogin();
	void DisplayMenuRegistration();
	void DisplayMenuUser();
	void DisplayMenuAdmin();

	void DisplayExpanses();
	void DisplayMenuEmployeeList();
	void DisplayMenuAddEmployee();

	void DisplayMenuAdminRemove(vector<bool> selected_employees);
	void DisplayMenuFindEmployee();
	void DisplayMenuSortEmployees(const vector<bool>& choosen_param, const vector<Employee>& sorted_employees);

	void DisplayMenuChange(const Employee& employee);
	void DisplayMenuAccountManagement(const vector<AccountManager>& accounts, int ban_input_pointer, const string& ban_reason_input);
	//void DisplayMenuAccountManagement(const vector<AccountManager>& accounts);
	//void DisplayMenuSortEmployees(const vector<bool> choosen_param, const vector<Employee>& sorted_employees);
#endif // !DISPLAYINGMENU_H
