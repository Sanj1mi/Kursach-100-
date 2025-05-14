/*

    Новый мииииф

    отличия:
    1) динам. считывание возможных позиций
    2) начало с 1й, а не последней

    ВСЕ






*/
#include "PointerNew.h"
//#include <iostream>
//#include <vector>
//#define NOMINMAX
//#include <Windows.h>
//#include <colors.h>
//#include <cstdint>

using std::cout;
using std::endl;
using std::vector;

bool up_flag = 0, down_flag = 0, enter_flag = 0;
bool Up_Now = 0, Down_Now = 0, Enter_Now = 0;

bool left_flag = 0, right_flag = 0;
bool Left_Now = 0, Right_Now = 0;

bool escape_flag = 0;
bool Escape_Now = 0;

int pointer = 0;
int screen_now = 0;
bool exit_display = 0;
std::string pointer_color = "\033[0m";

void New_Screen();
void Clear_Cin();
void Up_Or_Down_Button();
void Delete_Last_Pointer(int, int);
void Enter_Button();
void Creat_New_Pointer(int, int);
void Creat_New_Pointer(int, const vector<bool>&);
void Creat_New_Pointer(int, int, const bool*);
void Display_Pointer();
void Up_Or_Down_Logic(int, const vector<bool>&);
void Wait_For_Enter();
void Set_Pointer_Color(uint8_t, uint8_t, uint8_t);
void Hide_Cursor();
void Show_Cursor();
void RestorePointer(const vector<bool>& valid_positions);

void Up_Or_Down_Button() {
    if (GetAsyncKeyState(VK_UP) != 0 && up_flag == 0) {
        up_flag = 1;
        Up_Now = 1;
    }
    else if (GetAsyncKeyState(VK_UP) == 0 && up_flag == 1) {
        up_flag = 0;
    }

    if (GetAsyncKeyState(VK_DOWN) != 0 && down_flag == 0) {
        down_flag = 1;
        Down_Now = 1;
    }
    else if (GetAsyncKeyState(VK_DOWN) == 0 && down_flag == 1) {
        down_flag = 0;
    }
}

void Set_Pointer_Color(uint8_t red, uint8_t green, uint8_t blue) {
    pointer_color = crgb::ColorConstructRGB(1, red, green, blue);
}

void Display_Pointer() { cout << pointer_color << "-->" << crgb::RESET; }

void Enter_Button() {
    if (GetAsyncKeyState(VK_RETURN) != 0 && enter_flag == 0) {
        enter_flag = 1;
        Enter_Now = 1;
    }
    else if (GetAsyncKeyState(VK_RETURN) == 0 && enter_flag == 1) {
        enter_flag = 0;
    }
}

void Escape_Button() {
    if (GetAsyncKeyState(VK_ESCAPE) != 0 && escape_flag == 0) {
        escape_flag = 1;
        Escape_Now = 1;
    }
    else if (GetAsyncKeyState(VK_ESCAPE) == 0 && escape_flag == 1) {
        escape_flag = 0;
    }
}


void Hide_Cursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void Show_Cursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}



void Delete_Last_Pointer(int pointer, int MAX) {
    // Перемещаемся к строке указателя, столбец 1
    cout << "\033[" << pointer + 1 << ";1H";
    cout << "   "; // Очищаем ровно 3 символа (длина -->)
    cout << "\033[" << pointer + 1 << ";1H";
}

void Creat_New_Pointer(int pointer, int MAX) {
    // Перемещаемся к строке указателя, столбец 1
    cout << "\033[" << pointer + 1 << ";1H";
    Display_Pointer();
    Hide_Cursor();
    cout << "\033[B";   // костыль во измбежание удлаения
}

void Creat_New_Pointer(int pointer, const vector<bool>& valid_positions) {
    Creat_New_Pointer(pointer, static_cast<int>(valid_positions.size()));
}

void Creat_New_Pointer(int pointer, int MAX, const bool* valid_positions) {
    Creat_New_Pointer(pointer, MAX);
}

void Up_Or_Down_Logic(int MAX, const vector<bool>& valid_positions) {
    Up_Or_Down_Button();
    if (Up_Now || Down_Now) {
        Delete_Last_Pointer(pointer, MAX);
        if (Up_Now) {
            do {
                pointer = (pointer - 1 + MAX) % MAX;
            } while (!valid_positions[pointer]);
            Up_Now = 0;
        }
        else if (Down_Now) {
            do {
                pointer = (pointer + 1) % MAX;
            } while (!valid_positions[pointer]);
            Down_Now = 0;
        }
        Creat_New_Pointer(pointer, MAX);
    }
}

void Left_Or_Right_Button() {
    if (GetAsyncKeyState(VK_LEFT) != 0 && left_flag == 0) {
        left_flag = 1;
        Left_Now = 1;
    }
    else if (GetAsyncKeyState(VK_LEFT) == 0 && left_flag == 1) {
        left_flag = 0;
    }

    if (GetAsyncKeyState(VK_RIGHT) != 0 && right_flag == 0) {
        right_flag = 1;
        Right_Now = 1;
    }
    else if (GetAsyncKeyState(VK_RIGHT) == 0 && right_flag == 1) {
        right_flag = 0;
    }
}

void Left_Or_Right_Logic(const vector<bool>& valid_sublist, vector<bool>& sublist_state) {
    Left_Or_Right_Button();
    for (size_t i = 0; i < valid_sublist.size(); i++) {
        if (Right_Now && valid_sublist[i] && pointer == i) {
            sublist_state[i] = true;
            Right_Now = 0;
            screen_needs_update = true;
        }
        if (Left_Now && valid_sublist[i] && pointer == i) {
            sublist_state[i] = false;
            Left_Now = 0;
            screen_needs_update = true;
        }
    }
}

void Left_Or_Right_Logic() {
    Left_Or_Right_Button();
}
/*
крч пока демку сдлеаю, для админ прав, раскрывашку списка потом(хотя нет...)
 { 1, 1, 1, 0, 0, 0, 1 }           // valid_pos для возможных pointer "-->"
 { 0, 0, 1, 0, 0, 0, }                    // valid_sub для полей с возможно раскрывашкой [>>]
 { 0, 0, 0 }                    // sub_state крч.. я так подумал... тут такой же(проще для индексов )(0 - закрыт | 1 - открыт)
                                // менятся только тот, у которого индекс при valid_sub = 1 !!!

для чтения онли по полям ID:
    [ID:000001][>>]
        []
        []
        []
        []
    [ID:000002][>>]
        []
        []
        []
        []
    [ID:000003][>>]
        []
        []
        []
        []

для редактирования фулл:
[ID:000001][>>]
     -->[]
        []
        []
        []
[ID:000002][>>]
[ID:000003][>>]
        []
        []
        []
        []

*/

void RestorePointer(const vector<bool>& valid_positions) {
    pointer = 0;
    for (size_t i = 0; i < valid_positions.size(); ++i) {
        if (valid_positions[i]) {
            pointer = static_cast<int>(i); // индекс +1, т.к. pointer отсчёт с 1
            return;
        }
    }
    // Если все false — указатель остаётся на 1 (либо можно спрятать курсор/ошибку)
}
void New_Screen() { system("cls"); }

void Clear_Cin() { FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); }

void Wait_For_Enter() {
    std::cout << "[Нажмите Enter для продолжения]";
    while (true) {
        Enter_Button();
        if (Enter_Now) {
            Enter_Now = 0;
            break;
        }
    }
}

