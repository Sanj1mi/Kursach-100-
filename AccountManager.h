#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include <string>
#include <sstream>
#include <iomanip>
#include <random>
#include <regex>

#include "ENUM.h"

using namespace std;

class AccountManager {
private:
    int user_ID; // int, ��� �������
    string user_login;
    string user_password; // ������ ���
    string salt;
    UserAccountRole user_role = NonStatus;
    string ban_reason = "";

    static string GenerateSalt(size_t length = 16);
public:
    AccountManager();

    bool operator==(const AccountManager& other) const;

    static string HashPassword(const string& password, const string& salt);

    // �������
    int GetID() const;
    string GetLogin() const;
    string GetPassword() const;
    string GetSalt() const;
    UserAccountRole GetRole() const;
    string GetBanReason() const;
    void SetBanReason(const string& reason);

    // �������
    void SetUserLogin(const string& login);
    void SetUserPassword(const string& password);
    void SetRole(UserAccountRole role, const AccountManager& caller);
    void SetRoleReg(UserAccountRole role);
    // ������ ��� AccountRepository (����������/��������, ����������)
    friend class AccountRepository;
};

#endif