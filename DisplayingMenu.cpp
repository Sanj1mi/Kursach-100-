#include "DisplayingMenu.h"     
#include "PointerNew.h"
#include "GLOBALS.h"
#include "DynamicInput.h"

using namespace std;

void DisplayConfirmDialog(bool isYesSelected) {
    New_Screen();
    cout << crgb::GRAY << '+';
    for (size_t i = 0; i < 6; i++) {
        if (i == 3) {
            cout << " Подтвердить выход ";
        }
        cout << "---";
    }
    cout << "+\n\n";
    // Рисуем кнопки с подсветкой выбранного варианта
    cout << (isYesSelected ? crgb::GREEN + "\t[Yes]" + crgb::GRAY + "\t\t[No]" :
        crgb::GRAY + "\t[Yes]" + crgb::GREEN + "\t\t[No]") << crgb::RESET;
    
    cout << crgb::GRAY << "\n\n+";
    for (size_t i = 0; i < 6; i++) {
        if (i == 3) {
            cout << " Подсказки ";
        }
        cout << "----";
    }
    cout << "--+";
    cout << crgb::BLUE <<"\n[<][>]  " << crgb::RESET << "- выбор" << endl;
    cout << crgb::BLUE <<"[Enter] " << crgb::RESET << "- подтверждение" << endl;
    cout << crgb::BLUE <<"[Ecs]   " << crgb::RESET << "- отмена" << endl;
}

void DisplayDividingLine() {
    cout << crgb::GRAY << '+';
    for (size_t i = 0; i < 7; i++) {
        if (i == 3) {
            cout << " Mini Manual ";
        }
        cout << "----";
    }
    cout << "+\n" << crgb::RESET;
}
void DisplayConfirmOption() {
    New_Screen();
    cout << crgb::GRAY << R"(
        Вы действительно хотите выйти?
        [Ecs] - Нет    [Enter] - Да
    )" << crgb::RESET;

}
void DisplayExitArt() {
    cout << "Path:" << crgb::GREEN << " ~/Kursach/" << crgb::RESET << endl << endl;
    std::cout <<
        "  /\\_/\\ \n"
        " ( o.o ) \n"
        "  >" << crgb::RED << "^v^" << crgb::RESET << "< \n"
        "  /   \\ \n"
        " (     ) \n"
        "  \"\"\"\"\" \n";

    std::cout << R"(

  /\_/\
 ( -.- )  // Coding...
  /> <\
)" << std::endl;

}

void DisplayNyanCat() {
    std::cout << crgb::GRAY << R"(

  /\_/\
 ( -.- )  // Testing...
  /> <\
)" << std::endl;
}

void DisplayMiniManualWithHints() {
    DisplayDividingLine();
    string color;
    if (current_account.GetRole() == NonStatus) {
        color = crgb::BLUE;
    }
    else if (current_account.GetRole() == User) {
        color = crgb::GREEN;
    }
    else if (current_account.GetRole() == Admin) {
        color = crgb::RED;
    }
    cout << color << "[Ecs]" << crgb::RESET << "   - для выхода\n";
    cout << color << "[Enter]" << crgb::RESET << " - для подтверждения действия\n";
    cout << color << "[^][v]" << crgb::RESET << "  - для перемещения\n";
}

void DisplayMenuTitle() {
    New_Screen();
    cout << "Path:" << crgb::GRAY << " ~/Kursach/" << crgb::GREEN << "Title/" << crgb::RESET;
    cout << setw(81) << right <</*Тут пробелы*/ "[sign in]\n";
    cout << "   [Войти]" <<
        endl << "   [Регистрация]" <<
        endl << "   [Методичка]" <<
        endl << "   [Выход]\n";
    DisplayMiniManualWithHints();
}

void DisplayCurrentAccountStatus(UserAccountRole role, const AccountManager& current_account) {
    if (role == User || role == Admin) {
        cout << setw(70) << right << "(ID:" << current_account.GetID() << ")\n";
    }
    else {
        cout << setw(81) << "[sign in]\n";
    }
}

void DisplayMenuLogin() {
    New_Screen();
    cout << "Path:" << crgb::GRAY << " ~/Kursach/Title/" << crgb::GREEN << "Login/" << crgb::RESET << '\n';
    //DisplayCurrentAccountStatus(None, current_account);
    // Поле логина
    cout << (input_active && pointer == 1 ? crgb::GREEN : crgb::BLUE) << "   [Ввод]" << crgb::RESET << "(Логин) ";
    ConsoleInput::ColorizeInput(login_input, "ld", "", 20, -1);
    cout << login_input << "\n";

    // Поле пароля
    cout << (input_active && pointer == 2 ? crgb::GREEN : crgb::BLUE) << "   [Ввод]" << crgb::RESET << "(Пароль) ";
    if (show_password) {
        ConsoleInput::ColorizeInput(password_input, "lds", "", 13, -1);
        cout << password_input;
    }
    else {
        cout << GetMaskedPassword(password_input);
    }
    cout << "\n";


    // Показать пароль
    cout << (show_password ? crgb::GREEN : crgb::RESET) << "   [Показать пароль]" << crgb::RESET << endl
        << "   [Войти]" << ((pointer == 4 && isBanned) ? crgb::RED + "\t[Аккаунт заблокирован!!! причина: " + banReason + ']' : " ") 
        << ((pointer == 4 && isIncorrectInput) ? crgb::RED + "\t[Неправильный логин или пароль!]" : "") << crgb::RESET << endl
        << "   [Назад]\n";

    DisplayMiniManualWithHints();
}

/*

    [min-->max] [max-->min]
*/

// Маскировка пароля
string GetMaskedPassword(const string& input) {
    return string(input.length(), '*');
}

void DisplayMenuRegistration() {
    New_Screen();
    cout << "Path:" << crgb::GRAY << " ~/Kursach/Title/" << crgb::GREEN << "Register/" << crgb::RESET << '\n';
    //DisplayCurrentAccountStatus(None, current_account);
    cout << (input_active && pointer == 1 ? crgb::GREEN : crgb::BLUE) << "   [Ввод]" << crgb::RESET << "(Логин) ";
    ConsoleInput::ColorizeInput(login_input_reg, "ld", "", 20, -1);
    cout << login_input_reg << "\n";
    cout << (input_active && pointer == 2 ? crgb::GREEN : crgb::BLUE) << "   [Ввод]" << crgb::RESET << "(Пароль) ";
    if (show_password) {
        ConsoleInput::ColorizeInput(password_input_reg, "lds", "", 13, -1);
        cout << password_input_reg;
    }
    else {
        cout << GetMaskedPassword(password_input_reg);
    }
    cout << "\n";

    cout << (input_active && pointer == 3 ? crgb::GREEN : crgb::BLUE) << "   [Ввод]" << crgb::RESET << "(Подтвердить) ";
    if (show_password) {
        ConsoleInput::ColorizeInput(password_confirmation, "lds", "", 13, -1);
        cout << password_confirmation;
    }
    else {
        cout << GetMaskedPassword(password_confirmation);
    }
    cout << "\n";

    // Показать пароль
    cout << (show_password ? crgb::GREEN : crgb::RESET) << "   [Показать пароль]" << crgb::RESET << endl
        << "   [Зарегестрировать]" 
        << endl << "   [Назад]\n";

    DisplayMiniManualWithHints();

    if (confirm_error || password_error) {
        cout << crgb::GRAY << '+';
        for (size_t i = 0; i < 7; i++) {
            if (i == 3) {
                cout << " Errors ";
            }
            cout << "----";
        }
        cout << '+' << crgb::RESET;
    }

    cout << endl << ((pointer == 5 && confirm_error) ? crgb::RED + "[Ошибка подтверждения пароля]" : "")
        << endl << ((pointer == 5 && password_error) ? crgb::RED + "[Пароль должен содеражть: цифры + буквы + спец.символы]" : "")
        << crgb::RESET;
}

void DisplayMenuUser() {
    cout << "Path:" << crgb::GRAY << " ~/Kursach/Titile/" << crgb::GREEN << "User/" << crgb::RESET << endl;
    cout << "   [Вывод сотрудников]" << endl
        << "   [Поиск по параметру]" << endl
        << "   [Сортировка]\n";
    DisplayDividingLine();
    cout << crgb::GREEN << " [Esc]" << crgb::GRAY << " - выход в главное меню";
}

void DisplayMenuAdmin() {
    cout << "Path:" << crgb::GRAY << " ~/Kursach/Titile/" << crgb::RED <<"Admin/" << crgb::RESET << endl;
   /* cout << crgb::GRAY << "// тут уже ID будет(^^)\n";
    cout << "soo long coming soon..." << crgb::RESET;*/
    cout << crgb::GRAY << "   [Вывод сотрудников]" << endl
        << "   [Поиск по параметру]" << endl
        << "   [Сортировка]" << endl
        << crgb::RED << "   <" << crgb::GRAY << "Admin Roots" << crgb::RED << ">" << crgb::RESET;
    // Отображаем состояние подсписка
    if (sublist_state[4]) { // Для 4-го пункта (индекс 3)
        cout << crgb::RED << "<1>\n";
        cout << crgb::RED << "   <" << crgb::GRAY << "Добавление сотрудника\n";
        cout << crgb::RED << "   <" << crgb::GRAY << "Удаление сотрудника\n";
        cout << crgb::RED << "   <" << crgb::GRAY << "Изменение данных\n";
        cout << crgb::RED << "   <" << crgb::GRAY << "Управление пользователями\n" << crgb::RESET;
    }
    else {
        
        cout << crgb::RED << '<' << crgb::GRAY << '0' << crgb::RED << ">\n\n\n\n\n"; }
    DisplayDividingLine();
    cout << crgb::RED << " [Esc] " << crgb::GRAY << " - выход в главное меню\n";
    cout << crgb::RED << " [<][>]" << crgb::GRAY << " - управление подсписком";

}

void DisplayExpanses() {
    cout << endl << crgb::GRAY << '+';
    for (size_t i = 0; i < 7; i++) {
        if (i == 3) {
            cout << " Expanses ";
        }
        else {
            cout << "----";
        }
    }
    cout << "+\n" << crgb::RESET;
    cout << "\tМесяц: " << crgb::GREEN << month_expanses << crgb::RESET << endl;
    cout << "\tКвартал: " << crgb::GREEN << quarter_expanses << crgb::RESET << endl;
    cout << "\tГод: " << crgb::GREEN << year_expanses << crgb::RESET << endl;
    cout << "\tВсе время: " << crgb::GREEN << alltime_expanses << crgb::RESET << endl;
    
    cout << crgb::GRAY << '+';
    for (size_t i = 0; i < 8; i++) {
        cout << "----";
    }
    cout << "--+" << crgb::RESET;

}

void DisplayMenuEmployeeList() {
    New_Screen();
    cout << "Path:" << crgb::GRAY << " ~/Kursach/User/" << (current_account.GetRole() == Admin ? crgb::RED : crgb::GREEN) << "Employees/" << crgb::RESET << endl;

    const vector<Employee>& employees = EmployeeRepository::GetAllEmployees();
    for (size_t i = 0; i < employees.size(); i++) {
        const Employee& emp = employees[i];
        cout << "   [ID:" << setw(6) << setfill(' ') << emp.GetID() << "] "
            << "[ " << setw(15) << left << emp.GetName() << " | "
            << setw(15) << left << emp.GetSurname() << " | "
            << emp.GetDateOfStart() << " | "
            << fixed << setprecision(2) << (emp.GetSalaryRatio() * bid) << " ]" << endl;
    }
    cout << crgb::RESET;
    
    if (current_account.GetRole() == User) {
        DisplayExpanses();
    }
    cout << endl << (current_account.GetRole() == Admin ? crgb::RED : crgb::GREEN ) << " [Esc]" << crgb::GRAY << "  - назад";
    cout << endl << (current_account.GetRole() == Admin ? crgb::RED : crgb::GREEN) << " [^][v]" << crgb::GRAY << " - навигация";
    if (current_account.GetRole() == Admin) {
        cout << endl << crgb::RED << " [A]" << crgb::GRAY << "    - добавить";
    }
}

/*
Path: ~/xxx
+------- Учет затрат -------+
|   месяц:                  |
|   квартал:                |
|   год:                    |
|   все время:              |
+------- Mini manual -------+
[Esc]  - назад;
[^][v] - навигация
+------- List --------------+
-->[]
   []
   []
*/



/*
Path: ~/BAN
+---------- Accounts --------+
       [ ][ID:000013][Login: kissune]       // если бан то открывается возможность написать соо(его еще в файл добавить)
    -->[U][ID:XXXXXX][Login: user   ]       // нажимая enter меняем: [U]->[A]->[B]  // можно через [<] [>] и enter для подтверждения\

*/

// поиск по параметру:
/*

 path:
    [Ввод](Имя)
    [Ввод](Фамилия)
    [Ввод](Дата зачисления)                       // хх.хх.хххх
 -->[Ввод](З/П)
    [Search]
+------ Current list -------+
    [...]
    [...]
 */

// изменение
/*
 -->[ID:XXXXXX][Name...]                // после enter переходим к реадктированию
    [ID:XXXXXX][Name...]
    [ID:XXXXXX][Name...]

редактировка:
    [Ввод] "old" -->
    [Ввод] "old" -->
    [Ввод] "old" -->
 -->[Ввод] "old" -->
    [Save] 
*/

// сортирока
/*
Path:
    [ ](Имя)
    [ ](Фамилия)
    [ ](Дата зачисления)                       // хх.хх.хххх
 -->[*](З/П)
    [Sort]              // тут копия вектора (до выхода из проги)
+------- Sorted list -------+
*/

// удлаение
/*
Path:
-->[*][...]
   [*][...]
   [ ][...]
   [ ][...]

[Del] - удаление        // КСТАААААААААААААА ТОЧНОООООО
*/
void DisplayMenuAddEmployee() {
    New_Screen();
    cout << "Path:" << crgb::GRAY << " ~/Kursach/Admin/" << crgb::RED << "AddEmployee/" << crgb::RESET << endl;

    // Имя (цвет зависит от валидности)
    cout << (input_active && pointer == 1 ? crgb::GREEN : crgb::BLUE)
        << "   [Ввод]" << crgb::RESET << "(Имя) ";
    if (Employee::IsNameValid(name_input)) {
        cout << crgb::GREEN << name_input << crgb::RESET;
    }
    else {
        cout << crgb::RED << name_input << crgb::RESET;
    }
    cout << "\n";

    // Фамилия
    cout << (input_active && pointer == 2 ? crgb::GREEN : crgb::BLUE)
        << "   [Ввод]" << crgb::RESET << "(Фамилия) ";
    if (Employee::IsSurnameValid(surname_input)) {
        cout << crgb::GREEN << surname_input << crgb::RESET;
    }
    else {
        cout << crgb::RED << surname_input << crgb::RESET;
    }
    cout << "\n";

    // Дата
    cout << (input_active && pointer == 3 ? crgb::GREEN : crgb::BLUE)
        << "   [Ввод]" << crgb::RESET << "(Дата дд.мм.гггг) ";
    if (Employee::IsDateValid(date_of_start_input)) {
        cout << crgb::GREEN << date_of_start_input << crgb::RESET;
    }
    else {
        cout << crgb::RED << date_of_start_input << crgb::RESET;
    }
    cout << "\n";

    // Коэффициент
    cout << (input_active && pointer == 4 ? crgb::GREEN : crgb::BLUE)
        << "   [Ввод]" << crgb::RESET << "(Коэффициент 0.1-10.0) ";
    if (Employee::IsSalaryRatioValid(salary_ratio)) {
        cout << crgb::GREEN << salary_ratio << crgb::RESET;
    }
    else {
        cout << crgb::RED << salary_ratio_input << crgb::RESET;
    }
    cout << "\n";

    cout << "   [Сохранить]" << endl;
    cout << "   [Назад]" << endl;

    DisplayMiniManualWithHints();
}

void DisplayMenuAdminRemove(vector<bool> selected_employees) {
    if (screen_needs_update) {
        New_Screen();
        cout << "Path:" << crgb::GRAY << " ~/Kursach/Admin/" << crgb::RED << "DeleteEmployee/" << crgb::RESET << endl;

        const auto& employees = EmployeeRepository::GetAllEmployees();
        if (selected_employees.empty()) {
            selected_employees.resize(employees.size(), false);
        }

        for (size_t i = 0; i < employees.size(); i++) {
            const auto& emp = employees[i];
            cout << "   [ID:" << emp.GetID() << "] "
                << emp.GetName() << " " << emp.GetSurname() << endl;
        }

        cout << "\n" << crgb::RED << " [Del]" << crgb::RESET << " - удалить выделенного сотрудника\n";
        cout << crgb::GRAY << " [Esc]" << crgb::RESET << " - назад\n";

        valid_positions = vector<bool>(employees.size() + 1, true);
        valid_positions[0] = false;

        DisplayMiniManualWithHints();
        screen_needs_update = false;
    }
}

void DisplayMenuFindEmployee() {
    New_Screen();
    cout << "Path:" << crgb::GRAY << " ~/Kursach/"
        << (current_account.GetRole() == Admin ? crgb::RED : crgb::GREEN)
        << "FindEmployee/" << crgb::RESET << endl;

    // Поля для ввода параметров поиска
    cout << (input_active && pointer == 1 ? crgb::GREEN : crgb::BLUE)
        << "   [Ввод]" << crgb::RESET << "(Имя) ";
    ConsoleInput::ColorizeInput(name_input, "ld", "", 20, -1);
    cout << name_input << "\n";

    cout << (input_active && pointer == 2 ? crgb::GREEN : crgb::BLUE)
        << "   [Ввод]" << crgb::RESET << "(Фамилия) ";
    ConsoleInput::ColorizeInput(surname_input, "ld", "", 20, -1);
    cout << surname_input << "\n";

    cout << (input_active && pointer == 3 ? crgb::GREEN : crgb::BLUE)
        << "   [Ввод]" << crgb::RESET << "(Дата дд.мм.гггг) ";
    ConsoleInput::ColorizeInput(date_of_start_input, "d", "", 10, -1);
    cout << date_of_start_input << "\n";

    cout << (input_active && pointer == 4 ? crgb::GREEN : crgb::BLUE)
        << "   [Ввод]" << crgb::RESET << "(Зарплата от) ";
    ConsoleInput::ColorizeInput(to_string(salary_ratio), "d", "", 10, -1);
    cout << salary_ratio << "\n";

    cout << "   [Поиск]\n";
    cout << "   [Назад]\n";

    // Отображение результатов поиска
    if (!search_results.empty()) {
        cout << crgb::GRAY << "+------- Результаты -------+\n" << crgb::RESET;
        for (const auto& emp : search_results) {
            cout << "   [ID:" << setw(6) << setfill(' ') << emp.GetID() << "] "
                << "[ " << setw(15) << left << emp.GetName() << " | "
                << setw(15) << left << emp.GetSurname() << " | "
                << emp.GetDateOfStart() << " | "
                << fixed << setprecision(2) << (emp.GetSalaryRatio() * bid) << " ]" << endl;
        }
    }

    DisplayMiniManualWithHints();
}

void DisplayMenuSortEmployees(const vector<bool>& choosen_param, const vector<Employee>& sorted_employees) {
    New_Screen();
    cout << "Path:" << crgb::GRAY << " ~/Kursach/"
        << (current_account.GetRole() == Admin ? crgb::RED : crgb::GREEN)
        << "SortEmployyes/" << crgb::RESET << endl;

    // Параметры сортировки
    cout << "   [" << (choosen_param[0] ? '*' : ' ') << "][Имя]" << endl
        << "   [" << (choosen_param[1] ? '*' : ' ') << "][Фамилия]" << endl
        << "   [" << (choosen_param[2] ? '*' : ' ') << "][Дата]" << endl
        << "   [" << (choosen_param[3] ? '*' : ' ') << "][З/П]" << endl
        << "   [Сортировать]" << endl
        << "   [Назад]\n";

    // Разделитель
    cout << crgb::GRAY << '+';
    for (size_t i = 0; i < 9; i++) {
        if (i == 3) {
            cout << " Sorted ";
        }
        else {
            cout << "----";
        }
    }
    cout << "-+\n" << crgb::RESET;

    // Отображение отсортированного списка
    if (!sorted_employees.empty()) {
        for (const auto& emp : sorted_employees) {
            cout << "   [ID:" << setw(6) << setfill(' ') << emp.GetID() << "] "
                << "[ " << setw(15) << left << emp.GetName() << " | "
                << setw(15) << left << emp.GetSurname() << " | "
                << emp.GetDateOfStart() << " | "
                << fixed << setprecision(2) << (emp.GetSalaryRatio() * bid) << " ]" << endl;
        }
    }

    // Подсказки
    DisplayMiniManualWithHints();
    cout << crgb::RED << "[<][>] " << crgb::RESET << " - изменение направления сортировки";
    DisplayNyanCat();
}

void DisplayMenuChange(const Employee& employee) {
    New_Screen();
    cout << "Path:" << crgb::GRAY << " ~/Kursach/Admin/ChangeChoose/"
        << crgb::RED << "Change/" << crgb::RESET << endl;
    // Поля для ввода параметров поиска
    cout << (input_active && pointer == 1 ? crgb::GREEN : crgb::BLUE)
        << "   [Ввод]" << crgb::GRAY << '[' << employee.GetName() << "] --> " << crgb::RESET;
    ConsoleInput::ColorizeInput(name_input, "ld", "", 20, -1);
    cout << name_input << "\n";

    cout << (input_active && pointer == 2 ? crgb::GREEN : crgb::BLUE)
        << "   [Ввод]" << crgb::GRAY << '[' << employee.GetSurname() << "] --> " << crgb::RESET;
    ConsoleInput::ColorizeInput(surname_input, "ld", "", 20, -1);
    cout << surname_input << "\n";

    cout << (input_active && pointer == 3 ? crgb::GREEN : crgb::BLUE)
        << "   [Ввод]" << crgb::GRAY << '[' << employee.GetDateOfStart() << "] --> " << crgb::RESET;
    ConsoleInput::ColorizeInput(date_of_start_input, "d", "", 10, -1);
    cout << date_of_start_input << "\n";

    cout << (input_active && pointer == 4 ? crgb::GREEN : crgb::BLUE)
        << "   [Ввод]" << crgb::GRAY << '[' << employee.GetSalaryRatio() << "] --> " << crgb::RESET;
    ConsoleInput::ColorizeInput(salary_ratio_input, "d", "", 10, -1);
    cout << salary_ratio << "\n";

    cout << crgb::RED << "   [Сохранить]\n";
    cout << "   [Назад]\n" << crgb::RESET;
}

void DisplayMenuAccountManagement(const vector<AccountManager>& accounts, int ban_input_pointer, const string& ban_reason_input) {
    New_Screen();
    cout << "Path:" << crgb::GRAY << " ~/Kursach/Admin/"
        << crgb::RED << "Accounts/" << crgb::RESET << endl;

    for (size_t i = 0; i < accounts.size(); i++) {
        const AccountManager& acc = accounts[i];
        string role_char;
        switch (acc.GetRole()) {
        case Admin: role_char = "A"; break;
        case Banned: role_char = "B"; break;
        default: role_char = "U";
        }

        // Отображение строки аккаунта
        cout << (i + 1 == pointer ? "-->" : "   ")
            << "[" << role_char << "][ID:" << setw(6) << acc.GetID()
            << "][Login:" << setw(13) << left << acc.GetLogin() << "]";

        // Отображение причины бана
        if (acc.GetRole() == Banned && i != ban_input_pointer) {
            cout << " Reason: " << acc.GetBanReason();
        }
        else if (i == ban_input_pointer) {
            cout << " Reason: " << ban_reason_input;
        }
        cout << crgb::RESET << endl;
    }

    // Подсказки по управлению
    cout << crgb::GRAY << "+---------------------------------+\n" << crgb::RESET;
    cout << crgb::RED << "[Esc]" << crgb::GRAY << " - Back to main menu or cancel input\n";
    cout << crgb::RED << "[<][>]" << crgb::GRAY << " - Change role\n";
    cout << crgb::RED << "[Enter]" << crgb::GRAY << " - Confirm ban reason\n";
}
