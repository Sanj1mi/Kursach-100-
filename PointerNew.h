#ifndef POINTERNEW_H
#define POINTERNEW_H

/*

    Новый мииииф

    отличия:
    1) динам. считывание возможных позиций
    2) начало с 1й, а не последней

    ВСЕ






*/

#include <iostream>
#include <vector>
#define NOMINMAX
#include <Windows.h>
#include <colors.h>
#include <cstdint>

#include "GLOBALS.h"

using std::cout;
using std::endl;
using std::vector;

extern bool up_flag, down_flag, enter_flag;
extern bool Up_Now , Down_Now, Enter_Now;

extern bool left_flag , right_flag ;
extern bool Left_Now , Right_Now ;

extern bool escape_flag ;
extern bool Escape_Now ;

extern int pointer ;
extern int screen_now ;
extern bool exit_display ;
extern std::string pointer_color;

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

void Left_Or_Right_Button();
void Left_Or_Right_Logic(const vector<bool>& valid_sublist, vector<bool>& sublist_state);
void Left_Or_Right_Logic();

void Escape_Button();

#endif // !POINTERNEW_H

