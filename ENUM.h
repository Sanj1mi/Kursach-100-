#ifndef ENUM
#define ENUM
/*
    ��� �������� ��� ENUM (-_-)
    
    ����� ��� ��� ���������/������� ���
    ����������� ��������� ����-����
*/

// ����_���������
enum UserAccountRole {
    NonStatus = -2,
    Banned = -1,
    User = 0,
    Admin
};
// ��������_����
enum MenuHierarchy {
    MenuTitle,
    MenuLogin,
    MenuRegister,
    MenuManual,
    MenuMainLogic,
    MenuDisplayEmployees,
    MenuFindEmployee,
    MenuSortEmployees,

    MenuAdminAdd,
    MenuAdminChooseChange,
    MenuAdminChange,
    MenuAdminDelete,
    MenuAdminAccounts
};

enum MenuMinChoose {
    MinTitle = 5,
    MinLogin = 6,
    MinRegister = 7,
    MinMainLogic= 9,  
    MinDisplayEmp = 1,
    MinFindEmp = 1,
    MinSortEmp = 1,

    MinAdd = 6,
    MinChange = 1,
    MinDelete = 1,
    MinAccounts = 1
};

enum SalaryExpenses {                       // ����� ��� ������� ������(������� - 2.5 ������)
    Month = 3,
    Quarter = 2,
    Year = 1,
    AllTime = 0
};

enum EmployeeParameter {
    Name,
    SurName,
    DateOfStart,
    Salary
};

enum class SortParameter {
    Name,
    Surname,
    Date,
    Salary
};


#endif // !ENUM
