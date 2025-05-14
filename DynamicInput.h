#ifndef DYNAMICINPUT_H
#define DYNAMICINPUT_H

#define NOMINMAX
#include <windows.h>
#include <string>
#include <limits>
#include <iostream>
#include <cctype>
#include <algorithm>

class ConsoleInput {
private:
    static HANDLE hConsole;
    static HANDLE hStdin;
    static bool key_flags[256]; // Массив для отслеживания состояния клавиш
    static HKL lastLayout;      // Последняя раскладка

public:
    // Установка кодировки CP1251 и режима консоли
    static void SetConsole() {
        SetConsoleCP(1251);
        SetConsoleOutputCP(1251);
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        hStdin = GetStdHandle(STD_INPUT_HANDLE);
        ZeroMemory(key_flags, sizeof(key_flags));

        // Отключаем автоматическую обработку консоли
        DWORD mode;
        GetConsoleMode(hStdin, &mode);
        SetConsoleMode(hStdin, mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));

        lastLayout = GetKeyboardLayout(0);
    }

    // Проверка состояния клавиши
    static bool IsKeyPressed(int virtual_key) {
        if (GetAsyncKeyState(virtual_key) & 0x8000) {
            if (!key_flags[virtual_key]) {
                key_flags[virtual_key] = true;
                return true;
            }
        }
        else if (key_flags[virtual_key]) {
            key_flags[virtual_key] = false;
        }
        return false;
    }

    // Перевод виртуального кода в char с учётом раскладки
    static char GetCharFromKey(int key, bool forNumberInput = false) {
        HKL layout = GetKeyboardLayout(0);
        if (layout != lastLayout) {
            lastLayout = layout;
            ZeroMemory(key_flags, sizeof(key_flags));
            BYTE emptyKS[256] = { 0 };
            wchar_t dummy[2];
            for (int i = 0; i < 2; ++i)
                ToUnicodeEx(VK_MENU, MapVirtualKeyEx(VK_MENU, MAPVK_VK_TO_VSC, layout),
                    emptyKS, dummy, _countof(dummy), 0, layout);
        }

        BYTE ks[256];
        if (!GetKeyboardState(ks)) return 0;
        bool shift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
        ks[VK_SHIFT] = ks[VK_LSHIFT] = ks[VK_RSHIFT] = shift ? 0x80 : 0;
        bool caps = (GetKeyState(VK_CAPITAL) & 1) != 0;
        ks[VK_CAPITAL] = caps ? 1 : 0;

        UINT sc = MapVirtualKeyEx(key, MAPVK_VK_TO_VSC, layout);
        if (!sc) return 0;
        wchar_t wch[2] = { 0 };
        int rc = ToUnicodeEx(key, sc, ks, wch, _countof(wch), 0, layout);
        if (rc != 1 || !iswprint(wch[0])) return 0;

        if (forNumberInput && iswalpha(wch[0])) return 0;

        char out = 0;
        if (WideCharToMultiByte(CP_ACP, 0, wch, 1, &out, 1, nullptr, nullptr) == 1)
            return out;
        return 0;
    }

    // Шаблонный ввод числа
    template<typename T>
    static T ReadNumber(T min, T max, const std::string& mode = "+") {
        std::string input;
        T result;
        bool done = false;
        bool hasDecimalPoint = false;

        FlushConsoleInputBuffer(hStdin);

        std::string allowedDigits;
        if constexpr (std::is_integral_v<T> && !std::is_same_v<T, size_t>) {
            for (int i = 0; i <= 9; ++i) {
                std::string test = std::string(1, char('0' + i));
                try {
                    long long temp = std::stoll(test);
                    if (temp >= min && temp <= max && (mode != "+" || temp >= 0)) {
                        allowedDigits += char('0' + i);
                    }
                }
                catch (...) {}
            }
        }
        else if constexpr (std::is_same_v<T, size_t>) {
            for (int i = 0; i <= 9; ++i) {
                std::string test = std::string(1, char('0' + i));
                try {
                    unsigned long long temp = std::stoull(test);
                    if (temp >= min && temp <= max) {
                        allowedDigits += char('0' + i);
                    }
                }
                catch (...) {}
            }
        }
        else {
            allowedDigits = "0123456789";
        }

        while (!done) {
            bool inputChanged = false;
            for (int key = 8; key <= 255 && !done; key++) {
                if (IsKeyPressed(key)) {
                    if (key == VK_BACK && !input.empty()) {
                        if (input.back() == '.' || input.back() == ',') {
                            hasDecimalPoint = false;
                        }
                        input.pop_back();
                        inputChanged = true;
                    }
                    else if (key == VK_RETURN && !input.empty()) {
                        try {
                            size_t pos;
                            if constexpr (std::is_integral_v<T>) {
                                if constexpr (std::is_same_v<T, size_t>) {
                                    unsigned long long temp = std::stoull(input, &pos);
                                    if (pos == input.length() && temp >= min && temp <= max) {
                                        result = static_cast<T>(temp);
                                        done = true;
                                    }
                                }
                                else {
                                    long long temp = std::stoll(input, &pos);
                                    if (pos == input.length() && temp >= min && temp <= max &&
                                        (mode != "+" || temp >= 0)) {
                                        result = static_cast<T>(temp);
                                        done = true;
                                    }
                                }
                            }
                            else {
                                double temp = std::stod(input, &pos);
                                if (pos == input.length() && temp >= min && temp <= max &&
                                    (mode != "+" || temp >= 0)) {
                                    result = static_cast<T>(temp);
                                    done = true;
                                }
                            }
                        }
                        catch (...) {}
                        inputChanged = true;
                    }
                    else {
                        char ch = GetCharFromKey(key, true);
                        if (ch != 0) {
                            bool isValidChar = false;
                            if (ch == '-' && input.empty() && mode != "+" && min < 0 &&
                                !std::is_same_v<T, size_t>) {
                                isValidChar = true;
                            }
                            else if ((ch == '.' || ch == ',') && !hasDecimalPoint &&
                                std::is_floating_point_v<T>) {
                                isValidChar = true;
                                hasDecimalPoint = true;
                            }
                            else if (allowedDigits.find(ch) != std::string::npos) {
                                std::string test = input + ch;
                                try {
                                    if constexpr (std::is_integral_v<T>) {
                                        if constexpr (std::is_same_v<T, size_t>) {
                                            unsigned long long temp = std::stoull(test);
                                            if (temp >= min && temp <= max) {
                                                isValidChar = true;
                                            }
                                        }
                                        else {
                                            long long temp = std::stoll(test);
                                            if (temp >= min && temp <= max &&
                                                (mode != "+" || temp >= 0)) {
                                                isValidChar = true;
                                            }
                                        }
                                    }
                                    else {
                                        double temp = std::stod(test);
                                        if (temp >= min && temp <= max &&
                                            (mode != "+" || temp >= 0)) {
                                            isValidChar = true;
                                        }
                                    }
                                }
                                catch (...) {}
                            }
                            if (isValidChar) {
                                input += ch;
                                inputChanged = true;
                                if constexpr (std::is_floating_point_v<T>) {
                                    allowedDigits.clear();
                                    for (int i = 0; i <= 9; ++i) {
                                        std::string testDigit = input + char('0' + i);
                                        try {
                                            double temp = std::stod(testDigit);
                                            if (temp >= min && temp <= max &&
                                                (mode != "+" || temp >= 0)) {
                                                allowedDigits += char('0' + i);
                                            }
                                        }
                                        catch (...) {}
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if (inputChanged) {
                std::cout << "\r" << std::string(input.length() + 1, ' ') << "\r";
                ColorizeInput(input, mode, min, max);
                std::cout << input;
            }

            Sleep(10);
        }

        std::cout << "\r" << std::string(input.length(), ' ') << "\r" << result << "\n";
        SetConsoleTextAttribute(hConsole, 7);
        return result;
    }

    static std::string ReadString(size_t maxLength, const std::string& allowed = ".*",
        const std::string& forbidden = "", int specialLimit = -1, bool& input_active = *(bool*)nullptr,
        int cursor_row = 2, bool is_password = false, bool show_password = false) {
        std::string input;
        int specialCount = 0;
        bool done = false;

        auto is_char_allowed = [&](char ch) {
            if (allowed == ".*") return true;
            bool isDigit = isdigit(static_cast<unsigned char>(ch));
            bool isLatin = (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
            bool isCyrillic = ch < 0; // CP1251 кириллица
            bool isSpecial = !isalnum(static_cast<unsigned char>(ch)) && !isspace(static_cast<unsigned char>(ch));

            if (allowed.find('d') != std::string::npos && isDigit) return true;
            if (allowed.find('l') != std::string::npos && isLatin) return true;
            if (allowed.find('k') != std::string::npos && isCyrillic) return true;
            if (allowed.find('s') != std::string::npos && isSpecial) return true;
            return false;
            };

        std::cout << "\033[" << cursor_row << ";20H";
        while (!done) {
            bool inputChanged = false;
            for (int key = 8; key <= 255 && !done; key++) {
                if (IsKeyPressed(key)) {
                    if (key == VK_BACK && !input.empty()) {
                        if (!isalnum(static_cast<unsigned char>(input.back())) &&
                            !isspace(static_cast<unsigned char>(input.back()))) {
                            specialCount--;
                        }
                        input.pop_back();
                        inputChanged = true;
                        input_active = !input.empty();
                    }
                    else if (key == VK_RETURN && !input.empty()) {
                        done = true;
                        inputChanged = true;
                        input_active = false;
                    }
                    else if (key == VK_ESCAPE) {
                        input_active = false;
                        done = true;
                    }
                    else if (key == VK_UP || key == VK_DOWN) {
                        input_active = !input.empty();
                        std::cout << "\033[" << cursor_row << ";20H" << std::string(maxLength + 1, ' ') << "\033[" << cursor_row << ";20H";
                        if (is_password && !show_password) {
                            std::cout << std::string(input.length(), '*');
                        }
                        else {
                            ColorizeInput(input, allowed, forbidden, maxLength, specialLimit);
                            std::cout << input;
                        }
                        return input; // Возвращаем текущее состояние для обработки стрелок
                    }
                    else if (input.length() < maxLength) {
                        char ch = GetCharFromKey(key, false);
                        if (ch != 0 && is_char_allowed(ch) && forbidden.find(ch) == std::string::npos) {
                            bool is_special = (specialLimit >= 0) &&
                                !isalnum(static_cast<unsigned char>(ch)) &&
                                !isspace(static_cast<unsigned char>(ch));
                            if (!is_special || specialCount < specialLimit) {
                                input += ch;
                                if (is_special) specialCount++;
                                inputChanged = true;
                                input_active = true;
                            }
                        }
                    }
                }
            }

            if (inputChanged) {
                std::cout << "\033[" << cursor_row << ";20H" << std::string(maxLength + 1, ' ') << "\033[" << cursor_row << ";20H";
                if (is_password && !show_password) {
                    std::cout << std::string(input.length(), '*');
                }
                else {
                    ColorizeInput(input, allowed, forbidden, maxLength, specialLimit);
                    std::cout << input;
                }
            }

            Sleep(10);
        }

        std::cout << "\033[" << cursor_row << ";20H" << std::string(maxLength + 1, ' ') << "\033[" << cursor_row << ";20H";
        if (is_password && !show_password) {
            std::cout << std::string(input.length(), '*');
        }
        else {
            ColorizeInput(input, allowed, forbidden, maxLength, specialLimit);
            std::cout << input;
        }
        input_active = false;
        return input;
    }

    // Цветовая индикация для строк
    static void ColorizeInput(const std::string& input, const std::string& allowed,
        const std::string& forbidden, size_t maxLength, int specialLimit) {
        if (input.empty()) {
            SetConsoleTextAttribute(hConsole, 7);
            return;
        }

        bool valid = true;
        int specialCount = 0;
        for (char ch : input) {
            if (!IsCharValid(ch, allowed, forbidden, specialCount, specialLimit)) {
                valid = false;
                break;
            }
            if (!isalnum(static_cast<unsigned char>(ch)) &&
                !isspace(static_cast<unsigned char>(ch))) {
                specialCount++;
            }
        }

        if (valid && input.length() >= 4 && input.length() <= maxLength) {
            SetConsoleTextAttribute(hConsole, 10);
        }
        else {
            SetConsoleTextAttribute(hConsole, 12);
        }
    }
    // Проверка валидности символа
    static bool IsCharValid(char ch, const std::string& allowed, const std::string& forbidden,
        int specialCount, int specialLimit) {
        if (forbidden.find(ch) != std::string::npos) return false;
        if (specialLimit >= 0 && !isalnum(static_cast<unsigned char>(ch)) &&
            !isspace(static_cast<unsigned char>(ch)) && specialCount >= specialLimit) {
            return false;
        }
        if (allowed == ".*") return true;

        bool isDigit = isdigit(static_cast<unsigned char>(ch));
        bool isLatin = (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
        bool isCyrillic = ch < 0; // CP1251 кириллица
        bool isSpecial = !isalnum(static_cast<unsigned char>(ch)) &&
            !isspace(static_cast<unsigned char>(ch));

        if (allowed.find('d') != std::string::npos && isDigit) return true;
        if (allowed.find('l') != std::string::npos && isLatin) return true;
        if (allowed.find('k') != std::string::npos && isCyrillic) return true;
        if (allowed.find('s') != std::string::npos && isSpecial) return true;
        return false;
    }


private:
   

    // Цветовая индикация для чисел
    template<typename T>
    inline static void ColorizeInput(const std::string& input, const std::string& mode, T min, T max) {
        if (input.empty()) {
            SetConsoleTextAttribute(hConsole, 7);
            return;
        }

        try {
            size_t pos;
            if constexpr (std::is_integral_v<T>) {
                if constexpr (std::is_same_v<T, size_t>) {
                    unsigned long long temp = std::stoull(input, &pos);
                    if (pos == input.length() && temp >= min && temp <= max) {
                        SetConsoleTextAttribute(hConsole, 10);
                    }
                    else {
                        SetConsoleTextAttribute(hConsole, 12);
                    }
                }
                else {
                    long long temp = std::stoll(input, &pos);
                    if (pos == input.length() && temp >= min && temp <= max &&
                        (mode != "+" || temp >= 0)) {
                        SetConsoleTextAttribute(hConsole, 10);
                    }
                    else {
                        SetConsoleTextAttribute(hConsole, 12);
                    }
                }
            }
            else {
                double temp = std::stod(input, &pos);
                if (pos == input.length() && temp >= min && temp <= max &&
                    (mode != "+" || temp >= 0)) {
                    SetConsoleTextAttribute(hConsole, 10);
                }
                else {
                    SetConsoleTextAttribute(hConsole, 12);
                }
            }
        }
        catch (...) {
            SetConsoleTextAttribute(hConsole, 12);
        }
    }

   

    //// Проверка валидности пароля (только латиница и цифры, длина 4-13)
    //static bool IsPasswordValid(const std::string& input) {
    //    if (input.length() < 4 || input.length() > 13) return false;
    //    for (char ch : input) {
    //        if (!isalnum(static_cast<unsigned char>(ch)) || ch < 0) {
    //            return false; // Запрещаем не-латинские символы и кириллицу
    //        }
    //    }
    //    return true;
    //}
};



#endif // !DYNAMICINPUT_H