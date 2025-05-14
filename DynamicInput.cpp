#include "DynamicInput.h"

HANDLE ConsoleInput::hConsole = nullptr;
HANDLE ConsoleInput::hStdin = nullptr;
bool ConsoleInput::key_flags[256] = { false };
HKL ConsoleInput::lastLayout = nullptr;