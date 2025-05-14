#pragma once
//#include <iostream>
//#include <string>
//#include <sstream>                                          //
//#include <iomanip>
//#include <random>
//#include <fstream>
//#include <vector_re.h>
//
//using namespace std;
//
//enum UserAccountRole {
//    Banned = -1,
//    User = 0,
//    Admin = 1
//};
//
//
//int GenerateRandomUserID();
//class AccountManager;
//
//vector_re<AccountManager> vec_accounts;
//AccountManager current_user;
//
//
//class AccountManager {
//private:
//    string user_login;
//    string user_password;
//    string salt;
//    int user_ID = GenerateRandomUserID();
//    UserAccountRole user_role = User;
//
//    static string GenerateSalt(size_t length = 16) {
//        const string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
//        random_device rd;
//        mt19937 generator(rd());
//        uniform_int_distribution<> dist(0, chars.size() - 1);
//        string salt;
//        for (size_t i = 0; i < length; ++i) {
//            salt += chars[dist(generator)];
//        }
//        return salt;
//    }
//
//   
//
//public:
//    bool operator==(const AccountManager& input_user_data) {
//        return user_login == input_user_data.user_login && user_password == input_user_data.user_password;
//    }
//
//    bool operator==(const AccountManager& input_user_data) const {
//        return user_login == input_user_data.user_login && user_password == input_user_data.user_password;
//    }
//
//    friend void AddAccountObject(AccountManager& new_account);
//    friend bool IsInputDataCorrect(const string,const string);
//    friend int GenerateRandomUserID();
//    friend void LoadAccountsFromFile();
//
//    static string HashPassword(const string password, const string& salt) {
//        string salted = password + salt;
//        uint32_t hash = 0x67452301;
//
//        for (char c : salted) {                                                                 // проходимся по каждому символу
//            hash ^= (hash << 5) + (hash >> 2) + static_cast<uint32_t>(c);                       // XOR + побитовое смещение на 5 бит вдево(пустые --> 0) + страшие биты сдвигаются вправо(младшие теряются)
//            hash += (hash << 3) ^ (hash >> 7);
//            hash = (hash << 11) | (hash >> 21);
//        }
//        std::stringstream ss;                                                                   // Преобразование в шестнадцатеричную строку
//        ss << hex << setw(8) << setfill('0') << hash;
//        return ss.str();
//    }
//
//
//    string GetLogin() { return this->user_login; }
//    string GetPassword() { return this->user_password; }
//    string GetSalt() { return this->salt; }
//    int GetID() { return this->user_ID;  }
//    UserAccountRole GetRole() { return this->user_role;  }
//
//    void SetUserLogin(string input_login) { 
//        if (input_login.length() < 4) { 
//            cout << "Длина не менбше 4!"; 
//        } else {
//            this->user_login = input_login; 
//        }
//    }
//    void SetUserPaasword(string input_password) {
//        if (salt.empty()) {
//            salt = GenerateSalt();
//        }
//        user_password = HashPassword(input_password, salt);
//    }
//    void SetRole(UserAccountRole role, const AccountManager& caller) {                      // только админ может (default --> User)
//        if (caller.user_role != Admin) {
//            cout << "Только администратор может менять роли!";
//            return;
//        }
//        user_role = role;
//    }
//};
//
//
//void AddAccountObject(AccountManager& new_account) {
//    for (size_t i = 0; i < vec_accounts.get_size(); i++) {
//        if (new_account.GetLogin() == vec_accounts[i].GetLogin()) {
//            //error
//            cout << "LOGIN IS ALREADY EXIST";
//            return;
//        }
//    }
//    vec_accounts.push_back(new_account);
//}
//
//bool IsInputDataCorrect(const string input_login, const string input_password) {
//    for (size_t i = 0; i < vec_accounts.get_size(); ++i) {
//        if (vec_accounts[i].GetLogin() == input_login) {
//            if (vec_accounts[i].GetRole() == Banned) {
//                return false; // Аккаунт заблокирован
//            }
//            string hashed_input = AccountManager::HashPassword(input_password, vec_accounts[i].GetSalt());
//            if (hashed_input == vec_accounts[i].GetPassword()) {
//                current_user = vec_accounts[i];
//                return true;
//            }
//        }
//    }
//    return false;
//}
//
//int GenerateRandomUserID() {
//    random_device rd;
//    mt19937 gen(rd());
//    uniform_int_distribution<> distrib(100000, 999999);
//
//    while (true) {
//        int candidate = distrib(gen);
//        bool is_unique = true;
//
//        for (size_t i = 0; i < vec_accounts.get_size(); ++i) {
//            if (candidate == vec_accounts[i].GetID()) {
//                is_unique = false;
//                break;
//            }
//        }
//
//        if (is_unique) {
//            return candidate;
//        }
//    }
//}
//
//void SaveAccountsToFile() {
//    ofstream out("accounts.txt");
//    if (!out.is_open()) {
//        cerr << "Unable to open file";
//        return;
//    }
//
//    for (size_t i = 0; i < vec_accounts.get_size(); i++) {
//        out << "ID:" << vec_accounts[i].GetID()
//            << "Login:" << vec_accounts[i].GetLogin()
//            << "Password:" << vec_accounts[i].GetPassword()
//            << "Salt:" << vec_accounts[i].GetSalt()
//            << "Role:" << vec_accounts[i].GetRole() << '\n';
//            
//    }
//    out.close();
//}
//
//void LoadAccountsFromFile() {
//    ifstream in("accounts.txt");
//    if (!in.is_open()) {
//        cerr << "Ошибка открытия файла для чтения!" << endl;
//        return;
//    }
//
//    string line;
//    while (getline(in, line)) {
//        AccountManager acc;
//        stringstream ss(line);
//        string segment;
//
//        int id = 0;
//        string login, password, salt;
//        int role = 0;
//
//        while (getline(ss, segment, '|')) {
//            size_t pos = segment.find(':');
//            if (pos == string::npos) continue;
//            string key = segment.substr(0, pos);
//            string value = segment.substr(pos + 1);
//
//            if (key == "ID") id = stoi(value);
//            else if (key == "Login") login = value;
//            else if (key == "Password") password = value;
//            else if (key == "Salt") salt = value;
//            else if (key == "Role") role = stoi(value);
//        }
//
//        // Ручное восстановление объекта
//        acc.SetUserLogin(login);
//        acc.salt = salt;
//        acc.user_password = password; // уже хэш
//        acc.user_ID = id;
//        acc.user_role = static_cast<UserAccountRole>(role);
//
//        vec_accounts.push_back(acc);
//    }
//
//    in.close();
//}
//
//int main() {
//    LoadAccountsFromFile();
//
//    AccountManager new_acc;
//    new_acc.SetUserLogin("TestUser");
//    new_acc.SetUserPaasword("Pass123!");
//    AddAccountObject(new_acc);
//
//    SaveAccountsToFile();
//
//    return 0;
//    
//
//}
//void Left_Or_Right_Button() {
//    if (GetAsyncKeyState(VK_LEFT) != 0 && left_flag == 0) {
//        left_flag = 1;
//        Left_Now = 1;
//    }
//    else if (GetAsyncKeyState(VK_LEFT) == 0 && left_flag == 1) {
//        left_flag = 0;
//    }
//
//    if (GetAsyncKeyState(VK_RIGHT) != 0 && right_flag == 0) {
//        right_flag = 1;
//        Right_Now = 1;
//    }
//    else if (GetAsyncKeyState(VK_RIGHT) == 0 && right_flag == 1) {
//        right_flag = 0;
//    }
//}
//
//void Left_Or_Right_Logic(const vector<bool>& valid_sublist, vector<bool>& sublist_state) {
//    Left_Or_Right_Button();
//    if (Right_Now && valid_sublist[pointer]) {
//        sublist_state[pointer] = true;
//        Right_Now = 0;
//    }
//    if (Left_Now && valid_sublist[pointer]) {
//        sublist_state[pointer] = false;
//        Left_Now = 0;
//    }
//}
//void DisplayMenuLogin() {
//    static string login, password;
//    static bool show_password = false;
//    static bool is_login_entered = false, is_password_entered = false;
//    static InputError error = { "", 0 };
//
//    cout << "Path:" << crgb::GRAY << " ~/Kursach/Title/" << crgb::GREEN << "Login/" << crgb::RESET;
//    cout << setw(75) << "[sign in]\n";
//
//    Set_Pointer_Color(
//        (pointer == 1 && is_login_entered) || (pointer == 2 && is_password_entered) ? 0 : 200,
//        (pointer == 1 && is_login_entered) || (pointer == 2 && is_password_entered) ? 190 : 100,
//        (pointer == 1 && is_login_entered) || (pointer == 2 && is_password_entered) ? 0 : 200
//    );
//
//    cout << (pointer == 1 ? (is_login_entered ? crgb::GREEN : BLUE) : BLUE) << "   [Ввод]" << crgb::RESET << "(Логин) " << login;
//    if (error.row == 2) cout << crgb::RED << " " << error.message << crgb::RESET;
//    cout << endl;
//
//    cout << (pointer == 2 ? (is_password_entered ? crgb::GREEN : BLUE) : BLUE) << "   [Ввод]" << crgb::RESET << "(Пароль) "
//        << (show_password ? password : string(password.length(), '*'));
//    if (error.row == 3) cout << crgb::RED << " " << error.message << crgb::RESET;
//    cout << endl;
//
//    cout << (pointer == 3 ? crgb::GREEN : BLUE) << "   [Показать пароль]" << crgb::RESET << endl
//        << (pointer == 4 ? crgb::GREEN : BLUE) << "   [Войти]" << crgb::RESET << endl
//        << (pointer == 5 ? crgb::GREEN : BLUE) << "   [Назад]" << crgb::RESET << endl;
//    DisplayMiniManualWithHints();
//}
//
//void DisplayMenuRegistration() {
//    static string login, password, confirm_password;
//    static bool show_password = false;
//    static bool is_login_entered = false, is_password_entered = false, is_confirm_password_entered = false;
//    static InputError error = { "", 0 };
//
//    cout << "Path:" << crgb::GRAY << " ~/Kursach/Title/" << crgb::GREEN << "Register/" << crgb::RESET;
//    cout << setw(72) << "[sign in]\n";
//
//    Set_Pointer_Color(
//        (pointer == 1 && is_login_entered) || (pointer == 2 && is_password_entered) || (pointer == 3 && is_confirm_password_entered) ? 0 : 200,
//        (pointer == 1 && is_login_entered) || (pointer == 2 && is_password_entered) || (pointer == 3 && is_confirm_password_entered) ? 190 : 100,
//        (pointer == 1 && is_login_entered) || (pointer == 2 && is_password_entered) || (pointer == 3 && is_confirm_password_entered) ? 0 : 200
//    );
//
//    cout << (pointer == 1 ? (is_login_entered ? crgb::GREEN : BLUE) : BLUE) << "   [Ввод]" << crgb::RESET << "(Логин) " << login;
//    if (error.row == 2) cout << crgb::RED << " " << error.message << crgb::RESET;
//    cout << endl;
//
//    cout << (pointer == 2 ? (is_password_entered ? crgb::GREEN : BLUE) : BLUE) << "   [Ввод]" << crgb::RESET << "(Пароль) "
//        << (show_password ? password : string(password.length(), '*'));
//    if (error.row == 3) cout << crgb::RED << " " << error.message << crgb::RESET;
//    cout << endl;
//
//    cout << (pointer == 3 ? (is_confirm_password_entered ? crgb::GREEN : BLUE) : BLUE) << "   [Ввод]" << crgb::RESET << "(Подтвердить) "
//        << (show_password ? confirm_password : string(confirm_password.length(), '*'));
//    if (error.row == 4) cout << crgb::RED << " " << error.message << crgb::RESET;
//    cout << endl;
//
//    cout << (pointer == 4 ? crgb::GREEN : BLUE) << "   [Показать пароль]" << crgb::RESET << endl
//        << (pointer == 5 ? crgb::GREEN : BLUE) << "   [Зарегистрировать]" << crgb::RESET << endl
//        << (pointer == 6 ? crgb::GREEN : BLUE) << "   [Назад]" << crgb::RESET << endl;
//    DisplayMiniManualWithHints();
//}