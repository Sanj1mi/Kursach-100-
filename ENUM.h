#ifndef ENUM
#define ENUM
/*
    Тут хранятся все ENUM (-_-)
    
    нужны они как константы/макросы для
    определения состояние чего-либо
*/

// РОЛИ_АККАУНТОВ
enum UserAccountRole {
    NonStatus = -2,
    Banned = -1,
    User = 0,
    Admin
};
// ИЕРАРХИЯ_МЕНЮ
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

enum SalaryExpenses {                       // Енамы для времени поиска(квартал - 2.5 месяца)
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
