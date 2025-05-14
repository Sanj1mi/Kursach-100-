#include "MenuLogic.h"

// Проверка пароля на наличие буквы, цифры и спецсимвола
//bool IsValidPassword(const string& password) {
//    bool has_letter = false, has_digit = false, has_special = false;
//    for (char c : password) {
//        if (isalpha(static_cast<unsigned char>(c))) has_letter = true;
//        else if (isdigit(static_cast<unsigned char>(c))) has_digit = true;
//        else if ((c >= 33 && c <= 47) || (c >= 58 && c <= 64) ||
//            (c >= 91 && c <= 96) || (c >= 123 && c <= 126)) has_special = true;
//    }
//    return has_letter && has_digit && has_special;
//}
bool ConfirmExit() {
    bool isYesSelected = true;  // По умолчанию выбран "Yes"
    bool shouldExit = false;
    bool decisionMade = false;
    screen_needs_update = true;

    while (!decisionMade) {
        if (screen_needs_update) {
            DisplayConfirmDialog(isYesSelected);
            screen_needs_update = false;
        }

        // Обработка ввода
        Left_Or_Right_Logic();
        Enter_Button();
        Escape_Button();

        if (Left_Now || Right_Now) {
            isYesSelected = !isYesSelected;  // Переключаем выбор
            Left_Now = Right_Now = false;
            screen_needs_update = true;
        }

        if (Enter_Now) {
            Enter_Now = false;
            shouldExit = isYesSelected;
            decisionMade = true;
        }

        if (Escape_Now) {
            Escape_Now = false;
            decisionMade = true;
            shouldExit = false;
        }

        Sleep(50);
    }

    return shouldExit;
}

void RunningMenuLogic() {
    while (!main_exit) {
        Set_Pointer_Color(255, 192, 203);
        switch (current_menu) {
        case MenuTitle:
            if (RunningMenuTitle()) {
                return;
            }
            break;
        case MenuLogin:
            if (RunningMenuLogin()) {
                main_exit = true; // Выход (если Esc на титульное меню не возвращает)
            }
            break;
        case MenuRegister:
            if (RunningMenuRegister()) {
                main_exit = true;
            }
            break;
        case MenuMainLogic:
            if (RunningMenuMainLogic()) {
                    main_exit = true;
            }
            break;
        case MenuManual:
            if (RunningMenuManual()) {
                main_exit = true;
            }
            break;
        case MenuDisplayEmployees:
            if (RunningMenuDisplayEmployees()) {
                main_exit = true;
            }
            break;
        case MenuFindEmployee:
            if (RunningMenuFindEmployee()) {
                main_exit = true;
            }
            break;
        case MenuSortEmployees:
            if (RunningMenuSortByParam()) {
                main_exit = true;
            }
            break;
        case MenuAdminAdd:
            if (RunningMenuAdminAdd()) {
                main_exit = true;
            }
            break;
        case MenuAdminChooseChange:
            if (RunningMenuChooseChangeEmployee()) {
                main_exit = false;
            }
            break;
        case MenuAdminChange:
            if (RunningMenuChangeEmployee()) {
                main_exit = false;
            }
            break;
        case MenuAdminDelete:
            if (RunningMenuAdminDelete()) {
                main_exit = true;
            }
            break;
        case MenuAdminAccounts:
            if (RunningMenuAccountManagement()) {
                main_exit = false;
            }
            break;
        default:
            break;
        }
    }
}

// Обработка ввода для логина
bool RunningMenuLogin() {
    input_active = false;
    pointer = 1;
    screen_needs_update = true;
    login_input.clear();
    password_input.clear();
    login_error = false;
    show_password = false;
    
    const string login_allowed = "ld"; // Латиница и цифры
    const string password_allowed = "lds"; // Латиница, цифры, спецсимволы
    const string forbidden = ""; // Нет запрещённых символов
    bool should_exit = false;
    isIncorrectInput = false;

    while (!should_exit) {
        if (screen_needs_update) {
            Update_Valid_Positions_Menu(MenuLogin);
            DisplayMenuLogin();
            if (input_active && (pointer == 1 || pointer == 2)) {
                Set_Pointer_Color(0, 200, 0);
            }
            else {
                Set_Pointer_Color(255, 192, 203);
            }
            Creat_New_Pointer(pointer, valid_positions);
            screen_needs_update = false;
        }

        Up_Or_Down_Logic(static_cast<int>(valid_positions.size()), valid_positions);
        Enter_Button();

        if (Up_Now || Down_Now) {
            input_active = false;
            screen_needs_update = true;
        }

        // Обработка ввода символов
        for (int key = 8; key <= 255; key++) {
            if (ConsoleInput::IsKeyPressed(key)) {
                if (key == VK_BACK) {
                    if (pointer == 1 && !login_input.empty()) {
                        input_active = true;
                        login_input.pop_back();
                        screen_needs_update = true;
                    }
                    else if (pointer == 2 && !password_input.empty()) {
                        input_active = true;
                        password_input.pop_back();
                        screen_needs_update = true;
                    }
                }
                else
                    // Добавляем обработку Escape
                    Escape_Button();
                if (Escape_Now) {
                    Escape_Now = 0;
                    New_Screen();
                    current_menu = MenuTitle;
                    return false;
                }
                else {
                    char c = ConsoleInput::GetCharFromKey(key);
                    if (c != 0) {
                        if (pointer == 1 && login_input.length() < 20 &&
                            ConsoleInput::IsCharValid(c, login_allowed, forbidden, 0, -1)) {
                            input_active = true;
                            login_input += c;
                            screen_needs_update = true;
                        }
                        else if (pointer == 2 && password_input.length() < 13 &&
                            ConsoleInput::IsCharValid(c, password_allowed, forbidden, 0, -1)) {
                            input_active = true;
                            password_input += c;
                            screen_needs_update = true;
                        }
                    }
                }
            }
        }

        if (Enter_Now) {
            Enter_Now = false;
            if (pointer == 1 || pointer == 2) {
                input_active = true;
                screen_needs_update = true;
            }
            else if (pointer == 3) {
                show_password = !show_password;
                screen_needs_update = true;
            }
            else if (pointer == 4) {
                if (login_input.length() >= 4 && login_input.length() <= 20 &&
                    password_input.length() >= 4 && password_input.length() <= 13 /*&&
                    IsValidPassword(password_input)*/) {
                    const vector<AccountManager>& accounts = AccountRepository::GetAllAccounts();
                    bool accountFound = false;
                    string banReason;

                    for (const auto& acc : accounts) {
                        if (acc.GetLogin() == login_input) {
                            accountFound = true;
                            if (acc.GetRole() == Banned) {
                                isBanned = true;
                                banReason = acc.GetBanReason();
                                break;
                            }
                        }
                    }
                    
                    if (AccountRepository::IsInputDataCorrect(login_input, password_input)) {
                        current_menu = MenuMainLogic;
                        login_input.clear();
                        password_input.clear();
                        return false;
                        should_exit = true;
                    }
                    else {
                        isIncorrectInput = true;
                        login_error = true;
                        screen_needs_update = true;
                    }
                }
                else {
                    login_error = true;
                    screen_needs_update = true;
                }
            }
            else if (pointer == 5) {
                current_menu = MenuTitle;
                return false;
                should_exit = true;
            }
        }


        Sleep(10);
    }

    return should_exit;
}

bool RunningMenuRegister() {
    input_active = false;
    pointer = 1;
    screen_needs_update = true;
    login_input_reg.clear();
    password_input_reg.clear();
    password_confirmation.clear();
    login_error = false;
    password_error = false;
    confirm_error = false;
    show_password = false;


    const string login_allowed = "ld";     // Латиница и цифры
    const string password_allowed = "lds"; // Латиница, цифры, спецсимволы
    const string forbidden = "";           // Нет запрещённых символов
    bool should_exit = false;

    while (!should_exit) {
        if (screen_needs_update) {
            Update_Valid_Positions_Menu(MenuRegister);
            DisplayMenuRegistration();
            if (input_active && (pointer >= 1 && pointer <= 3)) {
                Set_Pointer_Color(0, 200, 0);
            }
            else {
                Set_Pointer_Color(255, 192, 203);
            }
            Creat_New_Pointer(pointer, valid_positions);
            screen_needs_update = false;
        }

        Up_Or_Down_Logic(static_cast<int>(valid_positions.size()), valid_positions);
        Enter_Button();

        if (Up_Now || Down_Now) {
            input_active = false;
            screen_needs_update = true;
        }

        // Обработка ввода символов
        for (int key = 8; key <= 255; key++) {
            if (ConsoleInput::IsKeyPressed(key)) {
                if (key == VK_BACK) {
                    input_active = true;
                    if (pointer == 1 && !login_input_reg.empty()) {
                        login_input_reg.pop_back();
                        screen_needs_update = true;
                    }
                    else if (pointer == 2 && !password_input_reg.empty()) {
                        password_input_reg.pop_back();
                        screen_needs_update = true;
                    }
                    else if (pointer == 3 && !password_confirmation.empty()) {
                        password_confirmation.pop_back();
                        screen_needs_update = true;
                    }
                }
                else
                    Escape_Button();
                if (Escape_Now) {
                    Escape_Now = 0;
                    New_Screen();
                    current_menu = MenuTitle;
                    return false;
                }
                else {
                    char c = ConsoleInput::GetCharFromKey(key);
                    if (c != 0) {
                        input_active = true;
                        if (pointer == 1 && login_input_reg.length() < 20 &&
                            ConsoleInput::IsCharValid(c, login_allowed, forbidden, 0, -1)) {
                            login_input_reg += c;
                            screen_needs_update = true;
                        }
                        else if (pointer == 2 && password_input_reg.length() < 13 &&
                            ConsoleInput::IsCharValid(c, password_allowed, forbidden, 0, -1)) {
                            password_input_reg += c;
                            screen_needs_update = true;
                        }
                        else if (pointer == 3 && password_confirmation.length() < 13 &&
                            ConsoleInput::IsCharValid(c, password_allowed, forbidden, 0, -1)) {
                            password_confirmation += c;
                            screen_needs_update = true;
                        }
                    }
                }
            }
        }

        if (Enter_Now) {
            Enter_Now = false;
            if (pointer >= 1 && pointer <= 3) {
                input_active = true;
                screen_needs_update = true;
            }
            else if (pointer == 4) { // Показать/скрыть пароль
                show_password = !show_password;
                screen_needs_update = true;
            }
            else if (pointer == 5) { // Зарегистрироваться
                bool login_ok = (login_input_reg.length() >= 4 && login_input_reg.length() <= 20);
                bool pass_ok = (password_input_reg.length() >= 4 && password_input_reg.length() <= 13 && AccountRepository::IsValidPassword(password_input_reg));
                bool confirm_ok = (password_input_reg == password_confirmation);
                bool login_free = AccountRepository::IsLoginFree(login_input_reg);

                login_error = !login_ok || !login_free;
                password_error = !pass_ok;
                confirm_error = !confirm_ok;


                if (!login_error && pass_ok && confirm_ok) {
                    try {
                        AccountManager new_account;
                        new_account.SetUserLogin(login_input_reg);
                        new_account.SetUserPassword(password_input_reg);
                        new_account.SetRoleReg(User);
                        AccountRepository::AddAccountObject(new_account); // Вызов метода создания аккаунта
                        login_input_reg.clear();
                        password_input_reg.clear();
                        password_confirmation.clear();
                        current_menu = MenuTitle;
                        return false;
                    }
                    catch (const runtime_error& e) {
                        //cout << e.what();
                        login_error = true;
                        //screen_needs_update = true;
                    }
                }

                screen_needs_update = true;
            }
            else if (pointer == 6) { // Назад
                current_menu = MenuTitle;
                return false;
            }
        }

        Sleep(10);
    }

    return should_exit;
}

bool RunningMenuTitle() {
    pointer = 1;
    screen_needs_update = true;

    while (true) {
        if (screen_needs_update) {
            New_Screen();
            DisplayMenuTitle();
            Update_Valid_Positions_Menu(MenuTitle);
            Creat_New_Pointer(pointer, valid_positions);
            screen_needs_update = false;
        }

        Up_Or_Down_Logic(static_cast<int>(valid_positions.size()), valid_positions);
        Enter_Button();
        if (Up_Now || Down_Now) {
            screen_needs_update = true;
        }
        if (Enter_Now) {
            Enter_Now = 0;
            switch (pointer) {
            case 1:
                current_menu = MenuLogin;
                return false;
            case 2:
                current_menu = MenuRegister;
                return false;
            case 3:
                current_menu = MenuManual;
                return false;
            case 4:
                return true;
            }
        }
        // Добавляем обработку Escape
        /*if (Escape_Now) {
            Escape_Now = 0;
            New_Screen();
            DisplayExitArt();
            return true;
        }*/
        Escape_Button();
        if (Escape_Now) {
            Escape_Now = false;
            if (ConfirmExit()) {
                New_Screen();
                DisplayExitArt();
                return true;
            }
            else {
                screen_needs_update = true;
            }
        }

        Sleep(50);
    }
}

void Update_Valid_Positions_Menu(MenuHierarchy current_menu) {
    switch (current_menu) {
    case MenuTitle:
        valid_positions = { 0, 1, 1, 1, 1 };
        break;
    case MenuLogin:
        valid_positions = { 0, 1, 1, 1, 1, 1 };
        break;
    case MenuRegister:
        valid_positions = { 0, 1, 1, 1, 1, 1, 1 };
        break;
    case MenuMainLogic:
        if (current_account.GetRole() == Admin) {
            valid_positions = { 0, 1, 1, 1, 1 }; // Основные пункты
            if (sublist_state[4]) { // Если подсписок открыт
                valid_positions.insert(valid_positions.end(), { 1, 1, 1, 1 }); // Добавляем пункты подсписка
            }
            else if (!sublist_state[4]) {
                valid_positions = { 0, 1, 1, 1, 1 }; // Для обычного пользователя
            }
        }
        else {
            valid_positions = { 0, 1, 1, 1 }; 
        }
        break;
    case MenuDisplayEmployees:
        valid_positions = { 0, 1 };
        break;
    case MenuFindEmployee:
        valid_positions = { 0, 1, 1, 1, 1, 1, 1 }; // 6 пунктов (4 поля ввода + поиск + назад)
        break;
    case MenuSortEmployees:
        valid_positions = { 0, 1, 1, 1, 1, 1, 1 };
        break;
    case MenuAdminAdd:
        valid_positions = { 0, 1, 1, 1, 1, 1, 1 };
        break;
    case MenuAdminChange:
        valid_positions = { 0, 1,1,1,1,1,1 };
        break;
    case MenuAdminDelete:
        valid_positions = { 0 };
        break;
    case MenuAdminAccounts:
        valid_positions = { 0 };
        break;
    default:
        break;
    }
}

bool RunningMenuManual() {
    New_Screen();
    cout << "Path:" << crgb::GRAY << " ~/Kursach/Title/" << crgb::GREEN << "Manual/" << crgb::RESET << endl;
    cout << "Демонстрация меню и ввода:\n\n";

    static int animation_frame = 0;
    static string input_text = "";
    static int menu_item = 1;

    // Menu items
    string menu_items[3] = { "Ввод", "Поле1", "Поле2" };

    // Display menu
    cout << "Пример меню:\n";
    bool anim_input = 0;
    for (int i = 0; i < 3; i++) {
        if (i + 1 == menu_item && menu_item == 1 && animation_frame < 4) {
            cout << crgb::GREEN << "-->";
            anim_input = true;
        }
        else {
            anim_input = false;
            if (i + 1 == menu_item) {
                cout << crgb::RESET << "-->";
            }
            else {
                cout << "   ";
            }
        }
        cout  << (anim_input ? crgb::GREEN : (i == 0 ? crgb::BLUE : crgb::RESET)) << "[" << menu_items[i] << "]";

        // Show input for active input field
        if (i + 1 == menu_item && menu_item == 1 && animation_frame < 4) {
            cout << " " << crgb::GREEN << input_text;
            if (animation_frame % 2 == 1) {
                cout << "_"; // Blinking cursor
            }
            cout << crgb::RESET;
        }
        cout << endl;
    }

    // Input feedback for input field
    if (menu_item == 1 && !input_text.empty() && animation_frame < 4) {
        cout << "\nСостояние ввода:\n";
        for (char c : input_text) {
            if (c >= 'a' && c <= 'z') { // Lowercase letters valid
                cout << crgb::GREEN << c;
            }
            else {
                cout << crgb::RED << c;
            }
        }
        cout << crgb::RESET << endl;
    }

    cout << endl << endl << crgb::GRAY << "[Enter]" << crgb::RESET << " - подтверждения действий";
    cout << endl << crgb::GRAY << "[Ecs]  " << crgb::RESET << " - отмена дейсвтия/выход";
    cout << endl << crgb::GRAY << "[^][v] " << crgb::RESET << " - перемещение указателя";
    cout << endl << crgb::GRAY << "[<][>] " << crgb::RESET << " - выбор по горизонтале";
    cout << endl << endl << crgb::RED << '<' << crgb::GRAY << "Admin only" << crgb::RED << '>' << crgb::RESET;
    cout << endl << crgb::RED << "[Del]  " << crgb::RESET << " - удаление(объекта)";
    cout << endl << crgb::RED << "[A]    " << crgb::RESET << " - добваление(объекта)";
    cout << endl << crgb::RED << "[<][>] " << crgb::RESET << " - подсписок/изменение роли";
    // Animation sequence
    if (animation_frame < 4) {
        menu_item = 1; // Active input field
        if (animation_frame == 2) input_text = "ab"; // Correct input
        if (animation_frame == 3) input_text = "ab1"; // Incorrect input
    }
    else if (animation_frame < 6) {
        menu_item = 2; // Move to Поле1
        input_text = "";
    }
    else if (animation_frame < 8) {
        menu_item = 3; // Move to Поле2
    }
    else {
        menu_item = 1; // Reset to Ввод
        input_text = "";
        animation_frame = -1; // Will increment to 0
    }

    // Handle exit
    Escape_Button();
    if (Escape_Now) {
        Escape_Now = false;
        current_menu = MenuTitle;
        return false;
    }

    // Update animation
    animation_frame++;
    screen_needs_update = true;

    // Wait 1 second
    Sleep(1500);
    return false;
}

bool RunningMenuMainLogic() {
    pointer = 1;
    screen_needs_update = true;
    
    // Инициализация векторов для подсписков
    valid_sublist = { false, false, false, false, true }; 
    sublist_state = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    while (true) {
        if (screen_needs_update) {
            New_Screen();
            if (current_account.GetRole() == User) {
                DisplayMenuUser();
                valid_positions = { 0, 1, 1, 1 };
            }
            else if (current_account.GetRole() == Admin) {
                DisplayMenuAdmin();
                Update_Valid_Positions_Menu(MenuMainLogic);
                // Если подсписок открыт, добавляем его элементы в valid_positions
                if (sublist_state[4]) {
                    valid_positions = { false, true, true, true, true, true, true, true, true };
                }
            }
            else {
                cout << "BAN";
            }
            Creat_New_Pointer(pointer, valid_positions);
            screen_needs_update = false;
        }

        Up_Or_Down_Logic(static_cast<int>(valid_positions.size()), valid_positions);
        Enter_Button();
        Left_Or_Right_Logic(valid_sublist, sublist_state);

        if (Up_Now || Down_Now) {
            screen_needs_update = true;
        }
        if ((Left_Now || Right_Now) && pointer == 4) {
            screen_needs_update = true;
        }
        if (Right_Now && pointer == 4) {
            Right_Now = 0;
            sublist_state[4] = true;
        }
        if (Left_Now && pointer == 4) {
            Left_Now = 0;
            sublist_state[4] = false;
        }
        if (Enter_Now) {
            Enter_Now = 0;
            if (current_account.GetRole() == User) {
                switch (pointer) {
                case 1:
                    current_menu = MenuDisplayEmployees;
                    return false;
                case 2:
                    current_menu = MenuFindEmployee;
                    return false;
                case 3:
                    current_menu = MenuSortEmployees;
                    return false;
                default:
                    break;
                }

            }
            else if (current_account.GetRole() == Admin) {
                switch (pointer) {
                case 1:
                    current_menu = MenuDisplayEmployees;
                    return false;
                case 2:
                    current_menu = MenuFindEmployee;
                    return false;
                case 3:
                    current_menu = MenuSortEmployees;
                    return false;
                case 5:
                    current_menu = MenuAdminAdd;
                    return false;
                case 6:
                    current_menu = MenuAdminDelete;
                    return false;
                case 7:
                    current_menu = MenuAdminChooseChange;
                    return false;
                case 8:
                    current_menu = MenuAdminAccounts;
                    return false;
                default:
                    break;
                }
            }
        }

        Escape_Button();
        if (Escape_Now) {
            Escape_Now = false;
            if (ConfirmExit()) {
                current_menu = MenuTitle;
                return false;
            }
            else {
                screen_needs_update = true;
            }
        }

    }
}


bool RunningMenuDisplayEmployees() {
    pointer = 1;
    screen_needs_update = true;
    EmployeeRepository employees;
    employees.GetAllEmployees();

    //const vector<Employee>& employees = EmployeeRepository::GetAllEmployees();
    valid_positions = vector<bool>(employees.GetVectorSize() + 1, true); // +1 для заголовка
    valid_positions[0] = 0;

    for (size_t i = 0; i < employees.GetVectorSize(); i++) {
        month_expanses = employees.CalculateSalaryExpenses(Month);
        quarter_expanses = employees.CalculateSalaryExpenses(Quarter);
        year_expanses = employees.CalculateSalaryExpenses(Year);
        alltime_expanses = employees.CalculateSalaryExpenses(AllTime);
    }

    while (true) {
        if (screen_needs_update) {
            DisplayMenuEmployeeList();
            Creat_New_Pointer(pointer, valid_positions);
            screen_needs_update = false;
        }

        Up_Or_Down_Logic(static_cast<int>(valid_positions.size()), valid_positions);

        // Обработка клавиш
        Enter_Button();
        Escape_Button();

        if (Up_Now || Down_Now) {
            screen_needs_update = true;
        }

        if (Enter_Now) {
            Enter_Now = false;
            // Логика выбора сотрудника (если нужно)
        }

        if (Escape_Now) {
            Escape_Now = false;
            current_menu = MenuMainLogic;
            return false;
        }

        if (current_account.GetRole() == Admin && GetAsyncKeyState('A') & 0x8000) {
            current_menu = MenuAdminAdd;
            return false;
        }

        Sleep(50);
    }
}

bool RunningMenuAdminAdd() {
    input_active = false;
    pointer = 1;
    screen_needs_update = true;
    name_input.clear();
    surname_input.clear();
    date_of_start_input.clear();
    salary_ratio_input = "0.0";
    salary_ratio = 0.0;

    while (true) {
        if (screen_needs_update) {
            Update_Valid_Positions_Menu(MenuAdminAdd);
            DisplayMenuAddEmployee();
            if (input_active && pointer >= 1 && pointer <= 4) {
                Set_Pointer_Color(0, 200, 0);
            }
            else {
                Set_Pointer_Color(255, 192, 203);
            }
            Creat_New_Pointer(pointer, valid_positions);
            screen_needs_update = false;
        }

        Up_Or_Down_Logic(static_cast<int>(valid_positions.size()), valid_positions);
        Enter_Button();
        Escape_Button();

        if (Up_Now || Down_Now) {
            input_active = false;
            screen_needs_update = true;
        }

        // Обработка ввода данных
        for (int key = 8; key <= 255; key++) {
            if (ConsoleInput::IsKeyPressed(key)) {
                if (key == VK_BACK) {
                    input_active = true;
                    if (pointer == 1 && !name_input.empty()) {
                        name_input.pop_back();
                    }
                    else if (pointer == 2 && !surname_input.empty()) {
                        surname_input.pop_back();
                    }
                    else if (pointer == 3 && !date_of_start_input.empty()) {
                        date_of_start_input.pop_back();
                    }
                    else if (pointer == 4) {
                        salary_ratio_input.pop_back();
                        if (salary_ratio_input.empty()) {
                            salary_ratio_input = "0";
                        }
                        try {
                            salary_ratio = stod(salary_ratio_input);
                        }
                        catch (...) {
                            salary_ratio = 0.0;
                        }
                        // Проверка границ
                        if (salary_ratio < 0.1) salary_ratio = 0.1;
                        if (salary_ratio > 10.0) salary_ratio = 10.0;
                        // Обработка backspace для коэффициента
                       /* string temp = to_string(salary_ratio);
                        if (!temp.empty()) {
                            temp.pop_back();
                            if (!temp.empty()) {
                                try {
                                    salary_ratio = stod(temp);
                                }
                                catch (...) {
                                    salary_ratio = 0.0;
                                }
                            }
                            else {
                                salary_ratio = 0.0;
                            }
                        }*/
                    }
                    screen_needs_update = true;
                }
                else {
                    char c = ConsoleInput::GetCharFromKey(key);
                    if (c != 0) {
                        input_active = true;
                        if (pointer == 1) {
                            // Имя - только буквы
                            if (isalpha(c) || (c >= 'А' && c <= 'я')) {
                                name_input += c;
                            }
                        }
                        else if (pointer == 2) {
                            // Фамилия - буквы и дефис
                            if (isalpha(c) || (c >= 'А' && c <= 'я') || c == '-') {
                                surname_input += c;
                            }
                        }
                        else if (pointer == 3) {
                            // Дата - цифры и точки с автоформатированием
                            if (isdigit(c)) {
                                if (date_of_start_input.length() < 10) {
                                    if (date_of_start_input.length() == 2 ||
                                        date_of_start_input.length() == 5) {
                                        date_of_start_input += '.';
                                    }
                                    date_of_start_input += c;
                                }
                            }
                        }
                        else if (pointer == 4) {
                            // Коэффициент - цифры и точка
                          /*  if (isdigit(c) || c == '.') {
                                string newValue = to_string(salary_ratio) + c;
                                try {
                                    double val = stod(newValue);
                                    if (val >= 0.1 && val <= 10.0) {
                                        salary_ratio = val;
                                    }
                                }
                                catch (...) {}
                            }*/
                            if (isdigit(c)) {
                                if (salary_ratio_input == "0") {
                                    salary_ratio_input = c;
                                }
                                else {
                                    salary_ratio_input += c;
                                }
                                try {
                                    salary_ratio = stod(salary_ratio_input);
                                }
                                catch (...) {
                                    salary_ratio = 0.0;
                                }
                            }
                            else if (c == '.' && salary_ratio_input.find('.') == string::npos) {
                                salary_ratio_input += c;
                                try {
                                    salary_ratio = stod(salary_ratio_input);
                                }
                                catch (...) {
                                    salary_ratio = 0.0;
                                }
                            }
                            // Проверка границ
                            if (salary_ratio < 0.1) salary_ratio = 0.1;
                            if (salary_ratio > 10.0) salary_ratio = 10.0;
                        }
                        screen_needs_update = true;
                    }
                }
            }
        }

        if (Enter_Now) {
            Enter_Now = false;
            if (pointer >= 1 && pointer <= 4) {
                input_active = true;
                screen_needs_update = true;
            }
            else if (pointer == 5) { // Сохранить
                // Проверка всех полей перед сохранением
                if (Employee::IsNameValid(name_input) &&
                    Employee::IsSurnameValid(surname_input) &&
                    Employee::IsDateValid(date_of_start_input) &&
                    Employee::IsSalaryRatioValid(salary_ratio)) {

                    try {
                        Employee newEmployee;
                        newEmployee.SetName(name_input);
                        newEmployee.SetSurname(surname_input);
                        newEmployee.SetDateOfStart(date_of_start_input);
                        newEmployee.SetSalaryRatio(salary_ratio);

                        EmployeeRepository::AddEmployee(newEmployee);
                        current_menu = MenuDisplayEmployees;
                        return false;
                    }
                    catch (const std::exception& e) {
                        cout << crgb::RED << "\nОшибка: " << e.what() << crgb::RESET;
                        Sleep(2000);
                        screen_needs_update = true;
                    }
                }
                else {
                    cout << crgb::RED << "\nПроверьте правильность введенных данных!" << crgb::RESET;
                    Sleep(2000);
                    screen_needs_update = true;
                }
            }
            else if (pointer == 6) { // Назад
                current_menu = MenuDisplayEmployees;
                return false;
            }
        }

        if (Escape_Now) {
            Escape_Now = false;
            current_menu = MenuDisplayEmployees;
            return false;
        }

        Sleep(50);
    }
}

bool RunningMenuAdminDelete() {
    pointer = 1;
    screen_needs_update = true;
    vector<bool> selected_employees;

    while (true) {
        DisplayMenuAdminRemove(selected_employees);
       

        Up_Or_Down_Logic(valid_positions.size(), valid_positions);
        Enter_Button();
        Escape_Button();

        // Обработка клавиши Del
        if (GetAsyncKeyState(VK_DELETE) & 0x8000) {
            // Небольшая задержка для обработки однократного нажатия
            Sleep(200);

            const auto& employees = EmployeeRepository::GetAllEmployees();
            if (pointer <= employees.size()) {
                try {
                    EmployeeRepository::RemoveEmployee(employees[pointer - 1].GetID());
                    screen_needs_update = true;

                    // Если удалили последнего сотрудника, выходим
                    if (EmployeeRepository::GetVectorSize() == 0) {
                        current_menu = MenuDisplayEmployees;
                        return false;
                    }

                    // Корректируем указатель, если он теперь вне диапазона
                    if (pointer > EmployeeRepository::GetVectorSize()) {
                        pointer = EmployeeRepository::GetVectorSize();
                    }
                }
                catch (const std::exception& e) {
                    cout << crgb::RED << "\nОшибка: " << e.what() << crgb::RESET;
                    Sleep(2000);
                    screen_needs_update = true;
                }
                
            }
        }

        if (Enter_Now) {
            Enter_Now = false;
            // Enter теперь просто выбирает сотрудника (можно добавить дополнительную логику)
            screen_needs_update = true;
        }

        if (Escape_Now) {
            Escape_Now = false;
            current_menu = MenuMainLogic;
            return false;
        }

        Sleep(50);
    }
}

bool RunningMenuFindEmployee() {
    pointer = 1;
    screen_needs_update = true;
    input_active = false;
    name_input.clear();
    surname_input.clear();
    date_of_start_input.clear();
    salary_ratio = 0.0;
    salary_ratio_input = "";
    search_results.clear();

    while (true) {
        if (screen_needs_update) {
            Update_Valid_Positions_Menu(MenuFindEmployee);
            DisplayMenuFindEmployee();

            if (input_active && (pointer >= 1 && pointer <= 4)) {
                Set_Pointer_Color(0, 200, 0);
            }
            else {
                Set_Pointer_Color(255, 192, 203);
            }

            Creat_New_Pointer(pointer, valid_positions);
            screen_needs_update = false;
        }

        Up_Or_Down_Logic(static_cast<int>(valid_positions.size()), valid_positions);
        Enter_Button();
        Escape_Button();

        if (Up_Now || Down_Now) {
            input_active = false;
            screen_needs_update = true;
        }

        // Обработка ввода данных
        for (int key = 8; key <= 255; key++) {
            if (ConsoleInput::IsKeyPressed(key)) {
                if (key == VK_BACK) {
                    input_active = true;
                    if (pointer == 1 && !name_input.empty()) {
                        name_input.pop_back();
                    }
                    else if (pointer == 2 && !surname_input.empty()) {
                        surname_input.pop_back();
                    }
                    else if (pointer == 3 && !date_of_start_input.empty()) {
                        date_of_start_input.pop_back();
                    }
                    else if (pointer == 4 && !salary_ratio_input.empty()) {
                        if (!salary_ratio_input.empty()) {
                            salary_ratio_input.pop_back();
                            if (salary_ratio_input.empty()) {
                                salary_ratio_input = "0";
                            }
                            try {
                                salary_ratio = stod(salary_ratio_input);
                            }
                            catch (...) {
                                salary_ratio = 0.0;
                            }
                        }
                    }
                    screen_needs_update = true;
                }
                else {
                    char c = ConsoleInput::GetCharFromKey(key);
                    if (c != 0) {
                        input_active = true;
                        if (pointer == 1) {
                            if (isalpha(c) || (c >= 'А' && c <= 'я')) {
                                name_input += c;
                            }
                        }
                        else if (pointer == 2) {
                            if (isalpha(c) || (c >= 'А' && c <= 'я') || c == '-') {
                                surname_input += c;
                            }
                        }
                        else if (pointer == 3) {
                            if (isdigit(c) || c == '.') {
                                if (date_of_start_input.length() < 10) {
                                    if (date_of_start_input.length() == 2 ||
                                        date_of_start_input.length() == 5) {
                                        date_of_start_input += '.';
                                    }
                                    date_of_start_input += c;
                                }
                            }
                        }
                        else if (pointer == 4) {
                            if (isdigit(c)) {
                                if (salary_ratio_input == "0") {
                                    salary_ratio_input = c;
                                }
                                else {
                                    salary_ratio_input += c;
                                }
                            }
                            else if (c == '.' && salary_ratio_input.find('.') == string::npos) {
                                salary_ratio_input += c;
                            }
                            try {
                                salary_ratio = stod(salary_ratio_input);
                            }
                            catch (...) {
                                salary_ratio = 0.0;
                            }
                        }
                        screen_needs_update = true;
                    }
                }
            }
        }

        if (Enter_Now) {
            Enter_Now = false;
            if (pointer >= 1 && pointer <= 4) {
                input_active = true;
                screen_needs_update = true;
            }
            else if (pointer == 5) { // Поиск
                // Выполняем поиск по всем заполненным параметрам
                vector<Employee> results = EmployeeRepository::GetAllEmployees();
                if (!name_input.empty()) {
                    vector<Employee> name_results = EmployeeRepository::FindEmployeesByName(name_input);
                    //results = filterResults(results, name_results);
                    results = name_results;
                }

                if (!surname_input.empty()) {
                    vector<Employee> surname_results = EmployeeRepository::FindEmployeesBySurname(surname_input);
                    //results = filterResults(results, surname_results);
                    results = surname_results;
                }

                if (!date_of_start_input.empty()) {
                    vector<Employee> date_results = EmployeeRepository::FindEmployeesByDate(date_of_start_input);
                    //results = filterResults(results, date_results);
                    results = date_results;
                }

                if ((salary_ratio) > 0) {
                    vector<Employee> salary_results = EmployeeRepository::FindEmployeesBySalary(stod(salary_ratio_input));
                    //results = filterResults(results, salary_results);
                    results = salary_results;
                }

                search_results = results;
                screen_needs_update = true;
            }
            else if (pointer == 6) { // Назад
                current_menu = MenuMainLogic;
                return false;
            }
        }

        if (Escape_Now) {
            Escape_Now = false;
            current_menu = MenuMainLogic;
            return false;
        }

        Sleep(50);
    } 
}

bool RunningMenuSortByParam() {
    pointer = 1;
    screen_needs_update = true;
    input_active = false;
    vector<bool> choosen_param = { false, false, false, false };
    vector<Employee> sorted_employees;
    bool ascending = true; // По умолчанию сортировка по возрастанию

    while (true) {
        if (screen_needs_update) {
            Update_Valid_Positions_Menu(MenuSortEmployees);
            DisplayMenuSortEmployees(choosen_param, sorted_employees);
            Set_Pointer_Color(255, 192, 203);
            Creat_New_Pointer(pointer, valid_positions);
            screen_needs_update = false;
        }

        Up_Or_Down_Logic(static_cast<int>(valid_positions.size()), valid_positions);
        Enter_Button();
        Escape_Button();
        Left_Or_Right_Logic(); // Для переключения между возрастанием/убыванием

        if (Up_Now || Down_Now || Left_Now || Right_Now) {
            screen_needs_update = true;
        }

        if (Enter_Now) {
            Enter_Now = false;
            if (pointer >= 1 && pointer <= 4) {
                // Выбор параметра сортировки (только один может быть выбран)
                for (size_t i = 0; i < choosen_param.size(); i++) {
                    choosen_param[i] = (i == pointer - 1);
                }
                screen_needs_update = true;
            }
            else if (pointer == 5) { // Sort
                // Определяем выбранный параметр
                SortParameter param = SortParameter::Name;
                if (choosen_param[1]) param = SortParameter::Surname;
                else if (choosen_param[2]) param = SortParameter::Date;
                else if (choosen_param[3]) param = SortParameter::Salary;

                // Выполняем сортировку
                sorted_employees = EmployeeRepository::SortEmployees(param, ascending);
                screen_needs_update = true;
            }
            else if (pointer == 6) { // Назад
                current_menu = MenuMainLogic;
                return false;
            }
        }

        if (Left_Now || Right_Now) {
            // Переключаем направление сортировки
            ascending = !ascending;
            Left_Now = false;
            Right_Now = false;
            screen_needs_update = true;
        }

        if (Escape_Now) {
            Escape_Now = false;
            current_menu = MenuMainLogic;
            return false;
        }
        Sleep(50);
    }
}                                                   // 11:11 11:11  САЛЮТ ЭТО АЛЬБОМ ЭСКАПИСТ ЕР БАН ЫЙ ВРОУТ НВА Х аф аф аф-аф аф
bool RunningMenuChooseChangeEmployee() {
    pointer = 1;
    screen_needs_update = true;
    const vector<Employee>& employees = EmployeeRepository::GetAllEmployees();
    valid_positions = vector<bool>(employees.size() + 1, true);
    valid_positions[0] = false;

    while (true) {
        if (screen_needs_update) {
            DisplayMenuEmployeeList();
            Set_Pointer_Color(255, 192, 203);
            Creat_New_Pointer(pointer, valid_positions);
            screen_needs_update = false;
        }

        Up_Or_Down_Logic(static_cast<int>(valid_positions.size()), valid_positions);
        Enter_Button();
        Escape_Button();
        if (Up_Now || Down_Now) {
            screen_needs_update = true;
        }

        if (Enter_Now) {
            Enter_Now = false;
            if (pointer >= 1 && pointer < valid_positions.size()) {
                // Выбор параметра сортировки (только один может быть выбран)
                index_for_change = pointer - 1;
                current_menu = MenuAdminChange;
                screen_needs_update = true;
                return false;
            }
        }

        if (Escape_Now) {
            Escape_Now = false;
            current_menu = MenuMainLogic;
            return false;
        }
        Sleep(50);
    }
}

bool RunningMenuChangeEmployee() {
    pointer = 1;
    screen_needs_update = true;
    name_input.clear();
    surname_input.clear();
    date_of_start_input.clear();
    salary_ratio = 0.0;
    salary_ratio_input = "0";  // Используем глобальную переменную

    vector<Employee> employees = EmployeeRepository::GetAllEmployees();
    Update_Valid_Positions_Menu(MenuAdminChange);

    while (true) {
        if (screen_needs_update) {
            DisplayMenuChange(employees[index_for_change]);
            if (input_active) {
                Set_Pointer_Color(200, 0, 0);
            }
            else {
            Set_Pointer_Color(255, 192, 203);
            }
            Creat_New_Pointer(pointer, valid_positions);
            screen_needs_update = false;
        }

        Up_Or_Down_Logic(static_cast<int>(valid_positions.size()), valid_positions);
        Enter_Button();
        Escape_Button();
        if (Up_Now || Down_Now) {
            screen_needs_update = true;
        }
        for (int key = 8; key <= 255; key++) {
            if (ConsoleInput::IsKeyPressed(key)) {
                if (key == VK_BACK) {
                    input_active = true;
                    if (pointer == 1 && !name_input.empty()) {
                        name_input.pop_back();
                    }
                    else if (pointer == 2 && !surname_input.empty()) {
                        surname_input.pop_back();
                    }
                    else if (pointer == 3 && !date_of_start_input.empty()) {
                        date_of_start_input.pop_back();
                    }
                    else if (pointer == 4 && !salary_ratio_input.empty()) {
                        // Обработка backspace для коэффициента
                        salary_ratio_input.pop_back();
                        if (salary_ratio_input.empty()) {
                            salary_ratio_input = "0";
                        }
                        try {
                            salary_ratio = stod(salary_ratio_input);
                        }
                        catch (...) {
                            salary_ratio = 0.0;
                        }
                        // Проверка границ
                        if (salary_ratio < 0.1) salary_ratio = 0.1;
                        if (salary_ratio > 10.0) salary_ratio = 10.0;
                        /* string temp = to_string(salary_ratio);
                        if (!temp.empty()) {
                            temp.pop_back();
                            if (!temp.empty()) {
                                try {
                                    salary_ratio = stod(temp);
                                }
                                catch (...) {
                                    salary_ratio = 0.0;
                                }
                            }
                            else {
                                salary_ratio = 0.0;
                            }
                        }*/
                    }
                    screen_needs_update = true;
                }
                else {
                    char c = ConsoleInput::GetCharFromKey(key);
                    if (c != 0) {
                        input_active = true;
                        if (pointer == 1) {
                            // Имя - только буквы
                            if (isalpha(c) || (c >= 'А' && c <= 'я')) {
                                name_input += c;
                            }
                        }
                        else if (pointer == 2) {
                            // Фамилия - буквы и дефис
                            if (isalpha(c) || (c >= 'А' && c <= 'я') || c == '-') {
                                surname_input += c;
                            }
                        }
                        else if (pointer == 3) {
                            // Дата - цифры и точки с автоформатированием
                            if (isdigit(c)) {
                                if (date_of_start_input.length() < 10) {
                                    if (date_of_start_input.length() == 2 ||
                                        date_of_start_input.length() == 5) {
                                        date_of_start_input += '.';
                                    }
                                    date_of_start_input += c;
                                }
                            }
                        }
                        else if (pointer == 4) {
                            if (isdigit(c)) {
                                if (salary_ratio_input == "0") {
                                    salary_ratio_input = c;
                                }
                                else {
                                    salary_ratio_input += c;
                                }
                                try {
                                    salary_ratio = stod(salary_ratio_input);
                                }
                                catch (...) {
                                    salary_ratio = 0.0;
                                }
                            }
                            else if (c == '.' && salary_ratio_input.find('.') == string::npos) {
                                salary_ratio_input += c;
                                try {
                                    salary_ratio = stod(salary_ratio_input);
                                }
                                catch (...) {
                                    salary_ratio = 0.0;
                                }
                            }
                            // Проверка границ
                            if (salary_ratio < 0.1) salary_ratio = 0.1;
                            if (salary_ratio > 10.0) salary_ratio = 10.0;
                            //// Коэффициент - цифры и точка
                            //if (isdigit(c) || c == '.') {
                            //    string newValue = to_string(salary_ratio) + c;
                            //    try {
                            //        double val = stod(newValue);
                            //        if (val >= 0.1 && val <= 10.0) {
                            //            salary_ratio = val;
                            //        }
                            //    }
                            //    catch (...) {}
                            //}
                        }
                        screen_needs_update = true;
                    }
                }
            }
        }
       
        if (Enter_Now) {            // ЩА будет
            Enter_Now = false;
            if (pointer >= 1 && pointer <= 4) {
                input_active = true;
                screen_needs_update = true;
            }
            else if (pointer == 5) { // Сохранить
                // Проверка всех полей перед сохранением
                try {
                    if (!name_input.empty()) {
                        employees[index_for_change].SetName(name_input);
                    }
                    if (!surname_input.empty()) {
                        employees[index_for_change].SetSurname(surname_input);
                    }
                    if (!date_of_start_input.empty()) {
                        employees[index_for_change].SetDateOfStart(date_of_start_input);
                    }
                    if (salary_ratio > 0.1 && salary_ratio <= 10) {
                        employees[index_for_change].SetSalaryRatio(salary_ratio);
                    }
                    EmployeeRepository::UpdateEmployee(employees[index_for_change]);
                    current_menu = MenuMainLogic;
                    return false;
                }
                catch (const std::exception& e) {
                    cout << crgb::RED << "\nОшибка: " << e.what() << crgb::RESET;
                    Sleep(2000);
                    screen_needs_update = true;
                }

                 
         
            }
            else if (pointer == 6) { // Назад
                current_menu = MenuAdminChooseChange;
                return false;
            }
            
        }

        if (Escape_Now) {
            Escape_Now = false;
            current_menu = MenuAdminChooseChange;
            return false;
        }
        Sleep(50);
    }
    /*
    -->[ID:XXXXXX][...]         // по нажатию клавишы [Enter] - запускать изменение
       [ID:XXXXXX][...]
        ...
    */

    /*
    Path: ~/Kursach/Amdmin/ChangeChoose/Change
    -->[Ввод][Имя]          | OldName  
       [Ввод][Фам]          | OldSurname
       [Ввод][Дат]          | OldDate
       [Ввод][Зар]          | OldSalary
       [Сохранить]
       [Назад] --> переходит в меню выбора для изменения
    
    
    */
}

bool RunningMenuAccountManagement() {
    pointer = 2; // Указатель на текущий аккаунт
    screen_needs_update = true;
    bool current_ban_reason_input = false; // Активен ли режим ввода причины бана
    string ban_reason_input; // Текущая введённая причина бана
    int ban_input_pointer = -1; // Индекс аккаунта, для которого вводится причина

    vector<AccountManager>& accounts = AccountRepository::GetAllAccounts();
    vector<bool> valid_pos(accounts.size() + 1, true);
    valid_pos[0] = false; // Зарезервировано
    valid_pos[1] = false; // Системный администратор

    while (true) {
        // Обновление экрана
        if (screen_needs_update) {
            DisplayMenuAccountManagement(accounts, ban_input_pointer, ban_reason_input);
            Creat_New_Pointer(pointer, valid_pos);
            screen_needs_update = false;
        }

        // Навигация по меню
        Up_Or_Down_Logic(valid_pos.size(), valid_pos);
        Left_Or_Right_Logic();
        Enter_Button();
        Escape_Button();

        // Выход из ввода или меню
        if (Escape_Now) {
            Escape_Now = false;
            if (current_ban_reason_input) {
                current_ban_reason_input = false;
                ban_reason_input.clear();
                ban_input_pointer = -1;
                screen_needs_update = true;
            }
            else {
                current_menu = MenuMainLogic;
                return false;
            }
        }

        // Смена роли (вне режима ввода)
        if (!current_ban_reason_input) {
            if (Left_Now) {
                Left_Now = false;
                UserAccountRole current_role = accounts[pointer - 1].GetRole();
                UserAccountRole new_role = current_role;
                if (current_role == Admin) new_role = User;
                else if (current_role == User) new_role = Banned;
                if (new_role != current_role) {
                    if (new_role == Banned) {
                        current_ban_reason_input = true;
                        accounts[pointer - 1].SetRole(new_role, current_account);
                        AccountRepository::SaveAccountsToFile();
                        ban_input_pointer = pointer - 1;
                        ban_input_pointer = pointer - 1;
                        ban_reason_input.clear();
                    }
                    else {
                        accounts[pointer - 1].SetRole(new_role, current_account);
                        AccountRepository::SaveAccountsToFile();
                        screen_needs_update = true;
                    }
                }
            }
            else if (Right_Now) {
                Right_Now = false;
                UserAccountRole current_role = accounts[pointer - 1].GetRole();
                UserAccountRole new_role = current_role;
                if (current_role == Banned) new_role = User;
                else if (current_role == User) new_role = Admin;
                if (new_role != current_role) {
                    accounts[pointer - 1].SetRole(new_role, current_account);
                    if (new_role != Banned) {
                        accounts[pointer - 1].SetBanReason(""); // Очищаем причину бана
                    }
                    AccountRepository::SaveAccountsToFile();
                    screen_needs_update = true;
                }
            }
        }
        // Динамический ввод причины бана
        if (current_ban_reason_input && ban_input_pointer == pointer - 1) {
            for (int key = 8; key <= 255; key++) {
                if (ConsoleInput::IsKeyPressed(key)) {
                    if (key == VK_BACK && !ban_reason_input.empty()) {
                        ban_reason_input.pop_back();
                        screen_needs_update = true;
                    }
                    else {
                        char c = ConsoleInput::GetCharFromKey(key);
                        if (c != 0 && isprint(c) && ban_reason_input.length() < 30) {
                            ban_reason_input += c;
                            screen_needs_update = true;
                        }
                    }
                }
            }
            if (Enter_Now) {
                Enter_Now = false;
                accounts[ban_input_pointer].SetRole(Banned, current_account);
                accounts[ban_input_pointer].SetBanReason(ban_reason_input);
                current_ban_reason_input = false;
                ban_input_pointer = -1;
                AccountRepository::SaveAccountsToFile();
                screen_needs_update = true;
            }
        }
        else if (current_ban_reason_input) {
            current_ban_reason_input = false;
            ban_reason_input.clear();
            ban_input_pointer = -1;
            screen_needs_update = true;
        }

        Sleep(50);
    }
    return false;
}
