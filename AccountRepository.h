#ifndef ACCOUNTREPOSITORY_H
#define ACCOUNTREPOSITORY_H

#include "AccountManager.h"
#include <vector>
#include <fstream>
#include "GLOBALS.h"

class AccountRepository {
private:
    static vector<AccountManager> vec_accounts;

    static void log(const string& message);

public:
    static void AddAccountObject(AccountManager& new_account);
    static bool IsInputDataCorrect(const string& input_login, const string& input_password);
    static bool IsValidPassword(const string& input_password);
    static void SaveAccountsToFile();
    static void LoadAccountsFromFile();
    static int GenerateRandomUserID();

    static string GetCurrentUserLogin();
    static bool IsLoginFree(const string new_user_login);
    static vector<AccountManager>& GetAllAccounts();
};

#endif