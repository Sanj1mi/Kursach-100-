#include "AccountRepository.h"

vector<AccountManager> AccountRepository::vec_accounts;

void AccountRepository::log(const string& message) {
    ofstream log("register_log.txt", ios::app);
    log << message << "\n";
    log.close();
}

void AccountRepository::AddAccountObject(AccountManager& new_account) {
    new_account.user_ID = GenerateRandomUserID();
    for (size_t i = 0; i < vec_accounts.size(); ++i) {
        if (vec_accounts[i].GetLogin() == new_account.GetLogin()) {
            throw runtime_error("Логин уже существует!");
        }
    }
    vec_accounts.push_back(new_account);
    SaveAccountsToFile();
    log("New user registered: ID=" + to_string(new_account.GetID()) + ", Login=" + new_account.GetLogin());
}

bool AccountRepository::IsInputDataCorrect(const string& input_login, const string& input_password) {
    for (size_t i = 0; i < vec_accounts.size(); ++i) {
        if (vec_accounts[i].GetLogin() == input_login) {
            if (vec_accounts[i].GetRole() == Banned) {
                return false;
            }
            string hashed_input = AccountManager::HashPassword(input_password, vec_accounts[i].GetSalt());
            if (hashed_input == vec_accounts[i].GetPassword()) {
                current_account = vec_accounts[i];
                return true;
            }
        }
    }
    return false;
}

bool AccountRepository::IsValidPassword(const string& input_password) {
    bool is_digit = 0;
    bool is_letter = 0;
    bool is_spec = 0;
    for (char c : input_password) {
        if (isdigit(c)) {
            is_digit = true;
        }
        if (isalpha(c)) {
            is_letter = true;
        }
        if (ispunct(c)) {
            is_spec = true;
        }
    }
    return is_digit && is_letter && is_spec;
}

void AccountRepository::SaveAccountsToFile() {
    ofstream out("accounts.txt");
    if (!out.is_open()) {
        throw runtime_error("Не удалось открыть accounts.txt для записи!");
    }
    for (size_t i = 0; i < vec_accounts.size(); ++i) {
        out << "ID:" << vec_accounts[i].GetID() << "|"
            << "Login:" << vec_accounts[i].GetLogin() << "|"
            << "Password:" << vec_accounts[i].GetPassword() << "|"
            << "Salt:" << vec_accounts[i].GetSalt() << "|"
            << "Role:" << static_cast<int>(vec_accounts[i].GetRole()) << "|"
            << "Mess:" << vec_accounts[i].GetBanReason() << "\n";
    }
    out.close();
}

void AccountRepository::LoadAccountsFromFile() {
    ifstream in("accounts.txt");
    if (!in.is_open()) {
        return; // Файл не существует (первый запуск)
    }
    vec_accounts.clear();
    string line;
    while (getline(in, line)) {
        AccountManager acc;
        stringstream ss(line);
        string segment;
        int id = 0;
        string login, password, salt, mess;
        int role = 0;
        while (getline(ss, segment, '|')) {
            size_t pos = segment.find(':');
            if (pos == string::npos) continue;
            string key = segment.substr(0, pos);
            string value = segment.substr(pos + 1);
            if (key == "ID") id = stoi(value);
            else if (key == "Login") login = value;
            else if (key == "Password") password = value;
            else if (key == "Salt") salt = value;
            else if (key == "Role") role = stoi(value);
            else if (key == "Mess") mess = value;
        }
        acc.user_ID = id; // Прямой доступ через friend
        acc.user_login = login;
        acc.user_password = password;
        acc.salt = salt;
        acc.user_role = static_cast<UserAccountRole>(role);
        acc.ban_reason = mess;
        vec_accounts.push_back(acc);
    }
    in.close();
}

int AccountRepository::GenerateRandomUserID() {
    if (vec_accounts.size() >= 1000000) {
        throw runtime_error("Достигнут лимит аккаунтов (1,000,000)!");
    }
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(100000, 999999);
    while (true) {
        int candidate = distrib(gen);
        bool is_unique = true;
        for (size_t i = 0; i < vec_accounts.size(); ++i) {
            if (candidate == vec_accounts[i].GetID()) {
                is_unique = false;
                break;
            }
        }
        if (is_unique) {
            return candidate;
        }
    }
}

bool AccountRepository::IsLoginFree(const string new_user_login) {
    for (size_t i = 0; i < vec_accounts.size(); i++) {
        if (new_user_login == vec_accounts[i].GetLogin()) {
            return false;
        }
    }
    return true;
}

string AccountRepository::GetCurrentUserLogin() { return current_account.GetLogin(); }

vector<AccountManager>& AccountRepository::GetAllAccounts() {
    return vec_accounts;
}
