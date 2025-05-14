#include "AccountManager.h"

AccountManager::AccountManager() : user_ID(0) {}

string AccountManager::GenerateSalt(size_t length) {
    const string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<> dist(0, chars.size() - 1);
    string salt;
    for (size_t i = 0; i < length; ++i) {
        salt += chars[dist(generator)];
    }
    return salt;
}

bool AccountManager::operator==(const AccountManager& other) const {
    return user_login == other.user_login && user_password == other.user_password;
}

string AccountManager::HashPassword(const string& password, const string& salt) {
    string salted = password + salt;
    uint32_t hash = 0x67452301;
    for (char c : salted) {
        hash ^= (hash << 5) + (hash >> 2) + static_cast<uint32_t>(c);
        hash += (hash << 3) ^ (hash >> 7);
        hash = (hash << 11) | (hash >> 21);
    }
    stringstream ss;
    ss << hex << setw(8) << setfill('0') << hash;
    return ss.str();
}

int AccountManager::GetID() const { return user_ID; }
string AccountManager::GetLogin() const { return user_login; }
string AccountManager::GetPassword() const { return user_password; }
string AccountManager::GetSalt() const { return salt; }
UserAccountRole AccountManager::GetRole() const { return user_role; }
string AccountManager::GetBanReason() const { return ban_reason; }
void AccountManager::SetBanReason(const string& reason) { ban_reason = reason; }

void AccountManager::SetUserLogin(const string& login) {
    if (login.length() < 4 || login.length() > 13) {
        throw runtime_error("Логин должен быть от 4 до 13 символов!");
    }
    if (!regex_match(login, regex("^[a-zA-Z0-9]+$"))) {
        throw runtime_error("Логин должен содержать только латиницу и цифры!");
    }
    //user_ID = AccountRepository::GenerateRandomUserID();
    user_login = login;
}

void AccountManager::SetUserPassword(const string& password) {
    if (password.length() < 4 || password.length() > 13) {
        throw runtime_error("Пароль должен быть от 4 до 13 символов!");
    }
    if (!regex_match(password, regex("^(?=.*[a-zA-Z])(?=.*[0-9])(?=.*[!@#$%^&*]).+$"))) {
        throw runtime_error("");

    }
    if (salt.empty()) {
        salt = GenerateSalt();
    }
    user_password = HashPassword(password, salt);
}

void AccountManager::SetRole(UserAccountRole role, const AccountManager& caller) {
    if (caller.user_role != Admin) {
        throw runtime_error("Только администратор может менять роли!");
    }
    user_role = role;
}

void AccountManager::SetRoleReg(UserAccountRole role) {
    user_role = role;
}

