#ifndef MENULOGIC
#define MENULOGIC
// INCLUDES
	#include<iostream>
	#include <chrono>
	#include "ENUM.h"
	#include "GLOBALS.h"	
	#include "PointerNew.h"
	#include "DynamicInput.h"
	#include "DisplayingMenu.h"
	#include "EmployeeRepository.h"
	

using namespace std;
// PROTOTYPES
	//bool IsValidPassword(const string& password);
	bool ConfirmExit();
	void RunningMenuLogic();
	void Update_Valid_Positions_Menu(MenuHierarchy current_menu);
	bool RunningMenuTitle();
	bool RunningMenuLogin();
	bool RunningMenuRegister();
	bool RunningMenuManual();
	bool RunningMenuMainLogic();

	bool RunningMenuDisplayEmployees();

	bool RunningMenuAdminAdd();
	bool RunningMenuAdminDelete();
	bool RunningMenuFindEmployee();

	bool RunningMenuSortByParam();

	bool RunningMenuChooseChangeEmployee();
	bool RunningMenuChangeEmployee();
	bool RunningMenuAccountManagement();
#endif // !MENULOGIC
